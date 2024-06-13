import {createRouter, createWebHashHistory, createWebHistory, RouteRecordRaw} from 'vue-router';
import MotorView from '../views/MotorView.vue';
import MonitorView from '../views/MonitorView.vue';
import {crashCourse} from './analytics';

const routes: Array<RouteRecordRaw> = [
	{
		path: '/motors',
		name: 'motors',
		component: MotorView
	},
	{
		path: '/monitor',
		name: 'monitor',
		component: MonitorView
	},
	{
		path: '/:pathname(.*)*',
		redirect: {
			name: 'motors'
		}
	}
];

const router = createRouter({
	history: import.meta.env.VITE_ROUTER === 'hash' ? createWebHashHistory() : createWebHistory('/'),
	routes
});

router.afterEach(() => crashCourse.value?.sendHit());

export default router;
