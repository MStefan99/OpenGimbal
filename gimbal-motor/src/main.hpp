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


enum class CommandType : uint8_t {
    Sleep = 0x0,
    Position = 0x1,
    Tone = 0x2,
    Haptic = 0x3,
    Offset = 0x4,
    Calibrate = 0x5,
    GetVariable = 0x6,
    SetVariable = 0x7
};


enum class Mode : uint8_t {
    Sleep = 0x0,
    Calibrate = 0x1,
    Drive = 0x2,
    Haptic = 0x3
};


enum class CalibrationMode : uint8_t {
    Zero = 0u,
    Pole = 1u
};


#endif	/* MAIN_HPP */
