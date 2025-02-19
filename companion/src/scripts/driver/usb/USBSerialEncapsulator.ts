import {SerialMessage} from '../serial/SerialMessage';
import {USBMessage, USBMessageType} from './USBMessage';
import {USBInterface} from './USBInterface';
import {ISerialInterface} from '../serial/SerialInterface';

export class USBSerialEncapsulator implements ISerialInterface {
	_usbInterface: USBInterface;

	constructor(usbInterface: USBInterface) {
		this._usbInterface = usbInterface;
	}

	send(message: SerialMessage): Promise<void> {
		return this._usbInterface.send(this._encapsulate(message));
	}

	async request(message: SerialMessage): Promise<SerialMessage> {
		return this._expose(await this._usbInterface.request(this._encapsulate(message)));
	}

	close(): Promise<void> {
		return this._usbInterface.close();
	}

	_encapsulate(message: SerialMessage): USBMessage {
		const buffer = new Uint8Array(message.buffer.byteLength + 1);
		const view = new DataView(buffer.buffer);

		view.setUint8(0, USBMessageType.Motor);
		buffer.set(message.buffer, 1);

		return new USBMessage(buffer);
	}

	_expose(message: USBMessage): SerialMessage {
		if (message.view.getUint8(0) !== USBMessageType.Motor) {
			throw new Error('Invalid message type');
		}

		const buffer = new Uint8Array(message.buffer.byteLength - 1);
		const view = new DataView(buffer.buffer);

		return new SerialMessage(buffer);
	}
}
