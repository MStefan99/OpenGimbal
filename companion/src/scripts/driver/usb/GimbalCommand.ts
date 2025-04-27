import {USBMessage} from './USBMessage';
import {exposeSerialMessage} from './USBSerialEncapsulator';
import {MotorCommand} from '../serial/MotorCommand';
import {RAD_TO_COUNTS} from '../../types';
import {clamp} from '../../util';

export enum GimbalCommandType {
	Disable = 0x00,
	Enable = 0x01,
	SetVariable = 0xd,
	GetVariable = 0xe,
	MotorPassthrough = 0x0f
}

export enum GimbalVariable {
	Orientation = 0x00,
	HandleOrientation = 0x01,
	Mode = 0x02,
	BatteryVoltage = 0x03
}

export enum GimbalMode {
	Horizon = 0x00,
	Follow = 0x01,
	FPV = 0x02,
	Tilt = 0x03
}

export const gimbalCommandNames: Record<GimbalCommandType, string> = {
	[GimbalCommandType.Disable]: 'Disable',
	[GimbalCommandType.Enable]: 'Enable',
	[GimbalCommandType.GetVariable]: 'Get variable',
	[GimbalCommandType.SetVariable]: 'Set variable',
	[GimbalCommandType.MotorPassthrough]: 'Motor passthrough'
};

export const getGimbalCommand: Record<GimbalCommandType, (buffer: Uint8Array) => GimbalCommand> = {
	[GimbalCommandType.Disable]: (buffer: Uint8Array) => new DisableCommand(buffer),
	[GimbalCommandType.Enable]: (buffer: Uint8Array) => new EnableCommand(buffer),
	[GimbalCommandType.GetVariable]: (buffer: Uint8Array) => new GetVariableCommand(buffer),
	[GimbalCommandType.SetVariable]: (buffer: Uint8Array) => new SetVariableCommand(buffer),
	[GimbalCommandType.MotorPassthrough]: (buffer: Uint8Array) => new MotorPassthroughCommand(buffer)
};

export class GimbalCommand extends USBMessage {
	get type(): GimbalCommandType {
		return this.view.getUint8(0);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${gimbalCommandNames[this.view.getUint8(0) as GimbalCommandType]} command`;
		}
	}
}

export class DisableCommand extends GimbalCommand {
	constructor(buffer: Uint8Array);
	constructor();

	constructor(buffer?: Uint8Array) {
		if (buffer) {
			super(buffer);
		} else {
			super(GimbalCommandType.Disable);
		}
	}
}

export class EnableCommand extends GimbalCommand {
	constructor(buffer: Uint8Array);
	constructor();

	constructor(buffer?: Uint8Array) {
		if (buffer) {
			super(buffer);
		} else {
			super(GimbalCommandType.Enable);
		}
	}
}

export class GetVariableCommand extends GimbalCommand {
	constructor(buffer: Uint8Array);
	constructor(variable: GimbalVariable);

	constructor(variable: Uint8Array | GimbalVariable) {
		if (variable instanceof Uint8Array) {
			super(variable);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, variable);

			super(GimbalCommandType.GetVariable, buffer);
		}
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

export class SetVariableCommand extends GimbalCommand {
	constructor(buffer: Uint8Array);
	constructor(variable: GimbalVariable, value: Uint8Array);
	constructor(variable: GimbalVariable, value: number, length: number);

	constructor(variable: Uint8Array | GimbalVariable, value?: number | Uint8Array, length?: number) {
		if (variable instanceof Uint8Array) {
			super(variable);
		} else if (!(value instanceof Uint8Array)) {
			value = Math.round(value);
			length = Math.floor(clamp(length + 1, 0, 13));

			const buffer = new Uint8Array(length);
			const view = new DataView(buffer.buffer);

			view.setUint8(0, variable);
			for (let i = 1; i < length; ++i) {
				view.setUint8(length - i, value);
				value >>= 8;
			}

			super(GimbalCommandType.SetVariable, buffer);
		} else {
			const buffer = new Uint8Array(value.byteLength + 1);
			const view = new DataView(buffer.buffer);

			view.setUint8(0, variable);
			buffer.set(value, 1);

			super(GimbalCommandType.SetVariable, buffer);
		}
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

export class SetOrientationCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(yaw: number, pitch: number, roll: number);

	constructor(buffer: Uint8Array | number, pitch?: number, roll?: number) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			const buf = new Uint8Array(6);
			const view = new DataView(buf.buffer);

			view.setInt16(0, buffer * RAD_TO_COUNTS, false);
			view.setInt16(2, pitch * RAD_TO_COUNTS, false);
			view.setInt16(4, roll * RAD_TO_COUNTS, false);

			super(GimbalVariable.Orientation, buf);
		}
	}

	get yaw(): number {
		// First byte is command type, second is variable ID
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

export class SetModeCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(mode: GimbalMode);

	constructor(buffer: Uint8Array | number, pitch?: number, roll?: number) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			const buf = new Uint8Array(1);
			const view = new DataView(buf.buffer);

			view.setUint8(0, buffer);

			super(GimbalVariable.Mode, buf);
		}
	}

	get mode(): number {
		// First byte is command type, second is variable ID
		return this.view.getUint8(2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Mode: ${GimbalMode[this.mode] ?? 'unknown'}`;
		}
	}
}

export class MotorPassthroughCommand extends GimbalCommand {
	constructor(buffer: Uint8Array);
	constructor(motorCommand: MotorCommand);

	constructor(srcAddr: Uint8Array | MotorCommand) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr.buffer);
		}
	}

	get motorCommand(): MotorCommand | null {
		const message = exposeSerialMessage(this);

		if (message === null) {
			return null;
		} else if (!(message instanceof MotorCommand)) {
			throw new Error('Could not parse motor passthrough command');
		}

		return message;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + '\n' + (this.motorCommand?.toString(type) ?? 'Empty');
		}
	}
}
