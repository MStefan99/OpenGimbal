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
#include "lib/inc/Mahony.hpp"
#include "lib/inc/LowPassFilter.hpp"
#include "lib/inc/motor.hpp"


enum class PowerMode : uint8_t {
    Sleep = 0x0,
    Active = 0x1
};

enum class GimbalMode : uint8_t {
    Horizon = 0x0,
    Follow = 0x1,
    FPV = 0x2
};

#define DV_OUT 0

#endif	/* MAIN_HPP */

