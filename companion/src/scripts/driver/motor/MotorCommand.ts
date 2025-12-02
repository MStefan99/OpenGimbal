import {clamp, mod} from '../../util';
import {MotorCommandType, MotorVariable, MotorMessage} from './MotorMessage';

export enum MotorCommandError {
	NoError,
	TooShort,
	SourceAddressInvalid,
	DestinationAddressInvalid,
	NonExistentType
}

export const motorCommandNames: Record<MotorCommandType, string> = {
	[MotorCommandType.Sleep]: 'Sleep',
	[MotorCommandType.Idle]: 'Idle',
	[MotorCommandType.Wake]: 'Wake',
	[MotorCommandType.Move]: 'Move',
	[MotorCommandType.Tone]: 'Tone',
	[MotorCommandType.Haptic]: 'Haptic',
	[MotorCommandType.AdjustOffset]: 'AdjustOffset',
	[MotorCommandType.GetVariable]: 'Get variable',
	[MotorCommandType.SetVariable]: 'Set variable'
};

export const motorCommands: Record<MotorCommandType, (buffer: Uint8Array) => MotorCommand> = {
	[MotorCommandType.Sleep]: (buffer: Uint8Array) => new SleepCommand(buffer),
	[MotorCommandType.Idle]: (buffer: Uint8Array) => new IdleCommand(buffer),
	[MotorCommandType.Wake]: (buffer: Uint8Array) => new WakeCommand(buffer),
	[MotorCommandType.Move]: (buffer: Uint8Array) => new MoveCommand(buffer),
	[MotorCommandType.Tone]: (buffer: Uint8Array) => new ToneCommand(buffer),
	[MotorCommandType.Haptic]: (buffer: Uint8Array) => new HapticCommand(buffer),
	[MotorCommandType.AdjustOffset]: (buffer: Uint8Array) => new AdjustOffsetCommand(buffer),
	[MotorCommandType.GetVariable]: (buffer: Uint8Array) => new GetVariableCommand(buffer),
	[MotorCommandType.SetVariable]: (buffer: Uint8Array) => new SetVariableCommand(buffer)
};

export const setVariableCommands: Record<
	MotorVariable,
	(buffer: Uint8Array) => SetVariableCommand
> = {
	[MotorVariable.Options]: (buffer: Uint8Array) => new SetOptionsVariableCommand(buffer),
	[MotorVariable.Offset]: (buffer: Uint8Array) => new SetOffsetVariableCommand(buffer),
	[MotorVariable.Position]: () => {
		throw new Error('Position is a read-only variable');
	}
};

export const motorErrorDescriptions: Record<MotorCommandError, string> = {
	[MotorCommandError.NoError]: 'No error',
	[MotorCommandError.TooShort]: 'Motor command must be at least two bytes long',
	[MotorCommandError.SourceAddressInvalid]: 'Source address must be 0',
	[MotorCommandError.DestinationAddressInvalid]: 'Source address must not be 0',
	[MotorCommandError.NonExistentType]: 'This command type does not exist'
};

export class MotorCommand extends MotorMessage {
	get type(): MotorCommandType {
		return this.view.getUint8(1) & 0xf;
	}

	get error(): MotorCommandError {
		if (this.length < 2) {
			return MotorCommandError.TooShort;
		} else if (this.srcAddr && this.srcAddr < 15 && this.destAddr && this.destAddr < 15) {
			return MotorCommandError.SourceAddressInvalid;
		} else if (!this.destAddr) {
			return MotorCommandError.DestinationAddressInvalid;
		} else if (!(this.type in MotorCommandType)) {
			return MotorCommandError.NonExistentType;
		}
		return MotorCommandError.NoError;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return (
				`${motorCommandNames[(this.view.getUint8(1) & 0xf) as MotorCommandType]} command` +
				`\n  Source address: ${this.view.getUint8(1) >> 4}` +
				`\n  Destination address: ${this.view.getUint8(0) & 0xf}`
			);
		}
	}
}

export class SleepCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number);

	constructor(srcAddr: Uint8Array | number, destAddr?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr, destAddr, MotorCommandType.Sleep);
		}
	}
}

export class IdleCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number);

	constructor(srcAddr: Uint8Array | number, destAddr?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr, destAddr, MotorCommandType.Idle);
		}
	}
}

export class WakeCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number);

	constructor(srcAddr: Uint8Array | number, destAddr?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr, destAddr, MotorCommandType.Wake);
		}
	}
}

export class MoveCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, torque: number, position: number);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, torque?: number, position?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			torque = Math.floor(clamp(torque, 0, 15));
			position = Math.floor(mod(position, 4096));

			const buffer = new Uint8Array(2);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, (torque << 4) | ((position & 0xf00) >> 8));
			view.setUint8(1, position & 0xff);

			super(srcAddr, destAddr, MotorCommandType.Move, buffer);
		}
	}

	get torque(): number {
		return this.view.getUint8(2) >> 4;
	}

	get position(): number {
		return ((this.view.getUint8(2) & 0xf) << 8) | this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Torque: ${this.torque}` + `\n  Position: ${this.position}`;
		}
	}
}

export class ToneCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, frequency: number);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, frequency?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			frequency = Math.floor(clamp(frequency, 50, 25000));

			const buffer = new Uint8Array(2);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, (frequency & 0xff00) >> 8);
			view.setUint8(1, frequency & 0xff);

			super(srcAddr, destAddr, MotorCommandType.Tone, buffer);
		}
	}

	get frequency(): number {
		return (this.view.getUint8(2) << 8) | this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Frequency: ${this.frequency}`;
		}
	}
}

export class HapticCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, intensity: number, duration: number);

	constructor(
		srcAddr: number | Uint8Array,
		destAddr?: number,
		intensity?: number,
		duration?: number
	) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			intensity = Math.floor(clamp(intensity, 0, 15));
			duration = Math.floor(clamp(duration, 0, 4095));

			const buffer = new Uint8Array(2);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, (intensity << 4) | ((duration & 0xf00) >> 8));
			view.setUint8(1, duration & 0xff);

			super(srcAddr, destAddr, MotorCommandType.Haptic, buffer);
		}
	}

	get intensity(): number {
		return this.view.getUint8(2) >> 4;
	}

	get duration(): number {
		return ((this.view.getUint8(2) & 0xf) << 8) | this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() + `\n  Intensity: ${this.intensity}` + `\n  Duration: ${this.duration}`
			);
		}
	}
}

export class AdjustOffsetCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, targetPosition: number);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, targetPosition?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			targetPosition = Math.floor(mod(targetPosition, 4096));

			const buffer = new Uint8Array(2);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, (targetPosition & 0xff00) >> 8);
			view.setUint8(1, targetPosition & 0xff);

			super(srcAddr, destAddr, MotorCommandType.AdjustOffset, buffer);
		}
	}

	get targetPosition(): number {
		return (this.view.getUint8(2) << 8) | this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Offset: ${this.targetPosition}`;
		}
	}
}

export class GetVariableCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, variable: MotorVariable);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, id?: MotorVariable) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, id);

			super(srcAddr, destAddr, MotorCommandType.GetVariable, buffer);
		}
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

export class SetVariableCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, id: MotorVariable, value: number, length: number);

	constructor(
		srcAddr: number | Uint8Array,
		destAddr?: number,
		variable?: MotorVariable,
		value?: number,
		length?: number
	) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			value = Math.round(value);
			length = Math.floor(clamp(length + 1, 0, 13));

			const buffer = new Uint8Array(length);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, variable);
			for (let i = 1; i < length; ++i) {
				view.setUint8(length - i, value);
				value >>= 8;
			}

			super(srcAddr, destAddr, MotorCommandType.SetVariable, buffer);
		}
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

export class SetOptionsVariableCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, calibration: boolean, inverted?: boolean);

	constructor(
		srcAddr: number | Uint8Array,
		destAddr?: number,
		calibration?: boolean,
		inverted?: boolean
	) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			let options = 0;
			if (calibration) {
				options |= 0x01;
			}
			if (inverted) {
				options |= 0x02;
			}

			super(srcAddr, destAddr, MotorVariable.Options, options, 1);
		}
	}

	get calibrated(): boolean {
		return !!(this.view.getUint8(3) & 0x01);
	}

	get inverted(): boolean {
		return !!(this.view.getUint8(3) & 0x01);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  ${this.calibrated ? 'Start' : 'Clear'} calibration` +
				`\n  ${this.inverted ? 'Direction inverted' : 'Direction not inverted'}`
			);
		}
	}
}

export class SetOffsetVariableCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, offset: number);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, offset?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr, destAddr, MotorVariable.Offset, mod(offset, 4096), 2);
		}
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
