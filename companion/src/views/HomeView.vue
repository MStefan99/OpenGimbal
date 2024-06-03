<template lang="pug">
#home.p-4
	datalist#ranges
		option(value="0")
		option(value="1024")
		option(value="2048")
		option(value="4096")
	h2.mb-4.text-xl.font-bold Motors
	button.mb-4(@click="enumerate()" :disabled="enumerating") Enumerate motors
	.flex.flex-wrap.gap-2
		.motor.min-w-48.card(v-for="motor in motors" :key="motor.address")
			.border-b.border-accent.pt-2
				p.text-accent.text-lg(v-if="motor.address < 15") Motor {{motor.address}}
				p.text-accent.text-lg(v-else) All motors
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Movement
				p Position
				RangeSlider.mb-2(
					:min="-4096"
					:max="4096"
					:scale="4096"
					v-model="positions[motor.address - 1]"
					@update:model-value="motor.move(positions[motor.address - 1], torques[motor.address - 1])")
				p Torque
				RangeSlider.mb-2(
					:min="0"
					:max="15"
					:scale="15"
					v-model="torques[motor.address - 1]"
					@update:model-value="motor.move(positions[motor.address - 1], torques[motor.address - 1])")
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Haptic
				p Haptic intensity
				RangeSlider.mb-2(
					:min="0"
					:max="255"
					:scale="255"
					v-model="hapticIntensities[motor.address - 1]")
				p Haptic duration
				RangeSlider.mb-2(:min="0" :max="255" :scale="255" v-model="hapticDurations[motor.address - 1]")
				button.mb-2(
					@click="motor.haptic(hapticIntensities[motor.address - 1], hapticDurations[motor.address - 1])") Start
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Sound
				p Tone frequency
				RangeSlider.mb-2(
					:min="50"
					:max="25000"
					:scale="1000"
					v-model="toneFrequencies[motor.address - 1]"
					@update:model-value="motor.tone(toneFrequencies[motor.address - 1])")
				button.block.mb-2(@click="toneFrequencies[motor.address - 1] = 25000; motor.silent()") Silent
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Setup
				p Range
				RangeSlider.mb-2(
					:min="0"
					:max="65535"
					:scale="2048"
					listID="ranges"
					v-model="ranges[motor.address - 1]"
					@update:model-value="(p) => motor.setRangeVariable(p)")
				p Offset
				RangeSlider.mb-2(
					:min="0"
					:max="4096"
					:scale="4096"
					v-model="offsets[motor.address - 1]"
					@update:model-value="(p) => motor.setOffsetVariable(p)")
				button.block.mb-2(@click="adjustOffset(motor)") Adjust offset
			.pt-2
				p.text-accent.font-bold Power
				button.block.mb-2(@click="motor.disable()") Disable
				button.block.mb-2(@click="motor.sleep()") Sleep
				button.block.mb-2(
					@click="motor.move(positions[motor.address - 1], torques[motor.address - 1])") Wake up
		p.text-zinc-600(v-if="enumerating") Enumerating motors, please wait...
		p.text-red.bold(v-else-if="!motors.length") No motors found. Please check the connection and try again.
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import {CalibrationBits, Motor} from '../scripts/driver/Motor';
import {activeDevice} from '../scripts/driver/driver';
import RangeSlider from '../components/RangeSlider.vue';
import {BitwiseRegister} from '../scripts/driver/BitwiseRegister';
import {delay} from '../scripts/util';

const motors = ref<Motor[]>([]);
const enumerating = ref<boolean>(false);

const positions = ref<number[]>([]);
const torques = ref<number[]>([]);
const ranges = ref<number[]>([]);
const offsets = ref<number[]>([]);
const toneFrequencies = ref<number[]>([]);
const hapticIntensities = ref<number[]>([]);
const hapticDurations = ref<number[]>([]);
const calibrations = ref<BitwiseRegister<CalibrationBits>[]>([]);

async function enumerate(): Promise<void> {
	enumerating.value = true;
	motors.value = [];

	motors.value = await activeDevice.value.enumerate();
	motors.value.length && motors.value.push(activeDevice.value.all);

	torques.value = new Array(15).fill(15);
	ranges.value = new Array(15);
	offsets.value = new Array(15);
	toneFrequencies.value = new Array(15).fill(25000);
	hapticIntensities.value = new Array(15).fill(255);
	hapticDurations.value = new Array(15).fill(5);
	calibrations.value = new Array(15);

	for (const motor of activeDevice.value.active) {
		ranges.value[motor.address - 1] = await motor.getRange();
		offsets.value[motor.address - 1] = await motor.getOffset();
		calibrations.value[motor.address - 1] = await motor.getCalibration();
	}

	enumerating.value = false;
}

async function adjustOffset(motor: Motor): Promise<void> {
	await motor.adjustOffset();
	await delay(10);
	offsets.value[motor.address - 1] = await motor.getOffset();
}

onMounted(enumerate);
</script>

<style scoped></style>
