<template lang="pug">
.viewer-wrapper(@click.self="$emit('close')")
	.device-viewer(v-if="device instanceof Gimbal")
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
			b.wrap-break-word {{device.serialNumber}}
		p Device version:
			|
			|
			b {{device.deviceVersionMajor}}.{{device.deviceVersionMinor}}.{{device.deviceVersionSubminor}}
		p USB version:
			|
			|
			b {{device.usbVersionMajor}}.{{device.usbVersionMinor}}.{{device.usbVersionSubminor}}
	.device-viewer(v-else)
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
@reference '../assets/style.css';

.viewer-wrapper {
	@apply fixed p-4 left-0 top-0 right-0 bottom-0;
}

.device-viewer {
	@apply relative card mx-auto max-w-fit top-1/2 -translate-y-1/2 shadow-xl backdrop-blur-lg dark:border-zinc-500 bg-white/70  dark:bg-zinc-500/60;
}

b {
	@apply text-accent dark:text-accent-bright;
}
</style>
