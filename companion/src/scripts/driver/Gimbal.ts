import {IUSBInterface} from './usb/USBInterface';
import {IMotorControl, IMotorManager, MotorManager} from './MotorManager';
import {USBSerialEncapsulator} from './usb/USBSerialEncapsulator';
import {GimbalResponse} from './usb/GimbalResponse';
import {
	DisableCommand,
	EnableCommand,
	GimbalCommand,
	SetModeVariableCommand,
	SetOrientationVariableCommand
} from './usb/GimbalCommand';
import {GimbalMode} from './usb/USBMessage';

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

	setOrientation(yaw: number, pitch: number, roll: number): Promise<void> {
		return this._hardwareInterface.send(new SetOrientationVariableCommand(yaw, pitch, roll));
	}

	setMode(mode: GimbalMode): Promise<void> {
		return this._hardwareInterface.send(new SetModeVariableCommand(mode));
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
