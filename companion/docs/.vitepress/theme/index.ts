// https://vitepress.dev/guide/custom-theme
import {h, VNode, watch} from 'vue';
import type {Theme} from 'vitepress';
import DefaultTheme from 'vitepress/theme';
import './style.css';
import {crashCourse} from './analytics';

export default {
	extends: DefaultTheme,
	Layout: (): VNode => {
		return h(DefaultTheme.Layout, null, {
			// https://vitepress.dev/guide/extending-default-theme#layout-slots
		});
	},
	enhanceApp({app, router, siteData}): void {
		watch(router.route, () => {
			console.log('New route', router.route.path);
			crashCourse?.value?.sendHit();
		});
	}
} satisfies Theme;
