/*
 * File:   main.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 5:35 PM
 */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <cmath>
#include <cstdio>

#include "device.h"

#include "adc.hpp"
#include "i2c.hpp"
#include "LowPassFilter.hpp"
#include "LSM6DSO32.hpp"
#include "Mahony.hpp"
#include "motor.hpp"
#include "MotorCommand.hpp"
#include "uart.hpp"
#include "usb.hpp"
#include "util.hpp"

enum class GimbalMode : uint8_t {
	Horizon = 0x0,
	Follow = 0x1,
	FPV = 0x2
};

enum class PowerMode : uint8_t {
	Sleep = 0x0,
	Idle = 0x1,
	Startup = 0x2,
	Active = 0x3,
	Shutdown = 0x4
};

constexpr uint16_t MIN_VOLTAGE {2900};
constexpr uint16_t MAX_VOLTAGE {3900};

#define DV_OUT 0

#endif /* MAIN_HPP */
