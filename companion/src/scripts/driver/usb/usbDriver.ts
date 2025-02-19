import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {Gimbal, IGimbal} from '../Gimbal';
import {USBInterface} from './USBInterface';
import {USBParser} from './USBParser';
import {USBSerialEncapsulator} from './USBSerialEncapsulator';

export const connectedUSBDevice = ref<IGimbal | null>(null);

export async function connectUSBDevice(
	demo: boolean = false,
	verbose: boolean = false
): Promise<IGimbal | null> {
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
					const usbInterface = new USBInterface(usbDevice, new USBParser(), verbose);
					const device = new Gimbal(usbInterface, new USBSerialEncapsulator(usbInterface));
					connectedUSBDevice.value = device;

					resolve(device);
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

	connectedUSBDevice.value = null;
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
