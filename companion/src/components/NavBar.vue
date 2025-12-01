<template lang="pug">
.navbar
	a.title(href="..") OpenGimbal
	nav(v-if="connectedDevice")
		RouterLink(:to="{name: 'control'}" v-if="connectedDevice instanceof Gimbal") Control
		RouterLink(:to="{name: 'motors'}") Motors
		RouterLink(:to="{name: 'timeline'}" v-if="appState.developerMode") Timeline
		RouterLink(:to="{name: 'developer'}" v-if="appState.developerMode") Developer
	span.grow.text-right(v-else)
		span.mr-2.grow.text-right.text-zinc-300 Click to connect →
		label#device-status(for="device-toggle") Not connected
	label#device-status.connected(v-if="connectedDevice" for="device-toggle") Connected
	input#device-toggle.hidden(type="checkbox" v-model="deviceSelectorOpen")
	Teleport(to="body")
		Transition
			DeviceSelector(v-if="deviceSelectorOpen" @close="deviceSelectorOpen = false")
</template>

<script setup lang="ts">
import DeviceSelector from './DeviceSelector.vue';
import {connectedDevice} from '../scripts/driver/driver';
import {appState} from '../scripts/store';
import {Gimbal} from '../scripts/driver/Gimbal';
import {ref} from 'vue';

const deviceSelectorOpen = ref<boolean>(false);
</script>

<style scoped>
@reference '../assets/style.css';

.navbar {
	@apply flex flex-wrap items-center py-2 mb-4 px-4 gap-4 shadow-md bg-accent text-white rounded-xl;
}

.title {
	@apply md:text-2xl font-bold;
}

nav {
	@apply grow flex flex-wrap gap-4;
}

nav a {
	@apply p-2 md:text-lg font-semibold rounded-xl;
}

nav a.router-link-active {
	@apply bg-zinc-50 text-accent;
}

label {
	@apply font-semibold cursor-pointer;
}
</style>
