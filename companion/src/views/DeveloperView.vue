<template lang="pug">
SubNavBar
	template(#navigation)
		RouterLink(:to="{name: 'device'}") Device
		RouterLink(:to="{name: 'usb'}") USB
		RouterLink(:to="{name: 'serial'}") Serial
	RouterView
</template>

<script setup lang="ts">
import SubNavBar from '../components/SubNavBar.vue';
import {connectedDevice} from '../scripts/driver/driver';
import {Gimbal} from '../scripts/driver/Gimbal';
</script>

<style scoped></style>
