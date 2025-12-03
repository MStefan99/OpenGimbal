<template lang="pug">
.selector-wrapper(@click.self="$emit('close')")
	.device-selector
		span.mr-2.font-semibold.text-lg(v-if="connecting") Connecting...
		template(v-else-if="connectedDevice")
			span.mr-2.font-semibold.text-lg Connected to
			button(@click="viewingDevice = connectedDevice") {{formatConnectedDevice()}}
			button.mt-2.block.w-full(@click="disconnectDevice(connectedDevice)") Disconnect
		div(v-else)
			p.font-semibold.text-lg Connect
			.flex.my-2
				button.grow(
					v-if="supportsUSB"
					:class="{'rounded-r-none': supportsSerial}"
					@click="connect('usb')") Connect over USB
				button.grow(
					v-if="supportsSerial"
					:class="{'rounded-l-none': supportsUSB}"
					@click="connect('serial')") Connect over Serial
				p(v-if="!secureContext || (!supportsUSB && !supportsSerial)") Not supported
			button.w-full.mt-2(@click="$emit('close')") Close
			.text-red-500.mt-4.text-center
				template(v-if="!secureContext")
					p OpenGimbal only works over secure connections.
					p Please connect over HTTPS and try again.
				div(v-else-if="!supportsUSB && !supportsSerial")
					p Your browser does not support USB or serial connections.
					p Please try another browser.
				template(v-else)
					template(v-if="!supportsUSB")
						p Your browser does not support USB connections.
						p Please try another browser.
					template(v-if="!supportsSerial")
						p Your browser does not support serial connections.
						p Please try another browser.
	Transition
		DeviceViewer(v-if="viewingDevice" @close="viewingDevice = false")
</template>

<script setup lang="ts">
import {ref} from 'vue';
import {alert, PopupColor} from '../scripts/popups';
import DeviceViewer from './DeviceViewer.vue';
import {MotorManager} from '../scripts/driver/MotorManager';
import {Gimbal} from '../scripts/driver/Gimbal';
import {connectDevice, connectedDevice, disconnectDevice} from '../scripts/driver/driver';

const emit = defineEmits<{(e: 'close'): void}>();
const viewingDevice = ref<boolean>(false);
const connecting = ref<boolean>(false);

const secureContext = window.isSecureContext;
const supportsUSB = 'usb' in navigator;
const supportsSerial = 'serial' in navigator;

function connect(type: 'usb' | 'serial'): void {
	connecting.value = true;

	connectDevice(type)
		.then(() => {
			connecting.value = false;
		})
		.catch((e) => {
			connecting.value = false;

			if (e.name === 'NotFoundError') {
				return;
			}

			alert('Failed to connect', PopupColor.Red, e.message ?? 'Unknown error');
			emit('close');
		});
}

function formatConnectedDevice(): string {
	if (connectedDevice.value instanceof Gimbal) {
		return `${connectedDevice.value.productName ?? 'Unknown'} v${connectedDevice.value.productVersion} + ${connectedDevice.value.motors.active.length}x D1`;
	}
	return `${connectedDevice.value.motors.active.length}x D1`;
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
