import {Message} from './Message';

export interface IParser {
	parse(data: Uint8Array): Message | null;
}
