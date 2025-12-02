import {reactive, ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {Gimbal, IGimbal} from '../Gimbal';
import {USBInterface} from './USBInterface';
import {ControllerParser} from './ControllerParser';
import {ControllerUSBEncapsulator} from './ControllerUSBEncapsulator';

export const connectedUSBDevice = ref<IGimbal | null>(null);
let connectedDevice: USBDevice | null = null;

export async function connectUSBDevice(verbose: boolean = false): Promise<IGimbal | null> {
	const usbDevice = await navigator.usb.requestDevice({
		filters: [
			{
				vendorId: 0x0424,
				productId: 0x0001
			}
		]
	});

	return new Promise((resolve, reject) => {
		return usbDevice
			.open()
			.then(() => usbDevice.selectConfiguration(1))
			.then(() => usbDevice.claimInterface(0))
			.then(() => {
				const usbInterface = new USBInterface(usbDevice, new ControllerParser(), verbose);
				const device = new Gimbal(usbInterface, new ControllerUSBEncapsulator(usbInterface));
				connectedUSBDevice.value = device;
				connectedDevice = usbDevice;

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

export function disconnectUSBDevice(device: IGimbal): void {
	device.close();

	connectedUSBDevice.value = connectedDevice = null;
}

if ('usb' in navigator) {
	navigator.usb.addEventListener('disconnect', (e: Event) => {
		const device = (e as USBConnectionEvent).device;

		if (device === connectedDevice) {
			connectedUSBDevice.value = connectedDevice = null;
		}
	});
}
