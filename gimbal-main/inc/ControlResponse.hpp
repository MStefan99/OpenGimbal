/*
 * File:   ControlResponse.hpp
 * Author: Mikhail
 *
 * Created on June 14, 2024, 5:11 PM
 */

#ifndef CONTROL_RESPONSE_HPP
#define CONTROL_RESPONSE_HPP

#include "device.h"

namespace ControlCommand {
	enum class CommandType : uint8_t {
		Calibrate = 0x0,
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
}

class ControlResponse {
public:
	using buffer_type = uint8_t[16];
	using size_type = uint8_t;
	using Variable = ControlCommand::Variable;
	using PowerMode = ControlCommand::PowerMode;
	using GimbalMode = ControlCommand::GimbalMode;

	enum class ResponseType : uint8_t {
		ReturnVariable = 0xf
	};

	ControlResponse(ResponseType type, uint8_t dataLength = 0);

	buffer_type& getBuffer();
	size_type    getLength();

protected:
	buffer_type _buffer = {0};
};

class ReturnVariableResponse: public ControlResponse {
public:
	template <class T>
	ReturnVariableResponse(ControlResponse::Variable variableID, T value);
};

template <class T>
ReturnVariableResponse::ReturnVariableResponse(ControlResponse::Variable variableID, T value):
  ControlResponse {ResponseType::ReturnVariable, 1 + sizeof(T)} {
	this->_buffer[1] = static_cast<uint8_t>(variableID);

	for (size_type i {0}; i < sizeof(T); ++i) {
		this->_buffer[sizeof(T) + 1 - i] = value;
		value >>= 8;
	}
}

#endif /* CONTROL_RESPONSE_HPP */
