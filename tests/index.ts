import {SerialPort} from 'serialport';
import {Motor} from "./Motor";
import {delay} from "./util";
import {MotorManager} from "./MotorManager";


type ErrorListener = (err: Error) => void;
type DataListener = (chunk: any) => void;
type EventListener = () => void;

export class MockPort extends EventTarget {
	path: string = 'Mock';

	constructor() {
		super();
		setImmediate(() => this.dispatchEvent(new Event('open')))
	}

	write(chunk: Uint8Array, cb: (err?: any) => void): void {
		cb();
	}

	on(event: 'error', listener: ErrorListener): void;
	on(event: 'data', listener: DataListener): void;
	on(event: string, listener: EventListener): void;
	on(event: string, cb: EventListener | DataListener | ErrorListener): void {
		if (event === 'close') {
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

function openPort(path: string, cb: (port: SerialPort | MockPort) => void) {
	const port: SerialPort = new SerialPort({
		path,
		baudRate: 115200,
		dataBits: 8,
		stopBits: 1,
		parity: 'odd'
	}, err => {
		if (err) {
			console.error(err.message);
			cb(new MockPort());
		}
	});
	cb(port);
}

function main() {
	openPort('COM3', port => {
		port.on('close', () => console.log(port.path, 'closed'));

		port.on('open', async () => {
			console.log(port.path, 'opened');

			const manager = new MotorManager(port);

			port.on('data', (data: Buffer) => console.log(
				Array.from(new Uint8Array(data))
					.map(b =>
						b.toString(16)
							.padStart(2, '0'))
			));

			const motor = new Motor(port, 1);

			await motor.setOffsetVariable(1024);
			await motor.setRangeVariable(3072);

			await motor.tone(247);
			await motor.move(0);
			await delay(210);

			for (let i = 15; i > 4; i -= 5) {
				await motor.tone(294);
				await motor.move(0, i);
				await delay(210);
				await motor.tone(392);
				await motor.move(0, i);
				await delay(210);
			}

			await motor.silent();

			for (let i = 0; i < 10; ++i) {
				await motor.move(Math.random() * 4096);
				await delay(1200);
			}
			await motor.sleep();

			port.close();
		});

		port.on('error', function (err) {
			console.log('Error: ', err.message);
		});
	});
}

main();
