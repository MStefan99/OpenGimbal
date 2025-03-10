import {ref} from 'vue';
import {IGimbal} from './Gimbal';
import {IMotorManager} from './MotorManager';
import {connectSerialDevice} from './serial/serialDriver';
import {connectUSBDevice} from './usb/usbDriver';

export const connectedDevice = ref<IGimbal | IMotorManager | null>(null);

export async function connectDevice(
	type: 'usb' | 'serial',
	demo: boolean = false
): Promise<IGimbal | IMotorManager | null> {
	if (demo) {
		throw new Error('Not implemented');
	} else {
		if (type === 'serial') {
			return (connectedDevice.value = await connectSerialDevice(demo, import.meta.env.DEV));
		} else {
			return (connectedDevice.value = await connectUSBDevice(demo, import.meta.env.DEV));
		}
	}
}

export function disconnectDevice(device: IGimbal | IMotorManager): void {
	device.close();
	connectedDevice.value = null;
}
