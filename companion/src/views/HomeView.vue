<template lang="pug">
#home.p-4
	h2.mb-4.text-xl.font-bold Motors
	button.mb-4(@click="enumerate()" :disabled="enumerating") Enumerate motors
	.flex.gap-2
		.motor.card(v-for="motor in motors" :key="motor.address")
			p Motor
				|
				|
				b {{motor.address}}
			RangeSlider.mb-2(
				:min="-4096"
				:max="4096"
				:scale="4096"
				@update:model-value="(p) => motor.move(p)")
			button(@click="motor.disable()") Disable motor
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import {Motor} from '../scripts/driver/Motor';
import {activeDevice} from '../scripts/driver/driver';
import RangeSlider from '../components/RangeSlider.vue';

const motors = ref<Motor[]>([]);
const enumerating = ref<boolean>(false);

async function enumerate(): Promise<void> {
	enumerating.value = true;
	motors.value = [];
	await activeDevice.value.enumerate();
	motors.value = activeDevice.value.active;
	enumerating.value = false;
}

onMounted(enumerate);
</script>

<style scoped></style>
