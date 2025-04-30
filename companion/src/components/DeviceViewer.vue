<template lang="pug">
.popup-wrapper(@click.self="$emit('close')")
	.device-viewer.usb.popup(v-if="device instanceof Gimbal")
		h2.text-xl Device information
		p Vendor:
			|
			|
			b {{device.manufacturerName}}
		p Product:
			|
			|
			b {{device.productName}}
		p Serial number:
			|
			|
			b {{device.serialNumber}}
		p Device version:
			|
			|
			b {{device.deviceVersionMajor}}.{{device.deviceVersionMinor}}.{{device.deviceVersionSubminor}}
		p USB version:
			|
			|
			b {{device.usbVersionMajor}}.{{device.usbVersionMinor}}.{{device.usbVersionSubminor}}
	.device-viewer.serial.popup(v-else)
		h2.text-xl Device information
		p Vendor ID:
			|
			|
			b {{device.vendorId ?? 'Unknown'}}
		p Product ID:
			|
			|
			b {{device.productId ?? 'Unknown'}}
</template>

<script setup lang="ts">
import {MotorManager} from '../scripts/driver/MotorManager';
import {Gimbal} from '../scripts/driver/Gimbal';

defineProps<{device: MotorManager | Gimbal}>();
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
