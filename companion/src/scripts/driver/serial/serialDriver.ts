import {ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {IMotorManager, MotorManager} from '../MotorManager';
import {SerialInterface} from './SerialInterface';
import {SerialParser} from './SerialParser';

export const connectedSerialDevice = ref<IMotorManager | null>(null);
let connectedPort: SerialPort | null = null;

export async function connectSerialDevice(
	demo: boolean = false,
	verbose: boolean = false
): Promise<IMotorManager | null> {
	if (demo) {
		throw new Error('Not implemented');

		// const device = new Device(new SimulatedSerialDevice());
		// connectedDevices.push(device);
		// activeDevice.value = device;
		//
		// return Promise.resolve(device);
	} else {
		const port = await navigator.serial.requestPort();

		return new Promise((resolve, reject) => {
			const serialInterface = new SerialInterface(port, new SerialParser(), 115200, verbose);
			serialInterface.open
				.then(() => {
					connectedPort = port;
					const manager = new MotorManager(serialInterface);
					connectedSerialDevice.value = manager;

					resolve(manager);
				})
				.catch((err) => {
					if (err instanceof DOMException) {
						err.name === 'InvalidStateError' &&
							alert(
								'This device is already connected',
								PopupColor.Red,
								'Please select another device to connect'
							);
					}
					console.error(err);
					reject(err);
				});
		});
	}
}

export function disconnectSerialDevice(device: IMotorManager): void {
	device.close();

	connectedSerialDevice.value = connectedPort = null;
}

if ('serial' in navigator) {
	navigator.serial.addEventListener('disconnect', (e: Event): void => {
		if (connectedPort === e.target) {
			connectedSerialDevice.value = connectedPort = null;
		}
	});
}
