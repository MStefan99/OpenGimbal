/* 
 * File:   servo.h
 * Author: mikha
 *
 * Created on November 20, 2021, 4:05 PM
 */

#ifndef SERVO_H
#define	SERVO_H

#include "device.h"

#include "util.hpp"


namespace servo {
	void init();
	
	// Channels start from 0
	void enable(uint8_t channel);
	void disable(uint8_t channel);
    
    // Values from -1500 to 1500
	void setChannel(uint8_t channel, int16_t angle);
}


#endif	/* PWM_H */

