<template lang="pug">
div
	nav.text-white.font-semibold
		span
			RouterLink.clickable(:to="{name: 'home'}") Control
			RouterLink.clickable(:to="{name: 'settings'}") Settings
</template>

<script setup lang="ts">
</script>

<style scoped>
label {
	display: block;
}

input {
	@apply border-2 border-accent rounded-xl w-full p-2 my-3 shadow;
}

input[type='submit'] {
	@apply bg-accent font-bold text-background text-xl shadow-md;
}
</style>
