<template lang="pug">
.popups
	.alerts
		TransitionGroup(name="popup")
			.alert(v-for="alert in activeAlerts" :key="alert.title" :class="alert.type")
				.title {{alert.title}}
				.details {{alert.details}}
	Transition(name="popup")
		.confirm(v-if="activeConfirm" :class="activeConfirm.confirm.type")
			.title {{activeConfirm.confirm.title}}
			.details {{activeConfirm.confirm.details}}
			.mt-4
				button.mr-4(@click="resolveConfirm(true)") Yes
				button(@click="resolveConfirm(false)") No
	Transition(name="popup")
		.prompt(v-if="activePrompt" :class="activePrompt.prompt.type")
			.title {{activePrompt.prompt.title}}
			.details {{activePrompt.prompt.details}}
			form.flex.mt-4(@submit.prevent="resolvePrompt()")
				input.mr-4.flex-grow(type="text" v-model="promptValue")
				button.mr-4(type="button" @click="rejectPrompt()") Cancel
				button(type="submit") Submit
</template>

<script setup lang="ts">
import {activeAlerts, activeConfirm, activePrompt} from '../scripts/popups';
import {ref} from 'vue';

const promptValue = ref<string>('');

function resolveConfirm(value: boolean): void {
	activeConfirm.value?.resolve(value);
}

function resolvePrompt(): void {
	activePrompt.value?.resolve(promptValue.value);
	promptValue.value = '';
}

function rejectPrompt(): void {
	activePrompt.value.reject(new Error('Prompt canceled'));
	promptValue.value = '';
}
</script>

<style scoped>
@reference '../assets/style.css';

.popups {
	@apply mx-4 fixed left-0 right-0 top-20 pointer-events-none;
}

.confirm,
.prompt {
	@apply relative max-w-full mx-auto shadow-xl pointer-events-auto;
}

.alert,
.confirm,
.prompt {
	@apply max-w-2xl mx-auto p-4 mb-4 border font-semibold rounded-xl right-0 m-4 shadow-xl backdrop-blur-lg
	text-accent dark:text-accent-bright border-accent/50 bg-accent/5 dark:bg-accent/30;
}

.title {
	@apply text-xl mb-6 font-bold;
}

.red {
	@apply text-red-800 dark:text-red-300 border-red-800/50 bg-red-800/5 dark:bg-red-800/30;
}

.yellow {
	@apply text-yellow-800 dark:text-yellow-300 border-yellow-800/50 bg-yellow-800/5 dark:bg-yellow-800/30;
}

.green {
	@apply text-green-800 dark:text-green-300 border-green-800/50 bg-green-800/5 dark:bg-green-800/30;
}

button {
	@apply hover:bg-accent-bright dark:hover:bg-accent-bright;
}

.red button {
	@apply text-red-800 dark:text-red-300 border border-red-800 dark:border-red-300 transition-colors;
}

.red button:hover {
	@apply bg-red-800 dark:bg-red-300 text-zinc-50;
}

.yellow button {
	@apply text-yellow-800 dark:text-yellow-300 border border-yellow-800 dark:border-yellow-300 transition-colors;
}

.yellow button:hover {
	@apply bg-yellow-800 dark:bg-yellow-300 text-zinc-50;
}

.green button {
	@apply text-green-800 dark:text-green-300 border border-green-800 dark:border-green-300 transition-colors;
}

.green button:hover {
	@apply bg-green-800 dark:bg-green-300 text-zinc-50;
}

form input {
	@apply inline-block font-medium w-0 bg-transparent grow;
}

.red form input {
	@apply border-red-800 dark:border-red-300 outline-red-800 dark:outline-red-300;
}

.yellow form input {
	@apply border-yellow-800 dark:border-yellow-300 outline-yellow-800 dark:outline-yellow-300;
}

.green form input {
	@apply border-green-800 dark:border-green-300 outline-green-800 dark:outline-green-300;
}

input:focus {
	@apply outline-2;
}

.move,
.popup-enter-active,
.popup-leave-active {
	transition: all 0.5s ease;
}

.popup-enter-from,
.popup-leave-to {
	transform: translateY(-50%);
	opacity: 0;
}
</style>
