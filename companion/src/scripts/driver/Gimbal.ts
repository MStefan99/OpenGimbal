import {IUSBInterface} from './usb/USBInterface';
import {IMotorManager, MotorManager} from './MotorManager';
import {USBSerialEncapsulator} from './usb/USBSerialEncapsulator';
import {CalibrationBits, Motor} from './Motor';
import {BitwiseRegister} from './BitwiseRegister';

export interface IGimbal extends IMotorManager {
	get manufacturerName(): string;

	get productName(): string;
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

	get active(): Motor[] {
		return this.motorManager.active;
	}

	get all(): Motor {
		return this.motorManager.all;
	}

	enumerate(): Promise<Motor[]> {
		return this.motorManager.enumerate();
	}

	getInitialCalibration(address: Motor['address']): BitwiseRegister<CalibrationBits> {
		return this.motorManager.getInitialCalibration(address);
	}

	motor(address: Motor['address']): Motor {
		return this.motorManager.motor(address);
	}

	get motors(): Motor[] {
		return this.motorManager.motors;
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}

	addEventListener(
		type: string,
		callback: EventListenerOrEventListenerObject | null,
		options?: AddEventListenerOptions | boolean
	): void {
		this.motorManager.addEventListener(type, callback, options);
	}

	dispatchEvent(event: Event): boolean {
		return this.motorManager.dispatchEvent(event);
	}

	removeEventListener(
		type: string,
		callback: EventListenerOrEventListenerObject | null,
		options?: EventListenerOptions | boolean
	): void {
		this.motorManager.removeEventListener(type, callback, options);
	}
}
