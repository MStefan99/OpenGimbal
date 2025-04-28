import {Message} from '../Message';

export enum GimbalCommandType {
	Disable = 0x00,
	Enable = 0x01,
	SetVariable = 0xd,
	GetVariable = 0xe,
	MotorPassthrough = 0x0f
}

export enum GimbalResponseType {
	ReturnVariable = 0xe,
	MotorPassthrough = 0xf
}

export enum GimbalVariable {
	Orientation = 0x00,
	HandleOrientation = 0x01,
	Mode = 0x02,
	BatteryVoltage = 0x03
}

export enum GimbalMode {
	Horizon = 0x00,
	Follow = 0x01,
	FPV = 0x02,
	Tilt = 0x03
}

export class USBMessage extends Message {
	constructor(buffer: Uint8Array);
	constructor(type: GimbalCommandType | GimbalResponseType, data?: Uint8Array);

	constructor(buffer: Uint8Array | GimbalCommandType | GimbalResponseType, data?: Uint8Array) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			super(new Uint8Array(1 + (data?.byteLength ?? 0)));

			this.view.setUint8(0, buffer);
			data && this.buffer.set(data, 1);
		}
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `Message ${this.view.getUint8(0)}`;
		}
	}
}
