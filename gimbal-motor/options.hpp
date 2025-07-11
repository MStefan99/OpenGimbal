/*
 * File:   options.hpp
 * Author: Mikhail
 *
 * Created on April 23, 2024, 5:13 PM
 */

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "Matrix.hpp"
#include "PID.hpp"
#include "util.hpp"


// Encoder counts per one full revolution
constexpr static uint16_t fullRevolution {4096};
/* Arriving position commands are interpolated to avoid jerky movements and noise
 * Maximum interpolation time can be adjusted here
 */
constexpr static uint32_t maxInterpolationTime {500};
// Minimum torque to get the motor moving
constexpr static uint8_t  idleTorque {140};
// Interval in milliseconds at which torque direction will flip in haptic mode
constexpr static uint8_t  hapticCycleDuration {25};
// Device address
constexpr static uint8_t  deviceAddress {2};
// Invert motor direction if PCB is flipped
constexpr static bool     invertDirection {false};

/* Timeouts will help save OpenGimbal battery if UART errors occur
 * but may cause issues when used with other devices. Set to 0 to disable
 */

// Enter sleep mode if no commands received in idle mode for a specified time
constexpr static uint32_t idleCommandTimeout {1000};
// Enter sleep mode if no commands received even in active mode
constexpr static uint32_t activeCommandTimeout {5000};

// Full state feedback gain matrix
constexpr auto K =
    Matrix<float, uint8_t, 1, 2> {
      {30, 8}
}
    * 15;

#endif /* OPTIONS_HPP */
