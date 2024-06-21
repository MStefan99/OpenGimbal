#include "ControlResponse.hpp"

ControlResponse::ControlResponse(ResponseType type, uint8_t dataLength) {
	this->_buffer[0] = (dataLength << 4u) | static_cast<uint8_t>(type);
}

ControlResponse::buffer_type& ControlResponse::getBuffer() {
	return this->_buffer;
}

ControlResponse::size_type ControlResponse::getLength() {
	return (this->_buffer[0] >> 4u) + 1;
}
