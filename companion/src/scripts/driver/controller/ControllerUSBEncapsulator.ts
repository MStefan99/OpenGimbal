import {MotorMessage} from '../motor/MotorMessage';
import {GimbalCommandType, ControllerMessage} from './ControllerMessage';
import {USBInterface} from './USBInterface';
import {ISerialInterface} from '../motor/SerialInterface';
import {ControllerParser} from './ControllerParser';
import {MotorParser} from '../motor/MotorParser';

export function encapsulateSerialMessage(message: MotorMessage): ControllerMessage {
	const buffer = new Uint8Array(message.buffer.byteLength + 1);
	const view = new DataView(buffer.buffer);

	view.setUint8(0, GimbalCommandType.MotorPassthrough);
	buffer.set(message.buffer, 1);

	return new ControllerParser().parseCommand(buffer);
}

export function exposeSerialMessage(message: ControllerMessage): MotorMessage | null {
	if (message.buffer.byteLength <= 1) {
		return null;
	}

	if (message.view.getUint8(0) !== GimbalCommandType.MotorPassthrough) {
		throw new Error('Invalid message type');
	}

	return new MotorParser().parse(
		new Uint8Array(message.buffer.byteLength - 1).fill(0).map((v, i) => message.buffer[i + 1])
	);
}

export class ControllerUSBEncapsulator implements ISerialInterface {
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

	send(message: MotorMessage): Promise<void> {
		return this._usbInterface.send(encapsulateSerialMessage(message));
	}

	async request(message: MotorMessage): Promise<MotorMessage> {
		return exposeSerialMessage(await this._usbInterface.request(encapsulateSerialMessage(message)));
	}

	close(): Promise<void> {
		return this._usbInterface.close();
	}
}
