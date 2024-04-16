<template lang="pug">
button(@click="appState.count++") {{appState.count}}
</template>

<script setup lang="ts">
import appState from '../scripts/store';
</script>

<style scoped></style>
