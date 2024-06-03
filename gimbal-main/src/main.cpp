#include <cstdio>
#include <cmath>

#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/LSM6DSO32.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/Mahony.hpp"
#include "lib/inc/LowPassFilter.hpp"

#define DV_OUT 0

static constexpr float attFactor {2048 / F_PI};
static constexpr float maxRestoringVelocity {F_2_PI / 100.0f}; // One revolution per second (100 iterations)


void setPosition(uint8_t* buf, uint8_t address, int16_t position) {
    position = util::mod(position, static_cast<int16_t>(4096));
    
    buf[0] = 0x4 << 4u | address;
    buf[1] = 0x1;
    buf[2] = 0xf0 | position >> 8u;
    buf[3] = position;
}

float sqrt2 = std::sqrt(2);


Vector3<float, uint8_t> calculateAngles(const Vector3<float, uint8_t>& eulerAngles) {
    float a = eulerAngles[0][0];
    float b = util::clamp(eulerAngles[1][0], -F_PI_4 + 1e-3f, F_PI_4 - 1e-3f);
    float g = eulerAngles[2][0];
    
    float cosA = std::cos(a);
    float sinB = std::sin(b);
    float cosB = std::cos(b);
    float sinG = std::sin(g);
    
    float cos2A = cosA * cosA;
    float sin2B = sinB * sinB;
    float cos2B = cosB * cosB;
    float sin2G = sinG * sinG;
   
    float sinM2 = sqrt2 * sinB;
    float sin2M2 = sinM2 * sinM2;
    
    float cos2M2 = 1 - sin2M2;
    float cosM2 = std::sqrt(cos2M2);
    
    auto signG {util::sign(util::abs(g) - F_PI_2)};

    return {
        {-2 * std::atan2((sqrt2 * (sqrt2 * sinB - util::sign(a) * std::sqrt(2 - 2 * cos2A * cos2B - 2 * sin2B))), (2 * (std::sqrt(1 - 2 * sin2B) + cosA * cosB)))},
        {std::asin(sinM2)},
        {F_PI_4 + signG * 2 * std::atan2((std::sqrt(2 - 2 * cos2B * sin2G - 2 * sin2B) + signG * 1), (std::sqrt(1 - 2 * sin2B) - sqrt2 * cosB * sinG))}
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

float getDifference(float angleA, float angleB = 0) {
    float diff {angleA - angleB};
    
    if (diff > F_PI_2) {
        return diff - F_PI;
    } else if (diff < -F_PI_2) {
        return diff + F_PI;
    } else {
        return diff;
    }
}

int main() {
    util::init();
    dma::init();
    i2c::init();
    LSM6DSO32::init();
    uart::init();
    
    PORT_REGS->GROUP[0].PORT_DIRSET = (0x1 << 17u);
    
    Mahony mahony {};
    
    float yawTarget {0};
    float pitchTarget {0};
    
    while (1) {
        LSM6DSO32::update();
        #if DV_OUT
            auto startUs = SysTick->VAL;
            auto startMs = util::getTime();
        #endif        
        mahony.updateIMU(LSM6DSO32::getRot(), LSM6DSO32::getAcc(), 0.01f);
        
        Quaternion handleOrientation {mahony.getQuat()};
        auto handleAngles {handleOrientation.toEuler()};     
        
        yawTarget += util::clamp(getDifference(handleAngles[0][0], yawTarget) / 100.0f, -maxRestoringVelocity, maxRestoringVelocity);
        pitchTarget += util::clamp(getDifference(handleAngles[1][0], pitchTarget) / 100.0f, -maxRestoringVelocity, maxRestoringVelocity);
        
        Quaternion phoneOrientation {Quaternion::fromEuler(
            yawTarget,
            pitchTarget,
            0
        )};
        Quaternion gimbalRotation {handleOrientation.conjugate() * phoneOrientation};
        
        auto eulerAngles {gimbalRotation.toEuler()};
        
        auto motorAngles {calculateAngles(eulerAngles)};
        
        #if DV_OUT
            //if (util::getTime() % 5 < 1) {
                Data data {};

                data.dt = (util::getTime() - startMs) * 1000 + (startUs - SysTick->VAL) / 48;
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
            int16_t position = motorAngles[i][0] * attFactor;
            if (position < 0) {
                position += 4096;
            }
            setPosition(buf + 4 * i, i + 1, position);
        }      
        uart::sendToMotors(buf, 12);
                
        util::sleep(10);
    }

    return 1;
}
