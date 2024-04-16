// Define your shared state object here
import StateVariable from './StateVariable';
import {traverse} from './util';

function createHandler<T extends object>(
	listener: () => void
) {
	const proxy = {
		get(target, prop, receiver) {
			const obj = Reflect.get(target, prop, receiver);

			if (obj === null) {
				return obj;
			} else if (
				(typeof obj === 'object' || typeof obj === 'function') &&
				Object.getOwnPropertyDescriptor(target, prop)?.configurable
			) {
				// @ts-ignore
				return new Proxy(obj, proxy);
			} else {
				return obj;
			}
		},
		set(target, prop, value, receiver) {
			Reflect.set(target, prop, value, receiver);
			listener();
			return true;
		}
	} as ProxyHandler<T>;

	return proxy;
}

export default function createState<T extends object>(
	state: T,
	listener: () => void
) {
	return new Proxy<T>(state, createHandler(listener));
}

export function updateState<T extends object>(state: T, variable: string | symbol, value: any) {
	let property = traverse(state, variable);

	if (property instanceof StateVariable) {
		property.set(value);
	} else {
		property = value;
	}
}
