import {IHardwareInterface} from '../HardwareInterface';
import {USBMessage} from './USBMessage';
import {IUSBParser} from './USBParser';

export interface IUSBInterface extends IHardwareInterface {
	readonly usbVersionMajor: number;
	readonly usbVersionMinor: number;
	readonly usbVersionSubminor: number;
	readonly deviceClass: number;
	readonly deviceSubclass: number;
	readonly deviceProtocol: number;
	readonly vendorId: number;
	readonly productId: number;
	readonly deviceVersionMajor: number;
	readonly deviceVersionMinor: number;
	readonly deviceVersionSubminor: number;
	readonly manufacturerName?: string | undefined;
	readonly productName?: string | undefined;
	readonly serialNumber?: string | undefined;

	send(message: USBMessage): Promise<void>;

	request(message: USBMessage): Promise<USBMessage>;

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

	get usbVersionMajor(): number {
		return this._usbDevice.usbVersionMajor;
	}

	get usbVersionMinor(): number {
		return this._usbDevice.usbVersionMinor;
	}

	get usbVersionSubminor(): number {
		return this._usbDevice.usbVersionSubminor;
	}

	get deviceClass(): number {
		return this._usbDevice.deviceClass;
	}

	get deviceSubclass(): number {
		return this._usbDevice.deviceSubclass;
	}

	get deviceProtocol(): number {
		return this._usbDevice.deviceProtocol;
	}

	get vendorId(): number {
		return this._usbDevice.vendorId;
	}

	get productId(): number {
		return this._usbDevice.productId;
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

	get manufacturerName(): string {
		return this._usbDevice.manufacturerName;
	}

	get productName(): string {
		return this._usbDevice.productName;
	}

	get serialNumber(): string {
		return this._usbDevice.serialNumber;
	}

	async send(message: USBMessage): Promise<void> {
		this._verbose && console.log('Sending', message.toString(), '\n', message);

		return (this._transferPromise = this._transferPromise
			.then(() => this._usbDevice.transferOut(1, message.buffer as Uint8Array<ArrayBuffer>))
			.catch((err) => {
				console.error('Send failed:', err);
				return Promise.reject(err);
			})
			.then(() => Promise.resolve())) as Promise<void>;
	}

	async request(message: USBMessage): Promise<USBMessage> {
		this._verbose && console.log('Sending', message.toString(), '\n', message);

		return (this._transferPromise = this._transferPromise
			.then(() => this._usbDevice.transferOut(1, message.buffer as Uint8Array<ArrayBuffer>))
			.then(() => this._usbDevice.transferIn(1, 0xff))
			.then((r) => {
				const message = this._parser.parseResponse(new Uint8Array(r.data.buffer));
				this._verbose && console.log('Received', message.toString(), '\n', message);

				return message;
			})
			.catch((err) => {
				console.error('Send failed:', err);
				return Promise.reject(err);
			})) as Promise<USBMessage>;
	}

	async close(): Promise<void> {
		return this._usbDevice.close();
	}
}
