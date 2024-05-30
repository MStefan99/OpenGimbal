import {BitwiseRegister} from './BitwiseRegister';
import {CalibrationBits} from './Device';
import {PIDCoefficients} from '../types';

export enum DeviceCommandType {
	GetVariable = 0x0,
	SetVariable = 0x1,
	Calibration = 0x2
}

export enum VariableID {
	Status = 0x0,
	Sensors = 0x1,
	Settings = 0x2,
	Inputs = 0x3,
	Mixes = 0x4,
	Trims = 0x5,
	Limits = 0x6,
	Outputs = 0x7,
	PIDs = 0x8
}

export const deviceCommandNames: Record<DeviceCommandType, string> = {
	[DeviceCommandType.GetVariable]: 'Get variable',
	[DeviceCommandType.SetVariable]: 'Set variable',
	[DeviceCommandType.Calibration]: 'Calibration'
};

export const deviceCommandParsers: Record<
	DeviceCommandType,
	(buffer: Uint8Array) => DeviceCommand
> = {
	[DeviceCommandType.GetVariable]: (buffer) => new GetVariableCommand(buffer),
	[DeviceCommandType.SetVariable]: (buffer) => new SetVariableCommand(buffer),
	[DeviceCommandType.Calibration]: (buffer) => new CalibrationCommand(buffer)
};

export const setVariableCommandParsers: Record<
	VariableID,
	(buffer: Uint8Array) => SetVariableCommand
> = {
	[VariableID.Status]: (buffer) => {
		throw new Error('Status cannot be set');
	},
	[VariableID.Sensors]: (buffer) => {
		throw new Error('Sensors cannot be set');
	},
	[VariableID.Settings]: (buffer) => {
		throw new Error('Settings cannot be set');
	},
	[VariableID.Inputs]: (buffer) => {
		throw new Error('Inputs cannot be set');
	},
	[VariableID.Mixes]: (buffer) => new SetArrayVariableCommand(buffer),
	[VariableID.Trims]: (buffer) => new SetArrayVariableCommand(buffer),
	[VariableID.Limits]: (buffer) => new SetArrayVariableCommand(buffer),
	[VariableID.Outputs]: (buffer) => {
		throw new Error('Outputs cannot be set');
	},
	[VariableID.PIDs]: (buffer) => new SetPIDsCommand(buffer)
};

export function parseDeviceCommand(data: Uint8Array): DeviceCommand {
	const genericResponse = new DeviceCommand(data);
	const specificResponse = deviceCommandParsers[genericResponse.type](data);

	if (specificResponse instanceof GetVariableCommand) {
		return new GetVariableCommand(data);
	} else if (specificResponse instanceof SetVariableCommand) {
		return setVariableCommandParsers[specificResponse.variableID](data);
	}

	return specificResponse;
}

export class DeviceCommand {
	buffer: Uint8Array;
	view: DataView;

	constructor(buffer: Uint8Array);
	constructor(cmdType: DeviceCommandType, cmdData?: Uint8Array);
	constructor(cmdType?: DeviceCommandType | Uint8Array, cmdData: Uint8Array = new Uint8Array()) {
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

	get type(): DeviceCommandType {
		return this.view.getUint8(0);
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${deviceCommandNames[this.view.getUint8(0) as DeviceCommandType]} command`;
		}
	}
}

export class CalibrationCommand extends DeviceCommand {
	constructor(buffer: Uint8Array);
	constructor(mode: BitwiseRegister<CalibrationBits>);
	constructor(mode: Uint8Array | BitwiseRegister<CalibrationBits>) {
		if (mode instanceof Uint8Array) {
			super(mode);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(1, mode.value);
			super(DeviceCommandType.Calibration, buffer);
		}
	}

	get calibrationMode(): Array<CalibrationBits> {
		return new Array(8)
			.fill(0)
			.map((v, i) => ((1 << i) & this.view.getUint8(2) ? i : null))
			.filter((v) => v !== null);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return (
				super.toString() +
				`\n  Mode: ${this.calibrationMode.map((v) => CalibrationBits[v]).join(', ')}`
			);
		}
	}
}

export class GetVariableCommand extends DeviceCommand {
	constructor(buffer: Uint8Array);
	constructor(id: VariableID);

	constructor(id: Uint8Array | VariableID) {
		if (id instanceof Uint8Array) {
			super(id);
		} else {
			const buffer = new Uint8Array(1);
			const view = new DataView(buffer.buffer);
			view.setUint8(0, id);
			super(DeviceCommandType.GetVariable, buffer);
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

export class SetVariableCommand extends DeviceCommand {
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

			super(DeviceCommandType.SetVariable, buffer);
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

export class SetPIDsCommand extends SetVariableCommand {
	constructor(buffer: Uint8Array);
	constructor(value: PIDCoefficients[] | Uint8Array);
	constructor(data: Uint8Array | PIDCoefficients[]) {
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

export class SetArrayVariableCommand extends SetVariableCommand {
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
