import {IParser} from '../Parser';
import {USBMessage} from './USBMessage';
import {GimbalCommand, GimbalCommandType, MotorPassthroughCommand} from './GimbalCommand';

export interface IUSBParser extends IParser {
	parse(data: Uint8Array): Array<USBMessage>;
}

export class USBParser implements IUSBParser {
	parse(data: Uint8Array): Array<USBMessage> {
		const responses = new Array<USBMessage>();

		if (!data.length) {
			return responses;
		}

		const genericCommand = new GimbalCommand(data);

		if (genericCommand.type === GimbalCommandType.MotorPassthrough) {
			const passthroughCommand = new MotorPassthroughCommand(data);
			responses.push(passthroughCommand);
		} else {
			responses.push(genericCommand);
		}

		return responses;
	}
}
