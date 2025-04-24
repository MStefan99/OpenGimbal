<template lang="pug">
.monitor.max-w-screen-lg.my-4.mx-auto.px-4
	h2.mb-4.text-xl.font-bold Options
	div
		label History size
		RangeSlider.inline-block.ml-2(v-model="historySize" :min="1" :max="100")
	.send
		h3.my-4.text-xl.font-bold Send command
		form(@submit.prevent="sendCommand(stringToBuffer(commandString))")
			input.block(type="text" v-model="commandString")
			button.mt-2(type="submit") Send
			button.mt-2.ml-2(type="button" @click="commandEntries = []") Clear history
	p.text-red.font-bold.my-2.
		Warning: This tool allows to execute any command on the device,
		and sending an invalid or malformed command could lead to unpredictable behavior or damage to the device
	h3.my-4.text-xl.font-bold History
	.command.card.my-4(v-for="entry in commandEntries" :key="entry.id")
		div(v-if="entry.command")
			p.text-lg.text-accent(v-if="!entry.command.srcAddr") Controller → Motor {{entry.command.destAddr}}
			p.text-lg.text-accent(v-else) Motor {{entry.command.destAddr}} → Controller
			p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
			p.whitespace-pre-wrap {{parser.parse(entry.command.buffer)}}
			button(v-if="!entry.command.srcAddr" @click="sendCommand(entry.command.buffer)") Re-send
		div(v-else)
			p.text-lg.text-accent No response
			p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
</template>

<script setup lang="ts">
import {onMounted, onUnmounted, ref, watch} from 'vue';
import {connectedDevice} from '../scripts/driver/driver';
import {SerialParser} from '../scripts/driver/serial/SerialParser';
import RangeSlider from '../components/RangeSlider.vue';
import {SerialMessage} from '../scripts/driver/serial/SerialMessage';
import {MotorCommand} from '../scripts/driver/serial/MotorCommand';
import {MotorResponse} from '../scripts/driver/serial/MotorResponse';

type CommandEntry = {
	time: number;
	command: SerialMessage;
	id: number;
};

let lastID: number = 0;
const commandEntries = ref<CommandEntry[]>([]);
const parser = new SerialParser();
const historySize = ref<number>(20);
const commandString = ref<string>('');

function stringToBuffer(str: string): Uint8Array {
	return Uint8Array.from(
		commandString.value.match(/[0-9a-fA-F]{1,2}/g).map((byte) => parseInt(byte, 16))
	);
}

function sendCommand(buffer: Uint8Array): void {
	const command = new MotorCommand(buffer);

	commandString.value = new Array(buffer.length)
		.fill(0)
		.map((_, i) => buffer[i].toString(16).padStart(2, '0').toUpperCase())
		.join(' ');

	commandEntries.value.unshift({
		time: Date.now(),
		command,
		id: lastID++
	});

	while (commandEntries.value.length > historySize.value) {
		commandEntries.value.pop();
	}

	connectedDevice.value
		.motor(command.destAddr)
		.request(command)
		.then((response: MotorResponse) => {
			commandEntries.value.unshift({
				time: Date.now(),
				command: response,
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
