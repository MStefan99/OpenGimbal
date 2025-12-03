import {IHardwareInterface} from '../HardwareInterface';
import {MotorResponse} from './MotorResponse';
import {ISerialParser, MotorParser} from './MotorParser';
import {Message} from '../Message';
import {MotorMessage} from './MotorMessage';
import {ControllerSerialCommand} from '../controller/ControllerSerialCommand';
import {
	ControllerSerialParser,
	IControllerSerialParser
} from '../controller/ControllerSerialParser';

const timeout = 20;

export interface ISerialInterface<
	CommandType extends Message,
	ResponseType extends Message
> extends IHardwareInterface {
	get open(): Promise<void>;

	send(message: CommandType, isochronous?: boolean): Promise<void>;

	request(message: CommandType): Promise<ResponseType>;

	close(): Promise<void>;
}

export class SerialInterface<
	CommandType extends Message,
	ResponseType extends Message
> implements ISerialInterface<CommandType, ResponseType> {
	private _port: SerialPort;
	private _parser: ISerialParser | IControllerSerialParser;
	private readonly _verbose: boolean;
	private readonly _openPromise: Promise<void>;
	private _sendPromise: Promise<void> = Promise.resolve();

	constructor(
		port: SerialPort,
		parser: ISerialParser | IControllerSerialParser,
		baudRate = 115200,
		verbose: boolean = false
	) {
		this._port = port;
		this._parser = parser;
		this._verbose = verbose;
		this._openPromise = this._port.open({
			baudRate,
			dataBits: 8,
			stopBits: 1,
			parity: 'odd'
		});
	}

	get open(): Promise<void> {
		return this._openPromise;
	}

	send(message: CommandType, isochronous?: boolean): Promise<void> {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

			this._sendPromise = this._sendPromise
				.then(() => this._port.writable.getWriter())
				.then((writer) =>
					writer.write(buffer).then(() => {
						writer.releaseLock();
						resolve();
					})
				)
				.catch((err) => {
					console.error('Send failed:', err);
					reject(err);
				});
		});
	}

	request(message: CommandType): Promise<ResponseType> {
		return new Promise((resolve, reject) => {
			const writeBuffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

			if (this._port.readable.locked) {
				console.warn('Serial readable stream locked');
				// TODO: add to queue
				return;
			}

			this._sendPromise = this._sendPromise
				.then(() => this._port.readable.cancel()) // Discard all previous data we're not interested in
				.then(() => this._port.writable.getWriter())
				.then((writer) => writer.write(writeBuffer).then(() => writer.releaseLock()))
				.then(async (): Promise<void> => {
					const readBuffer = new Uint8Array(16);
					let readBytes = 0;

					const reader = this._port.readable.getReader();
					setTimeout(() => {
						reader
							.cancel()
							.then(() =>
								reject(new Error(`Timed out waiting for a response to ${message.toString()}`))
							);
					}, timeout);

					try {
						while (true) {
							const {value: data, done} = await reader.read();
							if (done) {
								await reader.cancel();
								return;
							}
							readBuffer.set(data, readBytes);
							readBytes += data.byteLength;

							if (this._parser instanceof MotorParser) {
								const motorMessage = this._parser.parse(data);

								if (motorMessage) {
									this._verbose &&
										console.log('Received', motorMessage.toString(), '\n', motorMessage);
									await reader.cancel();

									resolve(motorMessage as unknown as ResponseType);
									return;
								}
							} else if (this._parser instanceof ControllerSerialParser) {
								const controllerSerialMessage = this._parser.parseResponse(data);

								if (controllerSerialMessage) {
									this._verbose &&
										console.log(
											'Received',
											controllerSerialMessage.toString(),
											'\n',
											controllerSerialMessage
										);
									await reader.cancel();

									resolve(controllerSerialMessage as unknown as ResponseType);
									return;
								}
							}

							readBytes = 0;
						}
					} finally {
						await reader.cancel();
					}
				})
				.catch((err) => {
					console.error('Send failed:', err);
					return reject(err);
				});
		});
	}

	async close(): Promise<void> {
		if (this._port.readable?.locked) {
			await this._port.readable.cancel('Port is being closed');
		}
		if (this._port.writable?.locked) {
			await this._port.writable.abort('Port is being closed');
		}

		return this._port.close();
	}
}
