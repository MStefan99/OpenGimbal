import {clamp} from '../../util';
import {Message} from '../Message';

export enum MotorCommandType {
	Sleep = 0x0,
	Idle = 0x1,
	Wake = 0x2,
	Move = 0x3,
	Tone = 0x4,
	Haptic = 0x5,
	AdjustOffset = 0x6,
	GetVariable = 0xe,
	SetVariable = 0xf
}

export enum MotorResponseType {
	ReturnVariable = 0xf
}

export enum MotorVariable {
	Options = 0x0,
	Offset = 0x1,
	Position = 0x2
}

export class MotorMessage extends Message {
	constructor(buffer: Uint8Array);
	constructor(
		srcAddr: number,
		destAddr: number,
		type: MotorCommandType | MotorResponseType,
		data?: Uint8Array
	);
	constructor(
		srcAddr: number | Uint8Array,
		destAddr?: number,
		type?: MotorCommandType | MotorResponseType,
		data: Uint8Array = new Uint8Array()
	) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
			const view = new DataView(srcAddr.buffer);
			this.buffer = Uint8Array.from({length: (view.getUint8(0) >> 4) + 1}, (v, i) =>
				i < view.byteLength ? view.getUint8(i) : 0
			);
		} else {
			srcAddr = Math.floor(clamp(srcAddr, 0, 14));
			destAddr = Math.floor(clamp(destAddr, 0, 15));

			super(new Uint8Array(2 + data.length));
			this.view.setUint8(0, ((this.buffer.byteLength - 1) << 4) | destAddr);
			this.view.setUint8(1, (srcAddr << 4) | type);
			this.buffer.set(data, 2);
		}
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	get srcAddr(): number {
		return this.view.getUint8(1) >> 4;
	}

	get destAddr(): number {
		return this.view.getUint8(0) & 0xf;
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return (
				`Message ${this.view.getUint8(1) & 0xf}` +
				`\n  Source address: ${this.view.getUint8(1) >> 4}` +
				`\n  Destination address: ${this.view.getUint8(0) & 0xf}`
			);
		}
	}
}
