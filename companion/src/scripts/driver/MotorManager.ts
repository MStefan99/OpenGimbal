import {Motor} from './Motor';
import {MotorResponse} from './MotorResponses';

export type MotorEntry = {
	motor: Motor;
	active: boolean;
};

export class MotorManager {
	readonly port: SerialPort;
	readonly _motorEntries: Array<MotorEntry>;

	constructor(port: SerialPort) {
		this.port = port;
		this._motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(port, i + 1),
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
		return new Motor(this.port, 15);
	}

	motor(address: Motor['address'] = 1): Motor {
		return this.motors.find((e) => e.address === address);
	}

	async parse(data: Uint8Array): Promise<MotorResponse[]> {
		const responses = new Array<MotorResponse>();

		for (const motor of this.motors) {
			responses.concat(await motor.parse(data));
		}

		return responses;
	}

	async enumerate(): Promise<Motor[]> {
		for (const entry of this._motorEntries) {
			try {
				entry.active = !!(await entry.motor.getCalibration()).value;
			} catch {
				// Nothing to do
			}
		}
		return this.motors;
	}
}
