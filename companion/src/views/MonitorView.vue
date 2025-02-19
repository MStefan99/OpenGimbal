<template lang="pug">
.monitor.max-w-screen-lg.my-4.mx-auto.px-4
	h2.mb-4.text-xl.font-bold Commands
	div
		label Slow mode
		input.ml-2(type="checkbox" v-model="slowMode")
	div
		label History size
		RangeSlider.inline-block.ml-2(v-model="historySize" :min="1" :max="200")
	.command.card.my-4(v-for="entry in commandEntries" :key="entry.id")
		p.text-lg.text-accent(v-if="!entry.command.srcAddr") Controller → Motor {{entry.command.destAddr}}
		p.text-lg.text-accent(v-else) Motor {{entry.command.destAddr}} → Controller
		p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
		p.whitespace-pre-wrap {{entry.command}}
</template>

<script setup lang="ts">
import {onMounted, onUnmounted, ref} from 'vue';
import {connectedSerialDevice} from '../scripts/driver/serial/serialDriver';
import {SerialParser} from '../scripts/driver/serial/SerialParser';
import {MotorCommand, motorCommandNames} from '../scripts/driver/serial/MotorCommand';
import {MotorResponse} from '../scripts/driver/serial/MotorResponse';
import RangeSlider from '../components/RangeSlider.vue';

type CommandEntry = {
	time: number;
	command: MotorCommand | MotorResponse;
	id: number;
};

let lastID: number = 0;
let lastUpdate: number = 0;
const commandEntries = ref<CommandEntry[]>([]);
const parser = new SerialParser();
const slowMode = ref<boolean>(true);
const historySize = ref<number>(20);

function onData(event: Event): void {
	if (!('detail' in event)) {
		return;
	}
	const customEvent = event as CustomEvent;

	const commands = parser.parse(customEvent.detail);

	if (slowMode.value) {
		const now = Date.now();
		if (now - lastUpdate < 200) {
			return;
		}
		lastUpdate = now;
	}

	if (commands.length) {
		const now = Date.now();
		commandEntries.value = commands
			.map(
				(command) =>
					({
						time: now,
						command,
						id: lastID++
					}) as CommandEntry
			)
			.concat(commandEntries.value);
	}

	while (commandEntries.value.length > historySize.value) {
		commandEntries.value.pop();
	}
}

function formatTime(time: number): string {
	if (time < 100) {
		return 'Just now';
	}

	return (time / 1000).toFixed(1) + ' sec ago';
}

onMounted(() => connectedSerialDevice.value.addEventListener('data', onData));
onUnmounted(() => connectedSerialDevice.value?.removeEventListener('data', onData));
</script>

<style scoped></style>
