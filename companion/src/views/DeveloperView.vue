<template lang="pug">
SubNavBar
	template(#navigation)
		RouterLink(:to="{name: 'device'}") Device
		RouterLink(:to="{name: 'debug'}") Debug
	RouterView
</template>

<script setup lang="ts">
import SubNavBar from '../components/SubNavBar.vue';
</script>

<style scoped></style>
