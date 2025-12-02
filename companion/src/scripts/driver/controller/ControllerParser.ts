import {
	ControllerCommand,
	gimbalCommands,
	MotorPassthroughCommand,
	SetVariableCommand,
	setVariableCommands
} from './ControllerCommand';
import {
	gimbalResponses,
	ControllerResponse,
	ReturnVariableResponse,
	returnVariableResponses
} from './ControllerResponse';

export interface IControllerParser {
	parseCommand(data: Uint8Array): ControllerCommand | null;

	parseResponse(data: Uint8Array): ControllerResponse | null;
}

export class ControllerParser implements IControllerParser {
	parseCommand(data: Uint8Array): ControllerCommand | null {
		if (!data.length) {
			return null;
		}

		const genericCommand = new ControllerCommand(data);
		const command = gimbalCommands[genericCommand.type](data);

		if (command instanceof SetVariableCommand) {
			return setVariableCommands[command.variable](data);
		} else {
			return command;
		}
	}

	parseResponse(data: Uint8Array): ControllerResponse | null {
		if (!data.length) {
			return null;
		}

		const genericResponse = new ControllerResponse(data);
		const response = gimbalResponses[genericResponse.type](data);

		if (response instanceof ReturnVariableResponse) {
			return returnVariableResponses[response.variable](data);
		} else {
			return response;
		}
	}
}
