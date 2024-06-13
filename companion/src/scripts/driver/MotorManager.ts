import {CalibrationBits, Motor} from './Motor';
import {MotorResponse} from './MotorResponse';
import {BitwiseRegister} from './BitwiseRegister';

export type MotorEntry = {
	motor: Motor;
	active: boolean;
	initialCalibration?: BitwiseRegister<CalibrationBits>;
};

export class MotorManager extends EventTarget {
	readonly port: SerialPort;
	readonly _motorEntries: MotorEntry[];
	_reader: ReadableStreamDefaultReader;

	constructor(port: SerialPort, debug?: boolean) {
		super();

		this.port = port;
		this._motorEntries = Array.from({length: 14}, (v, i) => ({
			motor: new Motor(port, i + 1, debug),
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
		this.dispatchEvent(new CustomEvent('data', {detail: data}));

		const responses = new Array<MotorResponse>();
		for (const motor of this.motors) {
			responses.concat(await motor.parse(data));
		}

		this.dispatchEvent(new CustomEvent('response', {detail: responses}));
		return responses;
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
