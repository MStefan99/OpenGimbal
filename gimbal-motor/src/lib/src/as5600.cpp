#include "lib/inc/AS5600.hpp"


void AS5600::init() {
    uint8_t reg {0x04};
    i2c::writeRegister(0x36, 0x07, &reg);
}

void AS5600::getAngle(void (*cb)(bool success, const dma::I2CTransfer& transfer)) {
    i2c::readRegister(0x36, 0x0e, 2, cb);
}
