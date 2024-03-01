import {SerialPort} from 'serialport';
import {MotorManager} from './MotorManager';
import {delay} from './util';
import {BitwiseRegister, CalibrationBits} from './BitMask';

type ErrorListener = (err: Error) => void;
type DataListener = (chunk: any) => void;
type EventListener = () => void;

export class MockPort extends EventTarget {
	path: string = 'Mock';

	constructor() {
		super();
		setImmediate(() => this.dispatchEvent(new Event('open')));
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
	const port: SerialPort = new SerialPort(
		{
			path,
			baudRate: 115200,
			dataBits: 8,
			stopBits: 1,
			parity: 'odd'
		},
		(err) => {
			if (err) {
				console.error(err.message);
				cb(new MockPort());
			}
		}
	);
	cb(port);
}

function main() {
	openPort('COM3', (port) => {
		port.on('close', () => console.log(port.path, 'closed'));

		port.on('open', async () => {
			console.log(port.path, 'opened');

			const manager = new MotorManager(port);
			port.on('data', (data: Buffer) => manager.parse(data));

			await manager.enumerate();
			for (const motor of manager.active) {
				console.log(
					`Motor ${motor.address} status:` +
						`\n  Calibration: ${(await motor.getCalibration()).map((b) => CalibrationBits[b]).join(', ')}` +
						`\n  Offset: ${await motor.getOffset()}` +
						`\n  Range: ${await motor.getRange()}`
				);
			}

			if (!manager.active.length) {
				console.error('No motors found');
			} else {
				const all = manager.all();

				await all.tone(247);
				await all.move(0);
				await delay(205);
				for (let i = 15; i > 4; i -= 5) {
					await all.tone(294);
					await all.move(0, i);
					await delay(205);
					await all.tone(392);
					await all.move(0, i);
					await delay(205);
				}
				await all.silent();

				await all.move();
				await delay(2000);
				for (let i = 0; i < 256; ++i) {
					await manager.motor(1).move(3 * i);
					await manager.motor(2).move(4096 - 4 * i);
					await manager.motor(3).move(2 * i);
					await delay(10);
				}
				for (const motor of manager.active) {
					await motor.move(0, 5);
					await delay(1000);
				}

				await all.disable();
			}

			port.close();
		});

		port.on('error', function (err) {
			console.log('Error: ', err.message);
		});
	});
}

main();
