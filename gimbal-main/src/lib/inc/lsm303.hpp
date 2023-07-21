/* 
 * File:   lsm303.h
 * Author: mikha
 *
 * Created on December 5, 2021, 10:04 PM
 */

#ifndef LSM303_H
#define	LSM303_H

#include "device.h"
#include "lib/inc/i2c.hpp"


namespace lsm303 {
	// I2C needs to be initialized first
	void init();
	void update();

	const int16_t* getAcc();
	const int16_t* getMag();

	int16_t getAccX();
	int16_t getAccY();
	int16_t getAccZ();

	int16_t getMagX();
	int16_t getMagY();
	int16_t getMagZ();
}


#endif	/* LSM303_H */

