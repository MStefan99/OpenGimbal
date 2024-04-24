/* 
 * File:   Command.hpp
 * Author: Mikhail
 *
 * Created on February 20, 2024, 8:23 PM
 */

#ifndef COMMAND_HPP
#define	COMMAND_HPP

#include "device.h"

#include "util.hpp"


class Command {
public:
    using data_type = uint8_t;
    using buffer_type = data_type[16];
    using size_type = uint8_t;
    
    enum class CommandType : uint8_t {
        Sleep = 0x0,
        Position = 0x1,
        Tone = 0x2,
        Haptic = 0x3,
        Offset = 0x4,
        Calibrate = 0x5,
        GetVariable = 0x6,
        SetVariable = 0x7
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
    
    Command(data_type srcAddr, data_type destAddr, data_type cmdType, data_type cmdLength, buffer_type cmdData);
    
    const buffer_type& getBuffer();
    size_type getLength();

protected:
    buffer_type _buffer = {0};
    size_type _length {0};
};


class GetVariableCommand : Command {
    GetVariableCommand() = default;
};


class ReturnVariableCommand : public Command {
public:
    template <class T>
    ReturnVariableCommand(data_type srcAddr, data_type destAddr, Command::Variable variableID, T value);
};

template <class T>
ReturnVariableCommand::ReturnVariableCommand(data_type srcAddr, data_type destAddr, Command::Variable variableID, T value): 
    Command {srcAddr, destAddr, 0x0, 1 + sizeof(T), nullptr} {
    this->_buffer[2] = static_cast<uint8_t>(variableID);
    for (size_type i {0}; i < sizeof(T); ++i) {
        this->_buffer[sizeof(T) + 2 - i] = value;
        value >>= 8;
    }
}

#endif	/* COMMAND_HPP */
