<template lang="pug">
p.text-red.font-bold.m-4(v-if="!(connectedDevice instanceof Gimbal)").
	This tool only works with a connected gimbal.
.control.w-fit.max-w-screen-lg.my-4.mx-auto.px-4(v-else)
	JoystickControl.mx-auto.mb-4(@move="update" ref="joystick")
	p Power
	.flex.flex-row.justify-evenly.gap-4.my-4
		button(@click="connectedDevice.enable()") Turn on
		button(@click="connectedDevice.disable()") Turn off
	p Mode
	.flex.flex-row.justify-evenly.gap-4.my-4
		button(@click="setMode(GimbalMode.Horizon)") Horizon
		button(@click="setMode(GimbalMode.Follow)") Follow
		button(@click="setMode(GimbalMode.FPV)") FPV
		button(@click="setMode(GimbalMode.Tilt)") Tilt
	p Move
	.flex.flex-row.justify-evenly.gap-4.my-4
		button(@click="reCenter()") Re-center
		button(@click="rotate()") Rotate
		button(@click="flip()") Flip
</template>

<script setup lang="ts">
import {connectedDevice} from '../scripts/driver/driver';
import JoystickControl from '../components/JoystickControl.vue';
import {Gimbal} from '../scripts/driver/Gimbal';
import {ref} from 'vue';
import {GimbalMode} from '../scripts/driver/usb/USBMessage';

type Orientation = {yaw: number; pitch: number; roll: number};
const joystick = ref();

let lastCommandSent = 0;
let currentMode: GimbalMode = GimbalMode.Follow;
let orientation: Orientation = {yaw: 0, pitch: 0, roll: 0};
let initialOrientation: Orientation = {yaw: 0, pitch: 0, roll: 0};

function setMode(mode: GimbalMode): Promise<void> {
	if (!(connectedDevice.value instanceof Gimbal)) {
		return;
	}

	currentMode = mode;

	return connectedDevice.value.setMode(mode);
}

function move(): Promise<void> {
	const now = Date.now();

	if (now - lastCommandSent < 128) {
		return;
	}

	lastCommandSent = now;
	if (!(connectedDevice.value instanceof Gimbal)) {
		return;
	}

	return connectedDevice.value.setOrientation(
		orientation.yaw + initialOrientation.yaw,
		orientation.pitch + initialOrientation.pitch,
		orientation.roll + initialOrientation.roll
	);
}

function update(x: number, y: number): Promise<void> {
	if (currentMode !== GimbalMode.Tilt) {
		orientation.yaw = -x * Math.PI;
	} else {
		orientation.roll = -x * Math.PI;
	}
	orientation.pitch = -y * (Math.PI / 2);

	return move();
}

function reCenter(): Promise<void> {
	orientation = {yaw: 0, pitch: 0, roll: 0};
	joystick.value.reCenter();

	return move();
}

function rotate(): Promise<void> {
	initialOrientation.roll += Math.PI / 2;

	if (initialOrientation.roll > Math.PI + 0.01) {
		initialOrientation.roll = 0;
	}

	return move();
}

function flip(): Promise<void> {
	initialOrientation.yaw += Math.PI;

	if (initialOrientation.yaw > Math.PI + 0.01) {
		initialOrientation.yaw = 0;
	}

	return move();
}
</script>

<style scoped>
@reference '../assets/style.css';

p {
	@apply text-accent dark:text-accent-bright font-bold border-b border-accent dark:border-accent-bright text-center;
}
</style>
