import {Parser} from '../Parser';
import {USBMessage} from './USBMessage';

export class USBParser extends Parser {
	parse(data: Uint8Array): Array<USBMessage> {
		const responses = new Array<USBMessage>();

		return responses;
	}
}
