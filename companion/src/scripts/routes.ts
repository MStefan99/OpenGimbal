import {createRouter, createWebHashHistory, createWebHistory, RouteRecordRaw} from 'vue-router';
import MotorView from '../views/MotorView.vue';
import DebugView from '../views/DebugView.vue';
import {crashCourse} from './analytics';

const routes: Array<RouteRecordRaw> = [
	{
		path: '/motors',
		name: 'motors',
		component: MotorView
	},
	{
		path: '/debug',
		name: 'debug',
		component: DebugView
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
