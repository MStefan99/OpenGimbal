#include "Response.hpp"

Response::Response(uint8_t srcAddr, uint8_t destAddr, ResponseType type, uint8_t dataLength) {
	this->_buffer[0] = ((dataLength + 1) << 4u) | destAddr;
	this->_buffer[1] = (srcAddr << 4u) | static_cast<uint8_t>(type);
}

Response::buffer_type& Response::getBuffer() {
	return this->_buffer;
}

Response::size_type Response::getLength() {
	return (this->_buffer[0] >> 4u) + 1;
}
