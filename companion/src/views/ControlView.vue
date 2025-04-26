<template lang="pug">
.control.max-w-screen-lg.my-4.mx-auto.px-4
	JoystickControl.my-4
	button(@click="connectedDevice.enable()") Turn on
	button.ml-4(@click="connectedDevice.disable()") Turn off
</template>

<script setup lang="ts">
import {connectedDevice} from '../scripts/driver/driver';
import JoystickControl from '../components/JoystickControl.vue';
</script>

<style scoped></style>
