<template lang="pug">
#home.p-4
	h2.mb-4.text-xl.font-bold Motors
</template>

<script setup lang="ts">
import {onMounted, onUnmounted, ref} from 'vue';
import {activeDevice} from '../scripts/driver/driver';
import {BitwiseRegister} from '../scripts/driver/BitwiseRegister';
import {ReceiverBits, SensorBits} from '../scripts/driver/Device';
import StatusIndicator from '../components/StatusIndicator.vue';

let polling: boolean = true;
const activeSensors = ref<BitwiseRegister<SensorBits>>(new BitwiseRegister<SensorBits>());
const yaw = ref<number>(0);
const pitch = ref<number>(0);
const roll = ref<number>(0);
const receiverStatus = ref<BitwiseRegister<ReceiverBits>>(new BitwiseRegister<ReceiverBits>());

async function start(): Promise<void> {
	const sensors = await activeDevice.value.getSensors();
	activeSensors.value = sensors.activeSensors;

	updateStatus();
}

async function updateStatus(): Promise<void> {
	const status = await activeDevice.value.getStatus();

	yaw.value = status.yaw;
	pitch.value = status.pitch;
	roll.value = status.roll;
	receiverStatus.value = status.receiverStatus;

	polling && setTimeout(updateStatus);
}

onMounted(start);
onUnmounted(() => (polling = false));
</script>

<style scoped>
.attitude {
	max-width: min(500px, 90vw);
}
</style>
