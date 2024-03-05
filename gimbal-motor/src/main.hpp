/* 
 * File:   main.hpp
 * Author: Mikhail
 *
 * Created on November 8, 2023, 5:24 PM
 */

#ifndef MAIN_HPP
#define	MAIN_HPP

#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

//#include <cstdio>
//#include <cinttypes>

#include "lib/inc/util.hpp"
#include "lib/inc/pwm.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/PID.hpp"
#include "lib/inc/as5600.hpp"
#include "lib/inc/MovementController.hpp"
#include "lib/inc/Command.hpp"
#include "lib/inc/LowPassFilter.hpp"


// Encoder counts per one full revolution
static constexpr uint16_t fullRevolution {4096};
// Minimum torque to get the motor moving
static constexpr uint8_t idleTorque {140};
// Device address
static constexpr uint8_t deviceAddress {1};
/* This value is used for sliding mode control
 *
 * The difference between the current and target angle will be divided by this value
 * and the result will be used as a target speed.
 * The lower the value, the faster and more aggressive the control will be
 * but raising it too high will result in overshoot and oscillation.
 */
static constexpr float speedMultiplier {100.0f};
// Motor dead zone in degrees
static constexpr float deadZone {5.0f};


enum class Mode : uint8_t {
    Sleep = 0x0,
    Calibrate = 0x1,
    Drive = 0x2,
    Haptic = 0x3
};

#endif	/* MAIN_HPP */
