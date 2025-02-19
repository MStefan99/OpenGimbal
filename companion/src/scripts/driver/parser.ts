import {Message} from './Message';

export abstract class Parser {
	abstract parse(data: Uint8Array): Array<Message>;
}
