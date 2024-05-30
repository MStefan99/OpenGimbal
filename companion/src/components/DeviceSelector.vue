<template lang="pug">
.popup-wrapper(@click.self="$emit('close')")
	.device-selector
		span.bold.block(v-if="connectedDevices.length") Connected devices
		span.bold.block(v-else) No devices connected
		.device(v-for="device of connectedDevices" :key="device.usbDevice.serialNumber")
			span.cursor-pointer(@click="viewedDevice = device") {{device.usbDevice.productName}}
			.flex.flex-row.flex-wrap.gap-4.grow
				button.green-outline.grow(v-if="device === activeDevice" disabled) Active device
				button.green.grow(v-else @click="activeDevice = device") Set active
				button.red.grow(@click="disconnectDevice(device)") Disconnect
		span.bold.block.my-2 Connect
		button.accent.bold.w-full(v-if="'usb' in navigator" @click="connect()")
			| {{connectedDevices.length ? 'Connect another' : 'Connect'}}
		button.bold.accent-outline.w-full.mt-2(@click="connect(true)") Connect demo device
		button.close.red-outline.w-full.mt-2(@click="$emit('close')") Close
		.text-red.mt-4(v-if="!('usb' in navigator)")
			p(v-if="!window.isSecureContext").
				This page cannot access USB devices because it is using an insecure connection.
				Please open this page securely (over HTTPS) to connect your device.
			p(v-else).
				It looks like your browser does not support USB devices. Please try another browser.
	Transition
		DeviceViewer(v-if="viewedDevice" :device="viewedDevice" @close="viewedDevice = null")
</template>

<script setup lang="ts">
import {ref} from 'vue';
import {alert, PopupColor} from '../scripts/popups';
import {Device} from '../scripts/driver/Device';
import {
	connectDevice,
	disconnectDevice,
	connectedDevices,
	activeDevice
} from '../scripts/driver/driver';
import DeviceViewer from './DeviceViewer.vue';

defineEmits<{(e: 'close'): void}>();
const viewedDevice = ref<Device | null>(null);

function connect(demo?: true): void {
	connectDevice(demo).catch(() =>
		alert('Failed to connect', PopupColor.Red, 'An error occurred while trying to connect device')
	);
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

@media screen and (prefers-color-scheme: dark) {
	.device-selector {
		border: 1px solid var(--color-foreground);
	}
}

@media screen and (min-width: 768px) {
	.close {
		display: none;
	}
}
</style>
