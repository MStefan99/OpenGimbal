import {controllerCommands, ControllerCommand} from './ControllerCommand';
import {ControllerParser, IControllerParser} from './ControllerParser';
import {ISerialParser} from '../motor/MotorParser';
import {ControllerMessage} from './ControllerMessage';
import {ControllerSerialCommand} from './ControllerSerialCommand';
import {MotorMessage} from '../motor/MotorMessage';
import {motorCommands, SetVariableCommand, setVariableCommands} from '../motor/MotorCommand';
import {
	motorResponses,
	ReturnVariableResponse,
	returnVariableResponses
} from '../motor/MotorResponse';
import {ControllerSerialResponse} from './ControllerSerialResponse';
import {Message} from '../Message';
import {IParser} from '../Parser';

export interface IControllerSerialParser {
	parseCommand(data: Uint8Array): ControllerSerialCommand | null;

	parseResponse(data: Uint8Array): ControllerSerialResponse | null;
}

export class ControllerSerialParser {
	parseCommand(data: Uint8Array): ControllerSerialCommand | null {
		return new ControllerSerialCommand(data);
	}

	parseResponse(data: Uint8Array): ControllerSerialResponse | null {
		return new ControllerSerialResponse(data);
	}
}
