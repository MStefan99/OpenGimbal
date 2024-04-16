class StateVariable<T> {
	private _value: T;
	private _writable: boolean = true;
	private _effect: (newValue: T, oldValue: T) => void | Promise<void> = null;

	set(newValue: T, options?: {
		force: boolean,
		quiet: boolean
	}): true {
		try {
			if (!options?.quiet && this._effect) {
				this._effect(newValue, this._value);
			}
		} finally {
			(this._writable || options?.force) && (this._value = newValue);
		}
		return true;
	}

	get() {
		return this._value;
	}

	constructor(value?: T) {
		this._value = value;
	}

	writable(writable: boolean) {
		this._writable = writable;
		return this;
	}

	effect(cb: (newValue: T, oldValue: T) => void | Promise<void>) {
		this._effect = cb;
		return this;
	}

	toJSON(): T {
		return this._value;
	}
}


export default StateVariable;
