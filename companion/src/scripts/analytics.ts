import {ref} from 'vue';
import {ErrorResponse} from './types';

type CrashCourse = {
	sendLog: (message: string, level: number, tag?: string) => Promise<ErrorResponse | void>;
	sendFeedback: (message: string) => Promise<ErrorResponse | void>;
	sendHit: () => Promise<ErrorResponse | void>;
};

export const crashCourse = ref<CrashCourse | null>(null);

async function loadSettings(): Promise<void> {
	if (!import.meta.env.VITE_CRASH_COURSE_URL || !import.meta.env.VITE_CRASH_COURSE_KEY) {
		console.warn(
			'Warning, Crash Course not configured! Errors will not be sent for further analysis.'
		);
		return;
	}

	try {
		const cc = (await import(
			/* @vite-ignore */
			`${import.meta.env.VITE_CRASH_COURSE_URL}/cc?k=${import.meta.env.VITE_CRASH_COURSE_KEY}`
		)) as CrashCourse;
		if (cc) {
			crashCourse.value = cc;
			cc.sendHit();
		}
	} catch (err) {
		console.warn(
			'Crash Course could not be loaded from',
			import.meta.env.VITE_CRASH_COURSE_URL + '.',
			'Errors will not be sent for further analysis.',
			err
		);
	}
}

loadSettings();
