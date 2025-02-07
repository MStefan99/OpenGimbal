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


/* This will enable diagnostic output for Data Visualizer
 * Only use when no other devices are connected as the data sent
 * might be misinterpreted by them
 */
#define DV_OUT 0

// Encoder counts per one full revolution
constexpr static uint16_t fullRevolution {4096};
/* Motor range can be limited to avoid tangling the wires
 * The range is the maximum motor deflection from the zero point in either direction
 * An offset of 2048 means the motor can take any position from -2048 to 2048,
 * or one full revolution. The range can be changed over UART.
 */
constexpr static uint16_t defaultRange {0};
/* Arriving position commands are interpolated to avoid jerky movements and noise
 * Maximum interpolation time can be adjusted here
 */
constexpr static uint32_t maxInterpolationTime {500};
// Minimum torque to get the motor moving
constexpr static uint8_t  idleTorque {140};
// Interval in milliseconds at which torque direction will flip in haptic mode
constexpr static uint8_t  hapticCycleDuration {25};
// Device address
constexpr static uint8_t  deviceAddress {1};


// Full state feedback gain matrix
constexpr auto K =
    Matrix<float, uint8_t, 1, 2> {
      {30, 8}
}
    * 15;

#endif /* OPTIONS_HPP */
