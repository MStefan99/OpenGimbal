/*
 * File:   as5600.hpp
 * Author: Mikhail
 *
 * Created on July 21, 2023, 6:34 PM
 */

#ifndef AS5600_HPP
#define AS5600_HPP

#include "AS5600_regs.h"
#include "device.h"

#include "i2c.hpp"

namespace AS5600 {
	// I2C needs to be initialized before using any methods below
	void init();

	void getAngle(void (*cb)(bool success, const dma::I2CTransfer& transfer) = nullptr);
}

#endif /* AS5600_HPP */
