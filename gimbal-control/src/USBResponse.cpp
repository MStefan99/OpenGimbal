#include "USBResponse.hpp"

USBResponse::USBResponse(ResponseType type, uint8_t* data, uint8_t len) {
	_buffer[0] = static_cast<uint8_t>(type);
	util::copy(_buffer + 1, data, len);
}
