import {USBMessage} from './USBMessage';
import {getMotorCommand, MotorCommand, MotorCommandType} from '../serial/MotorCommand';
import {encapsulateSerialMessage, exposeSerialCommand} from './USBSerialEncapsulator';
import {SerialParser} from '../serial/SerialParser';

export enum GimbalCommandType {
	MotorPassthrough = 0xf
}

export const gimbalCommandNames: Record<GimbalCommandType, string> = {
	[GimbalCommandType.MotorPassthrough]: 'Set variable'
};

export const getGimbalCommand: Record<GimbalCommandType, (buffer: Uint8Array) => GimbalCommand> = {
	[GimbalCommandType.MotorPassthrough]: (buffer: Uint8Array) => new MotorPassthroughCommand(buffer)
};

export class GimbalCommand extends USBMessage {
	get type(): GimbalCommandType {
		return this.view.getUint8(0);
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.length)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${gimbalCommandNames[(this.view.getUint8(1) & 0xf) as GimbalCommandType]} command`;
		}
	}
}

export class MotorPassthroughCommand extends GimbalCommand {
	constructor(buffer: Uint8Array);
	constructor(motorCommand: MotorCommand);

	constructor(srcAddr: Uint8Array | MotorCommand) {
		if (srcAddr instanceof Uint8Array) {
			super(srcAddr);
		} else {
			super(srcAddr.buffer);
		}
	}

	get motorCommand(): MotorCommand {
		const message = new SerialParser().parse(exposeSerialCommand(this).buffer)[0];

		if (!(message instanceof MotorCommand)) {
			throw new Error('Could not parse motor command');
		}

		return message;
	}

	override toString(type?: 'hex'): string {
		if (type === 'hex') {
			return super.toString(type);
		} else {
			return 'Encapsulated motor command\n' + this.motorCommand.toString(type);
		}
	}
}
