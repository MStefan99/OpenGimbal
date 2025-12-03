import {createRouter, createWebHashHistory, createWebHistory, RouteRecordRaw} from 'vue-router';
import MotorView from '../views/MotorView.vue';
import MotorCommandView from '../views/MotorCommandView.vue';
import {crashCourse} from './analytics';
import DeveloperView from '../views/DeveloperView.vue';
import GimbalView from '../views/GimbalView.vue';
import TimelineView from '../views/TimelineView.vue';
import DeviceView from '../views/DeviceView.vue';
import ControllerCommandView from '../views/ControllerCommandView.vue';

const routes: Array<RouteRecordRaw> = [
	{
		path: '/gimbal',
		name: 'gimbal',
		component: GimbalView
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
				path: 'device',
				name: 'device',
				component: DeviceView
			},
			{
				path: 'controller',
				name: 'controller',
				component: ControllerCommandView
			},
			{
				path: 'motor',
				name: 'motor',
				component: MotorCommandView
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
