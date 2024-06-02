<template lang="pug">
#home.p-4
	datalist#ranges
		option(value="0")
		option(value="1024")
		option(value="2048")
		option(value="4096")
	h2.mb-4.text-xl.font-bold Motors
	button.mb-4(@click="enumerate()" :disabled="enumerating") Enumerate motors
	.flex.gap-2
		.motor.card(v-for="motor in motors" :key="motor.address")
			.border-b.border-accent.pt-2
				p(v-if="motor.address < 15") Motor {{motor.address}}
				p(v-else) All motors
			.border-b.border-accent.pt-2
				p Position
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
			.border-b.border-accent.pt-2
				p Range
				RangeSlider.mb-2(
					:min="0"
					:max="65535"
					:scale="2048"
					listID="ranges"
					v-model="motorRanges[motor.address - 1]"
					@update:model-value="(p) => motor.setRangeVariable(p)")
				p Offset
				RangeSlider.mb-2(
					:min="0"
					:max="4096"
					:scale="4096"
					v-model="motorOffsets[motor.address - 1]"
					@update:model-value="(p) => motor.setOffsetVariable(p)")
				button.block.mb-2(@click="adjustOffset(motor)") Adjust offset
			.border-b.border-accent.pt-2
				p Tone frequency
				RangeSlider.mb-2(
					:min="50"
					:max="25000"
					:scale="1000"
					v-model="motorFrequencies[motor.address - 1]"
					@update:model-value="motor.tone(motorFrequencies[motor.address - 1])")
				button.block.mb-2(@click="motorFrequencies[motor.address - 1] = 25000; motor.silent()") Silent
			.pt-2
				p Options
				button.block.mb-2(@click="motor.disable()") Disable motor
				button.block.mb-2(@click="motor.sleep()") Sleep
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import {CalibrationBits, Motor} from '../scripts/driver/Motor';
import {activeDevice} from '../scripts/driver/driver';
import RangeSlider from '../components/RangeSlider.vue';
import {BitwiseRegister} from '../scripts/driver/BitwiseRegister';
import {delay} from "../scripts/util";

const motors = ref<Motor[]>([]);
const enumerating = ref<boolean>(false);

const motorPowers = ref<number[]>([]);
const motorFrequencies = ref<number[]>([]);
const motorRanges = ref<number[]>([]);
const motorOffsets = ref<number[]>([]);
const motorCalibrations = ref<BitwiseRegister<CalibrationBits>[]>([]);

async function enumerate(): Promise<void> {
	enumerating.value = true;
	motors.value = [];

	motors.value = await activeDevice.value.enumerate();
	motors.value.push(activeDevice.value.all);

	motorPowers.value = new Array(15).fill(15);
	motorFrequencies.value = new Array(15).fill(25000);
	motorRanges.value = new Array(15);
	motorOffsets.value = new Array(15);
	motorCalibrations.value = new Array(15);

	for (const motor of activeDevice.value.active) {
		motorRanges.value[motor.address - 1] = await motor.getRange();
		motorOffsets.value[motor.address - 1] = await motor.getOffset();
		motorCalibrations.value[motor.address - 1] = await motor.getCalibration();
	}

	enumerating.value = false;
}

async function adjustOffset(motor: Motor): Promise<void> {
	await motor.adjustOffset();
	await delay(10);
	motorOffsets.value[motor.address - 1] = await motor.getOffset();
}

onMounted(enumerate);
</script>

<style scoped></style>
