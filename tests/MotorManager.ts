import {Motor} from './Motor';
import {SerialPort} from 'serialport';
import {MockPort} from './index';
import {MotorResponse} from './MotorResponses';

export type MotorEntry = {
	motor: Motor;
	active: boolean;
};

export class MotorManager {
	readonly #port: SerialPort | MockPort;
	readonly #motorEntries: Array<MotorEntry>;

	constructor(port: SerialPort | MockPort) {
		this.#port = port;
		this.#motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(port, i + 1),
			active: false
		}));
	}

	get motors(): Array<Motor> {
		return this.#motorEntries.map((e) => e.motor);
	}

	get active(): Array<Motor> {
		return this.#motorEntries.filter((e) => e.active).map((e) => e.motor);
	}

	motor(address: Motor['address'] = 1): Motor {
		return this.motors.find(e => e.address === address);
	}

	all() {
		return new Motor(this.#port, 15);
	}

	async parse(data: Uint8Array): Promise<Array<MotorResponse>> {
		const responses = new Array<MotorResponse>();

		for (const motor of this.motors) {
			responses.concat(await motor.parse(data));
		}

		return responses;
	}

	async enumerate(): Promise<Array<Motor>> {
		for (const entry of this.#motorEntries) {
			try {
				entry.active = !!(await entry.motor.getCalibration()).length;
			} catch {
				// Nothing to do
			}
		}
		return this.motors;
	}
}
