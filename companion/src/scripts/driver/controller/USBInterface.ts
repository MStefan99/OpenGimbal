import {IHardwareInterface} from '../HardwareInterface';
import {ControllerMessage} from './ControllerMessage';
import {IControllerParser} from './ControllerParser';

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

	send(message: ControllerMessage, isochronous?: boolean): Promise<void>;

	request(message: ControllerMessage): Promise<ControllerMessage>;

	close(): Promise<void>;
}

export class USBInterface implements IHardwareInterface {
	_transferQueue = new Array<ControllerMessage>();
	_usbDevice: USBDevice;
	_parser: IControllerParser;
	readonly _verbose: boolean;

	constructor(usbDevice: USBDevice, parser: IControllerParser, verbose: boolean = false) {
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

	async _sendNext(): Promise<void> {
		return new Promise((resolve, reject) => {
			const message = this._transferQueue.shift();

			this._usbDevice
				.transferOut(1, message.buffer as Uint8Array<ArrayBuffer>)
				.then(() => resolve())
				.catch((err) => {
					console.error('Send failed:', err);
					return Promise.reject(err);
				})
				.then(() => {
					this._sendNext();
				});
		});
	}

	async send(message: ControllerMessage, isochronous?: boolean): Promise<void> {
		if (isochronous && this._transferQueue.length > 20) {
			console.warn('Send failed: queue full', message.toString(), '\n', message);
			return Promise.reject('Send failed1`');
		}

		this._verbose && console.log('Sending', message.toString(), '\n', message);
		this._transferQueue.push(message);
		if (this._transferQueue.length === 1) {
			return this._sendNext();
		}
	}

	async request(message: ControllerMessage): Promise<ControllerMessage> {
		this._verbose && console.log('Sending', message.toString(), '\n', message);

		return this._usbDevice
			.transferOut(1, message.buffer as Uint8Array<ArrayBuffer>)
			.then(() => this._usbDevice.transferIn(1, 0xff))
			.then((r) => {
				const message = this._parser.parseResponse(new Uint8Array(r.data.buffer));
				this._verbose && console.log('Received', message.toString(), '\n', message);

				return message;
			})
			.catch((err) => {
				console.error('Send failed:', err);
				return Promise.reject(err);
			}) as Promise<ControllerMessage>;
	}

	async close(): Promise<void> {
		return this._usbDevice.close();
	}
}
