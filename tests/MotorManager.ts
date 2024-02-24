import {Motor} from './Motor';
import {SerialPort} from 'serialport';
import {MockPort} from './index';
import {MotorResponse} from './MotorResponses';

export type MotorEntry = {
	motor: Motor;
	active: boolean;
};

export class MotorManager {
	#motorEntries: Array<MotorEntry>;

	constructor(port: SerialPort | MockPort) {
		this.#motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(port, i + 1),
			active: false
		}));
	}

	get motorEntries(): Array<MotorEntry> {
		return this.#motorEntries;
	}

	get motors(): Array<Motor> {
		return this.#motorEntries.map((e) => e.motor);
	}

	get active(): Array<Motor> {
		return this.#motorEntries.filter((e) => e.active).map((e) => e.motor);
	}

	async parse(data: Uint8Array): Promise<Array<MotorResponse>> {
		const responses = new Array<MotorResponse>();

		for (const motor of this.motors) {
			responses.concat(await motor.parse(data));
		}

		return responses;
	}

	async enumerate(): Promise<Array<MotorEntry>> {
		for (const entry of this.#motorEntries) {
			try {
				entry.active = !!(await entry.motor.getCalibration()).length;
			} catch {
				// Nothing to do
			}
		}
		return this.#motorEntries;
	}
}
