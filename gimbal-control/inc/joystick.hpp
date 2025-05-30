/*
 * File:   joystick.hpp
 * Author: Mikhail
 *
 * Created on June 28, 2024, 12:40 PM
 */

#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include "device.h"

#include "adc.hpp"
#include "nvm.hpp"

namespace joystick {
	constexpr uint16_t deadzone {500};
	using callback_type = void (*)(int16_t x, int16_t y);

	void update(callback_type cb = nullptr);

	int16_t getAxis(uint8_t axis);

	void updateCenter();
	void saveValues();
}

#endif /* JOYSTICK_HPP */
