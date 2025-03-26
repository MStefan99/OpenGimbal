import {clamp, mod} from '../../util';
import {BitwiseRegister} from '../BitwiseRegister';
import {SerialMessage} from './SerialMessage';
import {CalibrationBits} from '../Motor';

export enum MotorCommandType {
	Idle = 0x0,
	Sleep = 0x1,
	Move = 0x2,
	Tone = 0x3,
	Haptic = 0x4,
	AdjustOffset = 0x5,
	Calibrate = 0x6,
	GetVariable = 0xe,
	SetVariable = 0xf
}

export enum MotorVariableID {
	Calibration = 0x0,
	Offset = 0x1,
	Position = 0x2,
	Speed = 0x3,
	Error = 0xf
}

export const motorCommandNames: Record<MotorCommandType, string> = {
	[MotorCommandType.Idle]: 'Idle',
	[MotorCommandType.Sleep]: 'Sleep',
	[MotorCommandType.Move]: 'Move',
	[MotorCommandType.Tone]: 'Tone',
	[MotorCommandType.Haptic]: 'Haptic',
	[MotorCommandType.AdjustOffset]: 'AdjustOffset',
	[MotorCommandType.Calibrate]: 'Calibration',
	[MotorCommandType.GetVariable]: 'Get variable',
	[MotorCommandType.SetVariable]: 'Set variable'
};

export const motorCommands: Record<MotorCommandType, (buffer: Uint8Array) => MotorCommand> = {
	[MotorCommandType.Idle]: (buffer: Uint8Array) => new IdleCommand(buffer),
	[MotorCommandType.Sleep]: (buffer: Uint8Array) => new SleepCommand(buffer),
	[MotorCommandType.Move]: (buffer: Uint8Array) => new MoveCommand(buffer),
	[MotorCommandType.Tone]: (buffer: Uint8Array) => new ToneCommand(buffer),
	[MotorCommandType.Haptic]: (buffer: Uint8Array) => new HapticCommand(buffer),
	[MotorCommandType.AdjustOffset]: (buffer: Uint8Array) => new AdjustOffsetCommand(buffer),
	[MotorCommandType.Calibrate]: (buffer: Uint8Array) => new CalibrationCommand(buffer),
	[MotorCommandType.GetVariable]: (buffer: Uint8Array) => new GetVariableCommand(buffer),
	[MotorCommandType.SetVariable]: (buffer: Uint8Array) => new SetVariableCommand(buffer)
};

export const variableCommands: Partial<
	Record<MotorVariableID, (buffer: Uint8Array) => SetVariableCommand>
> = {
	[MotorVariableID.Calibration]: () => {
		throw new Error('Calibration is a read-only variable');
	},
	[MotorVariableID.Offset]: (buffer: Uint8Array) => new SetOffsetVariableCommand(buffer),
	[MotorVariableID.Speed]: (buffer: Uint8Array) => new SetSpeedVariableCommand(buffer),
	[MotorVariableID.Error]: () => {
		throw new Error('Error is a read-only variable');
	}
};

export class MotorCommand extends SerialMessage {
	get type(): MotorCommandType {
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
				`${motorCommandNames[(this.view.getUint8(1) & 0xf) as MotorCommandType]} command` +
				`\n  Source address: ${this.view.getUint8(1) >> 4}` +
				`\n  Destination address: ${this.view.getUint8(0) & 0xf}`
			);
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

export class CalibrationCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, mode: BitwiseRegister<CalibrationBits>);

	constructor(
		srcAddr: number | Uint8Array,
		destAddr?: number,
		mode?: BitwiseRegister<CalibrationBits>
	) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, mode.value);

			super(srcAddr, destAddr, MotorCommandType.Calibrate, buffer);
		}
	}

	get calibrationMode(): BitwiseRegister<CalibrationBits> {
		return new BitwiseRegister<CalibrationBits>(this.view.getUint8(2));
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

export class GetVariableCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, id: MotorVariableID);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, id?: MotorVariableID) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, id);

			super(srcAddr, destAddr, MotorCommandType.GetVariable, buffer);
		}
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

export class SetVariableCommand extends MotorCommand {
	constructor(buffer: Uint8Array);
	constructor(
		srcAddr: number,
		destAddr: number,
		id: MotorVariableID,
		value: number,
		length: number
	);

	constructor(
		srcAddr: number | Uint8Array,
		destAddr?: number,
		id?: MotorVariableID,
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
			view.setUint8(0, id);
			for (let i = 1; i < length; ++i) {
				view.setUint8(length - i, value);
				value >>= 8;
			}

			super(srcAddr, destAddr, MotorCommandType.SetVariable, buffer);
		}
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

export class SetOffsetVariableCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, offset: number);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, offset?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr, destAddr, MotorVariableID.Offset, mod(offset, 4096), 2);
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

export class SetSpeedVariableCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, range: number);

	constructor(srcAddr: number | Uint8Array, destAddr?: number, speed?: number) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr, destAddr, MotorVariableID.Speed, speed, 1);
		}
	}

	get speed(): number {
		return this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Maximum speed: ${this.speed}`;
		}
	}
}
