#include <cstdio>
#include <cmath>

#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/lsm6dso.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/Mahony.hpp"

#define DV_OUT true

static constexpr float FACTOR {2048 / PI};


void setPosition(uint8_t* buf, uint8_t address, int16_t position) {
    if (position < 0) {
        position += 4096;
    }
    
    buf[0] = 0x4 << 4u | address;
    buf[1] = 0x1;
    buf[2] = 0xf0 | position >> 8u;
    buf[3] = position;
}


Vector3<float, uint8_t> calculateAngles(float x, float y, float z) {
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
    
    float sinGamma = (-4 / sqrt2 * sinY - 2 * cosBeta * std::sqrt(cos2beta - sin2y + 1)) / (2 * (cos2beta + 1));
    float cos2gamma = 1 - sinGamma * sinGamma;
    float cosGamma = std::sqrt(cos2gamma);

    float sinAlpha = (sqrt2 * cosY * sinZ * cosBeta - 
        sinBeta * std::sqrt(2 * cos2gamma * cos2beta - 4 * cos2y * sin2z + cos2gamma * sin2beta)) /
        (cosGamma * (cos2beta + 1));
    
    return {{std::asin(sinAlpha)}, {std::asin(sinBeta)}, {std::asin(sinGamma) + QUARTER_PI}};
}

#if DV_OUT
struct Data {        
    uint8_t header {0x03};
    uint16_t dt {};
    float x {};
    float y {};
    float z {};
    float m1 {};
    float m2 {};
    float m3 {};
    uint8_t footer {0xfc};
} __attribute__((packed));
#endif

int main() {
    util::init();
    dma::init();
    i2c::init();
    lsm6dso::init();
    uart::init();
    
    PORT_REGS->GROUP[0].PORT_DIRSET = (0x1 << 17u);
    
    Mahony mahony {};
    
    while (1) {
        lsm6dso::update();
        #if DV_OUT
            auto start = SysTick->VAL;
        #endif        
        mahony.updateIMU(lsm6dso::getRot(), lsm6dso::getAcc(), 0.01f);
        
        auto orientation {mahony.getQuat().toEuler()};
        
        auto angles {calculateAngles(orientation[0][0], orientation[1][0], orientation[2][0])};
        
        #if DV_OUT
            //if (util::getTime() % 5 < 1) {
                Data data {};

                data.dt = (start - SysTick->VAL) / 48;
                data.x = orientation[0][0];
                data.y = orientation[1][0];
                data.z = orientation[2][0];
                data.m1 = angles[0][0];
                data.m2 = angles[1][0];
                data.m3 = angles[2][0];
                uart::sendToControl(reinterpret_cast<uint8_t*>(&data), sizeof(data));
            //}
        #endif
        
        uint8_t buf[12] {};
//        for (uint8_t i {0}; i < 3; ++i) {
//            int16_t position = angles[i][0] * FACTOR;
//            if (position < 0) {
//                position += 4096;
//            }
//            setPosition(buf + 4 * i, i + 1, position);
//        }
        setPosition(buf, 3, orientation[2][0] * FACTOR);
        
        uart::sendToMotors(buf, 4);
        util::sleep(10);
    }

    return 1;
}
