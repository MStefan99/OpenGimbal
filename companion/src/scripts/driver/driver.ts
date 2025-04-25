import {computed} from 'vue';
import {Gimbal, IGimbal} from './Gimbal';
import {IMotorManager} from './MotorManager';
import {
	connectedSerialDevice,
	connectSerialDevice,
	disconnectSerialDevice
} from './serial/serialDriver';
import {connectedUSBDevice, connectUSBDevice, disconnectUSBDevice} from './usb/usbDriver';

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
	if (device instanceof Gimbal) {
		disconnectUSBDevice(device);
	} else {
		disconnectSerialDevice(device);
	}
}
