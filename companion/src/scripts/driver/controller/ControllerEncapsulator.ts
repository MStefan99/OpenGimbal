import {MotorCommand} from '../motor/MotorCommand';
import {MotorResponse} from '../motor/MotorResponse';
import {IControllerInterface, IMotorInterface} from '../HardwareInterface';
import {MotorPassthroughCommand} from './ControllerCommand';
import {MotorPassthroughResponse} from './ControllerResponse';

export class ControllerEncapsulator implements IMotorInterface {
	_hardwareInterface: IControllerInterface;

	constructor(hardwareInterface: IControllerInterface) {
		this._hardwareInterface = hardwareInterface;
	}

	get open(): Promise<void> {
		return Promise.resolve();
	}

	send(command: MotorCommand): Promise<void> {
		return this._hardwareInterface.send(new MotorPassthroughCommand(command));
	}

	async request(command: MotorCommand): Promise<MotorResponse> {
		return (
			(await this._hardwareInterface.request(
				new MotorPassthroughCommand(command)
			)) as MotorPassthroughResponse
		).motorResponse;
	}

	close(): Promise<void> {
		return this._hardwareInterface.close();
	}
}
