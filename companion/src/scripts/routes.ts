import {createRouter, createWebHashHistory, createWebHistory, RouteRecordRaw} from 'vue-router';
import MotorView from '../views/MotorView.vue';
import DebugView from '../views/DebugView.vue';
import {crashCourse} from './analytics';
import DeveloperView from '../views/DeveloperView.vue';
import ControlView from '../views/ControlView.vue';
import TimelineView from '../views/TimelineView.vue';
import DeviceView from '../views/DeviceView.vue';

const routes: Array<RouteRecordRaw> = [
	{
		path: '/control',
		name: 'control',
		component: ControlView
	},
	{
		path: '/timeline',
		name: 'timeline',
		component: TimelineView
	},
	{
		path: '/motors',
		name: 'motors',
		component: MotorView
	},
	{
		path: '/developer',
		name: 'developer',
		component: DeveloperView,
		redirect: {name: 'debug'},
		children: [
			{
				path: '/device',
				name: 'device',
				component: DeviceView
			},
			{
				path: '/debug',
				name: 'debug',
				component: DebugView
			}
		]
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
