import {IUSBInterface} from './usb/USBInterface';
import {IMotorControl, IMotorManager, MotorManager} from './MotorManager';
import {USBSerialEncapsulator} from './usb/USBSerialEncapsulator';
import {EnableCommand, GimbalCommand, DisableCommand} from './usb/GimbalCommand';
import {GimbalResponse} from './usb/GimbalResponse';

export interface IGimbal extends IMotorManager {
	get manufacturerName(): string;

	get productName(): string;

	send(command: GimbalCommand): Promise<void>;

	request(command: GimbalCommand): Promise<GimbalResponse | null>;

	enable(): Promise<void>;

	disable(): Promise<void>;
}

export class Gimbal implements IGimbal {
	_hardwareInterface: IUSBInterface;
	motorManager: MotorManager;

	constructor(hardwareInterface: IUSBInterface, encapsulator: USBSerialEncapsulator) {
		this._hardwareInterface = hardwareInterface;
		this.motorManager = new MotorManager(encapsulator);
	}

	get manufacturerName(): string {
		return this._hardwareInterface.manufacturerName;
	}

	get productName(): string {
		return this._hardwareInterface.productName;
	}

	get motors(): IMotorControl {
		return this.motorManager.motors;
	}

	send(command: GimbalCommand): Promise<void> {
		return this._hardwareInterface.send(command);
	}

	request(command: GimbalCommand): Promise<GimbalResponse | null> {
		return this._hardwareInterface.request(command) as Promise<GimbalResponse | null>;
	}

	enable(): Promise<void> {
		return this._hardwareInterface.send(new EnableCommand());
	}

	disable(): Promise<void> {
		return this._hardwareInterface.send(new DisableCommand());
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
