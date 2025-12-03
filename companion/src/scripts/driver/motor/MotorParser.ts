import {motorCommands, setVariableCommands, SetVariableCommand} from './MotorCommand';
import {
	motorResponses,
	returnVariableResponses,
	ReturnVariableResponse,
	MotorResponse
} from './MotorResponse';
import {MotorMessage} from './MotorMessage';
import {IParser} from '../Parser';

export interface ISerialParser extends IParser {
	parse(data: Uint8Array): MotorMessage | null;
}

export class MotorParser implements ISerialParser {
	parse(data: Uint8Array): MotorMessage | null {
		if (!data.length) {
			return null;
		}

		const view = new DataView(data.buffer);

		if ((view.getUint8(0) & 0xf) !== 0) {
			const genericCommand = new MotorResponse(data);
			const command = motorCommands[genericCommand.type](data);

			if (command instanceof SetVariableCommand) {
				return setVariableCommands[command.variable](data);
			} else {
				return command;
			}
		} else {
			const genericResponse = new MotorResponse(data);
			const response = motorResponses[genericResponse.type](data);

			if (response instanceof ReturnVariableResponse) {
				return returnVariableResponses[response.variable](data);
			} else {
				return response;
			}
		}
	}
}
