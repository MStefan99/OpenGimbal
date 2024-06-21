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

#include "util.hpp"
#include "pwm.hpp"
#include "nvm.hpp"
#include "bldc.hpp"
#include "i2c.hpp"
#include "uart.hpp"
#include "PID.hpp"
#include "AS5600.hpp"
#include "MovementController.hpp"
#include "Response.hpp"
#include "LowPassFilter.hpp"
#include "Matrix.hpp"
#include "Kalman.hpp"


enum class Mode : uint8_t {
    Sleep = 0x0,
    Calibrate = 0x1,
    Drive = 0x2,
    Haptic = 0x3,
    Idle = 0x04
};

#endif	/* MAIN_HPP */
