#include "lib/inc/Command.hpp"


Command::Command(data_type srcAddr, data_type destAddr, data_type cmdType, data_type cmdLength, buffer_type cmdData) {
    this->_length = 2 + cmdLength;
    this->_buffer[0] = (this->_length << 4u) | destAddr;
    this->_buffer[1] = (srcAddr << 4u) | cmdType;
    
    if (cmdData) {
        util::copy(this->_buffer + 2, cmdData, cmdLength);
    }
}

const Command::buffer_type& Command::getBuffer() {
    return this->_buffer;
}

Command::size_type Command::getLength() {
    return this->_length;
}
