import {GimbalCommand, GimbalCommandType, MotorPassthroughCommand} from './GimbalCommand';
import {GimbalResponse, GimbalResponseType, MotorPassthroughResponse} from './GimbalResponse';

export interface IUSBParser {
	parseCommand(data: Uint8Array): GimbalCommand | null;

	parseResponse(data: Uint8Array): GimbalResponse | null;
}

export class USBParser implements IUSBParser {
	parseCommand(data: Uint8Array): GimbalCommand | null {
		if (!data.length) {
			return null;
		}

		const command = new GimbalCommand(data);

		if (command.type === GimbalCommandType.MotorPassthrough) {
			return new MotorPassthroughCommand(data);
		} else {
			return command;
		}
	}

	parseResponse(data: Uint8Array): GimbalResponse | null {
		if (!data.length) {
			return null;
		}

		const response = new GimbalResponse(data);

		if (response.type === GimbalResponseType.MotorPassthrough) {
			return new MotorPassthroughResponse(data);
		} else {
			return response;
		}
	}
}
