import {Message} from '../Message';

export enum USBMessageType {
	Motor = 0xf
}

export class USBMessage extends Message {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	get type(): USBMessageType {
		return this.view.getUint8(0);
	}

	override toString(type?: 'hex'): string {
		return '';
	}
}
