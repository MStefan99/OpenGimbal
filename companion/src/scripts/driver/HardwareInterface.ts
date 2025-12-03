import {Message} from './Message';
import {ControllerCommand} from './controller/ControllerCommand';
import {ControllerResponse} from './controller/ControllerResponse';
import {MotorCommand} from './motor/MotorCommand';
import {MotorResponse} from './motor/MotorResponse';
import {ControllerSerialCommand} from './controller/ControllerSerialCommand';
import {ControllerSerialResponse} from './controller/ControllerSerialResponse';

export interface IHardwareInterface {
	send(message: Message, isochronous?: boolean): Promise<void>;

	request(message: Message): Promise<Message>;

	close(): Promise<void>;
}

export interface IMotorInterface {
	send(command: MotorCommand, isochronous?: boolean): Promise<void>;

	request(command: MotorCommand): Promise<MotorResponse>;

	close(): Promise<void>;
}

export interface IControllerInterface extends IHardwareInterface {
	send(command: ControllerCommand, isochronous?: boolean): Promise<void>;

	request(command: ControllerCommand): Promise<ControllerResponse>;

	close(): Promise<void>;
}

export interface IControllerSerialInterface extends IHardwareInterface {
	send(command: ControllerSerialCommand, isochronous?: boolean): Promise<void>;

	request(command: ControllerSerialCommand): Promise<ControllerSerialResponse>;

	close(): Promise<void>;
}
