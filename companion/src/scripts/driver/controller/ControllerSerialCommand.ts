import {Message} from '../Message';
import {
	ControllerCommandType,
	ControllerMessage,
	ControllerResponseType
} from './ControllerMessage';
import {
	ControllerCommand,
	controllerCommandNames,
	controllerCommands,
	SetVariableCommand,
	setVariableCommands
} from './ControllerCommand';

export class ControllerSerialCommand extends Message {
	constructor(buffer: Uint8Array);
	constructor(type: ControllerCommand);

	constructor(buffer: Uint8Array | ControllerCommand) {
		if (buffer instanceof Uint8Array) {
			super(buffer);
		} else {
			super(new Uint8Array(1 + (buffer.length ?? 0)));

			this.view.setUint8(0, buffer.length);
			buffer.length && this.buffer.set(buffer.buffer, 1);
		}
	}

	get length(): number {
		return this.buffer.byteLength;
	}

	get controllerCommand(): ControllerCommand {
		const command = new ControllerCommand(
			new Uint8Array(this.length - 1).fill(0).map((v, i) => this.buffer[i + 1])
		);

		const controllerCommand = controllerCommands[command.type](command.buffer);

		if (controllerCommand instanceof SetVariableCommand) {
			return setVariableCommands[controllerCommand.variable](controllerCommand.buffer);
		} else {
			return controllerCommand;
		}
	}

	toString(type?: 'hex'): string {
		if (type === 'hex') {
			return new Array(this.buffer.byteLength)
				.fill(0)
				.map((v, idx) => this.view.getUint8(idx).toString(16).padStart(2, '0'))
				.join(' ');
		} else {
			return `${controllerCommandNames[this.view.getUint8(1) as ControllerCommandType]} command`;
		}
	}
}
