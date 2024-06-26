/*
 * File:   dma.h
 * Author: mikha
 *
 * Created on November 28, 2021, 1:42 PM
 */

#ifndef DMA_HPP
#define DMA_HPP

#include "device.h"

#include "RingBuffer.hpp"


constexpr static uint8_t I2C_BUFFER_LEN {8};

constexpr static uint8_t DMA_CH_COUNT {2};

constexpr static uint8_t DMA_CH_I2C_TX {0};
constexpr static uint8_t DMA_CH_I2C_RX {1};

namespace dma {
	enum I2CTransferType : uint8_t {
		Read,
		Write,
		WriteRead
	};

	struct __attribute__((packed)) I2CTransfer {
		uint8_t         devAddr;
		uint8_t         regAddr;
		uint8_t         buf[I2C_BUFFER_LEN];
		uint8_t         len;
		I2CTransferType type;
		void            (*cb)(bool success, const I2CTransfer& transfer);
		bool            littleEndian;
	};

	void init();
	void initI2C();

	void startTransfer(const I2CTransfer& transfer);
}

#endif /* DMA_HPP */
