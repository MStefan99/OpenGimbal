<template lang="pug">
.popups
	.alerts
		TransitionGroup(name="popup")
			.alert(v-for="alert in activeAlerts" :key="alert.title" :class="alert.type")
				.title(:class="alert.type") {{alert.title}}
				.details {{alert.details}}
	Transition(name="popup")
		.confirm(v-if="activeConfirm" :class="activeConfirm.confirm.type")
			.title {{activeConfirm.confirm.title}}
			.details {{activeConfirm.confirm.details}}
			.mt-4
				button.mr-4(@click="resolveConfirm(true)" :class="activeConfirm.confirm.type + '-outline'") Yes
				button(@click="resolveConfirm(false)") No
	Transition(name="popup")
		.prompt(v-if="activePrompt" :class="activePrompt.prompt.type")
			.title {{activePrompt.prompt.title}}
			.details {{activePrompt.prompt.details}}
			form.flex.mt-4(@submit.prevent="resolvePrompt()")
				input.mr-4.flex-grow(
					type="text"
					v-model="promptValue"
					:class="activePrompt.prompt.type + '-outline'")
				button.mr-4(type="button" @click="rejectPrompt()") Cancel
				button(type="submit" :class="activePrompt.prompt.type") Submit
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
.popups {
	position: fixed;
	left: 0;
	right: 0;
	top: 5vh;
}

.alerts {
	position: absolute;
	right: 5vw;
	margin: 2em;
}

.confirm,
.prompt {
	position: relative;
	width: min(768px, 90vw);
	margin: 0 auto;
	@apply shadow-xl;
}

.alert,
.confirm,
.prompt {
	border: 6px solid var(--color-accent);
	color: var(--color-accent);
	background-color: var(--color-popup);
	-webkit-backdrop-filter: blur(1em);
	backdrop-filter: blur(1em);
	border-radius: 1ch;
	padding: 1em;
	margin-bottom: 2em;
}

.title {
	font-weight: bold;
	@apply text-xl mb-6;
}

.green {
	border-color: var(--color-green);
	color: var(--color-green);
}

.yellow {
	border-color: var(--color-yellow);
	color: var(--color-yellow);
}

.red {
	border-color: var(--color-red);
	color: var(--color-red);
}

form input {
	display: inline-block;
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
