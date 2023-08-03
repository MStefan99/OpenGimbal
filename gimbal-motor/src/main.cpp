#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
#include "lib/inc/input.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/PID.hpp"
#include "lib/inc/as5600.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

// The total degrees for one full rotation
static constexpr uint16_t fullRotation {4096};
static uint16_t setAngle {2048};

void input::onInput(uint16_t value) {
//    setAngle = value;
}

bool dataReady {false};
uint16_t offset {0};

    // Temperature calibration values
//    uint8_t tempR = NVMTEMP[0] & 0xff;
//    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
//    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
//    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

uint16_t measureAngle() {
    uint16_t angle;
    
    dataReady = false;
    auto startTime {util::getTickCount()};
    
    as5600::getAngle(angle, [](bool success) {
        dataReady = true;
    });
    
    while (!dataReady && util::getTickCount() - startTime < 5) {
        __WFI();
    }
    
    return util::switchEndianness(angle);
}

void calibrate() {
    uint16_t angle {0};
    bldc::applyTorque(0, 255);
    
    do {
        util::sleep(2);
        offset = angle;
        angle = measureAngle();
    } while (offset != angle);

    uint16_t torqueAngle {0}; 
    if (!data::options.polePairs) {
        uint8_t polePairs {0};
        uint16_t lastPoleAngle {angle};
        int8_t direction {0};

        do {
            torqueAngle += 10;

            if (torqueAngle >= fullRotation) {
                torqueAngle = 0;
                if (angle > lastPoleAngle) {
                    ++direction;
                } else {
                    --direction;
                }
                lastPoleAngle = angle;
                ++polePairs;
            }
            bldc::applyTorque(torqueAngle, 255);

            angle = measureAngle();
        } while (util::abs(angle - offset) > 10 || polePairs == 0);
        
        if (direction > 0) {
            direction = 1;
        } else {
            direction = -1;
        }
        
        data::edit(data::options.polePairs, polePairs);
        data::edit(reinterpret_cast<const uint8_t&>(data::options.direction), reinterpret_cast<uint8_t&>(direction));
        data::write();
    }
    
    bldc::applyTorque(0, 0);
}

int main() {
    util::init();

    input::init();
    dma::init();
    i2c::init();
    as5600::init();
    bldc::init();

    calibrate();
    
    PORT_REGS->GROUP[0].PORT_DIRSET = 1;

    while (1) {
//        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
//        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
//        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));

        uint16_t angle = measureAngle();
        PORT_REGS->GROUP[0].PORT_OUTSET = 1;
        
        // Calculate electrical angle from encoder reading
        uint16_t eAngleCW = (data::options.polePairs * (fullRotation + angle - offset)) % fullRotation;
        // Flip the angle if the motor polarity is reversed
        uint16_t eAngle = data::options.direction < 0? fullRotation - eAngleCW : eAngleCW; 
        // Calculate difference between current and set angle
        uint16_t dAngle = (fullRotation + angle - setAngle) % fullRotation;
        
        // Apply torque perpendicular to the current rotor position, taking polarity into account
        bldc::applyTorque((dAngle > 2048) ^ (data::options.direction < 0)? eAngle + 1024: eAngle + 3072,
                // Vary applied power depending on distance from the setpoint
              util::min(util::abs(static_cast<int16_t>(6144 + angle - setAngle) % 4096 - 2048) * 9 / 5 + 145, 255));
        
        PORT_REGS->GROUP[0].PORT_OUTCLR = 1;
        if (util::getTickCount() % 2 == 0) {   
            setAngle += 1;
            if (setAngle > 4095) setAngle = 0;
        }
    }

    return 1;
}
