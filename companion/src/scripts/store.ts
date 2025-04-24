import {reactive} from 'vue';

type Store = {
	developerMode: boolean;
	setDeveloperMode: (mode: boolean) => void;
};

export const appState = reactive<Store>({
	developerMode: !!localStorage.getItem('dev-mode'),

	setDeveloperMode(active: boolean) {
		this.developerMode = active;
		if (active) {
			localStorage.setItem('dev-mode', 'true');
		} else {
			localStorage.removeItem('dev-mode');
		}
	}
});

export default appState;
