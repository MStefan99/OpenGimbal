import {defineConfig} from 'vite';
import vue from '@vitejs/plugin-vue';

// https://vitejs.dev/config/
export default defineConfig({
	server: {
		port: 3014,
		host: '0.0.0.0'
	},
	plugins: [vue()]
});
