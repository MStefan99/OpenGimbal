#include "lib/inc/lsm6dso.hpp"

static constexpr uint8_t LSM6DSO_ADDR {0x6a};

static constexpr float ACC_LSB {0.122f / 1000.0f}; //mg
static constexpr float ROT_LSB {8.75f / 1000.0f}; // dps

static float acc[3] {0};
static float rot[3] {0};


void lsm6dso::init() {
    uint8_t ctrl1_xl {0x40};
    uint8_t ctrl2_g {0x40};
    i2c::writeRegister(LSM6DSO_ADDR, 0x10, &ctrl1_xl);
    i2c::writeRegister(LSM6DSO_ADDR, 0x11, &ctrl2_g);
}


void lsm6dso::update() {
	i2c::readRegister(LSM6DSO_ADDR, 0x28, 6, [](bool success, const dma::I2CTransfer& transfer) {
        for (uint8_t i {0}; i < 3; ++i) {
            acc[i] = reinterpret_cast<const int16_t*>(transfer.buf)[i] * ACC_LSB;
        }
        i2c::readRegister(LSM6DSO_ADDR, 0x22, 6, [](bool success, const dma::I2CTransfer& transfer) {
           for (uint8_t i {0}; i < 3; ++i) {
                rot[i] = reinterpret_cast<const int16_t*>(transfer.buf)[i] * ROT_LSB;
            } 
        });
    });
}


Vector3<float, uint8_t> lsm6dso::getAcc() {
	return {{acc[2]}, {-acc[0]}, {-acc[1]}};
}


Vector3<float, uint8_t> lsm6dso::getRot() {
	return {{rot[2]}, {-rot[0]}, {-rot[1]}};
}
