import {IMotor, Motor, MotorOptions} from './Motor';
import {ISerialInterface} from './motor/SerialInterface';
import {MotorMessage} from './motor/MotorMessage';
import {MotorCommand} from './motor/MotorCommand';
import {MotorResponse} from './motor/MotorResponse';

export interface IMotorControl {
	get active(): IMotor[];

	get all(): IMotor;

	send(command: MotorCommand): Promise<void>;

	request(command: MotorCommand): Promise<MotorResponse | null>;

	motor(address?: IMotor['address']): IMotor;

	enumerate(): Promise<IMotor[]>;

	getInitialOptions(address?: IMotor['address']): MotorOptions;
}

export interface IMotorManager {
	get motors(): IMotorControl;

	close(): Promise<void>;
}

export type MotorEntry = {
	motor: Motor;
	active: boolean;
	initialOptions?: MotorOptions;
};

class MotorControl implements IMotorControl {
	readonly _hardwareInterface: ISerialInterface<MotorMessage, MotorMessage>;
	readonly _motorEntries: MotorEntry[];

	constructor(serialInterface: ISerialInterface<MotorMessage, MotorMessage>) {
		this._hardwareInterface = serialInterface;
		this._motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(serialInterface, i + 1),
			active: false
		}));
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

	constructor(serialInterface: ISerialInterface<MotorMessage, MotorMessage>) {
		this._control = new MotorControl(serialInterface);
	}

	get motors(): IMotorControl {
		return this._control;
	}

	close(): Promise<void> {
		return this._control.close();
	}
}
