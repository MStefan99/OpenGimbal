import {defineConfig} from 'vite';
import vue from '@vitejs/plugin-vue';
import pugPlugin from 'vite-plugin-pug';

// https://vitejs.dev/config/
export default defineConfig({
	server: {
		port: 3006,
		host: '0.0.0.0'
	},
	plugins: [vue(), pugPlugin()]
});
