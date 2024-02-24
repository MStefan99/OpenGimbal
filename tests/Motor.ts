import {SerialPort} from 'serialport';
import {Command} from './Command';
import {MockPort} from './index';
import {BitwiseRegister, CalibrationBits} from './BitMask';
import {
	MotorResponse,
	MotorResponseType,
	ReturnCalibrationVariableResponse,
	ReturnErrorVariableResponse,
	ReturnOffsetVariableResponse,
	ReturnRangeVariableResponse,
	ReturnVariableResponse
} from './MotorResponses';
import {
	CalibrationCommand,
	GetVariableCommand,
	HapticCommand,
	MotorVariableID,
	OffsetCommand,
	PositionCommand,
	SetOffsetVariableCommand,
	SetRangeVariableCommand,
	SleepCommand,
	ToneCommand
} from './MotorCommands';

const getMotorResponse: Record<MotorResponseType, (buffer: Uint8Array) => MotorResponse> = {
	[MotorResponseType.ReturnVariable]: (buffer) => new ReturnVariableResponse(buffer)
};

const getVariableResponse: Record<MotorVariableID, (buffer: Uint8Array) => ReturnVariableResponse> =
	{
		[MotorVariableID.Calibration]: (buffer) => new ReturnCalibrationVariableResponse(buffer),
		[MotorVariableID.Offset]: (buffer) => new ReturnOffsetVariableResponse(buffer),
		[MotorVariableID.Range]: (buffer) => new ReturnRangeVariableResponse(buffer),
		[MotorVariableID.Error]: (buffer) => new ReturnErrorVariableResponse(buffer)
	};

export class Motor {
	readonly #debug: boolean;
	readonly #address: number;
	#port: SerialPort | MockPort;
	#incomingBuffer = new Uint8Array(16);
	#incomingView = new DataView(this.#incomingBuffer.buffer);
	#bytesReceived = 0;
	#bytesRemaining = 0;
	#pendingRequests: Partial<Record<MotorVariableID, (response: MotorResponse) => void>> = {};

	constructor(port: SerialPort | MockPort, address: number, debug: boolean = false) {
		this.#address = address;
		this.#port = port;
		this.#debug = debug;
	}

	async #send(command: Command) {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(command.length).fill(0).map((v, i) => command.buffer[i]);

			this.#port.write(buffer, (err: any) => {
				if (err) {
					return reject(err);
				}
				this.#debug &&
					console.log('Command sent:', command.toString(), '\n', command.toString('hex'));
				resolve();
			});
		});
	}

	async #request<T extends MotorResponse>(command: GetVariableCommand) {
		return new Promise<T>((resolve, reject) => {
			const buffer = new Uint8Array(command.length).fill(0).map((v, i) => command.buffer[i]);

			this.#port.write(buffer, (err: any) => {
				if (err) {
					return reject(err);
				}
				this.#debug &&
					console.log('Request sent:', command.toString(), '\n', command.toString('hex'));
				this.#pendingRequests[command.variableID] = resolve as (response: MotorResponse) => void;
			});
		});
	}

	async parse(data: Uint8Array): Promise<Array<MotorResponse>> {
		const responses = new Array<MotorResponse>();

		for (const byte of data) {
			if (!this.#bytesRemaining) {
				this.#bytesRemaining = byte >> 4;
				this.#bytesReceived = 0;
			}

			this.#incomingView.setUint8(this.#bytesReceived++, byte);
			if (--this.#bytesRemaining === 0) {
				const genericResponse = new MotorResponse(this.#incomingBuffer);
				if (
					genericResponse.destAddr !== 0 || // Command not intended for this device
					genericResponse.srcAddr !== this.#address
				) {
					// Command coming from a different motor
					continue;
				}

				const response = getMotorResponse[genericResponse.type](this.#incomingBuffer);
				responses.push(response);

				if (response instanceof ReturnVariableResponse) {
					const variableResponse = getVariableResponse[response.variableID](this.#incomingBuffer);
					this.#debug &&
						console.log(
							'Response received:',
							variableResponse.toString(),
							'\n',
							response.toString('hex')
						);
					this.#pendingRequests[variableResponse.variableID](variableResponse);
					delete this.#pendingRequests[variableResponse.variableID];
				}
			}
		}

		return responses;
	}

	sleep() {
		return this.#send(new SleepCommand(0, this.#address));
	}

	// Technically not needed since any command would wake the device up, but it's nice to have an opposite of sleep()
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

	getCalibration() {
		return new Promise<CalibrationBits[]>((resolve) =>
			this.#request<ReturnCalibrationVariableResponse>(
				new GetVariableCommand(0, this.#address, MotorVariableID.Calibration)
			).then((res) => resolve(res.calibrationMode))
		);
	}

	getOffset() {
		return new Promise<number>((resolve) =>
			this.#request<ReturnOffsetVariableResponse>(
				new GetVariableCommand(0, this.#address, MotorVariableID.Offset)
			).then((res) => resolve(res.offset))
		);
	}

	getRange() {
		return new Promise<number>((resolve) =>
			this.#request<ReturnRangeVariableResponse>(
				new GetVariableCommand(0, this.#address, MotorVariableID.Range)
			).then((res) => resolve(res.range))
		);
	}

	setOffsetVariable(offset: number) {
		return this.#send(new SetOffsetVariableCommand(0, this.#address, offset));
	}

	setRangeVariable(range: number) {
		return this.#send(new SetRangeVariableCommand(0, this.#address, range));
	}
}
