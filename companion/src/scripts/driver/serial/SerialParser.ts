import {
	getMotorCommand,
	getVariableCommand,
	MotorCommand,
	MotorCommandType,
	SetVariableCommand
} from './MotorCommand';
import {
	getMotorResponse,
	getVariableResponse,
	MotorResponse,
	ReturnVariableResponse
} from './MotorResponse';
import {SerialMessage} from './SerialMessage';
import {IParser} from '../Parser';

export interface ISerialParser extends IParser {
	parse(data: Uint8Array): Array<SerialMessage>;
}

export class SerialParser implements ISerialParser {
	parse(data: Uint8Array): Array<SerialMessage> {
		let bytesRemaining = 0;
		let bytesProcessed = 0;
		const view = new DataView(data.buffer);
		const responses = new Array<SerialMessage>();

		for (const byte of data) {
			if (!bytesRemaining) {
				bytesRemaining = (byte >> 4) + 1;
				bytesProcessed = 0;
			}

			view.setUint8(bytesProcessed++, byte);
			if (--bytesRemaining === 0) {
				const buffer = new Uint8Array(bytesProcessed).fill(0).map((v, i) => data[i]);

				if ((view.getUint8(0) & 0xf) !== 0) {
					const genericCommand = new MotorCommand(buffer);
					const command = getMotorCommand[genericCommand.type](buffer);

					if (command instanceof SetVariableCommand) {
						responses.push(getVariableCommand[command.variableID](buffer));
					} else {
						responses.push(command);
					}
				} else {
					const genericResponse = new MotorResponse(buffer);
					const response = getMotorResponse[genericResponse.type](buffer);

					if (response instanceof ReturnVariableResponse) {
						responses.push(getVariableResponse[response.variableID](buffer));
					} else {
						responses.push(response);
					}
				}
			}
		}

		return responses;
	}
}
