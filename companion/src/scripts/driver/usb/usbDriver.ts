'use strict';

import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {Gimbal, IGimbal} from '../Gimbal';
import {USBInterface} from './USBInterface';
import {USBParser} from './USBParser';
import {USBSerialEncapsulator} from './USBSerialEncapsulator';

export const connectedUSBDevices = reactive<IGimbal[]>([]);
export const activeUSBDevice = ref<IGimbal | null>(null);

export async function connectUSBDevice(demo: boolean = false): Promise<IGimbal | null> {
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
					const usbInterface = new USBInterface(usbDevice, new USBParser());
					const device = new Gimbal(usbInterface, new USBSerialEncapsulator(usbInterface));
					connectedUSBDevices.push(device);
					activeUSBDevice.value = device;
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

export function disconnectUSBDevice(device: IGimbal): void {
	device.close();

	const idx = connectedUSBDevices.indexOf(device);
	connectedUSBDevices.splice(idx, 1);

	if (device === activeUSBDevice.value) {
		if (connectedUSBDevices.length) {
			activeUSBDevice.value =
				connectedUSBDevices[idx > connectedUSBDevices.length - 1 ? idx - 1 : idx];
		} else {
			activeUSBDevice.value = null;
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
