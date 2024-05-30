'use strict';

import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../popups';
import {MotorManager} from './MotorManager';

export const connectedDevices = reactive<MotorManager[]>([]);
export const activeDevice = ref<MotorManager | null>(null);

export async function connectDevice(demo?: true): Promise<MotorManager | null> {
	if (demo) {
		throw new Error('Not implemented');

		// const device = new Device(new SimulatedUSBDevice());
		// connectedDevices.push(device);
		// activeDevice.value = device;
		//
		// return Promise.resolve(device);
	} else {
		const port = await navigator.serial.requestPort();

		return new Promise((resolve, reject) => {
			if (connectedDevices.some((d) => (d as MotorManager)?.port === port)) {
				alert(
					'This device is already connected',
					PopupColor.Red,
					'Please select another device to connect'
				);
				reject();
				return null;
			}

			port
				.open({
					baudRate: 115200,
					dataBits: 8,
					stopBits: 1,
					parity: 'odd'
				})
				.then(() => {
					const manager = new MotorManager(port);
					connectedDevices.push(manager);
					activeDevice.value = manager;

					(async (): Promise<void> => {
						while (port.readable) {
							const reader = port.readable.getReader();
							try {
								// eslint-disable-next-line no-constant-condition
								while (true) {
									const {value, done} = await reader.read();
									if (done) {
										break;
									}
									await manager.parse(value);
								}
							} finally {
								reader.releaseLock();
							}
						}
					})();

					return manager;
				})
				.catch((err) => reject(err));
		});
	}
}

export function disconnectDevice(device: MotorManager): void {
	device.port.close();

	const idx = connectedDevices.indexOf(device);
	connectedDevices.splice(idx, 1);

	if (device === activeDevice.value) {
		if (connectedDevices.length) {
			activeDevice.value = connectedDevices[idx > connectedDevices.length - 1 ? idx - 1 : idx];
		} else {
			activeDevice.value = null;
		}
	}
}

// if ('serial' in navigator) {
// 	navigator.serial.addEventListener('disconnect', (e: Event) => {
// 		const device = (e as USBConnectionEvent).device;
// 		const foundDevice = connectedDevices.find((d) => d.usbDevice === device);
//
// 		if (foundDevice === undefined) {
// 			device.close();
// 		} else {
// 			disconnectDevice(foundDevice as MotorManager);
// 		}
// 	});
// }
