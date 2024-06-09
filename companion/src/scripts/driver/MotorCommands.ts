import {clamp, mod} from '../util';
import {BitwiseRegister} from './BitwiseRegister';
import {Command} from './Command';
import {CalibrationBits} from './Motor';

export enum MotorCommandType {
	Sleep = 0x0,
	Position = 0x1,
	Tone = 0x2,
	Haptic = 0x3,
	Offset = 0x4,
	Calibration = 0x5,
	GetVariable = 0x6,
	SetVariable = 0x7
}

export const motorCommandNames: Record<MotorCommandType, string> = {
	[MotorCommandType.Sleep]: 'Sleep',
	[MotorCommandType.Position]: 'Position',
	[MotorCommandType.Tone]: 'Tone',
	[MotorCommandType.Haptic]: 'Haptic',
	[MotorCommandType.Offset]: 'Offset',
	[MotorCommandType.Calibration]: 'Calibration',
	[MotorCommandType.GetVariable]: 'Get variable',
	[MotorCommandType.SetVariable]: 'Set variable'
};

export enum MotorVariableID {
	Calibration = 0x0,
	Offset = 0x1,
	Range = 0x2,
	Error = 0xf
}

export class MotorCommand extends Command {
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

export class SleepCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number) {
		super(srcAddr, destAddr, MotorCommandType.Sleep);
	}
}

export class PositionCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number, torque: number, position: number) {
		torque = Math.floor(clamp(torque, 0, 15));
		position = Math.floor(mod(position, 4096));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, (torque << 4) | ((position & 0xf00) >> 8));
		view.setUint8(1, position & 0xff);
		super(srcAddr, destAddr, MotorCommandType.Position, buffer);
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
	constructor(srcAddr: number, destAddr: number, frequency: number) {
		frequency = Math.floor(clamp(frequency, 50, 25000));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, (frequency & 0xff00) >> 8);
		view.setUint8(1, frequency & 0xff);
		super(srcAddr, destAddr, MotorCommandType.Tone, buffer);
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
	constructor(srcAddr: number, destAddr: number, intensity: number, duration: number) {
		intensity = Math.floor(clamp(intensity, 0, 15));
		duration = Math.floor(clamp(duration, 0, 4095));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, (intensity << 4) | ((duration & 0xf00) >> 8));
		view.setUint8(1, duration & 0xff);
		super(srcAddr, destAddr, MotorCommandType.Haptic, buffer);
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

export class OffsetCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number, offset: number) {
		offset = Math.floor(mod(offset, 4096));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, (offset & 0xff00) >> 8);
		view.setUint8(1, offset & 0xff);
		super(srcAddr, destAddr, MotorCommandType.Offset, buffer);
	}

	get offset(): number {
		return (this.view.getUint8(2) << 8) | this.view.getUint8(3);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Offset: ${this.offset}`;
		}
	}
}

export class CalibrationCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number, mode: BitwiseRegister<CalibrationBits>) {
		const buffer = new Uint8Array(1);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, mode.value);
		super(srcAddr, destAddr, MotorCommandType.Calibration, buffer);
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
	constructor(srcAddr: number, destAddr: number, id: MotorVariableID) {
		const buffer = new Uint8Array(1);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, id);
		super(srcAddr, destAddr, MotorCommandType.GetVariable, buffer);
	}

	get variableID(): MotorVariableID {
		return this.view.getUint8(2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${MotorVariableID[this.variableID]}`;
		}
	}
}

export class SetVariableCommand extends MotorCommand {
	constructor(
		srcAddr: number,
		destAddr: number,
		id: MotorVariableID,
		value: number,
		length: number
	) {
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

	get variableID(): MotorVariableID {
		return this.view.getUint8(2);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${MotorVariableID[this.variableID]}`;
		}
	}
}

export class SetOffsetVariableCommand extends SetVariableCommand {
	constructor(srcAddr: number, destAddr: number, offset: number) {
		super(srcAddr, destAddr, MotorVariableID.Offset, mod(offset, 4096), 2);
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

export class SetRangeVariableCommand extends SetVariableCommand {
	constructor(srcAddr: number, destAddr: number, range: number) {
		super(srcAddr, destAddr, MotorVariableID.Range, range, 2);
	}

	get range(): number {
		return (this.view.getUint8(3) << 8) | this.view.getUint8(4);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Range: ${this.range}`;
		}
	}
}
