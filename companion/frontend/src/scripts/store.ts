import {reactive} from 'vue';

type Store = {
	count: number;
};

export const appState = reactive<Store>({
	count: 0
});

function traverse(object: any, propertyName: string): any {
	const properties = propertyName.split('/');
	for (const property of properties) {
		if (!(property in object)) {
			throw new Error('Property "' + property + '" is not defined in', object);
		} else {
			object = object[property];
		}
	}
	return object;
}

const ws = new WebSocket(`ws://${location.hostname}:3014`);

// Handle incoming messages from the server
ws.onmessage = (event) => {
	const message = JSON.parse(event.data);

	switch (message.type) {
		case 'state':
			// Update the Vue store object with new state data
			Object.assign(appState, message.state);
			break;
		default:
			console.log(`Unknown message type: ${message.type}`);
	}
};

export default appState;
