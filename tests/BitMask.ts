'use strict';


enum CalibrationBits {
	Zero = 0x0,
	Pole = 0x1
}

type AllBits = CalibrationBits;

class BitwiseRegister<T extends AllBits> {
	private value: number = 0;

	set(bit: T) {
		this.value |= bit;
	}

	getValue(): number {
		return this.value;
	}
}