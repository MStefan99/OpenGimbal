import {SerialMessage} from './serial/SerialMessage';
import {Parser} from './Parser';
import {MotorResponse} from './serial/MotorResponse';

export abstract class HardwareInterface {
	constructor(parser: Parser, verbose: boolean) {}

	abstract send(command: SerialMessage): Promise<void>;

	abstract request<T extends MotorResponse>(command: SerialMessage): Promise<T>;

	abstract close(): Promise<void>;
}
