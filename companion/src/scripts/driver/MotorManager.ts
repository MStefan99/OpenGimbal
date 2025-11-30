import {IMotor, Motor, MotorOptions} from './Motor';
import {BitwiseRegister} from './BitwiseRegister';
import {ISerialInterface} from './serial/SerialInterface';
import {MotorCommand} from './serial/MotorCommand';
import {MotorResponse} from './serial/MotorResponse';

export interface IMotorControl {
	readonly vendorId: number | undefined;
	readonly productId: number | undefined;

	get active(): IMotor[];

	get all(): IMotor;

	send(command: MotorCommand): Promise<void>;

	request(command: MotorCommand): Promise<MotorResponse | null>;

	motor(address?: IMotor['address']): IMotor;

	enumerate(): Promise<IMotor[]>;

	getInitialOptions(address?: IMotor['address']): MotorOptions;
}

export interface IMotorManager {
	readonly vendorId: number | undefined;
	readonly productId: number | undefined;

	get motors(): IMotorControl;

	close(): Promise<void>;
}

export type MotorEntry = {
	motor: Motor;
	active: boolean;
	initialOptions?: MotorOptions;
};

class MotorControl implements IMotorControl {
	readonly _hardwareInterface: ISerialInterface;
	readonly _motorEntries: MotorEntry[];

	constructor(serialInterface: ISerialInterface) {
		this._hardwareInterface = serialInterface;
		this._motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(serialInterface, i + 1),
			active: false
		}));
	}

	get vendorId(): number | undefined {
		return this._hardwareInterface.vendorId;
	}

	get productId(): number | undefined {
		return this._hardwareInterface.productId;
	}

	get active(): Motor[] {
		return this._motorEntries.filter((e) => e.active).map((e) => e.motor);
	}

	get all(): Motor {
		return new Motor(this._hardwareInterface, 15);
	}

	send(command: MotorCommand): Promise<void> {
		return this._hardwareInterface.send(command);
	}

	request(command: MotorCommand): Promise<MotorResponse | null> {
		return this._hardwareInterface.request(command) as Promise<MotorResponse | null>;
	}

	motor(address: Motor['address'] = 1): Motor {
		return this._motorEntries.find((e) => e.motor.address === address).motor;
	}

	async enumerate(): Promise<Motor[]> {
		for (const entry of this._motorEntries) {
			try {
				entry.initialOptions = await entry.motor.getOptions();
				// Motor responded
				entry.active = entry.initialOptions !== null;
			} catch (e) {
				// Motor did not respond
				entry.active = false;
			}
		}
		return this.active;
	}

	getInitialOptions(address: Motor['address'] = 1): MotorOptions {
		return this._motorEntries[address - 1].initialOptions;
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}

export class MotorManager implements IMotorManager {
	readonly _control: MotorControl;

	constructor(serialInterface: ISerialInterface) {
		this._control = new MotorControl(serialInterface);
	}

	get vendorId(): number | undefined {
		return this._control.vendorId;
	}

	get productId(): number | undefined {
		return this._control.productId;
	}

	get motors(): IMotorControl {
		return this._control;
	}

	close(): Promise<void> {
		return this._control.close();
	}
}
