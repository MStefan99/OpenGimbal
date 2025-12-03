import {computed} from 'vue';
import {Gimbal, IGimbal} from './Gimbal';
import {IMotorManager} from './MotorManager';
import {
	connectedMotorDevice,
	connectSerialDevice,
	disconnectSerialDevice
} from './motor/serialDriver';
import {
	connectedControllerDevice,
	connectUSBDevice,
	disconnectUSBDevice
} from './controller/usbDriver';

export const connectedDevice = computed<IGimbal | IMotorManager | null>(
	() => connectedControllerDevice.value ?? connectedMotorDevice.value
);

export async function connectDevice(
	type: 'usb' | 'serial'
): Promise<IGimbal | IMotorManager | null> {
	if (type === 'serial') {
		return await connectSerialDevice(import.meta.env.DEV);
	} else {
		return await connectUSBDevice(import.meta.env.DEV);
	}
}

export function disconnectDevice(device: IGimbal | IMotorManager): void {
	if (device instanceof Gimbal) {
		disconnectUSBDevice(device);
	} else {
		disconnectSerialDevice(device);
	}
}
