export function delay(ms: number): Promise<void> {
	return new Promise((resolve) => setTimeout(resolve, ms));
}

export function clamp(val: number, min: number, max: number): number {
	return Math.max(min, Math.min(max, val));
}

// Modulo function that handles negative values properly
export function mod(a: number, b: number): number {
	return ((a % b) + b) % b;
}

export function scale(
	value: number,
	inMin: number,
	inMax: number,
	outMin: number,
	outMax: number
): number {
	return ((value - inMin) * (outMax - outMin)) / (inMax - inMin) + outMin;
}

export async function interpolate(
	startValue: number,
	endValue: number,
	duration: number,
	stepDelay: number,
	callback: (value: number, fraction: number) => void | Promise<void>
): Promise<void> {
	// Calculate the number of steps required to complete the interpolation
	const numSteps = Math.ceil(duration / stepDelay);

	// Calculate the difference between the start and end values
	const diff = endValue - startValue;

	// Interpolate a value for each step
	for (let i = 0; i <= numSteps; i++) {
		// Calculate the elapsed time as a fraction of the total duration
		const elapsedTimeFraction = i / numSteps;

		// Use linear interpolation to calculate the value at this step
		const currentValue = startValue + diff * elapsedTimeFraction;

		// Call the callback with the current value and the elapsed time fraction
		await callback(currentValue, elapsedTimeFraction);

		// Wait for the specified step delay before interpolating the next value
		await delay(stepDelay);
	}
}
