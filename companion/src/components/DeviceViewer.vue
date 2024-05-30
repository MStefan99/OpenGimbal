<template lang="pug">
.popup-wrapper(@click.self="$emit('close')")
	.device-viewer.popup
		h2.text-xl Device information
		p Vendor ID:
			|
			|
			b {{device.port.getInfo().usbVendorId}}
		p Product ID:
			|
			|
			b {{device.port.getInfo().usbProductId}}
</template>

<script setup lang="ts">
import {MotorManager} from '../scripts/driver/MotorManager';

defineProps<{device: MotorManager}>();
defineEmits<{(e: 'close'): void}>();
</script>

<style scoped>
.popup-wrapper {
	background-color: unset;
}

.popup {
	position: absolute;
}

.device-viewer {
	color: var(--color-foreground);
	background-color: var(--color-background);
}

@media screen and (prefers-color-scheme: dark) {
	.device-viewer {
		border: 1px solid var(--color-foreground);
	}
}
</style>
