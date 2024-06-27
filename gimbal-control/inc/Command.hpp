/*
 * File:   main_board.hpp
 * Author: Mikhail
 *
 * Created on January 30, 2024, 9:36 AM
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "device.h"

#include "uart.hpp"

class Command {
public:
	using buffer_type = uint8_t[16];
	using size_type = uint8_t;

	enum class CommandType : uint8_t {
		Move = 0x0,
		Calibrate = 0x1,
		GetVariable = 0xe,
		SetVariable = 0xf
	};

	enum class Variable : uint8_t {
		PowerMode = 0x0,
		GimbalMode = 0x1,
		BatteryVoltage = 0x2,
		YawOffset = 0x3,
		PitchOffset = 0x4,
		RollOffset = 0x5
	};

	enum class PowerMode : uint8_t {
		Active = 0x0,
		Idle = 0x1,
		Sleep = 0x2
	};

	enum class GimbalMode : uint8_t {
		Horizon = 0x0,
		Follow = 0x1,
		FPV = 0x2
	};

	Command(CommandType cmdType, uint8_t dataLength);

	buffer_type& getBuffer();
	size_type    getLength();

protected:
	buffer_type _buffer = {0};
};

class GetVariableCommand: public Command {
public:
	GetVariableCommand(Command::Variable variableID);
};

class SetVariableCommand: public Command {
public:
	template <class T>
	SetVariableCommand(Command::Variable variableID, T value);
};

template <class T>
SetVariableCommand::SetVariableCommand(Command::Variable variableID, T value):
  Command {CommandType::SetVariable, 1 + sizeof(T)} {
	this->_buffer[1] = static_cast<uint8_t>(variableID);

	for (size_type i {0}; i < sizeof(T); ++i) {
		this->_buffer[sizeof(T) + 1 - i] = value;
		value >>= 8;
	}
}

#endif /* COMMAND_HPP */
