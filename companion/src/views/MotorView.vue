<template lang="pug">
.motors.max-w-screen-lg.my-4.mx-auto.px-4
	datalist#ranges
		option(value="0")
		option(value="1024")
		option(value="2048")
		option(value="4096")
	h2.mb-4.text-xl.font-bold Motors
	button.mb-4(@click="enumerate()" :disabled="enumerating") Find motors
	div(v-if="motors.length")
		label Extended ranges
		input.ml-2(type="checkbox" v-model="extendedRanges")
	.flex.flex-wrap.gap-2
		.motor.min-w-48.card(v-for="motor in motors" :key="motor.address")
			.border-b.border-accent.pt-2
				p.text-accent.text-lg(v-if="motor.address < 15") Motor {{motor.address}}
				p.text-accent.text-lg(v-else) All motors
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Movement
				p Position
				RangeSlider.mb-2(
					:min="extendedRanges ? -4096 : -2048"
					:max="extendedRanges ? 4096 : 2048"
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
				RangeSlider.mb-2(:min="0" :max="15" :scale="15" v-model="hapticIntensities[motor.address - 1]")
				p Haptic duration
				RangeSlider.mb-2(
					:min="0"
					:max="extendedRanges ? 4095 : 100"
					:scale="extendedRanges ? 4095 : 100"
					v-model="hapticDurations[motor.address - 1]")
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
					:max="extendedRanges ? 65535 : 2048"
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
				button.mb-2(@click="adjustOffset(motor)") Adjust offset
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Power
				button.block.mb-2(@click="motor.disable()") Disable
				button.block.mb-2(@click="motor.sleep()") Sleep
				button.block.mb-2(
					@click="motor.move(positions[motor.address - 1], torques[motor.address - 1])") Wake up
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Recalibrate
				.mb-2
					label Zero calibration
					input.ml-2(
						type="checkbox"
						v-model="calibrationModes[motor.address - 1]"
						:value="CalibrationBits.Zero"
						@change="checkCalibrationMode(motor)")
				.mb-2
					label Pole calibration
					input.ml-2(
						type="checkbox"
						v-model="calibrationModes[motor.address - 1]"
						:value="CalibrationBits.Pole"
						@change="checkCalibrationMode(motor)")
				button.block.mb-2(@click="calibrate(motor)") Start
			.pt-2
				p.text-accent.font-bold Calibration
				template(v-if="motor.address < 15")
					.mb-2
						span Zero calibration
						StatusIndicator.ml-2(
							:value="calibrations[motor.address - 1]?.has(CalibrationBits.Zero) ?? false"
							active-text="Yes"
							inactive-text="No")
					.mb-2
						span Pole calibration
						StatusIndicator.ml-2(
							:value="calibrations[motor.address - 1]?.has(CalibrationBits.Pole) ?? false"
							active-text="Yes"
							inactive-text="No")
				p.text-zinc-500(v-else) See individual motors
		p.text-zinc-600(v-if="enumerating") Looking for motors, please wait...
		p.text-red.bold(v-else-if="!motors.length") No motors found. Please check the connection and try again.
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import {CalibrationBits} from '../scripts/driver/Motor';
import type {IMotor} from '../scripts/driver/Motor';
import RangeSlider from '../components/RangeSlider.vue';
import {BitwiseRegister} from '../scripts/driver/BitwiseRegister';
import {delay} from '../scripts/util';
import StatusIndicator from '../components/StatusIndicator.vue';
import {connectedDevice} from '../scripts/driver/driver';

const motors = ref<IMotor[]>([]);
const enumerating = ref<boolean>(false);
const calibrationModes = ref<CalibrationBits[][]>(
	new Array(15).fill([]).map<CalibrationBits[]>(() => [])
);
const extendedRanges = ref<boolean>(false);

const positions = ref<number[]>([]);
const torques = ref<number[]>([]);
const offsets = ref<number[]>([]);
const ranges = ref<number[]>([]);
const toneFrequencies = ref<number[]>([]);
const hapticIntensities = ref<number[]>([]);
const hapticDurations = ref<number[]>([]);
const calibrations = ref<BitwiseRegister<CalibrationBits>[]>([]);

async function enumerate(): Promise<void> {
	enumerating.value = true;
	motors.value = [];

	motors.value = await connectedDevice.value.enumerate();
	motors.value.length && motors.value.push(connectedDevice.value.all);

	positions.value = new Array(15).fill(0);
	torques.value = new Array(15).fill(0);
	offsets.value = new Array(15);
	ranges.value = new Array(15);
	toneFrequencies.value = new Array(15).fill(25000);
	hapticIntensities.value = new Array(15).fill(255);
	hapticDurations.value = new Array(15).fill(5);
	calibrations.value = new Array(15);

	for (const motor of connectedDevice.value.active) {
		calibrations.value[motor.address - 1] = connectedDevice.value.getInitialCalibration(
			motor.address
		);
		offsets.value[motor.address - 1] = await motor.getOffset();
		ranges.value[motor.address - 1] = await motor.getRange();
	}

	enumerating.value = false;
}

async function adjustOffset(motor: IMotor): Promise<void> {
	await motor.adjustOffset(positions.value[motor.address - 1]);
	await delay(10);

	if (torques.value[motor.address - 1] > 0) {
		await motor.move(positions.value[motor.address - 1], torques.value[motor.address - 1]);
	}

	if (motor.address < 15) {
		offsets.value[motor.address - 1] = await motor.getOffset();
	}
}

async function checkCalibrationMode(motor: IMotor): Promise<void> {
	const modes = calibrationModes.value[motor.address - 1];
	if (
		modes.some((v) => v === CalibrationBits.Pole) &&
		!modes.some((v) => v === CalibrationBits.Zero)
	) {
		modes.push(CalibrationBits.Zero);
	}
}

async function calibrate(motor: IMotor): Promise<void> {
	const mode = new BitwiseRegister<CalibrationBits>();

	for (const calibrationMode of calibrationModes.value[motor.address - 1]) {
		mode.set(calibrationMode);
	}

	await motor.calibrate(mode);

	if (motor.address === 15) {
		return;
	}

	// TODO: workaround due to a protocol limitation, needs improvement
	let interval = setInterval(async () => {
		const calibration = await motor.getCalibration();
		if (calibration.value !== calibrations.value[motor.address - 1].value) {
			clearInterval(interval);
			interval = null;
		}
		calibrations.value[motor.address - 1] = calibration;
	}, 2000);

	setTimeout(() => interval !== null && clearInterval(interval), 20000);
}

onMounted(enumerate);
</script>

<style scoped></style>
