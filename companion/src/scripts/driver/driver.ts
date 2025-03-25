import {computed} from 'vue';
import {IGimbal} from './Gimbal';
import {IMotorManager} from './MotorManager';
import {connectedSerialDevice, connectSerialDevice} from './serial/serialDriver';
import {connectedUSBDevice, connectUSBDevice} from './usb/usbDriver';

export const connectedDevice = computed<IGimbal | IMotorManager | null>(
	() => connectedUSBDevice.value ?? connectedSerialDevice.value
);

export async function connectDevice(
	type: 'usb' | 'serial',
	demo: boolean = false
): Promise<IGimbal | IMotorManager | null> {
	if (type === 'serial') {
		return await connectSerialDevice(demo, import.meta.env.DEV);
	} else {
		return await connectUSBDevice(demo, import.meta.env.DEV);
	}
}

export function disconnectDevice(device: IGimbal | IMotorManager): void {
	device.close();
}
