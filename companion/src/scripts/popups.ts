import {reactive, ref} from 'vue';

export enum PopupColor {
	Accent = 'accent',
	Green = 'green',
	Yellow = 'yellow',
	Red = 'red'
}

type Alert = {
	title: string;
	details?: string;
	type: PopupColor;
};

type Confirm = {
	title: string;
	details?: string;
	type: PopupColor;
};

type Prompt = {
	title: string;
	details?: string;
	type: PopupColor;
};

export const activeAlerts = reactive<Alert[]>([]);
export const activeConfirm = ref<{
	confirm: Confirm;
	resolve: (res: boolean) => void;
} | null>(null);
export const activePrompt = ref<{
	prompt: Prompt;
	resolve: (res: string) => void;
	reject: (err?: Error) => void;
} | null>(null);

export function alert(title: string, type: PopupColor, details?: string): Promise<void> {
	return new Promise<void>((resolve) => {
		const alert: Alert = {title, details, type};

		setTimeout(() => {
			activeAlerts.splice(activeAlerts.indexOf(alert), 1);
			resolve();
		}, 5000);

		activeAlerts.push(alert);
	});
}

export function confirm(title: string, type: PopupColor, details?: string): Promise<boolean> {
	if (activeConfirm.value || activePrompt.value) {
		throw new Error('Only one popup is allowed at a time');
	}

	return new Promise<boolean>(
		(resolve) =>
			(activeConfirm.value = {
				confirm: {title, details, type},
				resolve: (result: boolean): void => {
					resolve(result);
					activeConfirm.value = null;
				}
			})
	);
}

export function prompt(title: string, type: PopupColor, details?: string): Promise<string> {
	if (activeConfirm.value || activePrompt.value) {
		throw new Error('Only one popup is allowed at a time');
	}

	return new Promise<string>((resolve, reject) => {
		activePrompt.value = {
			prompt: {title, details, type},
			resolve: (result: string): void => {
				resolve(result);
				activePrompt.value = null;
			},
			reject: (err?: Error): void => {
				reject(err);
				activePrompt.value = null;
			}
		};
	});
}

export function clearPopups(): void {
	activeConfirm.value && activeConfirm.value.resolve(false);
	activePrompt.value && activePrompt.value.resolve('');

	activeAlerts.length = 0;
	activeConfirm.value = null;
	activePrompt.value = null;
}
