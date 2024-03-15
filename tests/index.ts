import {SerialPort} from 'serialport';
import {MotorManager} from './MotorManager';
import {delay, interpolate} from './util';
import {BitwiseRegister, CalibrationBits} from './BitMask';
import {Motor} from './Motor';

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
				if ((await motor.getOffset()) === 0) {
					await motor.adjustOffset();
					await delay(20);
				}

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
				async function run(motor: Motor) {
					await interpolate(0, 1024, 1000, 20, async (value) => await motor.move(value));
					await interpolate(1024, -1024, 1000, 20, async (value) => await motor.move(value));
					await interpolate(-1024, 2048, 2000, 20, async (value) => await motor.move(value));
					await interpolate(2048, 256, 500, 20, async (value) => await motor.move(value));
					await interpolate(256, 0, 1000, 20, async (value) => await motor.move(value));
				}

				await manager.all.move();
				await delay(1000);

				for (const motor of manager.active) {
					await run(motor);
				}
				await run(manager.all);

				await manager.all.disable();
			}

			port.close();
		});

		port.on('error', function (err) {
			console.log('Error: ', err.message);
		});
	});
}

main();
