/* 
 * File:   lsm6dso.hpp
 * Author: Mikhail
 *
 * Created on June 29, 2023, 6:04 PM
 */

#ifndef LSM6DSO_HPP
#define	LSM6DSO_HPP

#include "device.h"

#include "util.hpp"
#include "Matrix.hpp"
#include "lib/inc/i2c.hpp"

namespace lsm6dso {
	// I2C needs to be initialized first
	void init();
	void update();
    
    Vector3<float, uint8_t> getAcc();
	Vector3<float, uint8_t> getRot();
}

#endif	/* LSM6DSO_HPP */

