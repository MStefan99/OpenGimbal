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
#include "ControlResponse.hpp"
#include "dma.hpp"
#include "i2c.hpp"
#include "LowPassFilter.hpp"
#include "LSM6DSO32.hpp"
#include "Mahony.hpp"
#include "motor.hpp"
#include "MotorCommand.hpp"
#include "uart.hpp"
#include "util.hpp"


// Using the same modes as in the protocol for now but this allows for more states as needed
using PowerMode = ControlCommand::PowerMode;
using GimbalMode = ControlCommand::GimbalMode;

constexpr uint16_t MIN_VOLTAGE {2900};
constexpr uint16_t MAX_VOLTAGE {3900};

#define DV_OUT 0

#endif /* MAIN_HPP */
