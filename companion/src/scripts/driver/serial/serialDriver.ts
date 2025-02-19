'use strict';

import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {MotorManager} from '../MotorManager';
import {SerialInterface} from './SerialInterface';
import {SerialParser} from './SerialParser';

export const connectedSerialDevices = reactive<MotorManager[]>([]);
export const activeSerialDevice = ref<MotorManager | null>(null);

export async function connectSerialDevice(demo?: true): Promise<MotorManager | null> {
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
			port
				.open({
					baudRate: 115200,
					dataBits: 8,
					stopBits: 1,
					parity: 'odd'
				})
				.then(() => {
					const manager = new MotorManager(new SerialInterface(port, new SerialParser()));
					connectedSerialDevices.push(manager);
					activeSerialDevice.value = manager;
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

export function disconnectSerialDevice(device: MotorManager): void {
	device._hardwareInterface.close();

	const idx = connectedSerialDevices.indexOf(device);
	connectedSerialDevices.splice(idx, 1);

	if (device === activeSerialDevice.value) {
		if (connectedSerialDevices.length) {
			activeSerialDevice.value =
				connectedSerialDevices[idx > connectedSerialDevices.length - 1 ? idx - 1 : idx];
		} else {
			activeSerialDevice.value = null;
		}
	}
}

// TODO: close on disconnect
