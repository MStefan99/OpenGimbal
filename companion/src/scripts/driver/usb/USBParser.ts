import {
	GimbalCommand,
	gimbalCommands,
	MotorPassthroughCommand,
	SetVariableCommand,
	setVariableCommands
} from './GimbalCommand';
import {
	gimbalResponses,
	GimbalResponse,
	ReturnVariableResponse,
	returnVariableResponses
} from './GimbalResponse';

export interface IUSBParser {
	parseCommand(data: Uint8Array): GimbalCommand | null;

	parseResponse(data: Uint8Array): GimbalResponse | null;
}

export class USBParser implements IUSBParser {
	parseCommand(data: Uint8Array): GimbalCommand | null {
		if (!data.length) {
			return null;
		}

		const genericCommand = new GimbalCommand(data);
		const command = gimbalCommands[genericCommand.type](data);

		if (command instanceof SetVariableCommand) {
			return setVariableCommands[command.variable](data);
		} else {
			return command;
		}
	}

	parseResponse(data: Uint8Array): GimbalResponse | null {
		if (!data.length) {
			return null;
		}

		const genericResponse = new GimbalResponse(data);
		const response = gimbalResponses[genericResponse.type](data);

		if (response instanceof ReturnVariableResponse) {
			return returnVariableResponses[response.variable](data);
		} else {
			return response;
		}
	}
}
