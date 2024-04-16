import {createRouter, createWebHashHistory, createWebHistory, RouteRecordRaw} from 'vue-router';
import ControlView from '../views/ControlView.vue';
import SettingsView from '../views/SettingsView.vue';

const routes: Array<RouteRecordRaw> = [
	{
		path: '/',
		name: 'home',
		redirect: {
			name: 'control'
		}
	},
	{
		path: '/control',
		name: 'control',
		component: ControlView
	},
	{
		path: '/settings',
		name: 'settings',
		component: SettingsView
	},
	{
		path: '/:pathname(.*)*',
		redirect: {
			name: 'home'
		}
	}
];

const router = createRouter({
	history: import.meta.env.VITE_ROUTER === 'hash' ? createWebHashHistory() : createWebHistory('/'),
	routes
});

export default router;
