/* 
 * File:   MotorCommand.hpp
 * Author: Mikhail
 *
 * Created on June 11, 2024, 7:51 PM
 */

#ifndef MOTOR_COMMAND_HPP
#define	MOTOR_COMMAND_HPP

#include "device.h"


class MotorCommand {
public:
    using buffer_type = uint8_t[16];
    using size_type = uint8_t;
    
    enum class CommandType : uint8_t {
        Sleep = 0x0,
        Move = 0x1,
        Tone = 0x2,
        Haptic = 0x3,
        AdjustOffset = 0x4,
        Calibrate = 0x5,
        GetVariable = 0xe,
        SetVariable = 0xf
    };

    enum class Variable : uint8_t {
        Calibration = 0x0,
        Offset = 0x1,
        Range = 0x2,
        Error = 0xf
    };
    
    enum class CalibrationMode : uint8_t {
        Zero = 0u,
        Pole = 1u
    };
    
    MotorCommand(uint8_t srcAddr, uint8_t destAddr, CommandType type, uint8_t dataLength = 0);
    
    buffer_type& getBuffer();
    size_type getLength();
    
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

#endif	/* MOTOR_COMMAND_HPP */

