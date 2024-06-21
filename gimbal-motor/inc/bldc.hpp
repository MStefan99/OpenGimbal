/*
 * File:   bldc.hpp
 * Author: Mikhail
 *
 * Created on July 9, 2023, 4:33 PM
 */

#ifndef BLDC_HPP
#define BLDC_HPP

#include <cmath>

#include "device.h"

#include "util.hpp"

namespace bldc {
	void init();
	void enable();
	void disable();

	// Angle from 0 to 4096 (0 = 0deg, 4096 = 360deg), power from 0 to 255
	void applyTorque(uint16_t angle, uint8_t power = 100);
	void removeTorque();
	void tone(uint16_t frequency);
	void silent();
}

#endif /* BLDC_HPP */
