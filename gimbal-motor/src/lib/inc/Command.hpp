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
    
    Command(data_type srcAddr, data_type destAddr, data_type cmdType, data_type cmdLength, buffer_type cmdData);
    
    const buffer_type& getBuffer();
    size_type getLength();

protected:
    buffer_type buffer = {0};
    size_type length {0};
};


class GetVariableCommand : Command {
    GetVariableCommand() = default;
};


class ReturnVariableCommand : public Command {
public:
    template <class T>
    ReturnVariableCommand(data_type srcAddr, data_type destAddr, data_type variableLength, T variable);
};

template <class T>
ReturnVariableCommand::ReturnVariableCommand(data_type srcAddr, data_type destAddr, data_type variableIndex, T variable): 
Command {srcAddr, destAddr, 0x0, 1 + sizeof(T), nullptr} {
    for (size_type i {0}; i < sizeof(T); ++i) {
        this->buffer[sizeof(T) + 2 - i] = variable;
        variable >>= 8;
    }
}

#endif	/* COMMAND_HPP */

