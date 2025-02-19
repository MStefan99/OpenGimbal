export abstract class Message {
	buffer: Uint8Array;
	view: DataView;

	protected constructor(buffer: Uint8Array) {
		this.buffer = buffer;
		this.view = new DataView(buffer.buffer);
	}

	abstract get length(): number;

	abstract toString(type?: 'hex'): string;
}
