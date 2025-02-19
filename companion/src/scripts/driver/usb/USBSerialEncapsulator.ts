import {SerialMessage} from '../serial/SerialMessage';
import {USBMessage} from './USBMessage';
import {USBInterface} from './USBInterface';
import {ISerialInterface} from '../serial/SerialInterface';
import {GimbalCommandType} from './GimbalCommand';
import {USBParser} from './USBParser';

export function encapsulateSerialMessage(message: SerialMessage): USBMessage {
	const buffer = new Uint8Array(message.buffer.byteLength + 1);
	const view = new DataView(buffer.buffer);

	view.setUint8(0, GimbalCommandType.MotorPassthrough);
	buffer.set(message.buffer, 1);

	return new USBParser().parse(buffer)[0];
}

export function exposeSerialCommand(message: USBMessage): SerialMessage {
	if (message.view.getUint8(0) !== GimbalCommandType.MotorPassthrough) {
		throw new Error('Invalid message type');
	}

	return new SerialMessage(
		new Uint8Array(message.buffer.byteLength - 1).fill(0).map((v, i) => message.buffer[i + 1])
	);
}

export class USBSerialEncapsulator implements ISerialInterface {
	_usbInterface: USBInterface;

	constructor(usbInterface: USBInterface) {
		this._usbInterface = usbInterface;
	}

	send(message: SerialMessage): Promise<void> {
		return this._usbInterface.send(encapsulateSerialMessage(message));
	}

	async request(message: SerialMessage): Promise<SerialMessage> {
		return exposeSerialCommand(await this._usbInterface.request(encapsulateSerialMessage(message)));
	}

	close(): Promise<void> {
		return this._usbInterface.close();
	}
}
