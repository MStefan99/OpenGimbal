/* 
 * File:   pwm.hpp
 * Author: Mikhail
 *
 * Created on January 23, 2024, 12:50 PM
 */

#ifndef PWM_HPP
#define	PWM_HPP

#include "device.h"


namespace pwm {
    void init();
    
    void setDuty(uint8_t channel, uint8_t duty);
}

#endif	/* PWM_HPP */

