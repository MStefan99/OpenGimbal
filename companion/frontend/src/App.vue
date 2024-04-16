<template lang="pug">
header.no-print.shadow-md
	NavBar

main
	RouterView
	PopupContainer

footer
	//img.footer-logo.mx-auto(src="/logo.svg" alt="Crash Course logo")
</template>

<script setup lang="ts">
import NavBar from './components/NavBar.vue';
import PopupContainer from './components/PopupContainer.vue';
</script>
