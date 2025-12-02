<template lang="pug">
.usb.max-w-screen-lg.my-4.mx-auto.px-4
	p.text-red.font-bold.my-2(v-if="!(connectedDevice instanceof Gimbal)") Gimbal must be connected over USB
	template(v-else)
		.send
			h3.my-4.text-xl.font-bold Send commands to controller
			form(@submit.prevent="sendCommand(stringToBuffer(commandString))")
				input.block(type="text" v-model="commandString")
				button.mt-2(type="submit") Send
				button.mt-2.ml-2(type="button" @click="commandEntries = []") Clear history
		p.text-red.font-bold.my-2.
			Warning: This tool allows to execute any command on the device,
			and sending an invalid or malformed command could lead to unpredictable behavior or damage the device
		h3.my-4.text-xl.font-bold History
		.command.card.my-4(v-for="entry in commandEntries" :key="entry.id")
			div(v-if="entry.message")
				template(v-if="entry.message instanceof ControllerCommand")
					p.whitespace-pre-wrap {{parser.parseCommand(entry.message.buffer)}}
					button.mt-2(@click="sendCommand(entry.message.buffer)") Re-send
				template(v-else)
					p.whitespace-pre-wrap {{parser.parseResponse(entry.message.buffer)}}
			div(v-else)
				p.text-lg.text-accent No response
				p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
</template>

<script setup lang="ts">
import {onMounted, onUnmounted, ref, watch} from 'vue';
import {connectedDevice} from '../scripts/driver/driver';
import RangeSlider from '../components/RangeSlider.vue';
import {MotorResponse} from '../scripts/driver/motor/MotorResponse';
import {
	GetVariableCommand,
	ControllerCommand
} from '../scripts/driver/controller/ControllerCommand';
import {ControllerMessage} from '../scripts/driver/controller/ControllerMessage';
import {Gimbal} from '../scripts/driver/Gimbal';
import {ControllerParser} from '../scripts/driver/controller/ControllerParser';

type CommandEntry = {
	time: number;
	message: ControllerMessage;
	id: number;
};

let lastID: number = 0;
const commandEntries = ref<CommandEntry[]>([]);
const parser = new ControllerParser();
const historySize = ref<number>(20);
const commandString = ref<string>('');

function stringToBuffer(str: string): Uint8Array {
	return Uint8Array.from(
		commandString.value.match(/[0-9a-fA-F]{1,2}/g).map((byte) => parseInt(byte, 16))
	);
}

function bufferToString(buffer: Uint8Array): string {
	return new Array(buffer.length)
		.fill(0)
		.map((_, i) => buffer[i].toString(16).padStart(2, '0').toUpperCase())
		.join(' ');
}

function sendCommand(buffer: Uint8Array): void {
	commandString.value = bufferToString(buffer);

	if (!(connectedDevice.value instanceof Gimbal)) {
		return;
	}

	const command = parser.parseCommand(buffer);

	commandEntries.value.unshift({
		time: Date.now(),
		message: command,
		id: lastID++
	});

	commandEntries.value.length = Math.min(commandEntries.value.length, historySize.value);

	if (command instanceof GetVariableCommand) {
		connectedDevice.value
			.request(command)
			.then((response) => {
				commandEntries.value.unshift({
					time: Date.now(),
					message: response,
					id: lastID++
				});

				commandEntries.value.length = Math.min(commandEntries.value.length, historySize.value);
			})
			.catch((): null => null);
	} else {
		connectedDevice.value.send(command);
	}
}

function formatTime(time: number): string {
	if (time < 100) {
		return 'Just now';
	}

	return (time / 1000).toFixed(1) + ' sec ago';
}
</script>

<style scoped></style>
