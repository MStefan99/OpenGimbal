/*
 * File:   i2c.h
 * Author: mikha
 *
 * Created on November 23, 2021, 5:12 PM
 */

#ifndef I2C_HPP
#define I2C_HPP


#include "device.h"

#include "dma.hpp"
#include "util.hpp"

namespace i2c {
	void init();

	void write(
	    uint8_t  devAddr,
	    uint8_t* buf,
	    uint8_t  size = 1,
	    void (*cb)(bool success, const dma::I2CTransfer& transfer) = nullptr,
	    bool littleEndian = false
	);
	void read(
	    uint8_t devAddr,
	    uint8_t size = 1,
	    void (*cb)(bool success, const dma::I2CTransfer& transfer) = nullptr,
	    bool littleEndian = false
	);

	void writeRegister(
	    uint8_t  devAddr,
	    uint8_t  regAddr,
	    uint8_t* buf,
	    uint8_t  size = 1,
	    void (*cb)(bool success, const dma::I2CTransfer& transfer) = nullptr,
	    bool littleEndian = false
	);
	void readRegister(
	    uint8_t devAddr,
	    uint8_t regAddr,
	    uint8_t size = 1,
	    void (*cb)(bool success, const dma::I2CTransfer& transfer) = nullptr,
	    bool littleEndian = false
	);
}

#endif /* I2C_HPP */
