/* 
 * File:   dma.h
 * Author: mikha
 *
 * Created on November 28, 2021, 1:42 PM
 */

#ifndef DMA_H
#define	DMA_H

#include "device.h"
#include "tl/allocator.hpp"
#include "tl/list.hpp"


#define DMA_CH_COUNT 6

#define DMA_CH_I2C_TX 0
#define DMA_CH_I2C_RX 1
#define DMA_CH_UART_TX 2
#define DMA_CH_UART_RX 3
#define DMA_CH_SBUS_TX 4
#define DMA_CH_SBUS_RX 5


namespace dma {
	enum I2CTransferType : uint8_t {
		Read,
		Write,
		WriteRead
	};

	typedef struct __attribute__((packed)) {
		uint8_t devAddr;
		uint8_t regAddr;
		uint8_t* buf;
		uint8_t len;
		I2CTransferType type;
		sercom_registers_t* sercom;
        void (*cb)(bool);
	}
	I2CTransfer;

	typedef struct __attribute__((packed)) {
		uint8_t* buf;
		uint8_t len;
		sercom_registers_t* sercom;
        void (*cb)(bool);
	}
	UARTTransfer;


	void init();
	void initI2C();
	void initUART();
	void initSBUS(uint8_t* rxBuf, uint16_t len);


	void startTransfer(const I2CTransfer& transfer);
	void startTransfer(const UARTTransfer& transfer);
}

#endif	/* DMA_H */

