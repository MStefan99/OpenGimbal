import {Command} from './Command';
import {BitwiseRegister} from './BitwiseRegister';
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

export enum CalibrationBits {
	Zero = 0x0,
	Pole = 0x1
}

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
	readonly _debug: boolean;
	readonly _address: number;
	_port: SerialPort;
	_incomingBuffer = new Uint8Array(16);
	_incomingView = new DataView(this._incomingBuffer.buffer);
	_bytesReceived = 0;
	_bytesRemaining = 0;
	_pendingRequests: Partial<
		Record<
			MotorVariableID,
			{
				resolve: (response: MotorResponse) => void;
				reject: (err: Error) => void;
				timeout: number;
			}
		>
	> = {};

	constructor(port: SerialPort, address: number, debug: boolean = false) {
		this._address = address;
		this._port = port;
		this._debug = debug;
	}

	get address(): number {
		return this._address;
	}

	toString(): string {
		return `Motor ${this._address}`;
	}

	async _send(command: Command): Promise<void> {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(command.length).fill(0).map((v, i) => command.buffer[i]);

			const writer = this._port.writable.getWriter();
			writer
				.write(buffer)
				.then(() => {
					writer.releaseLock();
					resolve();
				})
				.catch(reject);
		});
	}

	async _request<T extends MotorResponse>(command: GetVariableCommand): Promise<T> {
		return new Promise<T>((resolve, reject) => {
			const buffer = new Uint8Array(command.length).fill(0).map((v, i) => command.buffer[i]);

			const writer = this._port.writable.getWriter();
			writer.write(buffer).then(() => {
				writer.releaseLock();

				this._pendingRequests[command.variableID] = {
					resolve: resolve as (response: MotorResponse) => void,
					reject,
					timeout: setTimeout(() => {
						reject(new Error('Timed out while waiting for motor response'));
					}, 20)
				};
			});
		});
	}

	async parse(data: Uint8Array): Promise<Array<MotorResponse>> {
		const responses = new Array<MotorResponse>();

		for (const byte of data) {
			if (!this._bytesRemaining) {
				this._bytesRemaining = byte >> 4;
				this._bytesReceived = 0;
			}

			this._incomingView.setUint8(this._bytesReceived++, byte);
			if (--this._bytesRemaining === 0) {
				const genericResponse = new MotorResponse(this._incomingBuffer);
				if (
					genericResponse.destAddr !== 0 || // Command not intended for this device
					genericResponse.srcAddr !== this._address
				) {
					// Command coming from a different motor
					continue;
				}

				const response = getMotorResponse[genericResponse.type](this._incomingBuffer);
				responses.push(response);

				if (response instanceof ReturnVariableResponse) {
					const variableResponse = getVariableResponse[response.variableID](this._incomingBuffer);
					this._debug &&
						console.log(
							'Response received:',
							variableResponse.toString(),
							'\n',
							response.toString('hex')
						);
					this._pendingRequests[variableResponse.variableID].resolve(variableResponse);
					clearTimeout(this._pendingRequests[variableResponse.variableID].timeout);
					delete this._pendingRequests[variableResponse.variableID];
				}
			}
		}

		return responses;
	}

	sleep(): Promise<void> {
		return this._send(new SleepCommand(0, this._address));
	}

	// Technically not needed since any command would wake the device up, but it's nice to have an opposite of sleep()
	wakeup(): Promise<void> {
		return this.disable();
	}

	move(position: number = 0, torque: number = 15): Promise<void> {
		return this._send(new PositionCommand(0, this._address, torque, position));
	}

	// Disables the motor, same as sending any position with the torque of 0
	disable(): Promise<void> {
		return this._send(new PositionCommand(0, this._address, 0, 0));
	}

	tone(frequency: number): Promise<void> {
		return this._send(new ToneCommand(0, this._address, frequency));
	}

	// Same as sending any frequency above 24kHz
	silent(): Promise<void> {
		return this._send(new ToneCommand(0, this._address, 25000));
	}

	haptic(intensity: number, duration: number = 50): Promise<void> {
		return this._send(new HapticCommand(0, this._address, intensity, duration));
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
	 * motor.adjustOffset(3072) // Offset was changed so that the current position is now 3072 even though the motor didn't move
	 * motor.move(3072) // Position is now 3072, which is the same as 1024 used to be, the motor still doesn't move
	 * motor.move(2048) // Motor moves from 3072 (old 1024) to 2048 (old 0)
	 */
	adjustOffset(offset: number = 0): Promise<void> {
		return this._send(new OffsetCommand(0, this._address, offset));
	}

	calibrate(
		mode: BitwiseRegister<CalibrationBits> = new BitwiseRegister<CalibrationBits>().set(
			CalibrationBits.Zero
		)
	): Promise<void> {
		return this._send(new CalibrationCommand(0, this._address, mode));
	}

	getCalibration(): Promise<BitwiseRegister<CalibrationBits>> {
		return new Promise<BitwiseRegister<CalibrationBits>>((resolve, reject) =>
			this._request<ReturnCalibrationVariableResponse>(
				new GetVariableCommand(0, this._address, MotorVariableID.Calibration)
			)
				.then((res) => resolve(res.calibrationMode))
				.catch((err) => reject(err))
		);
	}

	getOffset(): Promise<number> {
		return new Promise<number>((resolve, reject) =>
			this._request<ReturnOffsetVariableResponse>(
				new GetVariableCommand(0, this._address, MotorVariableID.Offset)
			)
				.then((res) => resolve(res.offset))
				.catch((err) => reject(err))
		);
	}

	getRange(): Promise<number> {
		return new Promise<number>((resolve, reject) =>
			this._request<ReturnRangeVariableResponse>(
				new GetVariableCommand(0, this._address, MotorVariableID.Range)
			)
				.then((res) => resolve(res.range))
				.catch((err) => reject(err))
		);
	}

	setOffsetVariable(offset: number): Promise<void> {
		return this._send(new SetOffsetVariableCommand(0, this._address, offset));
	}

	setRangeVariable(range: number): Promise<void> {
		return this._send(new SetRangeVariableCommand(0, this._address, range));
	}
}
