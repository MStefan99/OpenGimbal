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
#include "lib/inc/nvm.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/PID.hpp"
#include "lib/inc/AS5600.hpp"
#include "lib/inc/MovementController.hpp"
#include "lib/inc/Response.hpp"
#include "lib/inc/LowPassFilter.hpp"
#include "lib/inc/Matrix.hpp"
#include "lib/inc/Kalman.hpp"


enum class Mode : uint8_t {
    Sleep = 0x0,
    Calibrate = 0x1,
    Drive = 0x2,
    Haptic = 0x3,
    Idle = 0x04
};

#endif	/* MAIN_HPP */
