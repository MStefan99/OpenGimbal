#include "lib/inc/mpu6050.hpp"


static constexpr float G {9.81f};

static constexpr uint16_t ACC_LSB {16384};
static constexpr float ACC_FACTOR {G / ACC_LSB};

static constexpr uint16_t GYRO_LSB {131};
static constexpr float GYRO_FACTOR {DEG_TO_RAD / GYRO_LSB};


static constexpr uint8_t MPU6050_ADDR {0x68};


static int16_t rawAcc[3] {0};
static int16_t rawRot[3] {0};
static float acc[3] {0};
static float rot[3] {0};


void mpu6050::init() {
	uint8_t power1{0};
	i2c::writeRegister(MPU6050_ADDR, 0x6b, &power1, 1);
}


void mpu6050::update() {
	i2c::readRegister(MPU6050_ADDR, 0x3b, (uint8_t*) rawAcc, 6);
	i2c::readRegister(MPU6050_ADDR, 0x43, (uint8_t*) rawRot, 6, [](bool success) {
       for (uint8_t i {0}; i < 3; ++i) {
            acc[i] = util::switchEndianness(rawAcc[i]) * ACC_FACTOR;
            rot[i] = util::switchEndianness(rawRot[i]) * GYRO_FACTOR;
        } 
    });
}


const int16_t* mpu6050::getRawAcc() {
	return rawAcc;
}


const int16_t* mpu6050::getRawRot() {
	return rawRot;
}


const float* mpu6050::getAcc() {
	return acc;
}


const float* mpu6050::getRot() {
	return rot;
}
