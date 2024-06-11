#include "lib/inc/Command.hpp"


Command::Command(uint8_t srcAddr, uint8_t destAddr, CommandType type, uint8_t dataLength) {
    this->_buffer[0] = ((dataLength + 1) << 4u) | destAddr;
    this->_buffer[1] = (srcAddr << 4u) | static_cast<uint8_t>(type);
}

Command::buffer_type& Command::getBuffer() {
    return this->_buffer;
}

Command::size_type Command::getLength() {
    return (this->_buffer[0] >> 4u) + 1;
}
