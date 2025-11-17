/*
 * File:   i2c.h
 * Author: mikha
 *
 * Created on November 23, 2021, 5:12 PM
 */

#ifndef I2C_HPP
#define I2C_HPP


#include "device.h"

#include "RingBuffer.hpp"
#include "util.hpp"

namespace i2c {
	struct Transfer {
		uint8_t buf[8] {0};

		uint8_t devAddr;
		uint8_t length {0};
		uint8_t transferred {0};

		union {
			struct {
				bool read          : 1;
				bool regAddrWritten: 1;
			} flags;

			uint8_t allFlags {0};
		};

		void (*cb)(bool success, const Transfer& transfer) {nullptr};
	};

	void init();

	void write(
	    uint8_t  devAddr,
	    uint8_t  regAddr,
	    uint8_t* buf,
	    uint8_t  size = 1,
	    void     (*cb)(bool success, const Transfer& transfer) = nullptr
	);
	void read(
	    uint8_t devAddr,
	    uint8_t regAddr,
	    uint8_t size = 1,
	    void    (*cb)(bool success, const Transfer& transfer) = nullptr
	);
	bool busy();
}

#endif /* I2C_HPP */
