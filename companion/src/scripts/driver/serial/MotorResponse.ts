import {SerialMessage} from './SerialMessage';
import {MotorVariableID} from './MotorCommand';
import {BitwiseRegister} from '../BitwiseRegister';
import {CalibrationBits} from '../Motor';

export enum MotorResponseType {
	ReturnVariable = 0xf
}

export const motorResponseNames: Record<MotorResponseType, string> = {
	[MotorResponseType.ReturnVariable]: 'Return variable'
};

export const motorResponses: Record<MotorResponseType, (buffer: Uint8Array) => MotorResponse> = {
	[MotorResponseType.ReturnVariable]: (buffer) => new ReturnVariableResponse(buffer)
};

export const variableResponses: Record<
	MotorVariableID,
	(buffer: Uint8Array) => ReturnVariableResponse
> = {
	[MotorVariableID.Calibration]: (buffer) => new ReturnCalibrationVariableResponse(buffer),
	[MotorVariableID.Offset]: (buffer) => new ReturnOffsetVariableResponse(buffer),
	[MotorVariableID.Position]: (buffer) => new ReturnPositionVariableResponse(buffer),
	[MotorVariableID.Power]: (buffer) => new ReturnPowerVariableResponse(buffer),
	[MotorVariableID.Speed]: (buffer) => new ReturnSpeedVariableResponse(buffer),
	[MotorVariableID.Error]: (buffer) => new ReturnErrorVariableResponse(buffer)
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

	get variableID(): MotorVariableID {
		return this.view.getUint8(2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${MotorVariableID[this.variableID] ?? 'unknown'}`;
		}
	}
}

export class ReturnCalibrationVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get calibrationMode(): BitwiseRegister<CalibrationBits> {
		return new BitwiseRegister<CalibrationBits>(this.view.getUint8(3));
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  Mode: ${
					Object.entries(CalibrationBits)
						.map((e) => ({bit: +e[0], name: e[1]}))
						.filter((e) => Number.isInteger(e.bit) && this.calibrationMode.has(e.bit))
						.map((e) => e.name)
						.join(', ') || 'Not calibrated'
				}`
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

export class ReturnSpeedVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get speed(): number {
		return this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Speed: ${this.speed}`;
		}
	}
}

export class ReturnErrorVariableResponse extends ReturnVariableResponse {}
