import {Command} from "./Command";

export enum MotorResponseType {
	ReturnVariable = 0x0,
}

export const motorResponseNames: Record<MotorResponseType, string> = {
	[MotorResponseType.ReturnVariable]: 'Return variable',
}

export type motorResponseParsers = Record<MotorResponseType, (buffer: Uint8Array) => MotorResponse>;
export const motorResponseParsers: Record<MotorResponseType, (buffer: Uint8Array) => MotorResponse> = {
	[MotorResponseType.ReturnVariable]: (buffer => new MotorResponse(buffer))
}

export class MotorResponse extends Command {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx)
					.toString(16)
					.padStart(2, '0'))
				.join(' ');
		} else {
			return `${motorResponseNames[(this.view.getUint8(1) & 0xf) as MotorResponseType]} command`
				+ `\n  Source address: ${this.view.getUint8(1) >> 4}`
				+ `\n  Destination address: ${this.view.getUint8(0) & 0xf}`
		}
	}
}
