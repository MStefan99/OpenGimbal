import {
	DeviceCommand,
	GetVariableCommand,
	SetArrayVariableCommand,
	SetPIDsCommand,
	VariableID
} from './DeviceCommand';
import {
	DeviceResponse,
	parseDeviceResponse,
	ReturnArrayVariableResponse,
	ReturnPIDsResponse,
	ReturnSensorsResponse,
	ReturnSettingsResponse,
	ReturnStatusResponse
} from './DeviceResponse';
import {SimulatedUSBDevice} from './SimulatedUSBDevice';
import {markRaw, reactive} from 'vue';
import {PIDCoefficients} from '../types';

export enum CalibrationBits {
	Gyroscope = 0x0
}

export enum SensorBits {
	Compass = 0x0,
	Barometer = 0x1,
	GPS = 0x2
}

export enum ReceiverBits {
	Available = 0x0,
	Failsafe = 0x1
}

export enum ErrorBits {}

export class Device {
	usbDevice: USBDevice | SimulatedUSBDevice;
	_transferPromise: Promise<void | DeviceResponse> = Promise.resolve();
	settings = reactive<{
		inputChannelNumber: number;
		outputChannelNumber: number;
	}>({
		inputChannelNumber: 0,
		outputChannelNumber: 0
	});

	constructor(usbDevice: USBDevice | SimulatedUSBDevice) {
		this.usbDevice = markRaw(usbDevice);
		this._request<ReturnSettingsResponse>(new GetVariableCommand(VariableID.Settings)).then(
			(settings) => {
				this.settings.inputChannelNumber = settings.inputChannelNumber;
				this.settings.outputChannelNumber = settings.outputChannelNumber;
			}
		);
		this.getSettings().then((settings) => {
			this.settings.inputChannelNumber = settings.inputChannelNumber;
			this.settings.outputChannelNumber = settings.outputChannelNumber;
		});
	}

	get deviceVersion(): string {
		return (
			this.usbDevice.deviceVersionMajor +
			'.' +
			this.usbDevice.deviceVersionMinor +
			'.' +
			this.usbDevice.deviceVersionSubminor
		);
	}

	get usbVersion(): string {
		return (
			this.usbDevice.usbVersionMajor +
			'.' +
			this.usbDevice.usbVersionMinor +
			'.' +
			this.usbDevice.usbVersionSubminor
		);
	}

	async _send(command: DeviceCommand): Promise<void> {
		return (this._transferPromise = this._transferPromise
			.then(() => this.usbDevice.transferOut(1, command.buffer))
			.catch((err) => Promise.reject(err))
			.then(() => Promise.resolve())) as Promise<void>;
	}

	async _request<T extends DeviceResponse>(command: GetVariableCommand): Promise<T> {
		return (this._transferPromise = this._transferPromise
			.then(() => this.usbDevice.transferOut(1, command.buffer))
			.then(() => this.usbDevice.transferIn(1, 0xff))
			.then((r) => parseDeviceResponse(new Uint8Array(r.data.buffer)) as T)) as Promise<T>;
	}

	async getStatus(): Promise<ReturnStatusResponse> {
		return await this._request(new GetVariableCommand(VariableID.Status));
	}

	async getSensors(): Promise<ReturnSensorsResponse> {
		return await this._request(new GetVariableCommand(VariableID.Sensors));
	}

	async getSettings(): Promise<ReturnSettingsResponse> {
		return await this._request(new GetVariableCommand(VariableID.Settings));
	}

	async getInputs(): Promise<ReturnArrayVariableResponse> {
		return await this._request(new GetVariableCommand(VariableID.Inputs));
	}

	async getMixes(): Promise<ReturnArrayVariableResponse> {
		return await this._request(new GetVariableCommand(VariableID.Mixes));
	}

	async getTrims(): Promise<ReturnArrayVariableResponse> {
		return await this._request<ReturnArrayVariableResponse>(
			new GetVariableCommand(VariableID.Trims)
		);
	}

	async getLimits(): Promise<ReturnArrayVariableResponse> {
		return await this._request(new GetVariableCommand(VariableID.Limits));
	}

	async getOutputs(): Promise<ReturnArrayVariableResponse> {
		return await this._request(new GetVariableCommand(VariableID.Outputs));
	}

	async getPIDs(): Promise<ReturnPIDsResponse> {
		return await this._request(new GetVariableCommand(VariableID.PIDs));
	}

	async setMixes(mixes: number[] | number[][]): Promise<void> {
		return await this._send(new SetArrayVariableCommand(VariableID.Mixes, mixes.flat()));
	}

	async setTrims(trims: number[]): Promise<void> {
		return await this._send(new SetArrayVariableCommand(VariableID.Trims, trims));
	}

	async setLimits(limits: number[] | number[][]): Promise<void> {
		return await this._send(new SetArrayVariableCommand(VariableID.Limits, limits.flat()));
	}

	async setPIDs(pids: PIDCoefficients[]): Promise<void> {
		return await this._send(new SetPIDsCommand(pids));
	}
}
