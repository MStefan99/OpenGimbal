import {CalibrationBits, Motor} from './Motor';
import {MotorResponse} from './serial/MotorResponse';
import {BitwiseRegister} from './BitwiseRegister';
import {HardwareInterface} from './HardwareInterface';

export type MotorEntry = {
	motor: Motor;
	active: boolean;
	initialCalibration?: BitwiseRegister<CalibrationBits>;
};

export class MotorManager extends EventTarget {
	readonly _hardwareInterface: HardwareInterface;
	readonly _motorEntries: MotorEntry[];

	constructor(hardwareInterface: HardwareInterface) {
		super();

		this._hardwareInterface = hardwareInterface;
		this._motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(hardwareInterface, i + 1),
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

	getInitialCalibration(address: Motor['address']): BitwiseRegister<CalibrationBits> {
		return (
			this._motorEntries[address - 1].initialCalibration ?? new BitwiseRegister<CalibrationBits>()
		);
	}
}
