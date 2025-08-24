import {defineConfig} from 'vite';
import vue from '@vitejs/plugin-vue';
import tailwindcss from '@tailwindcss/vite';

// https://vitejs.dev/config/
export default defineConfig({
	server: {
		port: 3014,
		host: '0.0.0.0'
	},
	plugins: [vue(), tailwindcss()]
});
