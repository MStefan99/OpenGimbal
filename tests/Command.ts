import {clamp} from "./util";
import {MotorCommandType} from "./MotorCommands";


type CommandType = MotorCommandType;

export class Command {
	buffer: Uint8Array;
	view: DataView;

	constructor(buffer: Uint8Array);
	constructor(srcAddr: number, destAddr: number, cmdType: CommandType, cmdData?: Uint8Array);
	constructor(srcAddr: number | Uint8Array, destAddr?: number, cmdType?: CommandType, cmdData: Uint8Array = new Uint8Array()) {
		if (srcAddr instanceof Uint8Array) {
			this.buffer = srcAddr;
		} else {
			srcAddr = Math.floor(clamp(srcAddr, 0, 14));
			destAddr = Math.floor(clamp(destAddr, 0, 15));

			this.buffer = new Uint8Array(2 + cmdData.length);
			this.view = new DataView(this.buffer.buffer);
			this.view.setUint8(0, (this.buffer.byteLength << 4) | destAddr);
			this.view.setUint8(1, (srcAddr << 4) | cmdType);
			this.buffer.set(cmdData, 2);
		}
	}

	get length() {
		return this.view.getUint8(0 >> 4);
	}

	get srcAddr() {
		return this.view.getUint8(1) & 0xf;
	}

	get destAddr() {
		return this.view.getUint8(0) & 0xf;
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx)
					.toString(16)
					.padStart(2, '0'))
				.join(' ');
		} else {
			return `Command ${(this.view.getUint8(1) & 0xf)}`
				+ `\n  Source address: ${this.view.getUint8(1) >> 4}`
				+ `\n  Destination address: ${this.view.getUint8(0) & 0xf}`
		}
	}
}