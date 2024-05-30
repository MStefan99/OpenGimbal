<template lang="pug">
#home.p-4
	h2.mb-4.text-xl.font-bold Motors
	p(v-for="motor in motors" :key="motor.address") {{motor.address}}
</template>

<script setup lang="ts">
import {onMounted, ref} from 'vue';
import {Motor} from '../scripts/driver/Motor';
import {activeDevice} from '../scripts/driver/driver';

const motors = ref<Motor[]>([]);

async function start(): Promise<void> {
	await activeDevice.value.enumerate();
	motors.value = activeDevice.value.active;
}

onMounted(start);
</script>

<style scoped></style>
