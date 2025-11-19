#include "USBResponse.hpp"

USBResponse::USBResponse(ResponseType type, uint8_t dataLength) {
	this->_buffer[0] = static_cast<uint8_t>(type);
	this->_length = dataLength + 1;
}

USBResponse::buffer_type& USBResponse::getBuffer() {
	return this->_buffer;
}

USBResponse::size_type USBResponse::getLength() {
	return this->_length;
}

USBReturnVariableResponse::USBReturnVariableResponse(USBResponse::Variable variable, uint8_t* data, uint8_t len):
  USBResponse {ResponseType::ReturnVariable, static_cast<USBResponse::size_type>(1 + len)} {
	this->_buffer[1] = static_cast<uint8_t>(variable);

	util::copy(this->_buffer + 2, data, len);
}
