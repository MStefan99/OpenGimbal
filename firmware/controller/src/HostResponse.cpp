#include "HostResponse.hpp"

HostResponse::HostResponse(ResponseType type, uint8_t dataLength) {
	this->_buffer[0] = static_cast<uint8_t>(type);
	this->_length = dataLength + 1;
}

HostResponse::buffer_type& HostResponse::getBuffer() {
	return this->_buffer;
}

HostResponse::size_type HostResponse::getLength() {
	return this->_length;
}

HostReturnVariableResponse::HostReturnVariableResponse(
    HostResponse::Variable variable,
    const uint8_t*         data,
    uint8_t                len
):
  HostResponse {ResponseType::ReturnVariable, static_cast<HostResponse::size_type>(1 + len)} {
	this->_buffer[1] = static_cast<uint8_t>(variable);

	util::copy(this->_buffer + 2, data, len);
}
