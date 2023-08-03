#include "lib/inc/as5600.hpp"

static uint8_t reg {0x07};

void as5600::init() {    
    i2c::writeRegister(0x36, 0x07, &reg);
}

void as5600::getAngle(uint16_t& angle, void (*cb)(bool success)) {
    i2c::readRegister(0x36, 0x0e, reinterpret_cast<uint8_t*>(&angle), 2, cb);
}
