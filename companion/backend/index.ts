import WebSocket from 'ws';

class StateVariable {
	#value: any = {};
	#writable: boolean = true;
	#effect: (newValue: any, oldValue: any) => void | Promise<void> = null;

	set(newValue: any): true {
		try {
			if (this.#effect) {
				this.#effect(newValue, this.#value);
			}
		} finally {
			this.#writable && (this.#value = newValue);
		}
		return true;
	}

	constructor(value?: any) {
		this.#value = value;
	}

	writable(writable: boolean) {
		this.#writable = writable;
		return this;
	}

	effect(cb: (newValue: any, oldValue: any) => void | Promise<void>) {
		this.#effect = cb;
		return this;
	}

	toJSON() {
		return this.#value;
	}
}

// Define your shared state object here
const sharedState = {
	count: new StateVariable(0),
	immutable: new StateVariable(42).writable(false),
	nested: {a: {b: {c: {d: new StateVariable(69).effect((v) => console.log('Effect', v))}}}}
};

// Create a new WebSocket server
const wss = new WebSocket.Server({port: 3014});

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

// Handle incoming connections
wss.on('connection', (ws) => {
	console.log('Client connected');

	// Send the initial state to the client
	ws.send(JSON.stringify({type: 'state', state: sharedState}));

	// Listen for messages from the client
	ws.on('message', (data) => {
		const message = JSON.parse(data.toString());

		switch (message.type) {
			case 'update':
				// Update the shared state object and broadcast the update to all connected clients
				let variable = traverse(sharedState, message.variable);
				if (variable instanceof StateVariable) {
					variable.set(message.value);
				} else {
					variable = message.value;
				}

				wss.clients.forEach((client) => {
					if (client.readyState === WebSocket.OPEN) {
						client.send(JSON.stringify({type: 'state', state: sharedState}));
					}
				});
				break;
			default:
				console.log(`Unknown message type: ${message.type}`);
		}
	});

	// Handle disconnections
	ws.on('close', () => {
		console.log('Client disconnected');
	});
});
