/*
 * File:   Response.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 5:45 PM
 */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "device.h"

namespace Command {
	enum class CommandType : uint8_t {
		Sleep = 0x0,
		Idle = 0x1,
		Wake = 0x2,
		Move = 0x3,
		Tone = 0x4,
		Haptic = 0x5,
		AdjustOffset = 0x6,
		GetVariable = 0xe,
		SetVariable = 0xf
	};

	enum class Variable : uint8_t {
		Options = 0x00,
		Offset = 0x01,
		Position = 0x02,
		Speed = 0x03,
		Temperature = 0x04
	};
}

class Response {
public:
	using buffer_type = uint8_t[16];
	using size_type = uint8_t;
	using Variable = Command::Variable;

	enum class ResponseType : uint8_t {
		ReturnVariable = 0xf
	};

	Response(uint8_t srcAddr, uint8_t destAddr, ResponseType type, uint8_t dataLength = 0);

	buffer_type& getBuffer();
	size_type    getLength();

protected:
	buffer_type _buffer = {0};
};

class ReturnVariableResponse: public Response {
public:
	template <class T>
	ReturnVariableResponse(uint8_t srcAddr, uint8_t destAddr, Response::Variable variableID, T value);
};

template <class T>
ReturnVariableResponse::ReturnVariableResponse(
    uint8_t            srcAddr,
    uint8_t            destAddr,
    Response::Variable variableID,
    T                  value
):
  Response {srcAddr, destAddr, ResponseType::ReturnVariable, 1 + sizeof(T)} {
	this->_buffer[2] = static_cast<uint8_t>(variableID);

	for (size_type i {0}; i < sizeof(T); ++i) {
		this->_buffer[sizeof(T) + 2 - i] = value;
		value >>= 8;
	}
}

#endif /* RESPONSE_HPP */
