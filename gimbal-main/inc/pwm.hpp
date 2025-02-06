/*
 * File:   pwm.hpp
 * Author: Mikhail
 *
 * Created on January 23, 2024, 12:50 PM
 */

#ifndef PWM_HPP
#define PWM_HPP

#include <cmath>

#include "device.h"

namespace pwm {
	constexpr float gamma {4.0f};

	void init();

	void setDuty(uint8_t channel, uint16_t duty);
	void setBrightness(uint8_t channel, uint16_t brightness);
}

#endif /* PWM_HPP */
