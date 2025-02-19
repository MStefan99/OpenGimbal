import {IHardwareInterface} from '../HardwareInterface';
import {USBMessage} from './USBMessage';
import {IUSBParser} from './USBParser';

export interface IUSBInterface extends IHardwareInterface {
	get productName(): string;
	get manufacturerName(): string;

	get usbVersionMajor(): number;
	get usbVersionMinor(): number;
	get usbVersionSubminor(): number;

	get deviceVersionMajor(): number;
	get deviceVersionMinor(): number;
	get deviceVersionSubminor(): number;

	send(command: USBMessage): Promise<void>;
	request(command: USBMessage): Promise<USBMessage>;
	close(): Promise<void>;
}

export class USBInterface implements IHardwareInterface {
	_transferPromise: Promise<void | USBMessage> = Promise.resolve();
	_usbDevice: USBDevice;
	_parser: IUSBParser;
	readonly _verbose: boolean;

	constructor(usbDevice: USBDevice, parser: IUSBParser, verbose: boolean = false) {
		this._usbDevice = usbDevice;
		this._parser = parser;
		this._verbose = verbose;
	}

	get productName(): string {
		return this._usbDevice.productName;
	}

	get manufacturerName(): string {
		return this._usbDevice.manufacturerName;
	}

	get usbVersionMajor(): number {
		return this._usbDevice.usbVersionMajor;
	}

	get usbVersionMinor(): number {
		return this._usbDevice.usbVersionMinor;
	}

	get usbVersionSubminor(): number {
		return this._usbDevice.usbVersionSubminor;
	}

	get deviceVersionMajor(): number {
		return this._usbDevice.deviceVersionMajor;
	}

	get deviceVersionMinor(): number {
		return this._usbDevice.deviceVersionMinor;
	}

	get deviceVersionSubminor(): number {
		return this._usbDevice.deviceVersionSubminor;
	}

	async send(command: USBMessage): Promise<void> {
		return (this._transferPromise = this._transferPromise
			.then(() => this._usbDevice.transferOut(1, command.buffer))
			.catch((err) => Promise.reject(err))
			.then(() => Promise.resolve())) as Promise<void>;
	}

	async request(command: USBMessage): Promise<USBMessage> {
		return (this._transferPromise = this._transferPromise
			.then(() => this._usbDevice.transferOut(1, command.buffer))
			.then(() => this._usbDevice.transferIn(1, 0xff))
			.then((r) => this._parser.parse(new Uint8Array(r.data.buffer))[0])) as Promise<USBMessage>;
	}

	async close(): Promise<void> {
		return this._usbDevice.close();
	}
}
