<template lang="pug">
.control.w-fit.max-w-screen-lg.my-4.mx-auto.px-4
	JoystickControl.mx-auto.mb-4(@move="update")
	p.text-accent.font-bold.border-b.border-accent.text-center Power
	.flex.flex-row.justify-evenly.gap-4.my-4
		button(@click="connectedDevice.enable()") Turn on
		button(@click="connectedDevice.disable()") Turn off
	p.text-accent.font-bold.border-b.border-accent.text-center Mode
	.flex.flex-row.justify-evenly.gap-4.my-4
		button(@click="connectedDevice.setMode(GimbalMode.Horizon)") Horizon
		button(@click="connectedDevice.setMode(GimbalMode.Follow)") Follow
		button(@click="connectedDevice.setMode(GimbalMode.FPV)") FPV
		button(@click="connectedDevice.setMode(GimbalMode.Tilt)") Tilt
	p.text-accent.font-bold.border-b.border-accent.text-center Move
	.flex.flex-row.justify-evenly.gap-4.my-4
		button(@click="connectedDevice.setMode(GimbalMode.Horizon)") Rotate
		button(@click="connectedDevice.setMode(GimbalMode.Follow)") Flip
</template>

<script setup lang="ts">
import {connectedDevice} from '../scripts/driver/driver';
import JoystickControl from '../components/JoystickControl.vue';
import {Gimbal} from '../scripts/driver/Gimbal';
import {GimbalMode} from '../scripts/driver/usb/GimbalCommand';

let lastCommandSent = 0;
let initialYaw = 0;
let initialRoll = 0;

function update(x: number, y: number): void {
	if (!(connectedDevice.value instanceof Gimbal)) {
		return;
	}

	const now = Date.now();

	if (now - lastCommandSent < 128) {
		return;
	}

	connectedDevice.value.setOrientation(-x * Math.PI, -y * (Math.PI / 2), 0);
	lastCommandSent = now;
}

function rotate(): void {}

function flip(): void {}
</script>

<style scoped></style>
