#include "AS5600.hpp"

void AS5600::init() {
	uint8_t reg {AS5600_CONF_HYST_1LSB};
	i2c::write(AS5600_ADDR, AS5600_CONF_ADDR_0, &reg);
}

void AS5600::getAngle(void (*cb)(bool success, const i2c::Transfer& transfer)) {
	i2c::read(AS5600_ADDR, AS5600_ANGLE_ADDR_0, 2, cb);
}
