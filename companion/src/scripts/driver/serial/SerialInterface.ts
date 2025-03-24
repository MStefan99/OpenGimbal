import {IHardwareInterface} from '../HardwareInterface';
import {SerialMessage} from './SerialMessage';
import {MotorResponse} from './MotorResponse';
import {ISerialParser} from './SerialParser';

const fastTimeout = 20; // For 115200 baud
const slowTimeout = 40; // For 9600 baud

export interface ISerialInterface extends IHardwareInterface {
	open(baudRate: number): Promise<void>;

	send(message: SerialMessage, baudRate?: number): Promise<void>;

	request(message: SerialMessage, baudRate?: number): Promise<SerialMessage>;

	close(): Promise<void>;
}

export class SerialInterface implements ISerialInterface {
	private _port: SerialPort;
	private _parser: ISerialParser;
	private readonly _verbose: boolean;
	private _currentBaudRate: number;
	private _isOpen: boolean = false;

	constructor(port: SerialPort, parser: ISerialParser, verbose: boolean = false) {
		this._port = port;
		this._parser = parser;
		this._verbose = verbose;
	}

	async open(baudRate: number): Promise<void> {
		if (baudRate === this._currentBaudRate) {
			return;
		}

		this._verbose &&
			console.log(
				this._isOpen ? 'Setting baud rate to' : 'Opening serial port with baud rate of',
				baudRate
			);
		if (this._isOpen) {
			await this._port.close();
		}
		await this._port.open({
			baudRate,
			dataBits: 8,
			stopBits: 1,
			parity: 'odd'
		});

		this._currentBaudRate = baudRate;
		this._isOpen = true;
	}

	send(message: SerialMessage, baudRate?: number): Promise<void> {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

			Promise.resolve()
				.then(() => baudRate && this.open(baudRate))
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
				.then(() => baudRate && this.open(baudRate))
				.then(() => this._port.readable.cancel()) // Discard all previous data we're not interested in
				.then(() => this._port.writable.getWriter())
				.then((writer) => writer.write(writeBuffer).then(() => writer.releaseLock()))
				.then(async (): Promise<void> => {
					const readBuffer = new Uint8Array(16);
					let readBytes = 0;

					const reader = this._port.readable.getReader();
					setTimeout(
						() => {
							reader
								.cancel()
								.then(() =>
									reject(new Error(`Timed out waiting for a response to ${message.toString()}`))
								);
						},
						this._currentBaudRate === 115200 ? fastTimeout : slowTimeout
					);

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
		if (this._port.writable.locked) {
			await this._port.writable.abort('Port is being closed');
		}

		return this._port.close();
	}
}
