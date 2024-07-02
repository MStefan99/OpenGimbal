/*
 * File:   main.hpp
 * Author: Mikhail
 *
 * Created on March 4, 2024, 12:33 PM
 */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdio>

#include "device.h"

#include "adc.hpp"
#include "buttons.hpp"
#include "Command.hpp"
#include "nvm.hpp"
#include "pwm.hpp"
#include "uart.hpp"
#include "util.hpp"


using PowerMode = Command::PowerMode;

constexpr uint16_t MAX_SHORT_PRESS_TIME {600};
constexpr uint16_t MAX_LONG_PRESS_TIME {2000};
constexpr uint16_t MAX_PRESS_WAIT_TIME {400};
constexpr uint16_t LONG_PRESS_STEP_TIME {250};
constexpr uint16_t VOLTAGE_DISPLAY_TIME {250};

constexpr int16_t fullRevolution {4096};
constexpr int16_t halfRevolution {fullRevolution / 2};
constexpr int16_t quarterRevolution {fullRevolution / 4};


enum class DisplayState : uint8_t {
	Off = 0x0,
	GimbalMode = 0x1,
	BatteryLevel = 0x2
};


enum class GimbalMode : uint8_t {
	Horizon = 0x0,
	Follow = 0x1,
	FPV = 0x2,
	Tilt = 0x3
};


#define DV_OUT 0

#endif /* MAIN_HPP */
