/* 
 * File:   as5600.hpp
 * Author: Mikhail
 *
 * Created on July 21, 2023, 6:34 PM
 */

#ifndef AS5600_HPP
#define	AS5600_HPP

#include "device.h"

#include "i2c.hpp"

namespace as5600 {
    // I2C needs to be initialized before using any methods
    
    void update();
    uint16_t getAngle();
}

#endif	/* AS5600_HPP */

