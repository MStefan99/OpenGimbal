import {Message} from './Message';

export interface IHardwareInterface {
	send(message: Message): Promise<void>;

	request(message: Message): Promise<Message>;

	close(): Promise<void>;
}
