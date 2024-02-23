'use strict';

import {SerialPort} from "serialport";
import {
	CalibrationCommand,
	HapticCommand,
	MotorCommand, OffsetCommand,
	PositionCommand,
	SleepCommand,
	ToneCommand
} from "./MotorCommand";
import {MockPort} from "./index";
import {BitwiseRegister, CalibrationBits} from "./BitMask";


export class Motor {
	#address: number;
	#port: SerialPort | MockPort;

	constructor(port: SerialPort | MockPort, address: number) {
		this.#address = address;
		this.#port = port;
	}

	async #send(command: MotorCommand) {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(command.length)
				.fill(0)
				.map((v, i) => command.buffer[i]);

			this.#port.write(buffer, (err: any) => {
				if (err) {
					return reject(err);
				}
				console.log('Command sent:', command.toString());
				resolve();
			});
		})
	}

	sleep() {
		return this.#send(new SleepCommand(0, this.#address));
	}

	// Technically not needed as any command would wake the device up, but it's nice to have an opposite of sleep()
	wakeup() {
		return this.disable();
	}

	move(position: number, torque: number = 15) {
		return this.#send(new PositionCommand(0, this.#address, torque, position));
	}

	// Disables the motor, same as sending any position with the torque of 0
	disable() {
		return this.#send(new PositionCommand(0, this.#address, 0, 0));
	}

	tone(frequency: number) {
		return this.#send(new ToneCommand(0, this.#address, frequency));
	}

	// Same as sending any frequency above 24kHz
	silent() {
		return this.#send(new ToneCommand(0, this.#address, 25000));
	}

	haptic(intensity: number, duration: number) {
		return this.#send(new HapticCommand(0, this.#address, intensity, duration));
	}

	/* This is a tricky one
	 * This command adjusts the offset in a way that current position will be equal to the specified offset
	 * A useful application for this command would be to disable the motor, ask the user to move it into a known position
	 * (say, vertical) and then set the offset to a known value. This would make that offset (position)
	 * correspond to a known physical position
	 * If you want to change the offset directly, you can write it using a set variable command
	 *
	 * Example:
	 * motor.move(1024) // Position is now 1024
	 * motor.setOffset(3072) // Offset was changed so that the current position is now 3072 even though the motor didn't move
	 * motor.move(3072) // Position is now 3072, which is the same as 1024 used to be, the motor still doesn't move
	 * motor.move(2048) // Motor moves from 3072 (old 1024) to 2048 (old 0)
	 */
	setOffset(offset: number) {
		return this.#send(new OffsetCommand(0, this.#address, offset));
	}

	calibrate(mode: BitwiseRegister<CalibrationBits>) {
		return this.#send(new CalibrationCommand(0, this.#address, mode));
	}

	setVariable() {
	}

	getVariable() {
	}
}
