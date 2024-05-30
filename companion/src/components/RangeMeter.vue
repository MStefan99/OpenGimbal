<template lang="pug">
.meter
	.meter-bar
</template>

<script setup lang="ts">
import {computed} from 'vue';

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

function scale(
	value: number,
	inMin: number,
	inMax: number,
	outMin: number,
	outMax: number
): number {
	return ((value - inMin) * (outMax - outMin)) / (inMax - inMin) + outMin;
}

const percentage = computed(() => scale(props.value, props.min, props.max, 0, 100) + '%');
</script>

<style scoped>
.meter {
	position: relative;
	height: 1.5em;
	border-radius: 500px;
	overflow: hidden;
	background-color: var(--color-background);
	border: 1px solid var(--color-accent);
}

.meter .meter-bar {
	position: absolute;
	left: 0;
	width: v-bind(percentage);
	height: 100%;
	background-color: var(--color-accent);
	transition: inline-size 0.1s linear;
}
</style>
