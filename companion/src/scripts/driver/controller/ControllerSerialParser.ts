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
		let bytesRemaining = 0;
		let bytesProcessed = 0;
		const view = new DataView(data.buffer);

		for (const byte of data) {
			if (!bytesRemaining) {
				bytesRemaining = byte + 1;
				bytesProcessed = 0;
			}

			view.setUint8(bytesProcessed++, byte);
			if (--bytesRemaining === 0) {
				return new ControllerSerialCommand(
					new Uint8Array(bytesProcessed).fill(0).map((v, i) => data[i])
				);
			}
		}

		return null;
	}

	parseResponse(data: Uint8Array): ControllerSerialResponse | null {
		let bytesRemaining = 0;
		let bytesProcessed = 0;
		const view = new DataView(data.buffer);

		for (const byte of data) {
			if (!bytesRemaining) {
				bytesRemaining = byte + 1;
				bytesProcessed = 0;
			}

			view.setUint8(bytesProcessed++, byte);
			if (--bytesRemaining === 0) {
				return new ControllerSerialResponse(
					new Uint8Array(bytesProcessed).fill(0).map((v, i) => data[i])
				);
			}
		}

		return null;
	}
}
