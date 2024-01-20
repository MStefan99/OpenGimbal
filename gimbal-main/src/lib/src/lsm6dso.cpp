#include "lib/inc/lsm6dso.hpp"

static constexpr uint8_t LSM6DSO_ADDR {0x6a};

static constexpr float ACC_LSB {0.122 / 1000};
static constexpr float ROT_LSB {0.153 / 1000};

static float acc[3] {0};
static float rot[3] {0};


void lsm6dso::init() {
    uint8_t ctrl1_xl {0x30};
    uint8_t ctrl2_g {0x030};
    i2c::writeRegister(LSM6DSO_ADDR, 0x10, &ctrl1_xl);
    i2c::writeRegister(LSM6DSO_ADDR, 0x11, &ctrl2_g);
}


void lsm6dso::update() {
	i2c::readRegister(LSM6DSO_ADDR, 0x28, 6, [](bool success, const dma::I2CTransfer& transfer) {
       for (uint8_t i {0}; i < 3; ++i) {
            rot[i] = transfer.buf[i] * ROT_LSB;
        } 
    });
	i2c::readRegister(LSM6DSO_ADDR, 0x22, 6, [](bool success, const dma::I2CTransfer& transfer) {
       for (uint8_t i {0}; i < 3; ++i) {
            acc[i] = transfer.buf[i] * ACC_LSB;
        } 
    });
}


const float* lsm6dso::getAcc() {
	return acc;
}


const float* lsm6dso::getRot() {
	return rot;
}
