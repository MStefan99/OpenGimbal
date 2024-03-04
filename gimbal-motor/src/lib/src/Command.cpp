#include "lib/inc/Command.hpp"


Command::Command(data_type srcAddr, data_type destAddr, data_type cmdType, data_type cmdLength, buffer_type cmdData) {
    this->length = 2 + cmdLength;
    this->buffer[0] = (this->length << 4u) | destAddr;
    this->buffer[1] = (srcAddr << 4u) | cmdType;
    
    if (cmdData) {
        util::copy(this->buffer + 2, cmdData, cmdLength);
    }
}

const Command::buffer_type& Command::getBuffer() {
    return this->buffer;
}

Command::size_type Command::getLength() {
    return this->length;
}
