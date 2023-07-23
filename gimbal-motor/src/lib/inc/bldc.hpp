/* 
 * File:   bldc.hpp
 * Author: Mikhail
 *
 * Created on July 9, 2023, 4:33 PM
 */

#ifndef BLDC_HPP
#define	BLDC_HPP

#include "device.h"
#include <cmath>

#include "util.hpp"


namespace bldc {
    void init();
    
    // Angle from 0 to 3600 (1/10th of a degree), power from 0 to 100
    void applyTorque(uint16_t angle, uint8_t power = 100);
    void tone(uint16_t frequency);
    void silent();
}

#endif	/* BLDC_HPP */

