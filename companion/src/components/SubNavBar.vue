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
	background-color: var(--color-accent);
	color: var(--color-background);
}

.subnavbar nav {
	display: flex;
	flex-flow: row wrap;
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
	padding: 0.3em 1ch;
	display: block;
	transition: background-color 0.2s ease;
}

@media screen and (min-width: 768px) {
	nav {
		font-size: 1.2em;
	}
}
</style>
