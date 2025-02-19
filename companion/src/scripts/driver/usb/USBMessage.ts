import {Message} from '../Message';

export class USBMessage extends Message {
	constructor(buffer: Uint8Array) {
		super(buffer);
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `Message ${this.view.getUint8(0)}`;
		}
	}
}
