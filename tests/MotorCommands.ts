import {clamp} from "./util";
import {BitwiseRegister, CalibrationBits} from "./BitMask";
import {Command} from "./Command";

export enum MotorCommandType {
	Sleep = 0x0,
	Position = 0x1,
	Tone = 0x2,
	Haptic = 0x3,
	Offset = 0x4,
	Calibration = 0x5,
	GetVariable = 0x6,
	SetVariable = 0x7,
}

export const motorCommandNames: Record<MotorCommandType, string> = {
	[MotorCommandType.Sleep]: 'Sleep',
	[MotorCommandType.Position]: 'Position',
	[MotorCommandType.Tone]: 'Tone',
	[MotorCommandType.Haptic]: 'Haptic',
	[MotorCommandType.Offset]: 'Offset',
	[MotorCommandType.Calibration]: 'Calibration',
	[MotorCommandType.GetVariable]: 'Get variable',
	[MotorCommandType.SetVariable]: 'Set variable',
}

export enum MotorVariableID {
	Calibration = 0x0,
	Offset = 0x1,
	Range = 0x2,
	Error = 0xf
}


export class MotorCommand extends Command {
	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx)
					.toString(16)
					.padStart(2, '0'))
				.join(' ');
		} else {
			return `${motorCommandNames[(this.view.getUint8(1) & 0xf) as MotorCommandType]} command`
				+ `\n  Source address: ${this.view.getUint8(1) >> 4}`
				+ `\n  Destination address: ${this.view.getUint8(0) & 0xf}`
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
		position = Math.floor(clamp(position, 0, 4095));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, torque << 4 | ((position & 0xf00) >> 8));
		view.setUint8(1, position & 0xff);
		super(srcAddr, destAddr, MotorCommandType.Position, buffer);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Torque: ${this.view.getUint8(2) >> 4}`
				+ `\n  Position: ${((this.view.getUint8(2) & 0xf) << 8) | this.view.getUint8(3)}`
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

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Frequency: ${(this.view.getUint8(2) << 8) | this.view.getUint8(3)}`
		}
	}
}

export class HapticCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number, intensity: number, duration: number) {
		intensity = Math.floor(clamp(intensity, 0, 255));
		duration = Math.floor(clamp(duration, 0, 255));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, intensity);
		view.setUint8(1, duration);
		super(srcAddr, destAddr, MotorCommandType.Haptic, buffer);
	}

	override toString(type?: "hex"): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Intensity: ${this.view.getUint8(2)}`
				+ `\n  Duration: ${this.view.getUint8(3)}`
		}
	}
}

export class OffsetCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number, offset: number) {
		offset = Math.floor(clamp(offset, 0, 4095));

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, (offset & 0xff00) >> 8);
		view.setUint8(1, offset & 0xff);
		super(srcAddr, destAddr, MotorCommandType.Offset, buffer);
	}

	override toString(type?: "hex"): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Offset: ${(this.view.getUint8(2) << 8) | this.view.getUint8(3)}`
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

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Mode: ${new Array(8)
					.fill(0)
					.map((v, i) => (1 << i & this.view.getUint8(2)) ? i : null)
					.filter(v => v !== null)
					.map(v => CalibrationBits[v])
					.join(', ')}`;
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

	override toString(type?: "hex"): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString()
				+ `\n  Variable: ${MotorVariableID[(this.view.getUint8(2) << 8)]}`
		}
	}
}

export class SetVariableCommand extends MotorCommand {
	constructor(srcAddr: number, destAddr: number, id: MotorVariableID, value: number, length: number) {
		value = Math.round(value);
		length = Math.floor(clamp(length + 1, 0, 13));

		const buffer = new Uint8Array(length);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, id);
		for (let i = 1; i < length; ++i) {
			view.setUint8(length - i, value);
			value >>= 8;
		}
		super(srcAddr, destAddr, MotorCommandType.GetVariable, buffer);
	}
}

export class SetOffsetVariableCommand extends SetVariableCommand {
	constructor(srcAddr: number, destAddr: number, offset: number) {
		super(srcAddr, destAddr, MotorVariableID.Offset, offset, 2);
	}
}

export class SetRangeVariableCommand extends SetVariableCommand {
	constructor(srcAddr: number, destAddr: number, range: number) {
		super(srcAddr, destAddr, MotorVariableID.Range, range, 2);
	}
}
