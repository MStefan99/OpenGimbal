#include "lib/inc/as5600.hpp"


void as5600::init() {
    uint8_t reg {0x07};
    i2c::writeRegister(0x36, 0x07, &reg);
}

void as5600::getAngle(void (*cb)(bool success, const dma::I2CTransfer& transfer)) {
    i2c::readRegister(0x36, 0x0e, 2, cb);
}
