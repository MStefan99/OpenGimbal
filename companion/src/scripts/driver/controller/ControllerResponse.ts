import {
	GimbalMode,
	GimbalResponseType,
	GimbalVariable,
	ControllerMessage
} from './ControllerMessage';
import {MotorCommand} from '../motor/MotorCommand';
import {exposeSerialMessage} from './ControllerUSBEncapsulator';
import {MotorResponse} from '../motor/MotorResponse';
import {RAD_TO_COUNTS} from '../../types';
import {scale} from '../../util';

export const gimbalCommandNames: Record<GimbalResponseType, string> = {
	[GimbalResponseType.ReturnVariable]: 'Return variable',
	[GimbalResponseType.MotorPassthrough]: 'Motor passthrough'
};

export const gimbalResponses: Record<
	GimbalResponseType,
	(buffer: Uint8Array) => ControllerResponse
> = {
	[GimbalResponseType.ReturnVariable]: (buffer: Uint8Array) => new ReturnVariableResponse(buffer),
	[GimbalResponseType.MotorPassthrough]: (buffer: Uint8Array) =>
		new MotorPassthroughResponse(buffer)
};

export const returnVariableResponses: Record<
	GimbalVariable,
	(buffer: Uint8Array) => ReturnVariableResponse
> = {
	[GimbalVariable.Orientation]: (buffer) => new ReturnOrientationVariableResponse(buffer),
	[GimbalVariable.HandleOrientation]: (buffer) => new ReturnOrientationVariableResponse(buffer),
	[GimbalVariable.Mode]: (buffer) => new ReturnModeVariableResponse(buffer),
	[GimbalVariable.BatteryVoltage]: (buffer) => new ReturnBatteryVoltageVariableResponse(buffer)
};

export class ControllerResponse extends ControllerMessage {
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
			return `${gimbalCommandNames[this.view.getUint8(0) as GimbalResponseType]} command`;
		}
	}
}

export class ReturnVariableResponse extends ControllerResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get variable(): GimbalVariable {
		return this.view.getUint8(1);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${GimbalVariable[this.variable] ?? 'unknown'}`;
		}
	}
}

export class ReturnOrientationVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get yaw(): number {
		return this.view.getInt16(2, false) / RAD_TO_COUNTS;
	}

	get pitch(): number {
		return this.view.getInt16(4, false) / RAD_TO_COUNTS;
	}

	get roll(): number {
		return this.view.getInt16(6, false) / RAD_TO_COUNTS;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  Yaw: ${this.yaw}` +
				`\n  Pitch: ${this.pitch}` +
				`\n  Roll: ${this.roll}`
			);
		}
	}
}

export class ReturnModeVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get mode(): GimbalMode {
		return this.view.getUint8(2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Mode: ${GimbalMode[this.mode]}`;
		}
	}
}

export class ReturnBatteryVoltageVariableResponse extends ReturnVariableResponse {
	get voltage(): number {
		return scale(this.view.getUint8(2), 0, 255, 3, 4.2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Voltage: ${this.voltage}`;
		}
	}
}

export class MotorPassthroughResponse extends ControllerResponse {
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
