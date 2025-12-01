<template lang="pug">
.selector-wrapper(@click.self="$emit('close')")
	.device-selector
		template(v-if="connectedDevice")
			span.mr-2.font-semibold.text-lg Connected to
			button(@click="viewedDevice = connectedDevice") {{formatConnectedDevice()}}
			button.mt-2.block.w-full(@click="disconnectDevice(connectedDevice)") Disconnect
		div(v-else)
			p.font-semibold.text-lg Connect
			.flex.my-2
				button.rounded-r-none(v-if="'usb' in navigator" @click="connect()") Connect over USB
				button.rounded-l-none(@click="connect(true)") Connect over UART
			button.w-full.mt-2(@click="$emit('close')") Close
			.text-red-500.mt-4(v-if="!('usb' in navigator)")
				p(v-if="!window.isSecureContext").
					This page cannot access USB devices because it is using an insecure connection.
					Please open this page securely (over HTTPS) to connect your device.
				p(v-else).
					It looks like your browser does not support USB devices. Please try another browser.
			.text-red-500.mt-4(v-if="!('serial' in navigator)")
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
		.catch((e) => {
			if (e.name === 'NotFoundError') {
				return;
			}

			alert('Failed to connect', PopupColor.Red, e.message ?? 'Unknown error');
			emit('close');
		});
}

function formatConnectedDevice(): string {
	if (connectedDevice.value instanceof Gimbal) {
		return `OpenGimbal v${connectedDevice.value.deviceVersionMajor}.${connectedDevice.value.deviceVersionMinor}.${connectedDevice.value.deviceVersionSubminor}`;
	}
	return 'Unknown device';
}
</script>

<style scoped>
@reference '../assets/style.css';

.selector-wrapper {
	@apply fixed left-0 top-0 right-0 bottom-0 bg-zinc-900/50;
}

.device-selector {
	@apply fixed card rounded-xl right-0 m-4 shadow-xl backdrop-blur-lg text-accent dark:text-accent-bright dark:border-zinc-500 bg-white/70 dark:bg-zinc-500/60;
}
</style>
