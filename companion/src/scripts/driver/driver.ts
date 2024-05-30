'use strict';

import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../popups';
import {Device} from './Device';

export const connectedDevices = reactive<Device[]>([]);
export const activeDevice = ref<Device | null>(null);

export async function connectDevice(demo?: true): Promise<Device | null> {
	if (demo) {
		throw new Error('Not implemented');

		// const device = new Device(new SimulatedUSBDevice());
		// connectedDevices.push(device);
		// activeDevice.value = device;
		//
		// return Promise.resolve(device);
	} else {
		const usbDevice = await navigator.serial.requestPort({
			filters: []
		});

		return new Promise((resolve, reject) => {
			if (connectedDevices.some((d) => (d as Device)?.usbDevice === usbDevice)) {
				alert(
					'This device is already connected',
					PopupColor.Red,
					'Please select another device to connect'
				);
				reject();
				return null;
			}

			return usbDevice
				.open()
				.then(() => usbDevice.selectConfiguration(1))
				.then(() => usbDevice.claimInterface(0))
				.then(() => {
					const device = new Device(usbDevice);
					connectedDevices.push(device);
					activeDevice.value = device;
					return device;
				})
				.catch((err) => reject(err));
		});
	}
}

export function disconnectDevice(device: Device): void {
	if (device.usbDevice.opened) {
		device.usbDevice.close();
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

if ('usb' in navigator) {
	navigator.usb.addEventListener('disconnect', (e: Event) => {
		const device = (e as USBConnectionEvent).device;
		const foundDevice = connectedDevices.find((d) => d.usbDevice === device);

		if (foundDevice === undefined) {
			device.close();
		} else {
			disconnectDevice(foundDevice as Device);
		}
	});
}
