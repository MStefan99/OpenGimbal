<template lang="pug">
.viewer-wrapper(@click.self="$emit('close')")
	.device-viewer
		.gimbal
			h2.text-xl Gimbal
			template(v-if="connectedDevice instanceof Gimbal")
				p Connected over:
					|
					|
					b {{connectedDevice.connectedOver === 'usb' ? 'USB' : 'Serial'}}
				p Vendor:
					|
					|
					b {{connectedDevice.vendorName}}
				p Product:
					|
					|
					b {{connectedDevice.productName}}
				p Serial number:
					|
					|
					b.wrap-break-word {{connectedDevice.serialNumber}}
				p Device version:
					|
					|
					b {{connectedDevice.productVersion}}
				p(v-if="connectedDevice.connectedOver === 'usb'") USB version:
					|
					|
					b {{connectedDevice.usbVersion}}
			b(v-else) No gimbal connected
		.motors.mt-4
			h2.text-xl Motors
			p(v-for="motor in connectedDevice.motors.active" :key="motor.address") Motor
				|
				|
				b {{motor.address}}
				|
				| connected
			b(v-if="!connectedDevice.motors.active.length") No motors found
</template>

<script setup lang="ts">
import {MotorManager} from '../scripts/driver/MotorManager';
import {Gimbal} from '../scripts/driver/Gimbal';
import {connectedDevice} from '../scripts/driver/driver';

defineEmits<{(e: 'close'): void}>();
</script>

<style scoped>
@reference '../assets/style.css';

.viewer-wrapper {
	@apply fixed p-4 left-0 top-0 right-0 bottom-0;
}

.device-viewer {
	@apply relative card rounded-xl mx-auto max-w-fit top-1/2 -translate-y-1/2 shadow-xl backdrop-blur-lg dark:border-zinc-500 bg-white/70  dark:bg-zinc-500/60;
}

b {
	@apply text-accent dark:text-accent-bright;
}
</style>
