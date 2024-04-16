import StateVariable from './StateVariable';
import createState, {updateState} from './state';
import Server from './server';
import {traverse} from "./util";

const server = new Server((variable, value) => {
	updateState(state, variable, value);
	server.broadcastUpdate(variable, traverse(state, variable));
});

// @ts-ignore
const state = createState(
	{
		count: new StateVariable(0),
		immutable: new StateVariable(42).writable(false),
		nested: {a: {b: {c: {d: new StateVariable(69).effect((v) => console.log('Effect', v))}}}}
	},
	() => server.broadcastState(state)
);

server.broadcastState(state);

setInterval(() => state.count.set(state.count.get() + 1), 1000);