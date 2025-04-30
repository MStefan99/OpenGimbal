import {SerialMessage} from '../serial/SerialMessage';
import {GimbalCommandType, USBMessage} from './USBMessage';
import {USBInterface} from './USBInterface';
import {ISerialInterface} from '../serial/SerialInterface';
import {USBParser} from './USBParser';
import {SerialParser} from '../serial/SerialParser';

export function encapsulateSerialMessage(message: SerialMessage): USBMessage {
	const buffer = new Uint8Array(message.buffer.byteLength + 1);
	const view = new DataView(buffer.buffer);

	view.setUint8(0, GimbalCommandType.MotorPassthrough);
	buffer.set(message.buffer, 1);

	return new USBParser().parseCommand(buffer);
}

export function exposeSerialMessage(message: USBMessage): SerialMessage | null {
	if (message.buffer.byteLength <= 1) {
		return null;
	}

	if (message.view.getUint8(0) !== GimbalCommandType.MotorPassthrough) {
		throw new Error('Invalid message type');
	}

	return new SerialParser().parse(
		new Uint8Array(message.buffer.byteLength - 1).fill(0).map((v, i) => message.buffer[i + 1])
	);
}

export class USBSerialEncapsulator implements ISerialInterface {
	_usbInterface: USBInterface;

	constructor(usbInterface: USBInterface) {
		this._usbInterface = usbInterface;
	}

	get vendorId(): number | undefined {
		return this._usbInterface.vendorId;
	}

	get productId(): number | undefined {
		return this._usbInterface.productId;
	}

	get open(): Promise<void> {
		return Promise.resolve();
	}

	send(message: SerialMessage): Promise<void> {
		return this._usbInterface.send(encapsulateSerialMessage(message));
	}

	async request(message: SerialMessage): Promise<SerialMessage> {
		return exposeSerialMessage(await this._usbInterface.request(encapsulateSerialMessage(message)));
	}

	close(): Promise<void> {
		return this._usbInterface.close();
	}
}
