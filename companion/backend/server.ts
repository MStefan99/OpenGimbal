import WebSocket, {WebSocketServer} from 'ws';

class Server {
	#wss: WebSocketServer;
	#state: object | null = null;

	constructor(onUpdate: (variable: string | symbol, value: any) => void | Promise<void>) {
		// Create a new WebSocket server
		this.#wss = new WebSocket.Server({port: 3014});

		// Handle incoming connections
		this.#wss.on('connection', (ws) => {
			console.log('Client connected');

			this.#state && this.broadcastState(this.#state);

			// Listen for messages from the client
			ws.on('message', async (data) => {
				const message = JSON.parse(data.toString());

				switch (message.type) {
					case 'update':
						// Update the shared state object and broadcast the update to all connected clients
						await onUpdate(message.variable, message.value);
						return;
					default:
						console.log(`Unknown message type: ${message.type}`);
				}
			});

			// Handle disconnections
			ws.on('close', () => {
				console.log('Client disconnected');
			});
		});
	}

	broadcastState(state: object) {
		this.#state = state;

		this.#wss.clients.forEach((client) => {
			if (client.readyState === WebSocket.OPEN) {
				client.send(
					JSON.stringify({
						type: 'state',
						state
					})
				);
			}
		});
	}

	broadcastUpdate(variable: string | symbol, value: any) {
		this.#wss.clients.forEach((client) => {
			if (client.readyState === WebSocket.OPEN) {
				client.send(
					JSON.stringify({
						type: 'update',
						variable,
						value
					})
				);
			}
		});
	}
}

export default Server;
