import {IHardwareInterface} from '../HardwareInterface';
import {SerialMessage} from './SerialMessage';
import {MotorResponse} from './MotorResponse';
import {ISerialParser} from './SerialParser';

export interface ISerialInterface extends IHardwareInterface {
	send(message: SerialMessage): Promise<void>;

	request(message: SerialMessage): Promise<SerialMessage>;

	close(): Promise<void>;
}

export class SerialInterface implements ISerialInterface {
	private _port: SerialPort;
	private _parser: ISerialParser;
	private readonly _verbose: boolean;

	constructor(port: SerialPort, parser: ISerialParser, verbose: boolean = false) {
		this._port = port;
		this._parser = parser;
		this._verbose = verbose;
	}

	async send(message: SerialMessage): Promise<void> {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

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

	async request(message: SerialMessage): Promise<SerialMessage> {
		return new Promise((resolve, reject) => {
			const writeBuffer = new Uint8Array(message.length).fill(0).map((v, i) => message.buffer[i]);
			this._verbose && console.log('Sending', message.toString(), '\n', message);

			if (this._port.readable.locked) {
				console.warn('Serial readable stream locked');
				// TODO: add to queue
				return;
			}

			const writer = this._port.writable.getWriter();

			this._port.readable
				.cancel() // Discard all previous data we're not interested in
				.then(() => writer.write(writeBuffer))
				.then(() => writer.releaseLock())
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
					}, 20);

					try {
						while (true) {
							const {value, done} = await reader.read();
							if (done) {
								await reader.cancel();
								return;
							}
							readBuffer.set(value, readBytes);
							readBytes += value.byteLength;
							const serialMessages = this._parser.parse(value);

							const responses = serialMessages.filter((c) => c instanceof MotorResponse);
							if (responses.length) {
								this._verbose &&
									responses.forEach((r) => console.log('Received', r.toString(), '\n', r));

								await reader.cancel();
								resolve(responses[0]);
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
					console.error(err);
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
