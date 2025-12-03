import {
	ControllerCommand,
	controllerCommands,
	MotorPassthroughCommand,
	SetVariableCommand,
	setVariableCommands
} from './ControllerCommand';
import {
	controllerResponses,
	ControllerResponse,
	ReturnVariableResponse,
	returnVariableResponses
} from './ControllerResponse';
import {ControllerMessage} from './ControllerMessage';

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
		const command = controllerCommands[genericCommand.type](data);

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
		const response = controllerResponses[genericResponse.type](data);

		if (response instanceof ReturnVariableResponse) {
			return returnVariableResponses[response.variable](data);
		} else {
			return response;
		}
	}
}
