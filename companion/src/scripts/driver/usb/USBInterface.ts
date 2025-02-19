import {HardwareInterface} from '../HardwareInterface';
import {GetVariableCommand} from '../serial/MotorCommand';
import {USBMessage} from './USBMessage';
import {USBParser} from './USBParser';

export class USBInterface extends HardwareInterface {
	_transferPromise: Promise<void | USBMessage> = Promise.resolve();
	_usbDevice: USBDevice;
	_parser: USBParser;
	readonly _verbose: boolean;

	constructor(usbDevice: USBDevice, parser: USBParser, verbose: boolean = false) {
		super(parser, verbose);

		this._usbDevice = usbDevice;
		this._parser = parser;
		this._verbose = verbose;
	}

	async send(command: USBMessage): Promise<void> {
		return (this._transferPromise = this._transferPromise
			.then(() => this._usbDevice.transferOut(1, command.buffer))
			.catch((err) => Promise.reject(err))
			.then(() => Promise.resolve())) as Promise<void>;
	}

	async request<T extends USBMessage>(command: GetVariableCommand): Promise<T> {
		return (this._transferPromise = this._transferPromise
			.then(() => this._usbDevice.transferOut(1, command.buffer))
			.then(() => this._usbDevice.transferIn(1, 0xff))
			.then((r) => this._parser.parse(new Uint8Array(r.data.buffer))[0])) as Promise<T>;
	}

	async close(): Promise<void> {}
}
