#include "lib/inc/LSM6DSO32.hpp"

static constexpr float ACC_LSB {0.122f / 1000.0f}; //mg
static constexpr float ROT_LSB {8.75f / 1000.0f}; // dps

static float acc[3] {0};
static float rot[3] {0};


void LSM6DSO32::init() {
    uint8_t ctrl1_xl {LSM6DSO32_CTRL1_XL_ODR_XL_104Hz};
    uint8_t ctrl2_g {LSM6DSO32_CTRL2_G_ODR_G_104Hz};
    i2c::writeRegister(LSM6DSO32_ADDR_0, LSM6DSO32_CTRL1_XL_ADDR, &ctrl1_xl);
    i2c::writeRegister(LSM6DSO32_ADDR_0, LSM6DSO32_CTRL2_G_ADDR, &ctrl2_g);
}


void LSM6DSO32::update() {
	i2c::readRegister(LSM6DSO32_ADDR_0, LSM6DSO32_OUTX_L_A_ADDR, 6, [](bool success, const dma::I2CTransfer& transfer) {
        for (uint8_t i {0}; i < 3; ++i) {
            acc[i] = reinterpret_cast<const int16_t*>(transfer.buf)[i] * ACC_LSB;
        }
        i2c::readRegister(LSM6DSO32_ADDR_0, LSM6DSO32_OUTX_L_G_ADDR, 6, [](bool success, const dma::I2CTransfer& transfer) {
           for (uint8_t i {0}; i < 3; ++i) {
                rot[i] = reinterpret_cast<const int16_t*>(transfer.buf)[i] * ROT_LSB;
            } 
        });
    });
}


Vector3<float, uint8_t> LSM6DSO32::getAcc() {
	return {{acc[2]}, {-acc[0]}, {-acc[1]}};
}


Vector3<float, uint8_t> LSM6DSO32::getRot() {
	return {{rot[2]}, {-rot[0]}, {-rot[1]}};
}
