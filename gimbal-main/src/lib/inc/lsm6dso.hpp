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
#include "lib/inc/i2c.hpp"

namespace lsm6dso {
	// I2C needs to be initialized first
	void init();
	void update();
    
    const float* getAcc();
	const float* getRot();
	
	const int16_t* getRawAcc();
	const int16_t* getRawRot();
}

#endif	/* LSM6DSO_HPP */

