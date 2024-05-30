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

		window.USBInTransferResult = USBInTransferResult;
	}
}
