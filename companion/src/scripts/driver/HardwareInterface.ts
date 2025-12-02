import {Message} from './Message';

export interface IHardwareInterface {
	send(message: Message, isochronous?: boolean): Promise<void>;

	request(message: Message): Promise<Message>;

	close(): Promise<void>;
}
