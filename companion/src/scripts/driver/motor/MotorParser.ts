import {motorCommands, setVariableCommands, MotorCommand, SetVariableCommand} from './MotorCommand';
import {
	motorResponses,
	returnVariableResponses,
	MotorResponse,
	ReturnVariableResponse
} from './MotorResponse';
import {MotorMessage} from './MotorMessage';
import {IParser} from '../Parser';

export interface ISerialParser extends IParser {
	parse(data: Uint8Array): MotorMessage | null;
}

export class MotorParser implements ISerialParser {
	parse(data: Uint8Array): MotorMessage | null {
		let bytesRemaining = 0;
		let bytesProcessed = 0;
		const view = new DataView(data.buffer);

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
					const command = motorCommands[genericCommand.type](buffer);

					if (command instanceof SetVariableCommand) {
						return setVariableCommands[command.variable](buffer);
					} else {
						return command;
					}
				} else {
					const genericResponse = new MotorResponse(buffer);
					const response = motorResponses[genericResponse.type](buffer);

					if (response instanceof ReturnVariableResponse) {
						return returnVariableResponses[response.variable](buffer);
					} else {
						return response;
					}
				}
			}
		}

		return null;
	}
}
