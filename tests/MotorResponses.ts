import {Command} from "./Command";
import {clamp} from "./util";
import {MotorCommand, MotorCommandType, MotorVariableID} from "./MotorCommands";

export enum MotorResponseType {
	ReturnVariable = 0x0,
}

export const motorResponseNames: Record<MotorResponseType, string> = {
	[MotorResponseType.ReturnVariable]: 'Return variable',
}


export class MotorResponse extends Command {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get type(): MotorResponseType {
		return this.view.getUint8(1) & 0xf;
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

export class ReturnVariableResponse extends MotorResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get variableID(): MotorVariableID {
		return this.view.getUint8(2) << 8;
	}

	override toString(type?: "hex"): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Variable: ${MotorVariableID[this.variableID]}`
		}
	}
}

export class ReturnCalibrationVariableResponse extends ReturnVariableResponse {
}

export class ReturnOffsetVariableResponse extends ReturnVariableResponse {
}

export class ReturnRangeVariableResponse extends ReturnVariableResponse {
}

export class ReturnErrorVariableResponse extends ReturnVariableResponse {
}
