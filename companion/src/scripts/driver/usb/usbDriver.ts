'use strict';

import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {Gimbal} from '../Gimbal';
import {USBInterface} from './USBInterface';
import {USBParser} from './USBParser';

export const connectedDevices = reactive<Gimbal[]>([]);
export const activeDevice = ref<Gimbal | null>(null);

export async function connectDevice(demo?: true): Promise<Gimbal | null> {
	if (demo) {
		throw new Error('Not implemented');

		// const device = new Gimbal(new SimulatedUSBDevice());
		// connectedDevices.push(device);
		// activeDevice.value = device;
		//
		// return Promise.resolve(device);
	} else {
		const usbDevice = await navigator.usb.requestDevice({
			filters: [
				{vendorId: 0x0424} // TODO: add product ID(s)
			]
		});

		return new Promise((resolve, reject) => {
			return usbDevice
				.open()
				.then(() => usbDevice.selectConfiguration(1))
				.then(() => usbDevice.claimInterface(0))
				.then(() => {
					const device = new Gimbal(new USBInterface(usbDevice, new USBParser()));
					connectedDevices.push(device);
					activeDevice.value = device;
					return device;
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

export function disconnectDevice(device: Gimbal): void {
	device._hardwareInterface.close();

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

// TODO: fix
// if ('usb' in navigator) {
// 	navigator.usb.addEventListener('disconnect', (e: Event) => {
// 		const device = (e as USBConnectionEvent).device;
// 		const foundDevice = connectedDevices.find((d) => d._hardwareInterface === device);
//
// 		if (foundDevice === undefined) {
// 			device.close();
// 		} else {
// 			disconnectDevice(foundDevice as Gimbal);
// 		}
// 	});
// }
