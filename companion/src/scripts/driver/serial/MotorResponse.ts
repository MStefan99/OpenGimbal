import {MotorResponseType, MotorVariable, SerialMessage} from './SerialMessage';

export const motorResponseNames: Record<MotorResponseType, string> = {
	[MotorResponseType.ReturnVariable]: 'Return variable'
};

export const motorResponses: Record<MotorResponseType, (buffer: Uint8Array) => MotorResponse> = {
	[MotorResponseType.ReturnVariable]: (buffer) => new ReturnVariableResponse(buffer)
};

export const returnVariableResponses: Record<
	MotorVariable,
	(buffer: Uint8Array) => ReturnVariableResponse
> = {
	[MotorVariable.Options]: (buffer) => new ReturnOptionsVariableResponse(buffer),
	[MotorVariable.Offset]: (buffer) => new ReturnOffsetVariableResponse(buffer),
	[MotorVariable.Position]: (buffer) => new ReturnPositionVariableResponse(buffer)
};

export class MotorResponse extends SerialMessage {
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
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return (
				`${motorResponseNames[(this.view.getUint8(1) & 0xf) as MotorResponseType]} response` +
				`\n  Source address: ${this.view.getUint8(1) >> 4}` +
				`\n  Destination address: ${this.view.getUint8(0) & 0xf}`
			);
		}
	}
}

export class ReturnVariableResponse extends MotorResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get variable(): MotorVariable {
		return this.view.getUint8(2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${MotorVariable[this.variable] ?? 'unknown'}`;
		}
	}
}

export class ReturnOptionsVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get calibrated(): boolean {
		return !!(this.view.getUint8(3) & 0x01);
	}

	get inverted(): boolean {
		return !!(this.view.getUint8(3) & 0x02);
	}

	get highVoltageCompatible(): boolean {
		return !!(this.view.getUint8(3) & 0x04);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  ${this.calibrated ? 'Calibrated' : 'Not calibrated'}` +
				`\n  ${this.inverted ? 'Direction inverted' : 'Direction not inverted'}` +
				`\n  High voltage ${this.highVoltageCompatible ? 'compatible' : 'not compatible'}`
			);
		}
	}
}

export class ReturnOffsetVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get offset(): number {
		return (this.view.getUint8(3) << 8) | this.view.getUint8(4);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Offset: ${this.offset}`;
		}
	}
}

export class ReturnPositionVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get position(): number {
		return (this.view.getUint8(3) << 8) | this.view.getUint8(4);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Position: ${this.position}`;
		}
	}
}

export class ReturnPowerVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get power(): number {
		return this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Power: ${this.power}`;
		}
	}
}
