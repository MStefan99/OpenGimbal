#include "lib/inc/lsm303.hpp"


#define LSM303_ADDR_ACC 0x19
#define LSM303_ADDR_MAG 0x1d


static uint8_t accBuf[6];
static uint8_t magBuf[6];

static int16_t accData[3]{0};


void lsm303::init() {
	uint8_t ctrl_reg1{0x47};
	i2c::writeRegister(LSM303_ADDR_ACC, 0x20, &ctrl_reg1, 1);
}


void lsm303::update() {
	for (uint8_t i{0}; i < 3; ++i) {
	accData[i] = ((int16_t) (accBuf[i * 2] | accBuf[i * 2 + 1] << 8u) >> 4u);
}
	i2c::readRegister(LSM303_ADDR_ACC, 0xa8, accBuf, 6);
}


const int16_t* lsm303::getAcc() {
	return accData;
}


int16_t lsm303::getAccX() {
	return accData[0];
}


int16_t lsm303::getAccY() {
	return accData[1];
}


int16_t lsm303::getAccZ() {
	return accData[2];
}
