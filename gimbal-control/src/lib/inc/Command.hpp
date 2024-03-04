/* 
 * File:   main_board.hpp
 * Author: Mikhail
 *
 * Created on January 30, 2024, 9:36 AM
 */

#ifndef MAIN_BOARD_HPP
#define	MAIN_BOARD_HPP

#include "device.h"

#include "uart.hpp"


class Command {
public:
    using data_type = uint8_t;
    using buffer_type = data_type[16];
    using size_type = uint8_t;
    
    enum class OutgoingCommand: uint8_t {
        Sleep = 0x0,
        Move = 0x1,
        Calibrate = 0x2,
        GetVariable = 0x3,
        SetVariable = 0x4
    };
    
    enum class IncomingCommand: uint8_t {
        GetVariable = 0x0
    };
    
    enum class Variable: uint8_t {
        Mode = 0x0,
        Battery = 0x1,
        TempController = 0x2,
        TempMotors = 0x3,
        Error = 0xf
    };
    
    Command(data_type srcAddr, data_type destAddr, data_type cmdType, data_type cmdLength, buffer_type cmdData);
    
    const buffer_type& getBuffer();
    size_type getLength();

protected:
    buffer_type buffer = {0};
    size_type length {0};
};


class ReturnVariableCommand : public Command {
public:
    template <class T>
    ReturnVariableCommand(data_type srcAddr, data_type destAddr, Command::Variable variableID, T value);
};

template <class T>
ReturnVariableCommand::ReturnVariableCommand(data_type srcAddr, data_type destAddr, Command::Variable variableID, T value): 
    Command {srcAddr, destAddr, 0x0, 1 + sizeof(T), nullptr} {
    this->buffer[2] = static_cast<uint8_t>(variableID);
    for (size_type i {0}; i < sizeof(T); ++i) {
        this->buffer[sizeof(T) + 2 - i] = value;
        value >>= 8;
    }
}

#endif	/* MAIN_BOARD_HPP */
