#include "lib/inc/as5600.hpp"

void as5600::getAngle(uint16_t& angle, void (*cb)(bool success)) {
    i2c::readRegister(0x36, 0x0e, reinterpret_cast<uint8_t*>(&angle), 2, cb);
}
