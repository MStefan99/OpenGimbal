import {USBInterface} from './usb/USBInterface';

export class Gimbal {
	_hardwareInterface: USBInterface;

	constructor(hardwareInterface: USBInterface) {
		this._hardwareInterface = hardwareInterface;
	}
}
