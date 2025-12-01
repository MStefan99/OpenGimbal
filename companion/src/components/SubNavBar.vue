<template lang="pug">
.subnavbar-layout
	.subnavbar
		nav
			slot(name="navigation")
	.subnavbar-content
		slot
</template>

<script setup lang="ts"></script>

<style scoped>
@reference '../assets/style.css';

.subnavbar-layout {
	@apply my-4;
}

.subnavbar {
	@apply flex flex-wrap items-center py-2 mb-4 px-4 gap-4 bg-accent text-white rounded-xl;
}
</style>
