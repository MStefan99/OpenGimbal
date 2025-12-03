import {IMotorControl, IMotorManager, MotorManager} from './MotorManager';
import {
	ControllerResponse,
	ReturnBatteryVoltageVariableResponse,
	ReturnBCDVariableResponse,
	ReturnModeVariableResponse,
	ReturnOrientationVariableResponse,
	ReturnStringVariableResponse
} from './controller/ControllerResponse';
import {
	ControllerCommand,
	GetVariableCommand,
	SetModeVariableCommand,
	SetOrientationVariableCommand,
	SleepCommand,
	WakeCommand
} from './controller/ControllerCommand';
import {ControllerVariable, GimbalMode} from './controller/ControllerMessage';
import {IHardwareInterface} from './HardwareInterface';
import {ISerialInterface} from './motor/SerialInterface';
import {MotorMessage} from './motor/MotorMessage';
import {USBInterface} from './controller/USBInterface';
import {IMotor} from './Motor';

export type Orientation = {
	yaw: number;
	pitch: number;
	roll: number;
};

export interface IGimbal extends IMotorManager {
	productVersion: string;
	vendorName: string;
	productName: string;
	serialNumber: string;
	usbVersion: string | null;
	connectedOver: 'usb' | 'serial';

	enumerate(): Promise<IMotor[] | void>;

	send(command: ControllerCommand): Promise<void>;

	request(command: ControllerCommand): Promise<ControllerResponse | null>;

	sleep(): Promise<void>;

	wake(): Promise<void>;
}

export class Gimbal implements IGimbal {
	_hardwareInterface: IHardwareInterface;
	motorManager: MotorManager;

	_productVersion: string;
	_vendorName: string;
	_productName: string;
	_serialNumber: string;
	_usbVersion: string;
	_connectedOver: IGimbal['connectedOver'] = 'serial';

	constructor(
		hardwareInterface: IHardwareInterface,
		encapsulator: ISerialInterface<MotorMessage, MotorMessage>
	) {
		this._hardwareInterface = hardwareInterface;
		this.motorManager = new MotorManager(encapsulator);

		if (this._hardwareInterface instanceof USBInterface) {
			this._productVersion = `${this._hardwareInterface.deviceVersionMajor}.${this._hardwareInterface.deviceVersionMinor}.${this._hardwareInterface.deviceVersionSubminor}`;
			this._vendorName = this._hardwareInterface.manufacturerName;
			this._productName = this._hardwareInterface.productName;
			this._serialNumber = this._hardwareInterface.serialNumber;
			this._usbVersion = `${this._hardwareInterface.usbVersionMajor}.${this._hardwareInterface.usbVersionMinor}.${this._hardwareInterface.usbVersionSubminor}`;
			this._connectedOver = 'usb';
		}
	}

	enumerate(): Promise<IMotor[] | void> {
		const stringPromise =
			this._hardwareInterface instanceof USBInterface
				? Promise.resolve()
				: this.request(new GetVariableCommand(ControllerVariable.DeviceVersion))
						.then((res) => (this._productVersion = (res as ReturnBCDVariableResponse).value))
						.then(() => this.request(new GetVariableCommand(ControllerVariable.VendorName)))
						.then((res) => (this._vendorName = (res as ReturnStringVariableResponse).string))
						.then(() => this.request(new GetVariableCommand(ControllerVariable.ProductName)))
						.then((res) => (this._productName = (res as ReturnStringVariableResponse).string))
						.then(() => this.request(new GetVariableCommand(ControllerVariable.SerialNumber)))
						.then((res) => (this._serialNumber = (res as ReturnStringVariableResponse).string));

		return stringPromise.then(() => this.motors.enumerate());
	}

	get productVersion(): string {
		return this._productVersion;
	}

	get vendorName(): string {
		return this._vendorName;
	}

	get productName(): string {
		return this._productName;
	}

	get serialNumber(): string {
		return this._serialNumber;
	}

	get usbVersion(): string {
		return this._usbVersion;
	}

	get connectedOver(): IGimbal['connectedOver'] {
		return this._connectedOver;
	}

	get motors(): IMotorControl {
		return this.motorManager.motors;
	}

	send(command: ControllerCommand): Promise<void> {
		return this._hardwareInterface.send(command);
	}

	request(command: ControllerCommand): Promise<ControllerResponse | null> {
		return this._hardwareInterface.request(command) as Promise<ControllerResponse | null>;
	}

	sleep(): Promise<void> {
		return this._hardwareInterface.send(new SleepCommand());
	}

	wake(): Promise<void> {
		return this._hardwareInterface.send(new WakeCommand());
	}

	setOrientation(yaw: number, pitch: number, roll: number): Promise<void> {
		return this._hardwareInterface.send(new SetOrientationVariableCommand(yaw, pitch, roll));
	}

	async getOrientation(): Promise<Orientation> {
		const response = (await this._hardwareInterface.request(
			new GetVariableCommand(ControllerVariable.Orientation)
		)) as ReturnOrientationVariableResponse;

		return {
			yaw: response.yaw,
			pitch: response.pitch,
			roll: response.roll
		};
	}

	async getHandleOrientation(): Promise<Orientation> {
		const response = (await this._hardwareInterface.request(
			new GetVariableCommand(ControllerVariable.HandleOrientation)
		)) as ReturnOrientationVariableResponse;

		return {
			yaw: response.yaw,
			pitch: response.pitch,
			roll: response.roll
		};
	}

	setMode(mode: GimbalMode): Promise<void> {
		return this._hardwareInterface.send(new SetModeVariableCommand(mode));
	}

	async getMode(mode: GimbalMode): Promise<number> {
		return (
			(await this._hardwareInterface.request(
				new SetModeVariableCommand(mode)
			)) as ReturnModeVariableResponse
		).mode;
	}

	async getVoltage(): Promise<number> {
		return (
			(await this._hardwareInterface.request(
				new GetVariableCommand(ControllerVariable.BatteryVoltage)
			)) as ReturnBatteryVoltageVariableResponse
		).voltage;
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
