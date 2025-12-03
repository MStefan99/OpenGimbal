import {ControllerSerialCommand} from './ControllerSerialCommand';
import {ControllerSerialResponse} from './ControllerSerialResponse';

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
