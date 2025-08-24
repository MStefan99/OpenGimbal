<template lang="pug">
NavBar
main
	RouterView(v-if="connectedDevice")
	.no-device(v-else)
		p.mb-4 Start by clicking here ↑
		p Connect a device to start using OpenGimbal Companion
	PopupContainer

footer
	span OpenGimbal Companion
	span.clickable(v-if="crashCourse" @click="sendFeedback()") Send feedback
</template>

<script setup lang="ts">
import NavBar from './components/NavBar.vue';
import PopupContainer from './components/PopupContainer.vue';
import {connectedDevice} from './scripts/driver/driver';
import {crashCourse} from './scripts/analytics';
import {alert, PopupColor, prompt} from './scripts/popups';
import appState from './scripts/store';

let lastKeyPress = 0;
let inputStr = '';
const inputTimeout = 2000;

function sendFeedback(): void {
	prompt(
		'Send feedback',
		PopupColor.Accent,
		'Your feedback helps make OpenGimbal Companion better. Please type your message in the field below.'
	)
		.then((message) =>
			crashCourse.value?.sendFeedback(message).then((err) => {
				if (err) {
					alert(
						'Feedback was not sent',
						PopupColor.Red,
						err?.message || 'Failed to send feedback. Please check your connection and try again.'
					);
				} else {
					alert(
						'Feedback sent',
						PopupColor.Green,
						'Thank you! All feedback helps make OpenGimbal Companion better.'
					);
				}
			})
		)
		.catch((): null => null);
}

document.addEventListener('keydown', (event: KeyboardEvent) => {
	const now = Date.now();
	if (now - lastKeyPress >= inputTimeout) {
		inputStr = '';
	}
	lastKeyPress = now;
	inputStr += event.key;

	if (inputStr === 'developer') {
		appState.setDeveloperMode(true);
		alert(
			'Developer mode enabled',
			PopupColor.Accent,
			'Developer mode allows direct access to your device. Be careful when executing commands to avoid unexpected results'
		);
	} else if (inputStr === 'nodev') {
		appState.setDeveloperMode(false);
		alert('Developer mode disabled', PopupColor.Accent, 'Developer features have been disabled');
	}
});
</script>

<style>
@import './assets/style.css';

.no-device {
	@apply flex flex-row-reverse flex-wrap justify-between;
	margin: 16px;
	color: var(--color-accent);
	font-weight: bold;
	font-size: 1.2em;
}
</style>
