#include "LSM6DSO32.hpp"

constexpr static float ACC_LSB {0.122f / 1000.0f};  // mg
constexpr static float ROT_LSB {35.0f / 1000.0f};   // dps

static int16_t rawAccelerations[3] {0};
static int16_t rawAngularRates[3] {0};

static float rateOffsets[3] {0};

static float accelerations[3] {0};
static float angularRates[3] {0};

void LSM6DSO32::init() {
	uint8_t ctrl1_xl {LSM6DSO32_CTRL1_XL_ODR_XL_104Hz};
	uint8_t ctrl2_g {LSM6DSO32_CTRL2_G_ODR_G_104Hz | LSM6DSO32_CTRL2_G_FS_G_1000DPS};
	i2c::write(LSM6DSO32_ADDR_0, LSM6DSO32_CTRL1_XL_ADDR, &ctrl1_xl);
	i2c::write(LSM6DSO32_ADDR_0, LSM6DSO32_CTRL2_G_ADDR, &ctrl2_g);
}

void LSM6DSO32::update() {
	i2c::read(LSM6DSO32_ADDR_0, LSM6DSO32_OUTX_L_A_ADDR, 6, [](bool success, const i2c::Transfer& transfer) {
		for (uint8_t i {0}; i < 3; ++i) {
			accelerations[i] = (rawAccelerations[i] = reinterpret_cast<const int16_t*>(transfer.buf)[i]) * ACC_LSB;
		}
	});
	i2c::read(LSM6DSO32_ADDR_0, LSM6DSO32_OUTX_L_G_ADDR, 6, [](bool success, const i2c::Transfer& transfer) {
		for (uint8_t i {0}; i < 3; ++i) {
			angularRates[i] = (rawAngularRates[i] = reinterpret_cast<const int16_t*>(transfer.buf)[i]) * ROT_LSB;
		}
	});
}

Vector3<int16_t, uint8_t> LSM6DSO32::getRawAccelerations() {
	return {
	  {static_cast<int16_t>(-rawAccelerations[0])},
	  {static_cast<int16_t>(-rawAccelerations[1])},
	  {rawAccelerations[2]}
	};
}

Vector3<int16_t, uint8_t> LSM6DSO32::getRawAngularRates() {
	return {
	  {static_cast<int16_t>(-rawAngularRates[0])},
	  {static_cast<int16_t>(-rawAngularRates[1])},
	  {rawAngularRates[2]}
	};
}

void LSM6DSO32::setOffsets(const Vector3<float, uint8_t>& offsets) {
	for (uint8_t i {0}; i < 3; ++i) {
		rateOffsets[i] = offsets[i][0];
	}
}

Vector3<float, uint8_t> LSM6DSO32::getAccelerations() {
	return {{accelerations[2]}, {-accelerations[1]}, {accelerations[0]}};
}

Vector3<float, uint8_t> LSM6DSO32::getAngularRates() {
	return {{angularRates[2] - rateOffsets[2]}, {-angularRates[1] - rateOffsets[1]}, {angularRates[0] - rateOffsets[2]}};
}
