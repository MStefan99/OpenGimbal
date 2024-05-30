export class BitwiseRegister<T> {
	_value: number = 0;

	constructor(value?: number) {
		this._value = value;
	}

	set(bit: T): BitwiseRegister<T> {
		this._value |= 1 << Number(bit);
		return this;
	}

	has(bit: T): boolean {
		return !!(this._value & (1 << Number(bit)));
	}

	get value(): number {
		return this._value;
	}
}
