<template lang="pug">
.navbar
	#title-bar
		span#title OpenGimbal Companion
		nav
			RouterLink(:to="{name: 'motors'}") Motors
		label#device-status.connected(v-if="connectedDevice" for="device-toggle") Connected
		label#device-status(v-else for="device-toggle") Not connected
		input#device-toggle.hidden(type="checkbox" v-model="deviceSelectorOpen")
	Teleport(to="body")
		Transition
			DeviceSelector(v-if="deviceSelectorOpen" @close="deviceSelectorOpen = false")
</template>

<script setup lang="ts">
import DeviceSelector from './DeviceSelector.vue';
import {connectedDevice} from '../scripts/driver/driver';
import {ref} from 'vue';

const deviceSelectorOpen = ref<boolean>(false);
</script>

<style scoped>
#title-bar {
	@apply flex flex-row flex-nowrap justify-between items-center;
	background-color: var(--color-accent);
	color: var(--color-white);
}

#title-bar > :not(nav) {
	padding: 0.5em 1em;
}

#title-bar #title {
	font-size: 1.5em;
	font-weight: bold;
}

@media screen and (max-width: 768px) {
	#title-bar #title {
		display: none;
	}
}

#device-status {
	font-weight: bold;
	cursor: pointer;
	margin-left: auto;
	user-select: none;
	color: var(--color-light);
}

#device-status.connected {
	color: var(--color-white);
}

nav {
	@apply flex items-center;
	align-self: stretch;
}

nav a {
	@apply flex items-center font-bold;
	padding: 0 1ch 0;
	height: 100%;
	transition: background-color 0.2s;
}

nav a:hover {
	background-color: var(--color-accent-bright);
}

nav a.router-link-active {
	background-color: var(--color-background);
	color: var(--color-accent);
}

.navbar input[type='checkbox'].hidden {
	display: none;
}

@media screen and (min-width: 768px) {
	nav a {
		font-size: 1.2em;
	}
}
</style>
