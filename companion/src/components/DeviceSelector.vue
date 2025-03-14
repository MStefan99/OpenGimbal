<template lang="pug">
.popup-wrapper(@click.self="$emit('close')")
	.device-selector
		div(v-if="connectedDevice")
			span.bold.block Connected device
			.device
				span.cursor-pointer(@click="viewedDevice = connectedDevice") {{connectedDevice?.productName || 'Unknown'}}
				.flex.flex-row.flex-wrap.gap-4.grow
					button.red.grow(@click="disconnectDevice(connectedDevice)") Disconnect
		div(v-else)
			span.bold.block No devices connected
			span.bold.block.my-2 Connect
			form
				input#usb-radio(
					type="radio"
					name="type"
					value="usb"
					v-model="type"
					:checked="'usb' in navigator"
					:disabled="!('usb' in navigator)")
				label(for="usb-radio") USB
				input#serial-radio(
					type="radio"
					name="type"
					v-model="type"
					value="serial"
					:disabled="!('serial' in navigator)")
				label(for="serial-radio") Serial
			button.accent.bold.w-full.mt-2(v-if="'usb' in navigator" @click="connect()") Connect
			button.bold.accent-outline.w-full.mt-2(@click="connect(true)") Connect demo device
			button.close.red-outline.w-full.mt-2(@click="$emit('close')") Close
			.text-red.mt-4(v-if="!('usb' in navigator)")
				p(v-if="!window.isSecureContext").
					This page cannot access USB devices because it is using an insecure connection.
					Please open this page securely (over HTTPS) to connect your device.
				p(v-else).
					It looks like your browser does not support USB devices. Please try another browser.
			.text-red.mt-4(v-if="!('serial' in navigator)")
				p(v-if="!window.isSecureContext").
					This page cannot access serial devices because it is using an insecure connection.
					Please open this page securely (over HTTPS) to connect your device.
				p(v-else).
					It looks like your browser does not support serial devices. Please try another browser.
	Transition
		DeviceViewer(v-if="viewedDevice" :device="viewedDevice" @close="viewedDevice = null")
</template>

<script setup lang="ts">
import {ref} from 'vue';
import {alert, PopupColor} from '../scripts/popups';
import DeviceViewer from './DeviceViewer.vue';
import {MotorManager} from '../scripts/driver/MotorManager';
import {Gimbal} from '../scripts/driver/Gimbal';
import {connectDevice, connectedDevice, disconnectDevice} from '../scripts/driver/driver';

const emit = defineEmits<{(e: 'close'): void}>();
const viewedDevice = ref<MotorManager | Gimbal | null>(null);
const type = ref<'usb' | 'serial'>('usb');

function connect(demo?: boolean): void {
	connectDevice(type.value, demo)
		.then(() => emit('close'))
		.catch(() => {
			alert(
				'Failed to connect',
				PopupColor.Red,
				'An error occurred while trying to connect device'
			);
			emit('close');
		});
}
</script>

<style scoped>
.device-selector {
	position: fixed;
	right: 30px;
	top: 30px;
	max-width: min(768px, 90vw);
	margin-left: 30px;
	padding: 2rem min(2rem, 10%);
	border-radius: 1rem;
	color: var(--color-foreground);
	background-color: var(--color-background);
}

.popup-wrapper {
	overflow-y: auto;
}

@media screen and (max-width: 425px) {
	.device-selector {
		position: absolute;
		top: 0;
		right: 0;
		left: 0;
		border-radius: 0;
		max-width: unset;
		margin: 0;
	}
}

.device {
	@apply my-2 flex flex-wrap gap-4 items-center;
	border: 1px solid var(--color-accent);
	padding: 1em;
	border-radius: 8px;
}

.no-usb p {
	margin-bottom: 1em;
}

form {
	display: flex;
	flex-flow: row nowrap;
	column-gap: 0;
}

label {
	flex-grow: 1;
	text-align: center;
}

label:first-of-type {
	border-top-right-radius: 0;
	border-bottom-right-radius: 0;
}

label:last-of-type {
	border-top-left-radius: 0;
	border-bottom-left-radius: 0;
}

input[type='radio'] {
	display: none;
}

input[type='radio']:checked + label {
	background-color: var(--color-accent);
	color: var(--color-background);
}

@media screen and (prefers-color-scheme: dark) {
	.device-selector {
		border: 1px solid var(--color-accent);
	}
}

@media screen and (min-width: 768px) {
	.close {
		display: none;
	}
}
</style>
