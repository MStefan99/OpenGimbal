import {HardwareInterface} from '../HardwareInterface';
import {SerialMessage} from './SerialMessage';
import {GetVariableCommand, MotorVariableID} from './MotorCommand';
import {MotorResponse} from './MotorResponse';
import {SerialParser} from './SerialParser';

export class SerialInterface extends HardwareInterface {
	_port: SerialPort;
	_parser: SerialParser;
	readonly _verbose: boolean;

	constructor(port: SerialPort, parser: SerialParser, verbose: boolean = false) {
		super(parser, verbose);

		this._port = port;

		this._parser = parser;
		this._verbose = verbose;
	}

	async send(command: SerialMessage): Promise<void> {
		return new Promise<void>((resolve, reject) => {
			const buffer = new Uint8Array(command.length).fill(0).map((v, i) => command.buffer[i]);
			this._verbose && console.log('Sending', command.toString(), '\n', command);

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

	async request<T extends MotorResponse>(command: GetVariableCommand): Promise<T> {
		return new Promise<T>((resolve, reject) => {
			const writeBuffer = new Uint8Array(command.length).fill(0).map((v, i) => command.buffer[i]);
			this._verbose && console.log('Sending', command.toString(), '\n', command);

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
								reject(new Error(`Timed out waiting for a response to ${command.toString()}`))
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
								this._verbose && responses.forEach((r) => console.log('Received', r.toString(), r));

								await reader.cancel();
								resolve(responses[0] as T);
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
