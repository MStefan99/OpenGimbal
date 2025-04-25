import {createRouter, createWebHashHistory, createWebHistory, RouteRecordRaw} from 'vue-router';
import MotorView from '../views/MotorView.vue';
import SerialCommandView from '../views/SerialCommandView.vue';
import {crashCourse} from './analytics';
import DeveloperView from '../views/DeveloperView.vue';
import ControlView from '../views/ControlView.vue';
import TimelineView from '../views/TimelineView.vue';
import DeviceView from '../views/DeviceView.vue';
import USBCommandView from '../views/USBCommandView.vue';

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
		redirect: {name: 'device'},
		children: [
			{
				path: '/device',
				name: 'device',
				component: DeviceView
			},
			{
				path: '/usb',
				name: 'usb',
				component: USBCommandView
			},
			{
				path: '/serial',
				name: 'serial',
				component: SerialCommandView
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
