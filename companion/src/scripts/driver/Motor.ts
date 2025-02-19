import {SerialMessage} from './serial/SerialMessage';
import {BitwiseRegister} from './BitwiseRegister';
import {
	getMotorResponse,
	getVariableResponse,
	MotorResponse,
	ReturnCalibrationVariableResponse,
	ReturnOffsetVariableResponse,
	ReturnRangeVariableResponse,
	ReturnVariableResponse
} from './serial/MotorResponse';
import {
	CalibrationCommand,
	GetVariableCommand,
	HapticCommand,
	MotorVariableID,
	AdjustOffsetCommand,
	MoveCommand,
	SetOffsetVariableCommand,
	SetRangeVariableCommand,
	SleepCommand,
	ToneCommand
} from './serial/MotorCommand';
import {IHardwareInterface} from './HardwareInterface';
import {ISerialInterface} from './serial/SerialInterface';

export enum CalibrationBits {
	Zero = 0x0,
	Pole = 0x1
}

export interface IMotor {
	get address(): number;

	toString(): string;

	sleep(): Promise<void>;

	// Technically not needed since any command would wake the device up, but it's nice to have an opposite of sleep()
	wakeup(): Promise<void>;

	move(position?: number, torque?: number): Promise<void>;

	// Disables the motor, same as sending any position with the torque of 0
	disable(): Promise<void>;

	tone(frequency: number): Promise<void>;

	// Same as sending any frequency above 24kHz
	silent(): Promise<void>;

	haptic(intensity: number, duration?: number): Promise<void>;

	/* This is a tricky one
	 * This command adjusts the offset in a way that current position will be equal to the specified offset
	 * A useful application for this command would be to disable the motor, ask the user to move it into a known position
	 * (say, vertical) and then set the offset to a known value. This would make that offset (position)
	 * correspond to a known physical position
	 * If you want to change the offset directly, you can write it using a set variable command
	 *
	 * Example:
	 * motor.move(1024) // Position is now 1024
	 * motor.adjustOffset(3072) // Offset was changed so that the current position is now 3072 even though the motor didn't move
	 * motor.move(3072) // Position is now 3072, which is the same as 1024 used to be, the motor still doesn't move
	 * motor.move(2048) // Motor moves from 3072 (old 1024) to 2048 (old 0)
	 */
	adjustOffset(targetPosition?: number): Promise<void>;

	calibrate(mode?: BitwiseRegister<CalibrationBits>): Promise<void>;

	getCalibration(): Promise<BitwiseRegister<CalibrationBits>>;

	getOffset(): Promise<number>;

	getRange(): Promise<number>;

	setOffsetVariable(offset: number): Promise<void>;

	setRangeVariable(range: number): Promise<void>;
}

export class Motor implements IMotor {
	readonly _address: number;
	_hardwareInterface: ISerialInterface;

	constructor(hardwareInterface: ISerialInterface, address: number = 15) {
		this._hardwareInterface = hardwareInterface;
		this._address = address;
	}

	get address(): number {
		return this._address;
	}

	toString(): string {
		return `Motor ${this._address}`;
	}

	sleep(): Promise<void> {
		return this._hardwareInterface.send(new SleepCommand(0, this._address));
	}

	wakeup(): Promise<void> {
		return this.disable();
	}

	move(position: number = 0, torque: number = 15): Promise<void> {
		return this._hardwareInterface.send(new MoveCommand(0, this._address, torque, position));
	}

	disable(): Promise<void> {
		return this._hardwareInterface.send(new MoveCommand(0, this._address, 0, 0));
	}

	tone(frequency: number): Promise<void> {
		return this._hardwareInterface.send(new ToneCommand(0, this._address, frequency));
	}

	silent(): Promise<void> {
		return this._hardwareInterface.send(new ToneCommand(0, this._address, 25000));
	}

	haptic(intensity: number, duration: number = 5): Promise<void> {
		return this._hardwareInterface.send(new HapticCommand(0, this._address, intensity, duration));
	}

	adjustOffset(targetPosition: number = 0): Promise<void> {
		return this._hardwareInterface.send(new AdjustOffsetCommand(0, this._address, targetPosition));
	}

	calibrate(
		mode: BitwiseRegister<CalibrationBits> = new BitwiseRegister<CalibrationBits>().set(
			CalibrationBits.Zero
		)
	): Promise<void> {
		return this._hardwareInterface.send(new CalibrationCommand(0, this._address, mode));
	}

	getCalibration(): Promise<BitwiseRegister<CalibrationBits>> {
		return new Promise<BitwiseRegister<CalibrationBits>>((resolve, reject) =>
			this._hardwareInterface
				.request(new GetVariableCommand(0, this._address, MotorVariableID.Calibration))
				.then((res) => resolve((res as ReturnCalibrationVariableResponse).calibrationMode))
				.catch((err) => reject(err))
		);
	}

	getOffset(): Promise<number> {
		return new Promise<number>((resolve, reject) =>
			this._hardwareInterface
				.request(new GetVariableCommand(0, this._address, MotorVariableID.Offset))
				.then((res) => resolve((res as ReturnOffsetVariableResponse).offset))
				.catch((err) => reject(err))
		);
	}

	getRange(): Promise<number> {
		return new Promise<number>((resolve, reject) =>
			this._hardwareInterface
				.request(new GetVariableCommand(0, this._address, MotorVariableID.Range))
				.then((res) => resolve((res as ReturnRangeVariableResponse).range))
				.catch((err) => reject(err))
		);
	}

	setOffsetVariable(offset: number): Promise<void> {
		return this._hardwareInterface.send(new SetOffsetVariableCommand(0, this._address, offset));
	}

	setRangeVariable(range: number): Promise<void> {
		return this._hardwareInterface.send(new SetRangeVariableCommand(0, this._address, range));
	}
}
