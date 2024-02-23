'use strict';

import {CalibrationCommand, Command, PositionCommand, ToneCommand} from './Command';
import {SerialPort} from 'serialport';
import {BitwiseRegister, CalibrationBits} from "./BitMask";


function delay(ms: number) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

type ErrorCallback = (err: Error) => void;
type EventCallback = () => void;

class MockPort extends EventTarget {
	path: string = 'Mock';

	constructor() {
		super();
		setImmediate(() => this.dispatchEvent(new Event('open')))
	}

	write(chunk: Uint8Array, cb: (err?: any) => void): void {
		cb();
	}

	on(event: 'error', cb: ErrorCallback): void;
	on(event: string, cb: EventCallback): void;
	on(event: string, cb: EventCallback | ErrorCallback): void {
		if (event === 'close') {
			this.addEventListener(event, () => cb(new Error('Error: Mock port was closed')));
		} else {
			this.addEventListener(event, () => (cb as EventCallback)());
		}
	}

	close(): void {
		this.dispatchEvent(new Event('close'));
	}
}

async function sendCommand(serialPort: SerialPort | MockPort, command: Command) {
	return new Promise<void>((resolve, reject) => {
		const buffer = new Uint8Array(command.length)
			.fill(0)
			.map((v, i) => command.buffer[i]);

		serialPort.write(buffer, (err: any) => {
			if (err) {
				return reject(err);
			}
			console.log('Command sent:', command.toString());
			resolve();
		});
	})
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

			await sendCommand(port, new CalibrationCommand(0, 1,
				new BitwiseRegister().set(CalibrationBits.Zero).set(CalibrationBits.Pole)));

			await delay(1000);
			await sendCommand(port, new ToneCommand(0, 1, 247));
			await sendCommand(port, new PositionCommand(0, 1, 15, 0));
			await delay(210);

			for (let i = 15; i > 4; i -= 5) {
				await sendCommand(port, new ToneCommand(0, 1, 294));
				await sendCommand(port, new PositionCommand(0, 1, i, 0));
				await delay(210);
				await sendCommand(port, new ToneCommand(0, 1, 392));
				await sendCommand(port, new PositionCommand(0, 1, i, 0));
				await delay(210);
			}

			await sendCommand(port, new ToneCommand(0, 1, 25000));

			for (let i = 0; i < 10; ++i) {
				await sendCommand(port, new PositionCommand(0, 1, 15, Math.random() * 4096))
				await delay(1200);
			}
			await sendCommand(port, new PositionCommand(0, 1, 0, 0));

			port.close();
		});

		port.on('error', function (err) {
			console.log('Error: ', err.message);
		});
	});
}

main();
