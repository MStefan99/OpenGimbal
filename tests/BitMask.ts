'use strict';


export enum CalibrationBits {
	Zero = 0x0,
	Pole = 0x1
}

export type AllBits = CalibrationBits;

export class BitwiseRegister<T extends AllBits> {
	#value: number = 0;

	set(bit: T) {
		this.#value |= 1 << bit;
		return this;
	}

	get value(): number {
		return this.#value;
	}
}
