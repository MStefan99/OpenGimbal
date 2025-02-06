/*
 * File:   data.hpp
 * Author: Mikhail
 *
 * Created on June 22, 2023, 17:58 PM
 */

#ifndef DATA_HPP
#define DATA_HPP

#include "device.h"

#include "util.hpp"

namespace data {
	enum class CommandType : uint8_t {
		GetVariable = 0x0,
		SetVariable = 0x1,
		Calibration = 0x2
	};

	enum class ResponseType : uint8_t {
		ReturnVariable = 0x0
	};

	enum class VariableID : uint8_t {
		Status = 0x0,
		Sensors = 0x1
	};

	struct __attribute__((packed)) USBStatusResponse {
		const uint8_t responseType {static_cast<uint8_t>(ResponseType::ReturnVariable)};
		const uint8_t variableID {static_cast<uint8_t>(VariableID::Status)};
		int16_t       yaw;
		int16_t       pitch;
		int16_t       roll;
	};

	struct __attribute__((packed)) USBSensorsResponse {
		const uint8_t responseType {static_cast<uint8_t>(ResponseType::ReturnVariable)};
		const uint8_t variableID {static_cast<uint8_t>(VariableID::Sensors)};
		uint8_t       activeSensors;
		int8_t        temperature;
		int16_t       accelerations[3];  // ZYX
		int16_t       angularRates[3];   // Yaw - pitch - roll
	};

	extern USBStatusResponse  usbStatusResponse;
	extern USBSensorsResponse usbSensorsResponse;
}  // namespace data

#endif /* DATA_HPP */
