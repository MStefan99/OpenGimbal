import {VariableID} from './DeviceCommand';
import {ReceiverBits, SensorBits} from './Device';
import {
	accelerationFactor,
	attitudeFactor,
	angularRateFactor,
	magneticFieldFactor,
	PIDCoefficients
} from '../types';
import {BitwiseRegister} from './BitwiseRegister';

export enum DeviceResponseType {
	ReturnVariable = 0x0
}

export const deviceResponseNames: Record<DeviceResponseType, string> = {
	[DeviceResponseType.ReturnVariable]: 'Return variable'
};

const deviceResponseParsers: Record<DeviceResponseType, (buffer: Uint8Array) => DeviceResponse> = {
	[DeviceResponseType.ReturnVariable]: (buffer) => new ReturnVariableResponse(buffer)
};

const returnVariableResponseParsers: Record<
	VariableID,
	(buffer: Uint8Array) => ReturnVariableResponse
> = {
	[VariableID.Status]: (buffer) => new ReturnStatusResponse(buffer),
	[VariableID.Sensors]: (buffer) => new ReturnSensorsResponse(buffer),
	[VariableID.Settings]: (buffer) => new ReturnSettingsResponse(buffer),
	[VariableID.Inputs]: (buffer) => new ReturnArrayVariableResponse(buffer),
	[VariableID.Mixes]: (buffer) => new ReturnArrayVariableResponse(buffer),
	[VariableID.Trims]: (buffer) => new ReturnArrayVariableResponse(buffer),
	[VariableID.Limits]: (buffer) => new ReturnArrayVariableResponse(buffer),
	[VariableID.Outputs]: (buffer) => new ReturnArrayVariableResponse(buffer),
	[VariableID.PIDs]: (buffer) => new ReturnPIDsResponse(buffer)
};

export function parseDeviceResponse(data: Uint8Array): DeviceResponse {
	const genericResponse = new DeviceResponse(data);
	const specificResponse = deviceResponseParsers[genericResponse.type](data);

	if (specificResponse instanceof ReturnVariableResponse) {
		return returnVariableResponseParsers[specificResponse.variableID](data);
	}

	return specificResponse;
}

export class DeviceResponse {
	buffer: Uint8Array;
	view: DataView;

	constructor(buffer: Uint8Array);
	constructor(cmdType: DeviceResponseType, cmdData?: Uint8Array);
	constructor(cmdType?: DeviceResponseType | Uint8Array, cmdData: Uint8Array = new Uint8Array()) {
		if (cmdType instanceof Uint8Array) {
			this.buffer = cmdType;
			this.view = new DataView(this.buffer.buffer);
		} else {
			this.buffer = new Uint8Array(1 + cmdData.length);
			this.view = new DataView(this.buffer.buffer);
			this.view.setUint8(0, cmdType);
			this.buffer.set(cmdData, 1);
		}
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	get type(): DeviceResponseType {
		return this.view.getUint8(0);
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${deviceResponseNames[this.view.getUint8(0) as DeviceResponseType]} response`;
		}
	}
}

export class ReturnVariableResponse extends DeviceResponse {
	constructor(buffer: Uint8Array);
	constructor(id: VariableID, buffer: Uint8Array);
	constructor(id: Uint8Array | VariableID, value?: Uint8Array) {
		if (id instanceof Uint8Array) {
			super(id);
		} else {
			const buffer = new Uint8Array(1 + value.byteLength);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, id);
			buffer.set(value, 1);

			super(DeviceResponseType.ReturnVariable, buffer);
		}
	}

	get variableID(): VariableID {
		return this.view.getUint8(1);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return super.toString() + `\n  Variable: ${VariableID[this.variableID]}`;
		}
	}
}

export class ReturnStatusResponse extends ReturnVariableResponse {
	constructor(
		data:
			| Uint8Array
			| {
					yaw?: number;
					pitch?: number;
					roll?: number;
					receiverStatus?: BitwiseRegister<ReceiverBits>;
			  }
	) {
		if (data instanceof Uint8Array) {
			super(data);
		} else {
			const buffer = new Uint8Array(7);
			const view = new DataView(buffer.buffer);

			view.setInt16(0, (data.yaw ?? 0) / attitudeFactor, true);
			view.setInt16(2, (data.pitch ?? 0) / attitudeFactor, true);
			view.setInt16(4, (data.roll ?? 0) / attitudeFactor, true);
			view.setUint8(6, data.receiverStatus?.value ?? 0);

			super(VariableID.Status, buffer);
		}
	}

	get yaw(): number {
		return this.view.getInt16(2, true) * attitudeFactor;
	}

	get pitch(): number {
		return this.view.getInt16(4, true) * attitudeFactor;
	}

	get roll(): number {
		return this.view.getInt16(6, true) * attitudeFactor;
	}

	get receiverStatus(): BitwiseRegister<ReceiverBits> {
		return new BitwiseRegister<ReceiverBits>(this.view.getUint8(8));
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  Yaw: ${this.yaw}` +
				`\n  Pitch: ${this.pitch}` +
				`\n  Roll: ${this.roll}`
			);
		}
	}
}

export class ReturnSensorsResponse extends ReturnVariableResponse {
	constructor(
		data:
			| Uint8Array
			| {
					temperature?: number;
					activeSensors?: BitwiseRegister<SensorBits>;
					accelerations?: number[];
					angularRates?: number[];
					magneticFields?: number[];
					airPressure?: number;
			  }
	) {
		if (data instanceof Uint8Array) {
			super(data);
		} else {
			const buffer = new Uint8Array(22);
			const view = new DataView(buffer.buffer);

			view.setUint8(0, data.activeSensors?.value ?? 0);
			view.setInt8(1, data.temperature ?? 0);
			for (let i = 0; i < 3; ++i) {
				view.setInt16(
					2 + i * Int16Array.BYTES_PER_ELEMENT,
					(data.accelerations?.[i] ?? 0) / accelerationFactor,
					true
				);
				view.setInt16(
					8 + i * Int16Array.BYTES_PER_ELEMENT,
					(data.angularRates?.[i] ?? 0) / angularRateFactor,
					true
				);
				view.setInt16(
					14 + i * Int16Array.BYTES_PER_ELEMENT,
					(data.magneticFields?.[i] ?? 0) / magneticFieldFactor,
					true
				);
			}
			view.setUint16(20, data.airPressure ?? 0, true);

			super(VariableID.Sensors, buffer);
		}
	}

	get activeSensors(): BitwiseRegister<SensorBits> {
		return new BitwiseRegister<SensorBits>(this.view.getUint8(2));
	}

	get temperature(): number {
		return this.view.getInt8(3);
	}

	get accelerations(): number[] {
		return new Array<number>(3)
			.fill(0)
			.map(
				(v, i) =>
					this.view.getInt16(4 + i * Int16Array.BYTES_PER_ELEMENT, true) * accelerationFactor
			);
	}

	get angularRates(): number[] {
		return new Array<number>(3)
			.fill(0)
			.map(
				(v, i) =>
					this.view.getInt16(10 + i * Int16Array.BYTES_PER_ELEMENT, true) * angularRateFactor
			);
	}

	get magneticFields(): number[] {
		return new Array<number>(3)
			.fill(0)
			.map(
				(v, i) =>
					this.view.getInt16(16 + i * Int16Array.BYTES_PER_ELEMENT, true) * magneticFieldFactor
			);
	}

	get airPressure(): number {
		return this.view.getUint16(22, true);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  Active sensors: ${this.activeSensors}` +
				`\n  Temperature: ${this.temperature}` +
				`\n  Acceleration: ${this.accelerations}` +
				`\n  Angular rates: ${this.angularRates}` +
				`\n  Magnetic fields: ${this.magneticFields}` +
				`\n  Air pressure: ${this.airPressure}`
			);
		}
	}
}

export class ReturnSettingsResponse extends ReturnVariableResponse {
	constructor(data: {inputChannelNumber: number; outputChannelNumber: number} | Uint8Array) {
		if (data instanceof Uint8Array) {
			super(data);
		} else {
			const buffer = new Uint8Array(2);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, data.inputChannelNumber ?? 0);
			view.setUint8(1, data.outputChannelNumber ?? 0);
			super(VariableID.Settings, buffer);
		}
	}

	get inputChannelNumber(): number {
		return this.view.getUint8(2);
	}

	get outputChannelNumber(): number {
		return this.view.getUint8(3);
	}
}

export class ReturnPIDsResponse extends ReturnVariableResponse {
	constructor(data: PIDCoefficients[] | Uint8Array) {
		if (data instanceof Uint8Array) {
			super(data);
		} else {
			const buffer = new Uint8Array(2 + data.length * Float32Array.BYTES_PER_ELEMENT * 3);
			const view = new DataView(buffer.buffer);

			for (let i = 0; i < data.length; ++i) {
				view.setFloat32(2 + i * Float32Array.BYTES_PER_ELEMENT * 3, data[i].kp, true);
				view.setFloat32(
					2 + i * Float32Array.BYTES_PER_ELEMENT * 3 + Float32Array.BYTES_PER_ELEMENT,
					data[i].ki,
					true
				);
				view.setFloat32(
					2 + i * Float32Array.BYTES_PER_ELEMENT * 3 + Float32Array.BYTES_PER_ELEMENT * 2,
					data[i].kd,
					true
				);
			}

			super(VariableID.PIDs, buffer);
		}
	}

	get PIDs(): PIDCoefficients[] {
		return new Array<PIDCoefficients>((this.length - 4) / (Float32Array.BYTES_PER_ELEMENT * 3))
			.fill({kp: 0, ki: 0, kd: 0})
			.map((v, i) => ({
				kp: this.view.getFloat32(4 + 3 * Float32Array.BYTES_PER_ELEMENT * i, true),
				ki: this.view.getFloat32(
					4 + 3 * Float32Array.BYTES_PER_ELEMENT * i + Float32Array.BYTES_PER_ELEMENT,
					true
				),
				kd: this.view.getFloat32(
					4 + 3 * Float32Array.BYTES_PER_ELEMENT * i + Float32Array.BYTES_PER_ELEMENT * 2,
					true
				)
			}));
	}
}

export class ReturnArrayVariableResponse extends ReturnVariableResponse {
	constructor(buffer: Uint8Array);
	constructor(id: VariableID, value: number | number[]);
	constructor(id: Uint8Array | VariableID, value?: number | number[]) {
		if (id instanceof Uint8Array) {
			super(id);
		} else {
			if (!Array.isArray(value)) {
				value = [value];
			}

			const buffer = new Uint8Array(value.length * Int16Array.BYTES_PER_ELEMENT);
			const view = new DataView(buffer.buffer);

			for (let i = 0; i < value.length; i++) {
				view.setInt16(i * Int16Array.BYTES_PER_ELEMENT, value[i], true);
			}

			super(id, buffer);
		}
	}

	get values(): number[] {
		return new Array((this.length - 2) / Int16Array.BYTES_PER_ELEMENT)
			.fill(0)
			.map((v, i) => this.view.getInt16(2 + i * Int16Array.BYTES_PER_ELEMENT, true));
	}
}
