<template lang="pug">
.monitor.max-w-screen-lg.my-4.mx-auto.px-4
	h2.mb-4.text-xl.font-bold Commands
	.command.card.my-4(v-for="entry in commandEntries" :key="entry.id")
		p.text-lg.text-accent(v-if="!entry.command.srcAddr") Controller → Motor {{entry.command.destAddr}}
		p.text-lg.text-accent(v-else) Motor {{entry.command.destAddr}} → Controller
		p.text-slate-400 {{formatTime(Date.now() - entry.time)}}
		p.whitespace-pre-wrap {{entry.command}}
</template>

<script setup lang="ts">
import {onMounted, onUnmounted, ref} from 'vue';
import {activeDevice} from '../scripts/driver/driver';
import {Parser} from '../scripts/driver/parser';
import {MotorCommand, motorCommandNames} from '../scripts/driver/MotorCommand';
import {MotorResponse} from '../scripts/driver/MotorResponse';

type CommandEntry = {
	time: number;
	command: MotorCommand | MotorResponse;
	id: number;
};

let lastID: number = 0;
let lastUpdate: number = 0;
const commandEntries = ref<CommandEntry[]>([]);
const parser = new Parser();

function onData(event: CustomEvent): void {
	const commands = parser.parse(event.detail);

	const now = Date.now();
	if (now - lastUpdate < 200) {
		return;
	}
	lastUpdate = now;

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

	while (commandEntries.value.length > 20) {
		commandEntries.value.pop();
	}
}

function formatTime(time: number): string {
	if (time < 100) {
		return 'Just now';
	}

	return (time / 1000).toFixed(1) + ' sec ago';
}

onMounted(() => activeDevice.value.addEventListener('data', onData));
onUnmounted(() => activeDevice.value?.removeEventListener('data', onData));
</script>

<style scoped></style>
