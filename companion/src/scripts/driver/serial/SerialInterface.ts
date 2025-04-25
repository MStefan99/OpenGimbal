import {IHardwareInterface} from '../HardwareInterface';
import {SerialMessage} from './SerialMessage';
import {MotorResponse} from './MotorResponse';
import {ISerialParser} from './SerialParser';

const timeout = 20;

export interface ISerialInterface extends IHardwareInterface {
	get open(): Promise<void>;

	send(message: SerialMessage, baudRate?: number): Promise<void>;

	request(message: SerialMessage, baudRate?: number): Promise<SerialMessage>;

	close(): Promise<void>;
}

export class SerialInterface implements ISerialInterface {
	private _port: SerialPort;
	private _parser: ISerialParser;
	private readonly _verbose: boolean;
	private readonly _openPromise: Promise<void>;

	constructor(
		port: SerialPort,
		parser: ISerialParser,
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

	send(message: SerialMessage, baudRate?: number): Promise<void> {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

			Promise.resolve()
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

	request(message: SerialMessage, baudRate?: number): Promise<SerialMessage> {
		return new Promise((resolve, reject) => {
			const writeBuffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

			if (this._port.readable.locked) {
				console.warn('Serial readable stream locked');
				// TODO: add to queue
				return;
			}

			Promise.resolve()
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
							const {value, done} = await reader.read();
							if (done) {
								await reader.cancel();
								return;
							}
							readBuffer.set(value, readBytes);
							readBytes += value.byteLength;
							const serialMessage = this._parser.parse(value);

							if (serialMessage && serialMessage instanceof MotorResponse) {
								this._verbose &&
									console.log('Received', serialMessage.toString(), '\n', serialMessage);

								await reader.cancel();
								resolve(serialMessage);
								return;
							} else {
								readBytes = 0;
							}
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
