<template lang="pug">
.serial.max-w-screen-lg.my-4.mx-auto.px-4
	.send
		h3.my-4.text-xl.font-bold Send commands to motors
		form(@submit.prevent="sendCommand(stringToBuffer(commandString))")
			input.block(type="text" v-model="commandString" placeholder="31 03 F2 A6")
			button.mt-2(type="submit") Send
			button.mt-2.ml-2(type="button" @click="commandEntries = []") Clear history
	p.text-red.font-bold.my-2.
		Warning: This tool allows to execute any command on the device,
		and sending an invalid or malformed command could lead to unpredictable behavior or damage to the device
	h3.my-4.text-xl.font-bold History
	.command.card.my-4(v-for="entry in commandEntries" :key="entry.id")
		div(v-if="entry.message")
			p.text-lg.text-accent(v-if="!entry.message.srcAddr") Controller → Motor {{entry.message.destAddr}}
			p.text-lg.text-accent(v-else) Motor {{entry.message.destAddr}} → Controller
			p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
			p.whitespace-pre-wrap {{parser.parse(entry.message.buffer)}}
			button.mt-2(v-if="!entry.message.srcAddr" @click="sendCommand(entry.message.buffer)") Re-send
		div(v-else)
			p.text-lg.text-accent No response
			p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
</template>

<script setup lang="ts">
import {ref} from 'vue';
import {connectedDevice} from '../scripts/driver/driver';
import {SerialParser} from '../scripts/driver/serial/SerialParser';
import {SerialMessage} from '../scripts/driver/serial/SerialMessage';
import {
	MotorCommand,
	MotorCommandError,
	motorErrorDescriptions
} from '../scripts/driver/serial/MotorCommand';
import {MotorResponse} from '../scripts/driver/serial/MotorResponse';
import {alert, PopupColor} from '../scripts/popups';

type CommandEntry = {
	time: number;
	message: SerialMessage;
	id: number;
};

let lastID: number = 0;
const commandEntries = ref<CommandEntry[]>([]);
const parser = new SerialParser();
const historySize = ref<number>(20);
const commandString = ref<string>('');

function stringToBuffer(string: string): Uint8Array {
	return Uint8Array.from(string.match(/[0-9a-fA-F]{1,2}/g).map((byte) => parseInt(byte, 16)));
}

function bufferToString(buffer: Uint8Array): string {
	return new Array(buffer.length)
		.fill(0)
		.map((_, i) => buffer[i].toString(16).padStart(2, '0').toUpperCase())
		.join(' ');
}

function sendCommand(buffer: Uint8Array): void {
	const command = new MotorCommand(buffer);

	commandString.value = bufferToString(command.buffer);

	if (command.error !== MotorCommandError.NoError) {
		alert('Invalid command', PopupColor.Red, motorErrorDescriptions[command.error]);
		console.error(command);
		return;
	}

	commandEntries.value.unshift({
		time: Date.now(),
		message: command,
		id: lastID++
	});

	commandEntries.value.length = Math.min(commandEntries.value.length, historySize.value);

	connectedDevice.value.motors
		.request(command)
		.then((response: MotorResponse) => {
			commandEntries.value.unshift({
				time: Date.now(),
				message: response,
				id: lastID++
			});

			while (commandEntries.value.length > historySize.value) {
				commandEntries.value.pop();
			}
		})
		.catch((): null => null);
}

function formatTime(time: number): string {
	if (time < 100) {
		return 'Just now';
	}

	return (time / 1000).toFixed(1) + ' sec ago';
}
</script>

<style scoped></style>
