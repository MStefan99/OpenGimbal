'use strict';

import {Command, PositionCommand, SleepCommand, ToneCommand} from './Command';
import {SerialPort} from 'serialport';


function delay(ms: number) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

async function sendCommand(serialPort: SerialPort, command: Command, delayMs = 1000) {
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

async function main() {
	const port = new SerialPort({
		path: 'COM3',
		baudRate: 115200,
		dataBits: 8,
		stopBits: 1,
		parity: 'odd'
	});

	port.on('close', () => console.log(port.path, 'closed'))

	port.on('open', async () => {
		console.log(port.path, 'opened');

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
}

main();
