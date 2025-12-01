<template lang="pug">
.joystick(ref="joystick")
	.joystick-control(ref="joystickControl" :class="{centering}")
</template>

<script setup lang="ts">
import {onMounted, onUnmounted, ref} from 'vue';

const emit = defineEmits<{(e: 'move', x: number, y: number): void}>();
const props = defineProps<{
	autoCenter?: boolean;
}>();
defineExpose({
	reCenter
});

type Vector2 = {
	x: number;
	y: number;
};

const centering = ref<boolean>(false);
let isMoving = false;
let moveOffset: Vector2 = {
	x: 0,
	y: 0
};

const joystick = ref();
const joystickControl = ref();

function reCenter(): void {
	const joystickCenter = getCenter(joystick.value);

	joystickControl.value.style.left = '50%';
	joystickControl.value.style.top = '50%';
	centering.value = true;

	setTimeout(() => {
		centering.value = false;
	}, 200);
}

function getCenter(element: Element): Vector2 & {width: number; height: number} {
	const bounds = element.getBoundingClientRect();
	return {
		x: bounds.x + bounds.width / 2,
		y: bounds.y + bounds.height / 2,
		width: bounds.width,
		height: bounds.height
	};
}

function downListener(e: MouseEvent): void {
	if (e.target !== joystickControl.value) {
		return;
	}

	isMoving = true;
	const controlCenter = getCenter(joystickControl.value);

	moveOffset = {
		x: e.x - controlCenter.x,
		y: e.y - controlCenter.y
	};
}

function moveListener(e: MouseEvent): void {
	if (!isMoving) {
		return;
	}

	const joystickCenter = getCenter(joystick.value);
	const controlCenter = getCenter(joystickControl.value);
	const desiredPosition: Vector2 = {
		x: e.x - moveOffset.x,
		y: e.y - moveOffset.y
	};

	const difference: Vector2 = {
		x: desiredPosition.x - joystickCenter.x,
		y: desiredPosition.y - joystickCenter.y
	};

	/* Convert to polar coordinates, normalize the distance and then convert back to cartesian coordinates.
		There's probably a better way to do this, but I'm too tired to think of it right now.
	 */
	const angle = Math.atan2(difference.y, difference.x);
	const distance = Math.sqrt(Math.pow(difference.x, 2) + Math.pow(difference.y, 2));
	const maxDistance = (joystickCenter.width - controlCenter.width) / 2;
	const cappedDistance = Math.min(distance, maxDistance);
	const visualDistance = cappedDistance / (joystickCenter.width / 2);

	joystickControl.value.style.left = `${visualDistance * Math.cos(angle) * 50 + 50}%`;
	joystickControl.value.style.top = `${visualDistance * Math.sin(angle) * 50 + 50}%`;

	const normalizedDistance = cappedDistance / maxDistance;
	emit('move', Math.cos(-angle) * normalizedDistance, Math.sin(-angle) * normalizedDistance);
}

function upListener(e: MouseEvent): void {
	isMoving = false;

	if (props.autoCenter) {
		reCenter();
	}
}

onMounted(() => document.addEventListener('mousedown', downListener));
onMounted(() => document.addEventListener('mousemove', moveListener));
onMounted(() => document.addEventListener('mouseup', upListener));

onUnmounted(() => document.removeEventListener('mousedown', downListener));
onUnmounted(() => document.removeEventListener('mousemove', moveListener));
onUnmounted(() => document.removeEventListener('mouseup', upListener));
</script>

<style scoped>
@reference '../assets/style.css';

.joystick {
	width: min(100%, 320px);
	@apply relative aspect-square rounded-full border border-accent dark:border-accent-bright;
}

.joystick-control {
	@apply relative w-20 h-20 rounded-full bg-accent dark:bg-accent-bright left-1/2 top-1/2 -translate-1/2;
}

.joystick-control.centering {
	@apply transition-all;
}
</style>
