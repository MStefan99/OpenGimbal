export abstract class Message {
	buffer: Uint8Array;
	view: DataView;

	constructor(buffer: Uint8Array) {
		this.buffer = buffer;
	}

	abstract get length(): number;

	abstract toString(type?: 'hex'): string;
}
