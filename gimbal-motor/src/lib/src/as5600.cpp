#include "lib/inc/as5600.hpp"

static uint8_t angle[2] {0};
    
void as5600::update() {
    i2c::readRegister(0x36, 0x0e, angle, 2);
}

uint16_t as5600::getAngle() {
    return angle[0] << 8 | angle[1];
}
