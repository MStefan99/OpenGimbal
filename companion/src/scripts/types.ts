export const accelerationFactor = 0.122 / 1000;
export const angularRateFactor = 8.75 / 1000;
export const magneticFieldFactor = 1.5 / 1000;
export const attitudeFactor = 1 / 10430;
export const RAD_TO_DEG = 180 / Math.PI;
export const DEG_TO_RAD = Math.PI / 180;

export type ErrorResponse = {
	error: string;
	message: string;
};
