import {
	ControllerCommandType,
	GimbalMode,
	ControllerVariable,
	ControllerMessage
} from './ControllerMessage';
import {MotorCommand} from '../motor/MotorCommand';
import {RAD_TO_COUNTS} from '../../types';
import {clamp} from '../../util';
import {MotorParser} from '../motor/MotorParser';

export const controllerCommandNames: Record<ControllerCommandType, string> = {
	[ControllerCommandType.Disable]: 'Disable',
	[ControllerCommandType.Enable]: 'Enable',
	[ControllerCommandType.Discovery]: 'Discovery',
	[ControllerCommandType.GetVariable]: 'Get variable',
	[ControllerCommandType.SetVariable]: 'Set variable',
	[ControllerCommandType.MotorPassthrough]: 'Motor passthrough'
};

export const controllerCommands: Record<
	ControllerCommandType,
	(buffer: Uint8Array) => ControllerCommand
> = {
	[ControllerCommandType.Disable]: (buffer: Uint8Array) => new DisableCommand(buffer),
	[ControllerCommandType.Enable]: (buffer: Uint8Array) => new EnableCommand(buffer),
	[ControllerCommandType.Discovery]: (buffer: Uint8Array) => new DiscoveryCommand(buffer),
	[ControllerCommandType.GetVariable]: (buffer: Uint8Array) => new GetVariableCommand(buffer),
	[ControllerCommandType.SetVariable]: (buffer: Uint8Array) => new SetVariableCommand(buffer),
	[ControllerCommandType.MotorPassthrough]: (buffer: Uint8Array) =>
		new MotorPassthroughCommand(buffer)
};

export const setVariableCommands: Record<
	ControllerVariable,
	(buffer: Uint8Array) => SetVariableCommand
> = {
	[ControllerVariable.Orientation]: (buffer: Uint8Array) =>
		new SetOrientationVariableCommand(buffer),
	[ControllerVariable.HandleOrientation]: () => {
		throw new Error('Handle orientation is a read-only variable');
	},
	[ControllerVariable.Mode]: (buffer) => new SetModeVariableCommand(buffer),
	[ControllerVariable.BatteryVoltage]: () => {
		throw new Error('Battery voltage is a read-only variable');
	},
	[ControllerVariable.DeviceVersion]: () => {
		throw new Error('Device version is a read-only variable');
	},
	[ControllerVariable.VendorName]: () => {
		throw new Error('Vendor name is a read-only variable');
	},
	[ControllerVariable.ProductName]: () => {
		throw new Error('Product name is a read-only variable');
	},
	[ControllerVariable.SerialNumber]: () => {
		throw new Error('Serial number is a read-only variable');
	}
};

export class ControllerCommand extends ControllerMessage {
	get type(): ControllerCommandType {
		return this.view.getUint8(0);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${controllerCommandNames[this.view.getUint8(0) as ControllerCommandType]} command`;
		}
	}
}

export class DisableCommand extends ControllerCommand {
	constructor(buffer: Uint8Array);
	constructor();

	constructor(buffer?: Uint8Array) {
		if (buffer) {
			super(buffer);
		} else {
			super(ControllerCommandType.Disable);
		}
	}
}

export class EnableCommand extends ControllerCommand {
	constructor(buffer: Uint8Array);
	constructor();

	constructor(buffer?: Uint8Array) {
		if (buffer) {
			super(buffer);
		} else {
			super(ControllerCommandType.Enable);
		}
	}
}

export class DiscoveryCommand extends ControllerCommand {
	constructor(buffer: Uint8Array);
	constructor();

	constructor(buffer?: Uint8Array) {
		if (buffer) {
			super(buffer);
		} else {
			super(ControllerCommandType.Discovery);
		}
	}
}

export class GetVariableCommand extends ControllerCommand {
	constructor(buffer: Uint8Array);
	constructor(variable: ControllerVariable);

	constructor(variable: Uint8Array | ControllerVariable) {
		if (variable instanceof Uint8Array) {
			super(variable);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, variable);

			super(ControllerCommandType.GetVariable, buffer);
		}
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

export class SetVariableCommand extends ControllerCommand {
	constructor(buffer: Uint8Array);
	constructor(variable: ControllerVariable, value: Uint8Array);
	constructor(variable: ControllerVariable, value: number, length: number);

	constructor(
		variable: Uint8Array | ControllerVariable,
		value?: number | Uint8Array,
		length?: number
	) {
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

			super(ControllerCommandType.SetVariable, buffer);
		} else {
			const buffer = new Uint8Array(value.byteLength + 1);
			const view = new DataView(buffer.buffer);

			view.setUint8(0, variable);
			buffer.set(value, 1);

			super(ControllerCommandType.SetVariable, buffer);
		}
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

export class SetOrientationVariableCommand extends SetVariableCommand {
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

			super(ControllerVariable.Orientation, buf);
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

export class SetModeVariableCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(mode: GimbalMode);

	constructor(buffer: Uint8Array | number, pitch?: number, roll?: number) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			const buf = new Uint8Array(1);
			const view = new DataView(buf.buffer);

			view.setUint8(0, buffer);

			super(ControllerVariable.Mode, buf);
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

export class MotorPassthroughCommand extends ControllerCommand {
	constructor(buffer: Uint8Array);
	constructor(motorCommand: MotorCommand);

	constructor(srcAddr: Uint8Array | MotorCommand) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			const buffer = new Uint8Array(srcAddr.buffer.byteLength + 1);
			const view = new DataView(buffer.buffer);

			view.setUint8(0, ControllerCommandType.MotorPassthrough);
			buffer.set(srcAddr.buffer, 1);

			super(buffer);
		}
	}

	get motorCommand(): MotorCommand | null {
		const message = new MotorParser().parse(
			new Uint8Array(this.buffer.byteLength - 1).fill(0).map((v, i) => message.buffer[i + 1])
		);

		if (message === null) {
			return null;
		} else if (!(message instanceof MotorCommand)) {
			throw new Error('Could not parse motor passthrough command');
		}

		if (message.view.getUint8(0) !== ControllerCommandType.MotorPassthrough) {
			throw new Error('Invalid message type');
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
