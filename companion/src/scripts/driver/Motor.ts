import {BitwiseRegister} from './BitwiseRegister';
import {
	MotorResponse,
	ReturnCalibrationVariableResponse,
	ReturnOffsetVariableResponse,
	ReturnPositionVariableResponse
} from './serial/MotorResponse';
import {
	AdjustOffsetCommand,
	CalibrationCommand,
	GetVariableCommand,
	HapticCommand,
	IdleCommand,
	MotorCommand,
	MoveCommand,
	SetOffsetVariableCommand,
	SleepCommand,
	ToneCommand,
	WakeCommand
} from './serial/MotorCommand';
import {ISerialInterface} from './serial/SerialInterface';
import {resolveComponent} from 'vue';
import {CalibrationBits, MotorVariable} from './serial/SerialMessage';

export interface IMotor {
	get address(): number;

	toString(): string;

	send(command: MotorCommand): Promise<void>;

	request(command: MotorCommand): Promise<MotorResponse | null>;

	idle(): Promise<void>;

	sleep(): Promise<void>;

	wake(): Promise<void>;

	move(position?: number, torque?: number): Promise<void>;

	// Disables the motor, same as sending any position with the torque value of 0
	disable(): Promise<void>;

	tone(frequency: number): Promise<void>;

	// Same as sending any frequency above 24kHz
	silent(): Promise<void>;

	haptic(intensity: number, duration?: number): Promise<void>;

	/* This command adjusts the offset in a way that current position will be equal to the specified offset
	 * A useful application for this command would be to disable the motor, ask the user to move it into a known position
	 * (say, vertical) and then set the offset to a known value. This would make that offset (position)
	 * correspond to a known physical position
	 * If you want to change the offset directly, you can write it using a set variable command
	 *
	 * Example:
	 * motor.move(1024) // Motor moves to position 1024
	 * motor.adjustOffset(3072) // Offset is changed so that the current position is now 3072 even though the motor doesn't move
	 * motor.move(3072) // Position is now 3072, which is the same as 1024 used to be, the motor still doesn't move
	 * motor.move(2048) // Motor moves from 3072 (previously 1024) to 2048 (previously 0)
	 */
	adjustOffset(targetPosition?: number): Promise<void>;

	calibrate(mode?: BitwiseRegister<CalibrationBits>): Promise<void>;

	getCalibration(): Promise<BitwiseRegister<CalibrationBits>>;

	getOffset(): Promise<number>;

	setOffset(offset: number): Promise<void>;

	getPosition(): Promise<number>;
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

	send(command: MotorCommand): Promise<void> {
		return this._hardwareInterface.send(command);
	}

	request(command: MotorCommand): Promise<MotorResponse> {
		return this._hardwareInterface.request(command) as Promise<MotorResponse>;
	}

	disable(): Promise<void> {
		return this.move(0, 0);
	}

	idle(): Promise<void> {
		return this._hardwareInterface.send(new IdleCommand(0, this._address));
	}

	sleep(): Promise<void> {
		return this._hardwareInterface.send(new SleepCommand(0, this._address));
	}

	wake(): Promise<void> {
		return this._hardwareInterface.send(new WakeCommand(0, this._address));
	}

	move(position: number = 0, torque: number = 15): Promise<void> {
		return this._hardwareInterface.send(new MoveCommand(0, this._address, torque, position));
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
				.request(new GetVariableCommand(0, this._address, MotorVariable.Calibration))
				.then((res) => resolve((res as ReturnCalibrationVariableResponse).calibrationMode))
				.catch((err) => reject(err))
		);
	}

	getOffset(): Promise<number> {
		return new Promise<number>((resolve, reject) =>
			this._hardwareInterface
				.request(new GetVariableCommand(0, this._address, MotorVariable.Offset))
				.then((res) => resolve((res as ReturnOffsetVariableResponse).offset))
				.catch((err) => reject(err))
		);
	}

	setOffset(offset: number): Promise<void> {
		return this._hardwareInterface.send(new SetOffsetVariableCommand(0, this._address, offset));
	}

	getPosition(): Promise<number> {
		return new Promise<number>((resolve, reject) =>
			this._hardwareInterface
				.request(new GetVariableCommand(0, this._address, MotorVariable.Position))
				.then((res) => resolve((res as ReturnPositionVariableResponse).position))
				.catch((err) => reject(err))
		);
	}
}
