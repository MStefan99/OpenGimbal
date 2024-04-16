'use strict';

import {createApp} from 'vue';
import App from './App.vue';
import router from './scripts/routes';
import './assets/style.css';

const app = createApp(App);

app.config.errorHandler = (err: unknown): void => {
	console.error('Vue error', err);
};

app.config.warnHandler = (err: unknown): void => {
	console.warn('Vue error', err);
};

app.use(router).mount('#app');
