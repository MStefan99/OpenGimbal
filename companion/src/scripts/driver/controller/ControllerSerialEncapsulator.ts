import {
	IControllerInterface,
	IControllerSerialInterface,
	IMotorInterface
} from '../HardwareInterface';
import {MotorCommand} from '../motor/MotorCommand';
import {MotorResponse} from '../motor/MotorResponse';
import {ControllerSerialCommand} from './ControllerSerialCommand';
import {ControllerCommand, MotorPassthroughCommand} from './ControllerCommand';
import {ControllerResponse, MotorPassthroughResponse} from './ControllerResponse';

export class ControllerSerialEncapsulator implements IControllerInterface, IMotorInterface {
	_hardwareInterface: IControllerSerialInterface;

	constructor(hardwareInterface: IControllerSerialInterface) {
		this._hardwareInterface = hardwareInterface;
	}

	get open(): Promise<void> {
		return Promise.resolve();
	}

	send(command: MotorCommand | ControllerCommand): Promise<void> {
		if (command instanceof MotorCommand) {
			return this._hardwareInterface.send(
				new ControllerSerialCommand(new MotorPassthroughCommand(command))
			);
		} else {
			return this._hardwareInterface.send(new ControllerSerialCommand(command));
		}
	}

	async request(command: MotorCommand): Promise<MotorResponse>;
	async request(command: ControllerCommand): Promise<ControllerResponse>;
	async request(
		command: MotorCommand | ControllerCommand
	): Promise<MotorResponse | ControllerResponse> {
		if (command instanceof MotorCommand) {
			return (
				(
					await this._hardwareInterface.request(
						new ControllerSerialCommand(new MotorPassthroughCommand(command))
					)
				).controllerResponse as MotorPassthroughResponse
			).motorResponse;
		} else {
			return (await this._hardwareInterface.request(new ControllerSerialCommand(command)))
				.controllerResponse;
		}
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
