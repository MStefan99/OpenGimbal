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
		Idle = 0x0,
		Sleep = 0x1,
		Move = 0x2,
		Tone = 0x3,
		Haptic = 0x4,
		AdjustOffset = 0x5,
		Calibrate = 0x6,
		GetVariable = 0xe,
		SetVariable = 0xf
	};

	enum class Variable : uint8_t {
		Calibration = 0x0,
		Offset = 0x1,
		Position = 0x2,
		Speed = 0x3,
		Temperature = 0x4
	};

	enum class CalibrationMode : uint8_t {
		Zero = 0u,
		Pole = 1u
	};

	MotorCommand() = default;
	MotorCommand(uint8_t srcAddr, uint8_t destAddr, CommandType type, uint8_t dataLength = 0);

	buffer_type& getBuffer();
	size_type    getLength();

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
