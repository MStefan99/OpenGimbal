<template lang="pug">
.motors.max-w-screen-lg.my-4.mx-auto.px-4
	datalist#ranges
		option(value="0")
		option(value="1024")
		option(value="2048")
		option(value="4096")
	h2.mb-4.text-xl.font-bold Motors
	button.mb-4(@click="enumerate()" :disabled="enumerating") Find motors
	.mb-2(v-if="!enumerating && motors.length")
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
					v-model="positions[motor.address]"
					@update:model-value="motor.move(positions[motor.address], torques[motor.address])")
				p Torque
				RangeSlider.mb-2(
					:min="0"
					:max="15"
					:scale="15"
					v-model="torques[motor.address]"
					@update:model-value="motor.move(positions[motor.address], torques[motor.address])")
				button.block.mb-2(@click="motor.move(positions[motor.address], torques[motor.address])") Move
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Haptic
				p Haptic intensity
				RangeSlider.mb-2(:min="0" :max="15" :scale="15" v-model="hapticIntensities[motor.address]")
				p Haptic duration
				RangeSlider.mb-2(
					:min="0"
					:max="extendedRanges ? 4095 : 100"
					:scale="extendedRanges ? 4095 : 100"
					v-model="hapticDurations[motor.address]")
				button.mb-2(
					@click="motor.haptic(hapticIntensities[motor.address], hapticDurations[motor.address])") Start
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Sound
				p Tone frequency
				RangeSlider.mb-2(
					:min="50"
					:max="25000"
					:scale="1000"
					v-model="toneFrequencies[motor.address]"
					@update:model-value="motor.tone(toneFrequencies[motor.address])")
				button.block.mb-2(@click="toneFrequencies[motor.address] = 25000; motor.silent()") Silent
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Setup
				p Offset
				RangeSlider.mb-2(
					:min="0"
					:max="4096"
					:scale="4096"
					v-model="offsets[motor.address]"
					@update:model-value="motor.setOffset(offsets[motor.address])")
				button.mb-2(@click="adjustOffset(motor)") Adjust offset
			.border-b.border-accent.pt-2
				p.text-accent.font-bold Power
				button.block.mb-2(@click="motor.disable()") Disable
				button.block.mb-2(@click="motor.idle()") Idle
				button.block.mb-2(@click="motor.sleep()") Sleep
				button.block.mb-2(@click="motor.wake()") Wake
			.pt-2
				p.text-accent.font-bold Properties
				template(v-if="motor.address < 15")
					.mb-2
						span Calibrated
						span.cursor-pointer(@click="calibrate(motor)")
							StatusIndicator.ml-2(
								:value="options[motor.address].calibrated"
								active-text="Yes"
								inactive-text="No")
					.mb-2
						span Inverted
						span.cursor-pointer(@click="invert(motor)")
							StatusIndicator.ml-2(
								:value="!options[motor.address].inverted"
								active-text="No"
								inactive-text="Yes")
					.mb-2
						span High voltage compatible
						span(
							@click="alert('Cannot change this option', PopupColor.Red, 'This is a read-only option that cannot be changed')")
							StatusIndicator.ml-2(
								:value="options[motor.address].highVoltageCompatible"
								active-text="Yes"
								inactive-text="No")
					p.text-zinc-500 Click options to toggle them
				p.text-zinc-500(v-else) See individual motors
		p.text-zinc-600(v-if="enumerating") Looking for motors, please wait...
		p.text-red.bold(v-else-if="!motors.length") No motors found. Please check the connection and try again.
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import type {IMotor, MotorOptions} from '../scripts/driver/Motor';
import RangeSlider from '../components/RangeSlider.vue';
import {delay} from '../scripts/util';
import StatusIndicator from '../components/StatusIndicator.vue';
import {connectedDevice} from '../scripts/driver/driver';
import {alert, PopupColor} from '../scripts/popups';
import {Gimbal} from '../scripts/driver/Gimbal';

const motors = ref<IMotor[]>([]);
const enumerating = ref<boolean>(false);
const extendedRanges = ref<boolean>(false);

const positions = ref<number[]>([]);
const torques = ref<number[]>([]);
const offsets = ref<number[]>([]);
const toneFrequencies = ref<number[]>([]);
const hapticIntensities = ref<number[]>([]);
const hapticDurations = ref<number[]>([]);
const options = ref<MotorOptions[]>([]);

async function enumerate(): Promise<void> {
	if (connectedDevice.value instanceof Gimbal) {
		await connectedDevice.value.disable();
	}

	enumerating.value = true;
	motors.value = [];

	const detectedMotors = await connectedDevice.value.motors.enumerate();
	detectedMotors.length && detectedMotors.push(connectedDevice.value.motors.all);

	positions.value = new Array(16).fill(0);
	torques.value = new Array(16).fill(0);
	offsets.value = new Array(16).fill(0);
	toneFrequencies.value = new Array(16).fill(25000);
	hapticIntensities.value = new Array(16).fill(255);
	hapticDurations.value = new Array(16).fill(5);
	options.value = new Array(16).fill({});

	try {
		for (const motor of connectedDevice.value.motors.active) {
			options.value[motor.address] = await motor.getOptions();
			offsets.value[motor.address] = await motor.getOffset();
		}
		motors.value = detectedMotors;
	} catch (err) {
		motors.value = [];
		console.error(err);
		alert(
			'Communication error',
			PopupColor.Red,
			'An issue occurred during communication, please try again'
		);
	}

	enumerating.value = false;
}

async function adjustOffset(motor: IMotor): Promise<void> {
	await motor.adjustOffset(positions.value[motor.address]);
	await delay(10);

	const broadcastAddress = connectedDevice.value.motors.all.address;

	if (torques.value[motor.address] > 0) {
		await motor.move(positions.value[motor.address], torques.value[motor.address]);
	} else if (torques.value[broadcastAddress] > 0) {
		await motor.move(positions.value[broadcastAddress], torques.value[broadcastAddress]);
	} else if (motor.address === broadcastAddress) {
		for (const motor of connectedDevice.value.motors.active) {
			if (torques.value[motor.address] > 0) {
				await motor.move(positions.value[motor.address], torques.value[motor.address]);
			}
		}
	}

	if (motor.address < 15) {
		offsets.value[motor.address] = await motor.getOffset();
	}
}

async function calibrate(motor: IMotor): Promise<void> {
	if (motor.address === 15) {
		return;
	}

	await motor.setOptions(
		!options.value[motor.address].calibrated,
		options.value[motor.address].inverted
	);

	// TODO: workaround due to a protocol limitation, needs improvement
	async function calibrationChanged(): Promise<boolean> {
		const calibration = (await motor.getOptions()).calibrated;
		const changed = calibration !== options.value[motor.address].calibrated;
		options.value[motor.address].calibrated = calibration;
		return changed;
	}

	if (!(await calibrationChanged())) {
		let interval = setInterval(async () => {
			if (await calibrationChanged()) {
				clearInterval(interval);
				interval = null;
			}
		}, 2000);

		setTimeout(() => interval !== null && clearInterval(interval), 20000);
	}
}

async function invert(motor: IMotor): Promise<void> {
	if (motor.address === 15) {
		return;
	}

	await motor.setOptions(
		options.value[motor.address].calibrated,
		!options.value[motor.address].inverted
	);
	options.value[motor.address] = await motor.getOptions();
}

onMounted(enumerate);
</script>

<style scoped></style>
