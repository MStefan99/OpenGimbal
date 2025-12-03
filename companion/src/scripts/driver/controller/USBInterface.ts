import {IControllerInterface, IHardwareInterface} from '../HardwareInterface';
import {IControllerParser} from './ControllerParser';
import {ControllerCommand} from './ControllerCommand';
import {ControllerResponse} from './ControllerResponse';
import {ControllerMessage} from './ControllerMessage';

export interface IUSBInterface extends IControllerInterface {
	readonly usbVersionMajor: number;
	readonly usbVersionMinor: number;
	readonly usbVersionSubminor: number;
	readonly deviceClass: number;
	readonly deviceSubclass: number;
	readonly deviceProtocol: number;

	send(message: ControllerCommand, isochronous?: boolean): Promise<void>;

	request(message: ControllerCommand): Promise<ControllerResponse>;

	close(): Promise<void>;
}

function delay(ms: number): Promise<null> {
	return new Promise((resolve) => setTimeout(resolve, ms));
}

export class USBInterface implements IUSBInterface {
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

			if (!message) {
				return;
			}

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

	async send(message: ControllerCommand, isochronous?: boolean): Promise<void> {
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

	async request(message: ControllerCommand): Promise<ControllerResponse> {
		this._verbose && console.log('Sending', message.toString(), '\n', message);

		return this._usbDevice
			.transferOut(1, message.buffer as Uint8Array<ArrayBuffer>)
			.then(() => delay(5))
			.then(() => this._usbDevice.transferIn(1, 0xff))
			.then((r) => {
				const message = this._parser.parseResponse(new Uint8Array(r.data.buffer));
				this._verbose && console.log('Received', message.toString(), '\n', message);

				return message;
			})
			.catch((err) => {
				console.error('Send failed:', err);
				return Promise.reject(err);
			}) as Promise<ControllerResponse>;
	}

	async close(): Promise<void> {
		return this._usbDevice.close();
	}
}
