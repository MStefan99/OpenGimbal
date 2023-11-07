/* 
 * File:   dma.h
 * Author: mikha
 *
 * Created on November 28, 2021, 1:42 PM
 */

#ifndef DMA_H
#define	DMA_H

#include "device.h"
#include "RingBuffer.hpp"


static constexpr uint8_t I2C_BUFFER_LEN {8};
static constexpr uint8_t UART_BUFFER_LEN {16};


static constexpr uint8_t DMA_CH_COUNT {4};

static constexpr uint8_t DMA_CH_I2C_TX {0};
static constexpr uint8_t DMA_CH_I2C_RX {1};
static constexpr uint8_t DMA_CH_UART_TX {2};
static constexpr uint8_t DMA_CH_UART_RX {3};


namespace dma {
	enum I2CTransferType : uint8_t {
		Read,
		Write,
		WriteRead
	};
    
    enum UARTTransferType: uint8_t {
        Out,
        In
    };

	struct __attribute__((packed)) I2CTransfer {
		uint8_t devAddr;
		uint8_t regAddr;
		uint8_t buf[I2C_BUFFER_LEN];
		uint8_t len;
		I2CTransferType type;
        void (*cb)(bool success, const I2CTransfer& transfer);
		bool littleEndian;
	};

	struct __attribute__((packed)) UARTTransfer {
		uint8_t buf[UART_BUFFER_LEN];
		uint8_t len;
        UARTTransferType type;
        void (*cb)(bool success, const UARTTransfer& transfer);
        bool littleEndian;
	};

	void init();
	void initI2C();
	void initUART();

	void startTransfer(const I2CTransfer& transfer);
	void startTransfer(const UARTTransfer& transfer);
}

#endif	/* DMA_H */

