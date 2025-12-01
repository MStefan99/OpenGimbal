<template lang="pug">
nav
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

<style scoped>
@reference '../assets/style.css';

nav {
	@apply flex flex-wrap items-center py-2 mb-4 px-4 gap-2 bg-accent text-white rounded-xl;
}

nav a {
	@apply py-1 px-2 font-semibold rounded-xl;
}

nav a.router-link-active {
	@apply bg-zinc-50 text-accent;
}
</style>
