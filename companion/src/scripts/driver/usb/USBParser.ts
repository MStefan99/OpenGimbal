import {GimbalCommand, GimbalCommandType, MotorPassthroughCommand} from './GimbalCommand';
import {GimbalResponse, GimbalResponseType, MotorPassthroughResponse} from './GimbalResponse';

export interface IUSBParser {
	parseCommand(data: Uint8Array): Array<GimbalCommand>;
	parseResponse(data: Uint8Array): Array<GimbalResponse>;
}

export class USBParser implements IUSBParser {
	parseCommand(data: Uint8Array): Array<GimbalCommand> {
		const commands = new Array<GimbalCommand>();

		if (!data.length) {
			return commands;
		}

		const command = new GimbalCommand(data);

		if (command.type === GimbalCommandType.MotorPassthrough) {
			const passthroughCommand = new MotorPassthroughCommand(data);
			commands.push(passthroughCommand);
		} else {
			commands.push(command);
		}

		return commands;
	}

	parseResponse(data: Uint8Array): Array<GimbalResponse> {
		const responses = new Array<GimbalResponse>();

		if (!data.length) {
			return responses;
		}

		const response = new GimbalResponse(data);

		if (response.type === GimbalResponseType.MotorPassthrough) {
			const passthroughResponse = new MotorPassthroughResponse(data);
			responses.push(passthroughResponse);
		} else {
			responses.push(response);
		}

		return responses;
	}
}
