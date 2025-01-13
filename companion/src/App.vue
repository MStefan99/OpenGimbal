<template lang="pug">
NavBar
main
	RouterView(v-if="activeDevice")
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
import {activeDevice} from './scripts/driver/driver';
import PopupContainer from './components/PopupContainer.vue';
import {crashCourse} from './scripts/analytics';
import {PopupColor, alert, prompt} from './scripts/popups';

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
</script>

<style>
.no-device {
	@apply flex flex-row-reverse flex-wrap justify-between;
	margin: 16px;
	color: var(--color-accent);
	font-weight: bold;
	font-size: 1.2em;
}
</style>
