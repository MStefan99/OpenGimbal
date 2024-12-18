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
					resolve(manager);

					(async (): Promise<void> => {
						while (port.readable) {
							manager._reader = port.readable.getReader();
							try {
								while (true) {
									const {value, done} = await manager._reader.read();
									if (done) {
										return;
									}
									await manager.parse(value);
								}
							} finally {
								manager._reader.releaseLock();
							}
						}
					})().catch((err) => {
						console.error(err);
						disconnectDevice(manager);
					});

					return manager;
				})
				.catch((err) => reject(err));
		});
	}
}

export function disconnectDevice(device: MotorManager): void {
	if (device.port.readable?.locked) {
		device._reader.cancel().then(() => device.port.close());
	} else {
		device.port.close();
	}

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
