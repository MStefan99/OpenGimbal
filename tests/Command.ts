'use strict';

import {BitwiseRegister, CalibrationBits} from "./BitMask";

enum CommandType {
	Sleep = 0x0,
	Position = 0x1,
	Tone = 0x2,
	Haptic = 0x3,
	Offset = 0x4,
	Calibration = 0x5,
	GetVariable = 0x6,
	SetVariable = 0x7,
}

export const commandNames: Record<CommandType, string> = {
	[CommandType.Sleep]: 'Sleep',
	[CommandType.Position]: 'Position',
	[CommandType.Tone]: 'Tone',
	[CommandType.Haptic]: 'Haptic',
	[CommandType.Offset]: 'Offset',
	[CommandType.Calibration]: 'Calibration',
	[CommandType.GetVariable]: 'Get variable',
	[CommandType.SetVariable]: 'Set variable',
}

export class Command {
	buffer = new Uint8Array(16);
	view = new DataView(this.buffer.buffer);
	length = 0;

	constructor(srcAddr: number, destAddr: number, cmdType: CommandType, cmdData: Uint8Array = new Uint8Array()) {
		srcAddr = Math.floor(srcAddr);
		destAddr = Math.floor(destAddr);

		this.length = 2 + cmdData.length; // Command byte + type byte + data
		this.view.setUint8(0, (this.length << 4) | destAddr);
		this.view.setUint8(1, (srcAddr << 4) | cmdType);
		this.buffer.set(cmdData, 2);
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx)
					.toString(16)
					.padStart(2, '0'))
				.join(' ');
		} else {
			return `${commandNames[(this.view.getUint8(1) & 0xf) as CommandType]} command`
				+ `\n  Source address: ${this.view.getUint8(1) >> 4}`
				+ `\n  Destination address: ${this.view.getUint8(0) & 0xf}`
		}
	}
}

export class SleepCommand extends Command {
	constructor(srcAddr: number, destAddr: number) {
		super(srcAddr, destAddr, CommandType.Sleep);
	}
}

export class PositionCommand extends Command {
	constructor(srcAddr: number, destAddr: number, torque: number, position: number) {
		torque = Math.floor(torque);
		position = Math.floor(position);

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, torque << 4 | ((position & 0xf00) >> 8));
		view.setUint8(1, position & 0xff);
		super(srcAddr, destAddr, CommandType.Position, buffer);
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

export class ToneCommand extends Command {
	constructor(srcAddr: number, destAddr: number, frequency: number) {
		frequency = Math.floor(frequency);

		const buffer = new Uint8Array(2);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, (frequency & 0xff00) >> 8);
		view.setUint8(1, frequency & 0xff);
		super(srcAddr, destAddr, CommandType.Tone, buffer);
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

export class CalibrationCommand extends Command {
	constructor(srcAddr: number, destAddr: number, mode: BitwiseRegister<CalibrationBits>) {

		const buffer = new Uint8Array(1);
		const view = new DataView(buffer.buffer);
		view.setUint8(0, mode.value);
		super(srcAddr, destAddr, CommandType.Calibration, buffer);
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
