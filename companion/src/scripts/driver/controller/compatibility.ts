type USBTransferStatus = 'ok' | 'stall' | 'babble';

export function addUSBCompatibilityClasses(): void {
	if (!('USBOutTransferResult' in window)) {
		class USBOutTransferResult {
			readonly bytesWritten: number;
			readonly status: USBTransferStatus;

			constructor(status: USBTransferStatus, bytesWriten?: number) {
				this.status = status;
				this.bytesWritten = bytesWriten;
			}
		}

		//@ts-expect-error TS detects the necessary classes correctly but not on window object
		window.USBOutTransferResult = USBOutTransferResult;
	}

	if (!('USBInTransferResult' in window)) {
		class USBInTransferResult {
			readonly data?: DataView | undefined;
			readonly status?: USBTransferStatus | undefined;

			constructor(status: USBTransferStatus, data?: DataView) {
				this.status = status;
				this.data = data;
			}
		}

		//@ts-expect-error See above
		window.USBInTransferResult = USBInTransferResult;
	}
}
