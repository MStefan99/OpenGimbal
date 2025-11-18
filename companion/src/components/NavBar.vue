<template lang="pug">
.navbar
	#title-bar
		a#title(href="..") OpenGimbal
		nav
			RouterLink(
				:to="{name: 'control'}"
				v-if="connectedDevice instanceof Gimbal || appState.developerMode") Control
			RouterLink(:to="{name: 'timeline'}" v-if="appState.developerMode") Timeline
			RouterLink(:to="{name: 'motors'}") Motors
			RouterLink(:to="{name: 'developer'}" v-if="appState.developerMode") Developer
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
import {appState} from '../scripts/store';
import {Gimbal} from '../scripts/driver/Gimbal';
import {ref} from 'vue';

const deviceSelectorOpen = ref<boolean>(false);
</script>

<style scoped>
@import '../assets/style.css';

#title-bar {
	@apply flex flex-row m-2 rounded-lg shadow-md flex-nowrap justify-between items-center;
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
	@apply flex items-center my-2 gap-2;
	align-self: stretch;
}

nav a {
	@apply flex items-center font-bold rounded-lg;
	padding: 0 1ch 0;
	height: 100%;
	transition: background-color 0.2s;
}

nav a:hover {
	background-color: var(--color-accent-bright);
}

nav a.router-link-active {
	background-color: var(--color-background);
}

@media not (prefers-color-scheme: dark) {
	nav a.router-link-active {
		color: var(--color-accent);
	}
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
