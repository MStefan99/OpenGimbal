import {
	ControllerMessage,
	ControllerResponseType,
	ControllerVariable,
	GimbalMode
} from './ControllerMessage';
import {MotorResponse} from '../motor/MotorResponse';
import {RAD_TO_COUNTS} from '../../types';
import {scale} from '../../util';
import {MotorParser} from '../motor/MotorParser';

export const controllerResponseNames: Record<ControllerResponseType, string> = {
	[ControllerResponseType.MotorDiscovery]: 'Motor discovery',
	[ControllerResponseType.Discovery]: 'Discovery',
	[ControllerResponseType.ReturnVariable]: 'Return variable',
	[ControllerResponseType.MotorPassthrough]: 'Motor passthrough'
};

export const controllerResponses: Record<
	ControllerResponseType,
	(buffer: Uint8Array) => ControllerResponse
> = {
	[ControllerResponseType.MotorDiscovery]: (buffer: Uint8Array) =>
		new MotorDiscoveryResponse(buffer),
	[ControllerResponseType.Discovery]: (buffer: Uint8Array) => new DiscoveryResponse(buffer),
	[ControllerResponseType.ReturnVariable]: (buffer: Uint8Array) =>
		new ReturnVariableResponse(buffer),
	[ControllerResponseType.MotorPassthrough]: (buffer: Uint8Array) =>
		new MotorPassthroughResponse(buffer)
};

export const returnVariableResponses: Record<
	ControllerVariable,
	(buffer: Uint8Array) => ReturnVariableResponse
> = {
	[ControllerVariable.Orientation]: (buffer) => new ReturnOrientationVariableResponse(buffer),
	[ControllerVariable.HandleOrientation]: (buffer) => new ReturnOrientationVariableResponse(buffer),
	[ControllerVariable.Mode]: (buffer) => new ReturnModeVariableResponse(buffer),
	[ControllerVariable.BatteryVoltage]: (buffer) => new ReturnBatteryVoltageVariableResponse(buffer),
	[ControllerVariable.DeviceVersion]: (buffer) => new ReturnBCDVariableResponse(buffer),
	[ControllerVariable.VendorName]: (buffer) => new ReturnStringVariableResponse(buffer),
	[ControllerVariable.ProductName]: (buffer) => new ReturnStringVariableResponse(buffer),
	[ControllerVariable.SerialNumber]: (buffer) => new ReturnStringVariableResponse(buffer)
};

export class ControllerResponse extends ControllerMessage {
	get type(): ControllerResponseType {
		return this.view.getUint8(0);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${controllerResponseNames[this.view.getUint8(0) as ControllerResponseType]} response`;
		}
	}
}

export class MotorDiscoveryResponse extends ControllerResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}
}

export class DiscoveryResponse extends ControllerResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}
}

export class ReturnVariableResponse extends ControllerResponse {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get variable(): ControllerVariable {
		return this.view.getUint8(1);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${ControllerVariable[this.variable] ?? 'unknown'}`;
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

export class ReturnBCDVariableResponse extends ReturnVariableResponse {
	get value(): string {
		const bcd = this.view.getUint16(2, false);
		return `${bcd >> 8}.${bcd & 0xf0}.${bcd & 0x0f}`;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Value: ${this.value}`;
		}
	}
}

export class ReturnStringVariableResponse extends ReturnVariableResponse {
	get string(): string {
		const arr = new Array<number>((this.view.byteLength - 2) / 2)
			.fill(0)
			.map((v, i) => this.view.getUint16(2 + 2 * i, true));
		return arr.map((v) => String.fromCharCode(v)).join('');
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Value: ${this.string}`;
		}
	}
}

export class MotorPassthroughResponse extends ControllerResponse {
	constructor(buffer: Uint8Array);
	constructor(motorCommand: MotorResponse);

	constructor(srcAddr: Uint8Array | MotorResponse) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			const buffer = new Uint8Array(srcAddr.buffer.byteLength + 1);
			const view = new DataView(buffer.buffer);

			view.setUint8(0, ControllerResponseType.MotorPassthrough);
			buffer.set(srcAddr.buffer, 1);

			super(buffer);
		}
	}

	get motorResponse(): MotorResponse | null {
		if (this.type !== ControllerResponseType.MotorPassthrough) {
			throw new Error('Invalid response type');
		}

		const response = new MotorParser().parse(
			new Uint8Array(this.buffer.byteLength - 1).fill(0).map((v, i) => this.buffer[i + 1])
		);

		if (response === null) {
			return null;
		} else if (!(response instanceof MotorResponse)) {
			throw new Error('Could not parse motor passthrough response');
		}

		return response;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return 'Encapsulated motor response:\n' + (this.motorResponse?.toString(type) ?? 'Empty');
		}
	}
}
