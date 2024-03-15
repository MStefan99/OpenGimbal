export function delay(ms: number): Promise<void> {
	if (ms < 50) { // High-resolution delay is taxing on the CPU, only using for small delays
		return new Promise((resolve) => {
			const startTime = process.hrtime();

			function check() {
				if (process.hrtime(startTime)[1] / 1000000 < ms) {
					process.nextTick(check);
				} else {
					resolve();
				}
			}

			check();
		});
	} else {
		return new Promise((resolve) => setTimeout(resolve, ms));
	}
}

export function clamp(val: number, min: number, max: number) {
	return Math.max(min, Math.min(max, val));
}

// Modulo function that handles negative values properly
export function mod(a: number, b: number): number {
	return ((a % b) + b) % b;
}

export async function time(callback: () => void | Promise<void>) {
	const startTime = process.hrtime();
	await callback();
	return process.hrtime(startTime)[1] / 1000000;
}

export async function interpolate(
	startValue: number,
	endValue: number,
	duration: number,
	stepDelay: number,
	callback: (value: number, fraction: number) => void | Promise<void>
) {
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
