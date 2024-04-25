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

float sqrt2 = std::sqrt(2);


Vector3<float, uint8_t> calculateAngles(const Vector3<float, uint8_t>& eulerAngles) {
    float sinA = std::sin(eulerAngles[0][0]);
    float cosA = std::cos(eulerAngles[0][0]);
    float sinB = std::sin(eulerAngles[1][0]);
    float cosB = std::cos(eulerAngles[1][0]);
    float sinG = std::sin(eulerAngles[2][0]);
    float cosG = std::cos(eulerAngles[2][0]);
    
    float sin2A = sinA * sinA;
    float cos2A = cosA * cosA;
    float sin2B = sinB * sinB;
    float cos2B = cosB * cosB;
    float sin2G = sinG * sinG;
    float cos2G = cosG * cosG;
   
    float sinM2 = sqrt2 * sinB;
    float sin2M2 = sinM2 * sinM2;
    
    float cos2M2 = 1 - sin2M2;
    float cosM2 = std::sqrt(cos2M2);
    
    return {
        {-2 * std::atan2((sqrt2 * (sinM2 + std::sqrt(cos2M2 - 2 * cos2A * cos2B + 1))), (2 * (cosM2 + cosA * cosB)))},
        {std::asin(sinM2)},
        {2 * std::atan2((sqrt2 * std::sqrt(cos2M2 - 2 * cos2B * cos2G + 1) - cosM2 + 1), (cosM2 + 2 * cosB * cosG + 1))}
    };
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
        
        Quaternion handleOrientation {mahony.getQuat()};
        Quaternion phoneOrientation {};
        Quaternion gimbalRotation {phoneOrientation * handleOrientation.conjugate()};
        
        auto eulerAngles {gimbalRotation.toEuler()};
        
        auto motorAngles {calculateAngles(eulerAngles)};
        
        #if DV_OUT
            //if (util::getTime() % 5 < 1) {
                Data data {};

                data.dt = (start - SysTick->VAL) / 48;
                data.x = eulerAngles[0][0];
                data.y = eulerAngles[1][0];
                data.z = eulerAngles[2][0];
                data.m1 = motorAngles[0][0];
                data.m2 = motorAngles[1][0];
                data.m3 = motorAngles[2][0];
                uart::sendToControl(reinterpret_cast<uint8_t*>(&data), sizeof(data));
            //}
        #endif
        
        uint8_t buf[12] {};
        for (uint8_t i {0}; i < 3; ++i) {
            int16_t position = motorAngles[i][0] * FACTOR;
            if (position < 0) {
                position += 4096;
            }
            setPosition(buf + 4 * i, i + 1, position);
        }
//        setPosition(buf, 3, orientation[2][0] * FACTOR);        
        uart::sendToMotors(buf, 12);
                
        util::sleep(10);
    }

    return 1;
}
