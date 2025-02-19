import {Message} from './Message';

export interface IHardwareInterface {
	send(command: Message): Promise<void>;

	request(command: Message): Promise<Message>;

	close(): Promise<void>;
}
