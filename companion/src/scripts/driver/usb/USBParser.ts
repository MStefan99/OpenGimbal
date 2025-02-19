import {IParser} from '../Parser';
import {USBMessage} from './USBMessage';

export interface IUSBParser extends IParser {
	parse(data: Uint8Array): Array<USBMessage>;
}

export class USBParser implements IUSBParser {
	parse(data: Uint8Array): Array<USBMessage> {
		const responses = new Array<USBMessage>();

		return responses;
	}
}
