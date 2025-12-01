<template lang="pug">
.meter
	.meter-bar
</template>

<script setup lang="ts">
import {computed} from 'vue';
import {scale} from '../scripts/util';

const props = withDefaults(
	defineProps<{
		min: number;
		max: number;
		value: number;
	}>(),
	{
		min: 0,
		max: 1,
		value: 0
	}
);

const percentage = computed(() => scale(props.value, props.min, props.max, 0, 100) + '%');
</script>

<style scoped>
@reference '../assets/style.css';

.meter {
	@apply relative h-4 rounded-full overflow-hidden border-accent;
}

.meter .meter-bar {
	@apply absolute left-0 h-full bg-accent transition-all;
	width: v-bind(percentage);
}
</style>
