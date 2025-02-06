/*
 * File:   lsm6dso.hpp
 * Author: Mikhail
 *
 * Created on June 29, 2023, 6:04 PM
 */

#ifndef LSM6DSO32_HPP
#define LSM6DSO32_HPP

#include "device.h"
#include "LSM6DSO32_regs.h"

#include "i2c.hpp"
#include "Matrix.hpp"
#include "util.hpp"

namespace LSM6DSO32 {
	// I2C needs to be initialized first
	void init();
	void update();

	Vector3<int16_t, uint8_t> getRawAccelerations();
	Vector3<int16_t, uint8_t> getRawAngularRates();

	void setOffsets(const Vector3<float, uint8_t>& offsets);

	Vector3<float, uint8_t> getAccelerations();
	Vector3<float, uint8_t> getAngularRates();
}

#endif /* LSM6DSO32_HPP */
