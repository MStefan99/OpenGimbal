import {Message} from '../Message';

export class USBMessage extends Message {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	override toString(type?: 'hex'): string {
		return '';
	}
}
