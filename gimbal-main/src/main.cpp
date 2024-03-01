#include <cstdio>
#include <cmath>

#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/lsm6dso.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/AttitudeEstimator.hpp"

static constexpr float FACTOR {2048 / PI};
using angles = float[3];


void setPosition(uint8_t* buf, uint8_t address, uint16_t position) {
    buf[0] = 0x4 << 4u | address;
    buf[1] = 0x1;
    buf[2] = 0xf0 | position >> 8u;
    buf[3] = position;
}


void calculateAngles(float x, float y, float z, angles& result) {
    float sinX = std::sin(x);  
    float sin2x = sinX * sinX;
    float sinY = std::sin(y);
    float sin2y = sinY * sinY;
    float cosY = std::cos(y);
    float cos2y = cosY * cosY;
    float sinZ = std::sin(z);
    float sin2z = sinZ * sinZ;
    float sqrt2 = std::sqrt(2);
    
    float cosBeta = std::sqrt(1 - 2 * cos2y * sin2x);
    float cos2beta = cosBeta * cosBeta;
   
    float sinBeta = 2 / sqrt2 * cosY * sinX;
    float sin2beta = sinBeta * sinBeta;
    result[1] = std::asin(sinBeta);
    
    float sinGamma = (-4 / sqrt2 * sinY - 2 * cosBeta * std::sqrt(cos2beta - sin2y + 1)) / (2 * (cos2beta + 1));
    float cos2gamma = 1 - sinGamma * sinGamma;
    float cosGamma = std::sqrt(cos2gamma);

    result[2] = std::asin(sinGamma) + QUARTER_PI;

    float sinAlpha = (sqrt2 * cosY * sinZ * cosBeta - 
        sinBeta * std::sqrt(2 * cos2gamma * cos2beta - 4 * cos2y * sin2z + cos2gamma * sin2beta)) /
        (cosGamma * (cos2beta + 1));
    
    result[0] = std::asin(sinAlpha);
}


int main() {
    util::init();
    dma::init();
    i2c::init();
    lsm6dso::init();
    uart::init();
    
    PORT_REGS->GROUP[0].PORT_DIRSET = (0x1 << 17u);
    
    AttitudeEstimator estimator {0, 0};
    
    while (1) {
        lsm6dso::update();
        util::sleep(10);
        estimator.update(lsm6dso::getRot(), lsm6dso::getAcc(), 0.01f);
        
        angles a {0.0f};
        
        calculateAngles(estimator.getPitch() - HALF_PI, HALF_PI - estimator.getRoll(), 0, a);
        
        if (std::isnan(a[0]) || std::isnan(a[1]) || std::isnan(a[2])) {
            continue;
        }
        
        uint8_t buf[12] {};
        for (uint8_t i {0}; i < 3; ++i) {
            int16_t position = a[i] * FACTOR;
            if (position < 0) {
                position += 4096;
            }
            setPosition(buf + 4 * i, i + 1, position);
        }
        
        uart::sendToMotors(buf, 12);
    }

    return 1;
}
