export function delay(ms: number) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

export function clamp(val: number, min: number, max: number) {
	return Math.max(min, Math.min(max, val));
}
