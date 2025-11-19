/*
 * File:   MotorCommand.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 7:51 PM
 */

#ifndef MOTOR_COMMAND_HPP
#define MOTOR_COMMAND_HPP

#include "device.h"

class MotorCommand {
public:
	using buffer_type = uint8_t[16];
	using size_type = uint8_t;

	enum class CommandType : uint8_t {
		Sleep = 0x0,
		Idle = 0x1,
		Wake = 0x2,
		Move = 0x3,
		Tone = 0x4,
		Haptic = 0x5,
		AdjustOffset = 0x6,
		Calibrate = 0x7,
		GetVariable = 0xe,
		SetVariable = 0xf
	};

	enum class Variable : uint8_t {
		Calibration = 0x00,
		Offset = 0x01,
		Position = 0x02,
		Speed = 0x03,
		Temperature = 0x04
	};

	enum class CalibrationMode : uint8_t {
		Zero = 0u,
		Pole = 1u
	};

	MotorCommand() = default;
	MotorCommand(uint8_t srcAddr, uint8_t destAddr, CommandType type, uint8_t dataLength = 0);

	buffer_type& getBuffer();
	size_type    getLength();

	const buffer_type& getBuffer() const;
	const size_type    getLength() const;

protected:
	buffer_type _buffer = {0};
};

namespace MotorResponse {
	using Variable = MotorCommand::Variable;
	using CalibrationMode = MotorCommand::CalibrationMode;

	enum class ResponseType : uint8_t {
		ReturnVariable = 0xf
	};
}

#endif /* MOTOR_COMMAND_HPP */
