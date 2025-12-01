<template lang="pug">
.slider
	datalist(v-if="list" :id="'stops-' + idString")
		option(v-for="stop in list" :key="stop" :value="stop")
	//- tabindex enables :focus-within in Safari
	.bar(tabindex="-1" :class="{vertical: vertical}")
		input(
			type="range"
			v-model="sliderModel"
			@input="$emit('update:modelValue', value)"
			@change="$emit('change', value)"
			:disabled="disabled"
			:list="list ? 'stops-' + idString : listID"
			:min="min ?? -max"
			:max="max"
			:step="step")
		.track
		.value.short {{formatShortValue(value / scale)}}
		.value.long {{Math.round((value / scale) * 100)}}%
</template>

<script setup lang="ts">
import {computed, ref, watch} from 'vue';
import {clamp} from '../scripts/util';

const props = withDefaults(
	defineProps<{
		modelValue?: number;
		disabled?: boolean;
		vertical?: boolean;
		min?: number;
		max?: number;
		scale?: number;
		step?: number;
		list?: number[];
		listID?: string;
	}>(),
	{
		vertical: true,
		max: 1000,
		scale: 1000,
		step: 1
	}
);
defineEmits<{(e: 'update:modelValue', value: number): void; (e: 'change', value: number): void}>();
watch(
	() => props.modelValue,
	() => (sliderModel.value = props.modelValue?.toString() ?? '0')
);

const min = computed(() => props.min ?? -props.max);
const sliderModel = ref<string>(props.modelValue?.toString() ?? '0');
const value = computed(() => clamp(+sliderModel.value, min.value, props.max));
const percentage = computed(() => (value.value - min.value) / (props.max - min.value));

const id = new Uint8Array(4);
crypto.getRandomValues(id);
const idString = Array.from(id, (byte) => byte.toString(16).padStart(2, '0')).join('');

function formatShortValue(value: number): string {
	if (Math.abs(value) < 1) {
		return value.toFixed(2).replace(/(-)?\d+\./, '$1.');
	} else {
		return value.toFixed(1);
	}
}
</script>

<style scoped>
@reference '../assets/style.css';

.bar {
	@apply relative my-auto -mx-4 transition-all;
	--width: calc(4ch + var(--spacing) * 8);
	--height: 1.2em;
	width: var(--width);
}

.bar:hover {
	--width: calc(6ch + 16px);
}

.bar:focus-within {
	--width: 200px;
	--height: 80px;
}

.bar:after {
	@apply content-[''] absolute pointer-events-none top-0 bottom-0 left-4 right-4 border border-accent rounded;
}

.bar input[type='range'] {
	@apply align-middle opacity-0 transition-all;
	height: var(--height);
	width: var(--width);
}

.track {
	@apply absolute rounded pointer-events-none left-4 bottom-0 h-full bg-accent;
	width: max(8px, calc((100% - var(--spacing) * 8) * v-bind(percentage)));
}

.slider.red .bar:after {
	@apply border-red-500;
}

.slider.red .track {
	@apply bg-red-500;
}

.slider.orange .bar:after {
	@apply border-orange-500;
}

.slider.orange .track {
	@apply bg-orange-500;
}

.slider.yellow .bar:after {
	@apply border-yellow-500;
}

.slider.yellow .track {
	@apply bg-yellow-500;
}

.slider.green .bar:after {
	@apply border-green-500;
}

.slider.green .track {
	@apply bg-green-500;
}

.slider.blue .bar:after {
	@apply border-blue-500;
}

.slider.blue .track {
	@apply bg-blue-500;
}

input:disabled + .track {
	transition: width 100ms ease;
}

.value {
	@apply absolute font-bold pointer-events-none left-1/2 top-1/2 -translate-1/2 transition-all;
}

.bar:not(:hover) .value.long,
.bar:hover .value.short {
	opacity: 0;
}

input[type='range']::-webkit-slider-thumb {
	-webkit-appearance: none;
}
</style>
