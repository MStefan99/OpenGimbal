import {Motor} from "./Motor";
import {SerialPort} from "serialport";
import {MockPort} from "./index";

export class MotorManager {
	#motors: Array<{
		motor: Motor,
		active: boolean
	}>;

	constructor(port: SerialPort | MockPort) {
		this.#motors = Array.from({length: 10}, (v, i) =>
			({
				motor: new Motor(port, i),
				active: false
			}));
		this.enumerate();
	}

	async enumerate() {
		for (let i = 0; i < 15; i++) {

		}
	}
}
