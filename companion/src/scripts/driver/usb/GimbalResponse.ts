import {USBMessage} from './USBMessage';
import {MotorCommand} from '../serial/MotorCommand';
import {exposeSerialMessage} from './USBSerialEncapsulator';
import {SerialParser} from '../serial/SerialParser';
import {MotorResponse} from '../serial/MotorResponse';

export enum GimbalResponseType {
	MotorPassthrough = 0xf
}

export const gimbalCommandNames: Record<GimbalResponseType, string> = {
	[GimbalResponseType.MotorPassthrough]: 'Motor passthrough'
};

export const getGimbalResponse: Record<GimbalResponseType, (buffer: Uint8Array) => GimbalResponse> =
	{
		[GimbalResponseType.MotorPassthrough]: (buffer: Uint8Array) =>
			new MotorPassthroughResponse(buffer)
	};

export class GimbalResponse extends USBMessage {
	get type(): GimbalResponseType {
		return this.view.getUint8(0);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${gimbalCommandNames[(this.view.getUint8(1) & 0xf) as GimbalResponseType]} command`;
		}
	}
}

export class MotorPassthroughResponse extends GimbalResponse {
	constructor(buffer: Uint8Array);
	constructor(motorCommand: MotorCommand);

	constructor(srcAddr: Uint8Array | MotorCommand) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr.buffer);
		}
	}

	get motorResponse(): MotorResponse | null {
		const message = exposeSerialMessage(this);

		if (message === null) {
			return null;
		} else if (!(message instanceof MotorResponse)) {
			throw new Error('Could not parse motor passthrough response');
		}

		return message;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return 'Encapsulated motor response\n' + (this.motorResponse?.toString(type) ?? 'Empty');
		}
	}
}
