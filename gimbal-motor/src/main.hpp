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
    Sleep = 0x1,
    Position = 0x2,
    Tone = 0x3,
    Haptic = 0x4,
    Offset = 0x5,
    Calibrate = 0x6,
    GetVariable = 0x7,
    SetVariable = 0x8
};


#endif	/* MAIN_HPP */
