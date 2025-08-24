<template lang="pug">
.device
	h2.mb-4.text-xl.font-bold Sensor data
	p.font-bold Battery
	div
		p Voltage: {{voltage.toFixed(2)}}V
		RangeMeter(:min="3" :max="4.2" :value="voltage")
	p.font-bold Orientation
	div
		p Yaw: {{yaw.toFixed(2)}}°
		RangeMeter(:min="-180" :max="180" :value="yaw")
		p Pitch: {{pitch.toFixed(2)}}°
		RangeMeter(:min="-180" :max="180" :value="pitch")
		p Roll: {{roll.toFixed(2)}}°
		RangeMeter(:min="-180" :max="180" :value="roll")
</template>

<script setup lang="ts">
import RangeMeter from '../components/RangeMeter.vue';
import {onMounted, onUnmounted, ref} from 'vue';
import {connectedDevice} from '../scripts/driver/driver';
import {Gimbal} from '../scripts/driver/Gimbal';
import {RAD_TO_DEG} from '../scripts/types';

const voltage = ref<number>(0);
const yaw = ref<number>(0);
const pitch = ref<number>(0);
const roll = ref<number>(0);

let intervalHandle: ReturnType<typeof setInterval> | null = null;

async function update(): Promise<void> {
	if (!(connectedDevice.value instanceof Gimbal)) {
		return;
	}

	voltage.value = await connectedDevice.value.getVoltage();
	const orientation = await connectedDevice.value.getHandleOrientation();
	if (orientation) {
		yaw.value = orientation.yaw * RAD_TO_DEG;
		pitch.value = orientation.pitch * RAD_TO_DEG;
		roll.value = orientation.roll * RAD_TO_DEG;
	}
}

onMounted(() => (intervalHandle = setInterval(update, 200)));
onUnmounted(() => clearInterval(intervalHandle));
</script>

<style scoped>
@import '../assets/style.css';

.device {
	@apply m-4 flex flex-col;
}
</style>
