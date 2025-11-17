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
@import '../assets/style.css';

.subnavbar {
	@apply m-2;
	color: var(--color-background);
}

.subnavbar nav {
	@apply flex flex-row flex-wrap my-2 rounded-lg shadow-md;
	color: var(--color-white);
	background-color: var(--color-accent);
	border-top: 1px solid var(--color-background);
}

.subnavbar nav :deep(a:hover) {
	background-color: var(--color-background);
	color: var(--color-accent);
}

.subnavbar nav :deep(a.router-link-active) {
	background-color: var(--color-background);
	color: var(--color-accent);
}

.subnavbar nav :deep(a) {
	@apply m-2 px-2 rounded block transition-colors;
}

@media screen and (min-width: 768px) {
	nav {
		font-size: 1.2em;
	}
}
</style>
