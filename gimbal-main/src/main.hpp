/* 
 * File:   main.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 5:35 PM
 */

#ifndef MAIN_HPP
#define	MAIN_HPP

#include <cstdio>
#include <cmath>

#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/LSM6DSO32.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/adc.hpp"
#include "lib/inc/Mahony.hpp"
#include "lib/inc/LowPassFilter.hpp"
#include "lib/inc/MotorCommand.hpp"
#include "lib/inc/ControlResponse.hpp"
#include "lib/inc/motor.hpp"


// Using the same modes as in the protocol for now but this allows for more states as needed
using PowerMode = ControlCommand::PowerMode;
using GimbalMode = ControlCommand::GimbalMode;

constexpr uint16_t MIN_VOLTAGE {2900};
constexpr uint16_t MAX_VOLTAGE {3900};

#define DV_OUT 0

#endif	/* MAIN_HPP */

