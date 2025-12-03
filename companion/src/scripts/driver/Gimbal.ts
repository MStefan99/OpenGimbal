import {IUSBInterface} from './controller/USBInterface';
import {IMotorControl, IMotorManager, MotorManager} from './MotorManager';
import {ControllerEncapsulator} from './controller/ControllerEncapsulator';
import {
	ControllerResponse,
	ReturnBatteryVoltageVariableResponse,
	ReturnModeVariableResponse,
	ReturnOrientationVariableResponse
} from './controller/ControllerResponse';
import {
	SleepCommand,
	WakeCommand,
	GetVariableCommand,
	ControllerCommand,
	SetModeVariableCommand,
	SetOrientationVariableCommand
} from './controller/ControllerCommand';
import {GimbalMode, ControllerVariable} from './controller/ControllerMessage';
import {IHardwareInterface} from './HardwareInterface';
import {ISerialInterface} from './motor/SerialInterface';
import {MotorMessage} from './motor/MotorMessage';
import {Message} from './Message';

export type Orientation = {
	yaw: number;
	pitch: number;
	roll: number;
};

export interface IGimbal extends IMotorManager {
	send(command: ControllerCommand): Promise<void>;

	request(command: ControllerCommand): Promise<ControllerResponse | null>;

	sleep(): Promise<void>;

	wake(): Promise<void>;
}

export class Gimbal implements IGimbal {
	_hardwareInterface: IHardwareInterface;
	motorManager: MotorManager;

	constructor(
		hardwareInterface: IHardwareInterface,
		encapsulator: ISerialInterface<MotorMessage, MotorMessage>
	) {
		this._hardwareInterface = hardwareInterface;
		this.motorManager = new MotorManager(encapsulator);
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
