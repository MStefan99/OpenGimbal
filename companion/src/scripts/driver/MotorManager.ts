import {CalibrationBits, IMotor, Motor} from './Motor';
import {BitwiseRegister} from './BitwiseRegister';
import {ISerialInterface} from './serial/SerialInterface';

export interface IMotorManager {
	get motors(): IMotor[];

	get active(): IMotor[];

	get all(): IMotor;

	motor(address?: IMotor['address']): IMotor;

	enumerate(): Promise<IMotor[]>;

	getInitialCalibration(address?: IMotor['address']): BitwiseRegister<CalibrationBits>;

	close(): Promise<void>;
}

export type MotorEntry = {
	motor: Motor;
	active: boolean;
	initialCalibration?: BitwiseRegister<CalibrationBits>;
};

export class MotorManager implements IMotorManager {
	readonly _hardwareInterface: ISerialInterface;
	readonly _motorEntries: MotorEntry[];

	constructor(serialInterface: ISerialInterface) {
		this._hardwareInterface = serialInterface;
		this._motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(serialInterface, i + 1),
			active: false
		}));
	}

	get motors(): Motor[] {
		return this._motorEntries.map((e) => e.motor);
	}

	get active(): Motor[] {
		return this._motorEntries.filter((e) => e.active).map((e) => e.motor);
	}

	get all(): Motor {
		return new Motor(this._hardwareInterface, 15);
	}

	motor(address: Motor['address'] = 1): Motor {
		return this.motors.find((e) => e.address === address);
	}

	async enumerate(): Promise<Motor[]> {
		for (const entry of this._motorEntries) {
			try {
				entry.initialCalibration = await entry.motor.getCalibration();
				// Motor responded
				entry.active = true;
			} catch (e) {
				// Motor did not respond
				entry.active = false;
			}
		}
		return this.active;
	}

	getInitialCalibration(address: Motor['address'] = 1): BitwiseRegister<CalibrationBits> {
		return (
			this._motorEntries[address - 1].initialCalibration ?? new BitwiseRegister<CalibrationBits>()
		);
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
