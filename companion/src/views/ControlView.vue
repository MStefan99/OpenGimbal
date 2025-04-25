<template lang="pug">
.control.max-w-screen-lg.my-4.mx-auto.px-4
	button(@click="connectedDevice.enable()") Turn on
	button.ml-4(@click="connectedDevice.sleep()") Turn off
</template>

<script setup lang="ts">
import {connectedDevice} from '../scripts/driver/driver';
</script>

<style scoped></style>
