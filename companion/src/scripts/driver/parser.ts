import {
	getMotorCommand,
	getVariableCommand,
	MotorCommand,
	MotorCommandType,
	SetVariableCommand
} from './MotorCommand';
import {
	getMotorResponse,
	getVariableResponse,
	MotorResponse,
	ReturnVariableResponse
} from './MotorResponse';

export class Parser {
	_incomingBuffer = new Uint8Array(20);
	_incomingView = new DataView(this._incomingBuffer.buffer);
	_bytesReceived = 0;
	_bytesRemaining = 0;

	parse(data: Uint8Array): Array<MotorCommand | MotorResponse> {
		const responses = new Array<MotorCommand | MotorResponse>();

		for (const byte of data) {
			if (!this._bytesRemaining) {
				this._bytesRemaining = (byte >> 4) + 1;
				this._bytesReceived = 0;
			}

			this._incomingView.setUint8(this._bytesReceived++, byte);
			if (--this._bytesRemaining === 0) {
				if (this._incomingView.getUint8(0) >> 4 > 1) {
					if ((this._incomingView.getUint8(0) & 0xf) !== 0) {
						const genericCommand = new MotorCommand(this._incomingBuffer);

						if (genericCommand.type.toString() in getMotorCommand) {
							const command = getMotorCommand[genericCommand.type](this._incomingBuffer);

							if (command instanceof SetVariableCommand) {
								responses.push(getVariableCommand[command.variableID](this._incomingBuffer));
							}

							responses.push(command);
						}
					} else {
						const genericResponse = new MotorResponse(this._incomingBuffer);

						if (genericResponse.type.toString() in getMotorResponse) {
							const response = getMotorResponse[genericResponse.type](this._incomingBuffer);

							if (response instanceof ReturnVariableResponse) {
								responses.push(getVariableResponse[response.variableID](this._incomingBuffer));
							}

							responses.push(response);
						}
					}
				}
			}
		}

		return responses;
	}
}
