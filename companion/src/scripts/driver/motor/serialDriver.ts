import {ref} from 'vue';
import {alert, PopupColor} from '../../popups';
import {IMotorManager, MotorManager} from '../MotorManager';
import {SerialInterface} from './SerialInterface';
import {MotorParser} from './MotorParser';
import {ControllerSerialParser} from '../controller/ControllerSerialParser';
import {Gimbal} from '../Gimbal';
import {ControllerSerialEncapsulator} from '../controller/ControllerSerialEncapsulator';
import {ControllerEncapsulator} from '../controller/ControllerEncapsulator';
import {DiscoveryCommand} from '../controller/ControllerCommand';
import {ControllerSerialCommand} from '../controller/ControllerSerialCommand';
import {ControllerSerialResponse} from '../controller/ControllerSerialResponse';
import {DiscoveryResponse} from '../controller/ControllerResponse';
import {Message} from '../Message';
import {connectedDevice} from '../driver';
import {connectedControllerDevice} from '../controller/usbDriver';
import {MotorCommand} from './MotorCommand';
import {MotorResponse} from './MotorResponse';

export const connectedMotorDevice = ref<IMotorManager | null>(null);
let connectedPort: SerialPort | null = null;

export async function connectSerialDevice(verbose: boolean = false): Promise<IMotorManager | null> {
	const port = await navigator.serial.requestPort();

	return new Promise((resolve, reject) => {
		const controllerSerialInterface = new SerialInterface<
			ControllerSerialCommand,
			ControllerSerialResponse
		>(port, new ControllerSerialParser(), 115200, verbose);

		controllerSerialInterface.open
			.then(() => {
				const controllerSerialEncapsulator = new ControllerSerialEncapsulator(
					controllerSerialInterface
				);
				const controllerEncapsulator = new ControllerEncapsulator(controllerSerialEncapsulator);

				const gimbal = new Gimbal(
					new ControllerSerialEncapsulator(controllerSerialInterface),
					controllerEncapsulator
				);
				return gimbal.request(new DiscoveryCommand()).then((res) => {
					if (res instanceof DiscoveryResponse) {
						// Detected controller over serial
						connectedPort = port;
						connectedControllerDevice.value = gimbal;

						resolve(gimbal);
						return true;
					} else {
						connectedPort = port;
						const serialInterface = new SerialInterface<MotorCommand, MotorResponse>(
							port,
							new MotorParser(),
							115200,
							verbose
						);
						// Port already open, no need to re-open
						const manager = new MotorManager(serialInterface);
						connectedMotorDevice.value = manager;

						resolve(manager);
						return false;
					}
				});
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

export function disconnectSerialDevice(device: IMotorManager): void {
	device.close();

	connectedMotorDevice.value = connectedPort = null;
}

if ('serial' in navigator) {
	navigator.serial.addEventListener('disconnect', (e: Event): void => {
		if (connectedPort === e.target) {
			connectedMotorDevice.value = connectedPort = null;
		}
	});
}
