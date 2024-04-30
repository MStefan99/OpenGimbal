#include "lib/inc/AS5600.hpp"


void AS5600::init() {
    uint8_t reg {AS5600_CONF_HYST_1LSB};
    i2c::writeRegister(AS5600_ADDR, AS5600_CONF_ADDR_0, &reg);
}

void AS5600::getAngle(void (*cb)(bool success, const dma::I2CTransfer& transfer)) {
    i2c::readRegister(AS5600_ADDR, AS5600_ANGLE_ADDR_0, 2, cb);
}
