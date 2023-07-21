/* 
 * File:   mpu6050.h
 * Author: MStefan99
 *
 * Created on March 24, 2022, 7:44 PM
 */

#ifndef MPU6050_H
#define	MPU6050_H

#include "device.h"

#include "util.hpp"
#include "lib/inc/i2c.hpp"


namespace mpu6050 {
	// I2C needs to be initialized first
	void init();
	void update();
    
    const float* getAcc();
	const float* getRot();
	
	const int16_t* getRawAcc();
	const int16_t* getRawRot();
}

#endif	/* MPU6050_H */

