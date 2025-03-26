/*
 * File:   main.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 5:35 PM
 */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <cmath>

#include "device.h"

#include "adc.hpp"
#include "buttons.hpp"
#include "i2c.hpp"
#include "joystick.hpp"
#include "LSM6DSO32.hpp"
#include "Mahony.hpp"
#include "motor.hpp"
#include "MotorCommand.hpp"
#include "pwm.hpp"
#include "uart.hpp"
#include "usb.hpp"
#include "util.hpp"


enum class DisplayState : uint8_t {
	GimbalMode = 0x0,
	BatteryLevel = 0x1
};

enum class GimbalMode : uint8_t {
	Horizon = 0x0,
	Follow = 0x1,
	FPV = 0x2,
	Tilt = 0x3
};

enum class PowerMode : uint8_t {
	Sleep = 0x0,
	Idle = 0x1,
	Startup = 0x2,
	Active = 0x3,
	Shutdown = 0x4
};

constexpr uint16_t MIN_VOLTAGE {2800};
constexpr uint16_t MAX_VOLTAGE {3800};

constexpr uint16_t MAX_SHORT_PRESS_TIME {500};
constexpr uint16_t MAX_LONG_PRESS_TIME {2000};
constexpr uint16_t MAX_PRESS_WAIT_TIME {600};
constexpr uint16_t LONG_PRESS_STEP_TIME {250};
constexpr uint16_t VOLTAGE_DISPLAY_TIME {250};
constexpr uint16_t USB_PASSTHROUGH_TIMEOUT {2};

constexpr float controlBoardAngle {-30.0f * F_DEG_TO_RAD};

constexpr int16_t fullRevolution {4096};
constexpr int16_t halfRevolution {fullRevolution / 2};
constexpr int16_t quarterRevolution {fullRevolution / 4};

#define DV_OUT 0

#endif /* MAIN_HPP */
