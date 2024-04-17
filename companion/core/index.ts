import {MotorManager} from './MotorManager';
import {delay, interpolate} from './util';
import {CalibrationBits} from './BitMask';
import {Motor} from './Motor';
import {openPort} from "./Port";

function main(): void {
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
				//eslint-disable-next-line
				async function run(motor: Motor): Promise<void> {
					await interpolate(0, 1024, 1000, 20, async (value) => await motor.move(value));
					await interpolate(1024, -1024, 1000, 20, async (value) => await motor.move(value));
					await interpolate(-1024, 2048, 2000, 20, async (value) => await motor.move(value));
					await interpolate(2048, 256, 500, 20, async (value) => await motor.move(value));
					await interpolate(256, 0, 1000, 20, async (value) => await motor.move(value));
				}

				await manager.all.move();
				await delay(1000);

				for (let i = 0; i < 3; i++) {
					await manager.motor(1).haptic(255, 1);
					await delay(200);
				}
				await delay(1000);

				await manager.all.disable();
			}

			port.close();
		});

		port.on('error', function (err: Error) {
			console.log('Error: ', err.message);
		});
	});
}

main();
