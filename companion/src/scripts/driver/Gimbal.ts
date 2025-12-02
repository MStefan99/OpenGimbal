import {IUSBInterface} from './controller/USBInterface';
import {IMotorControl, IMotorManager, MotorManager} from './MotorManager';
import {ControllerUSBEncapsulator} from './controller/ControllerUSBEncapsulator';
import {
	ControllerResponse,
	ReturnBatteryVoltageVariableResponse,
	ReturnModeVariableResponse,
	ReturnOrientationVariableResponse
} from './controller/ControllerResponse';
import {
	DisableCommand,
	EnableCommand,
	GetVariableCommand,
	ControllerCommand,
	SetModeVariableCommand,
	SetOrientationVariableCommand
} from './controller/ControllerCommand';
import {GimbalMode, GimbalVariable} from './controller/ControllerMessage';

export type Orientation = {
	yaw: number;
	pitch: number;
	roll: number;
};

export interface IGimbal extends IMotorManager {
	readonly usbVersionMajor: number;
	readonly usbVersionMinor: number;
	readonly usbVersionSubminor: number;
	readonly deviceClass: number;
	readonly deviceSubclass: number;
	readonly deviceProtocol: number;
	readonly vendorId: number;
	readonly productId: number;
	readonly deviceVersionMajor: number;
	readonly deviceVersionMinor: number;
	readonly deviceVersionSubminor: number;

	send(command: ControllerCommand): Promise<void>;

	request(command: ControllerCommand): Promise<ControllerResponse | null>;

	enable(): Promise<void>;

	disable(): Promise<void>;
}

export class Gimbal implements IGimbal {
	_hardwareInterface: IUSBInterface;
	motorManager: MotorManager;

	constructor(hardwareInterface: IUSBInterface, encapsulator: ControllerUSBEncapsulator) {
		this._hardwareInterface = hardwareInterface;
		this.motorManager = new MotorManager(encapsulator);
	}

	get usbVersionMajor(): number {
		return this._hardwareInterface.usbVersionMajor;
	}

	get usbVersionMinor(): number {
		return this._hardwareInterface.usbVersionMinor;
	}

	get usbVersionSubminor(): number {
		return this._hardwareInterface.usbVersionSubminor;
	}

	get deviceClass(): number {
		return this._hardwareInterface.deviceClass;
	}

	get deviceSubclass(): number {
		return this._hardwareInterface.deviceSubclass;
	}

	get deviceProtocol(): number {
		return this._hardwareInterface.deviceProtocol;
	}

	get vendorId(): number {
		return this._hardwareInterface.vendorId;
	}

	get productId(): number {
		return this._hardwareInterface.productId;
	}

	get deviceVersionMajor(): number {
		return this._hardwareInterface.deviceVersionMajor;
	}

	get deviceVersionMinor(): number {
		return this._hardwareInterface.deviceVersionMinor;
	}

	get deviceVersionSubminor(): number {
		return this._hardwareInterface.deviceVersionSubminor;
	}

	get manufacturerName(): string | undefined {
		return this._hardwareInterface.manufacturerName;
	}

	get productName(): string | undefined {
		return this._hardwareInterface.productName;
	}

	get serialNumber(): string | undefined {
		return this._hardwareInterface.serialNumber;
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

	enable(): Promise<void> {
		return this._hardwareInterface.send(new EnableCommand());
	}

	disable(): Promise<void> {
		return this._hardwareInterface.send(new DisableCommand());
	}

	setOrientation(yaw: number, pitch: number, roll: number): Promise<void> {
		return this._hardwareInterface.send(new SetOrientationVariableCommand(yaw, pitch, roll));
	}

	async getOrientation(): Promise<Orientation> {
		const response = (await this._hardwareInterface.request(
			new GetVariableCommand(GimbalVariable.Orientation)
		)) as ReturnOrientationVariableResponse;

		return {
			yaw: response.yaw,
			pitch: response.pitch,
			roll: response.roll
		};
	}

	async getHandleOrientation(): Promise<Orientation> {
		const response = (await this._hardwareInterface.request(
			new GetVariableCommand(GimbalVariable.HandleOrientation)
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
				new GetVariableCommand(GimbalVariable.BatteryVoltage)
			)) as ReturnBatteryVoltageVariableResponse
		).voltage;
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
