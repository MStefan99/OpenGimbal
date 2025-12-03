import {Message} from '../Message';
import {ControllerResponseType} from './ControllerMessage';
import {
	ControllerResponse,
	controllerResponseNames,
	controllerResponses,
	ReturnVariableResponse,
	returnVariableResponses
} from './ControllerResponse';

export class ControllerSerialResponse extends Message {
	constructor(buffer: Uint8Array);
	constructor(type: ControllerResponse);

	constructor(buffer: Uint8Array | ControllerResponse) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			super(new Uint8Array(1 + (buffer.length ?? 0)));

			this.view.setUint8(0, buffer.length);
			buffer.length && this.buffer.set(buffer.buffer, 1);
		}
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	get controllerResponse(): ControllerResponse {
		const response = new ControllerResponse(
			new Uint8Array(this.length - 1).fill(0).map((v, i) => this.buffer[i + 1])
		);

		const controllerResponse = controllerResponses[response.type](response.buffer);

		if (controllerResponse instanceof ReturnVariableResponse) {
			return returnVariableResponses[controllerResponse.variable](controllerResponse.buffer);
		} else {
			return controllerResponse;
		}
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return (
				`${controllerResponseNames[this.view.getUint8(1) as ControllerResponseType]} response:` +
				`\n${this.controllerResponse.toString()}`
			);
		}
	}
}
