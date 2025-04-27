/*
 * File:   USBCommand.hpp
 * Author: Mikhail
 *
 * Created on June 22, 2023, 17:58 PM
 */

#ifndef USB_COMMAND_HPP
#define USB_COMMAND_HPP

#include "device.h"

#include "util.hpp"

namespace USBCommand {
	enum class CommandType : uint8_t {
		Disable = 0x0,
		Enable = 0x1,
		SetVariable = 0x0d,
		GetVariable = 0x0e,
		MotorPassthrough = 0x0f
	};

	enum class Variable : uint8_t {
		Orientation = 0x00,
		HandleOrientation = 0x01,
		Mode = 0x02,
		BatteryVoltage = 0x03
	};
}

class USBResponse {
public:
	using buffer_type = uint8_t[16];
	using size_type = uint8_t;

	using Variable = USBCommand::Variable;

	enum class ResponseType : uint8_t {
		ReturnVariable = 0x0,
		MotorPassthrough = 0xf
	};

	struct __attribute__((packed)) USBOrientationResponse {
		int16_t yaw;
		int16_t pitch;
		int16_t roll;
	};

	struct __attribute__((packed)) USBSensorResponse {
		int16_t accelerations[3];  // ZYX
		int16_t angularRates[3];   // Yaw - pitch - roll
	};

	USBResponse() = default;
	USBResponse(ResponseType type, uint8_t* data = nullptr, uint8_t len = 0);

	buffer_type& getBuffer();
	size_type    getLength();

	const buffer_type& getBuffer() const;
	const size_type    getLength() const;

protected:
	buffer_type _buffer = {0};
};


#endif /* DATA_HPP */
