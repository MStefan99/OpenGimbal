/* 
 * File:   main.hpp
 * Author: Mikhail
 *
 * Created on March 4, 2024, 12:33 PM
 */

#ifndef MAIN_HPP
#define	MAIN_HPP

#include <cstdio>

#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/buttons.hpp"
#include "lib/inc/pwm.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/Command.hpp"


using GimbalMode = Command::GimbalMode;
using PowerMode = Command::PowerMode;

constexpr uint16_t MAX_SHORT_PRESS_TIME {7500};
constexpr uint16_t MAX_PRESS_WAIT_TIME {1000};
constexpr uint16_t LONG_PRESS_STEP_TIME {250};
constexpr uint16_t VOLTAGE_DISPLAY_TIME {250};


enum class DisplayState : uint8_t {
    Off = 0x0,
    ShortPress = 0x1,
    PressWait = 0x2,
    LongPress = 0x3,
    GimbalMode = 0x4,
    BatteryLevel = 0x5
};

#endif	/* MAIN_HPP */

