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
#include "lib/inc/Matrix.hpp"
#include "lib/inc/Kalman.hpp"

/* This will enable diagnostic output for Data Visualizer
 * Only use when no other devices are connected as the data sent
 * might be misinterpreted by them
 */
#define DV_OUT 0

// Encoder counts per one full revolution
static constexpr uint16_t fullRevolution {4096};
// Minimum torque to get the motor moving
static constexpr uint8_t idleTorque {140};
// Interval in milliseconds at which torque direction will flip in haptic mode
static constexpr uint8_t hapticCycleDuration {25};
// Device address
static constexpr uint8_t deviceAddress {1};

/* There are two controllers used in different situations.
 * LQG is used when there is a load (a smartphone, for example) is attached
 * to a motor for smooth control. However, if no load is attached to the motor,
 * system characteristics change significantly and a simple proportional
 * controller with no filtering/smoothing can provide better results due to a
 * much lower phase delay.
*/

// LQG settings
        
// LQG feedback gain matrix
constexpr auto K = Matrix<float, uint8_t, 1, 2>{{31.622776601683820,8.015332382588245}};
/* Controller gain
 * In the ideal case this wouldn't be needed but we need this adjustment since
 * moment of inertia of the load connected to the motor can't be known
 * in advance.
 * 
 * Higher values make controller more aggressive at the cost of power and 
 * possible oscillations/vibrations.
 */
constexpr float gain {10.0f};

// Kalman filter matrices
constexpr auto x0 = Matrix<float,
		unsigned, 3, 1> {{0},
                         {0},
                         {0}};
constexpr auto P0 = Matrix<float,
		unsigned, 3, 3> {{PI, 0,  0 },
                         {0,  5,  0 },
                         {0,  0,  20}};
constexpr auto Q = Matrix<float,
		unsigned, 3, 3> {{1e-4, 0, 0},
                         {0, 1e-4, 0},
                         {0, 0, 1e-4}};
constexpr auto R = Matrix<float,
		unsigned, 1, 1> {{30}};
        
// Mode switch settings

// Switch into no-load mode if acceleration exceeds this value (rad/s)
constexpr float switchAcceleration {0.2f};
// Smoothly interpolate between LQG and proportional controllers over this range (rad)
constexpr float switchSmoothness {0.1f};

// Proportional controller settings

// Gain for proportional controller (rad -> torque)
constexpr float pGain {400.0f};

enum class Mode : uint8_t {
    Sleep = 0x0,
    Calibrate = 0x1,
    Drive = 0x2,
    Haptic = 0x3,
    Idle = 0x04
};

#endif	/* MAIN_HPP */
