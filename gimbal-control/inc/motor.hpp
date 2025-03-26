/*
 * File:   motor.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 5:51 PM
 */

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "MotorCommand.hpp"
#include "uart.hpp"

namespace motor {
	constexpr uint8_t all {15};

	void process(uart::DefaultCallback cb);

	void sleep(uint8_t address = all);
	void move(uint8_t address = all, uint16_t position = 0, uint8_t torque = 15);
	void tone(uint8_t address = all, uint16_t frequency = 25000);
	void haptic(uint8_t address = all, uint8_t intensity = 15, uint16_t duration = 5);
	void adjustOffset(uint8_t address = all, uint16_t targetPosition = 0);
	void calibrate(uint8_t address = all, uint8_t mode = 3);

	void send(const uint8_t* buf, uint8_t len, void (*cb)() = nullptr);

	// TODO: set and get variable
}

#endif /* MOTOR_HPP */
