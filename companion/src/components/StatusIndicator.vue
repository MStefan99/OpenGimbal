<template lang="pug">
.status-indicator
	.status.active(v-if="value ^ inverted") {{inverted ? inactiveText : activeText}}
	.status.inactive(v-if="!value ^ inverted") {{inverted ? activeText : inactiveText}}
</template>

<script setup lang="ts">
const props = withDefaults(
	defineProps<{
		value: boolean;
		activeText?: string;
		inactiveText?: string;
		inverted?: boolean;
	}>(),
	{
		activeText: 'On',
		inactiveText: 'Off'
	}
);
</script>

<style scoped>
@reference '../assets/style.css';

.status-indicator {
	@apply inline;
}

.status {
	@apply px-2 inline-block text-white rounded;
}

.status.inactive {
	@apply bg-red-500;
}

.status.active {
	@apply bg-green-500;
}
</style>
