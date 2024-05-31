<template lang="pug">
#home.p-4
	h2.mb-4.text-xl.font-bold Motors
	button.mb-4(@click="enumerate()" :disabled="enumerating") Enumerate motors
	.flex.gap-2
		.motor.card(v-for="motor in motors" :key="motor.address")
			p(v-if="motor.address < 15") Motor
				|
				|
				b {{motor.address}}
			p(v-else) All motors
			p.border-t.border-accent Position
			RangeSlider.mb-2(
				:min="-4096"
				:max="4096"
				:scale="4096"
				@update:model-value="(p) => motor.move(p, motorPowers[motor.address - 1])")
			p Torque
			RangeSlider.mb-2(
				:min="0"
				:max="15"
				:scale="15"
				v-model="motorPowers[motor.address - 1]"
				@update:model-value="(p) => motor.move(p, motorPowers[motor.address - 1])")
			p.border-t.border-accent Tone frequency
			RangeSlider.mb-2(
				:min="50"
				:max="25000"
				:scale="1000"
				v-model="motorFrequencies[motor.address - 1]"
				@update:model-value="motor.tone(motorFrequencies[motor.address - 1])")
			button.block.mb-2(@click="motorFrequencies[motor.address - 1] = 25000; motor.silent()") Silent
			p.mb-2.border-t.border-accent Options
			button.block.mb-2(@click="motor.adjustOffset()") Adjust offset
			button.block.mb-2(@click="motor.disable()") Disable motor
			button.block.mb-2(@click="motor.sleep()") Sleep
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import {Motor} from '../scripts/driver/Motor';
import {activeDevice} from '../scripts/driver/driver';
import RangeSlider from '../components/RangeSlider.vue';

const motors = ref<Motor[]>([]);
const enumerating = ref<boolean>(false);

const motorPowers = ref<number[]>([]);
const motorFrequencies = ref<number[]>([]);

async function enumerate(): Promise<void> {
	enumerating.value = true;
	motors.value = [];
	motors.value = await activeDevice.value.enumerate();
	motors.value.push(activeDevice.value.all);
	motorPowers.value = new Array(15).fill(15);
	motorFrequencies.value = new Array(15).fill(25000);
	enumerating.value = false;
}

onMounted(enumerate);
</script>

<style scoped></style>
