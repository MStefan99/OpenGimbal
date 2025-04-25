import {Message} from '../Message';
import {GimbalCommandType} from './GimbalCommand';

export class USBMessage extends Message {
	constructor(buffer: Uint8Array);
	constructor(type: GimbalCommandType, data?: Uint8Array);

	constructor(buffer: Uint8Array | GimbalCommandType, data?: Uint8Array) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			super(new Uint8Array(1 + (data?.byteLength ?? 0)));

			this.view.setUint8(0, buffer);
			data && this.buffer.set(data, 1);
		}
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
