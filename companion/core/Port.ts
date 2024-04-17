import {SerialPort} from 'serialport';

export type ErrorListener = (err: Error) => void;
export type DataListener = (chunk: Buffer) => void;
export type EventListener = () => void;

export class MockPort extends EventTarget {
	path: string = 'Mock';

	constructor() {
		super();
		setImmediate(() => this.dispatchEvent(new Event('open')));
	}

	write(chunk: Uint8Array, cb: (err?: Error) => void): void {
		cb();
	}

	on(event: 'error', listener: ErrorListener): void;
	on(event: 'close', listener: ErrorListener): void;
	on(event: 'data', listener: DataListener): void;
	on(event: string, listener: EventListener): void;
	on(event: string, cb: EventListener | DataListener | ErrorListener): void {
		if (event === 'close') {
			// @ts-expect-error Type definitions cover this case
			this.addEventListener(event, () => cb(new Error('Error: Mock port was closed')));
		} else if (event === 'data') {
			this.addEventListener(event, () => (cb as DataListener)(null));
		} else {
			this.addEventListener(event, () => (cb as EventListener)());
		}
	}

	close(): void {
		this.dispatchEvent(new Event('close'));
	}
}

export function openPort(
	path: string,
	cb: (port: SerialPort | MockPort) => void
): SerialPort | MockPort {
	const port: SerialPort = new SerialPort(
		{
			path,
			baudRate: 115200,
			dataBits: 8,
			stopBits: 1,
			parity: 'odd'
		},
		(err: Error) => {
			if (err) {
				console.error(err.message);
				cb(new MockPort());
			}
		}
	);
	cb(port);

	return port;
}
