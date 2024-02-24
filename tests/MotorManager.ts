import {Motor} from './Motor';
import {SerialPort} from 'serialport';
import {MockPort} from './index';
import {MotorResponse} from './MotorResponses';

export class MotorManager {
	#motorEntries: Array<{
		motor: Motor;
		active: boolean;
	}>;

	constructor(port: SerialPort | MockPort) {
		this.#motorEntries = Array.from({length: 10}, (v, i) => ({
			motor: new Motor(port, i),
			active: false
		}));
		this.enumerate();
	}

	async parse(data: Uint8Array): Promise<Array<MotorResponse>> {
		return (await Promise.all(this.#motorEntries.map((m) => m.motor.parse(data)))).flat();
	}

	async enumerate() {
		this.#motorEntries.map(async (motor) => ({
			...motor,
			active: (await motor.motor.getCalibration()).length
		}));
	}
}
