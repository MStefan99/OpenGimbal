#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include <cstdio>
#include <cinttypes>

#include "main.hpp"
#include "lib/inc/util.hpp"
#include "lib/inc/pwm.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/uart.hpp"
#include "lib/inc/PID.hpp"
#include "lib/inc/as5600.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

static constexpr uint8_t torqueLUT[16] = {0, 64, 101, 128, 148, 165, 179, 191, 202, 212, 221, 229, 236, 243, 249, 255};

// The total degrees for one full rotation
static constexpr uint16_t fullRotation {4096};
static uint16_t targetAngle {0};
static uint8_t maxTorque {255};


void setTargetAngle(uint16_t angle, uint8_t torque) {
    if (torque > 15) {torque = 15;}
    targetAngle = angle;
    maxTorque = torqueLUT[torque];
}

void processCommand(const uart::DefaultCallback::buffer_type& buffer) {
    if ((buffer.buffer[0] & 0x0f) != 0x1) {
        return; // Command intended for another device
    }
    
    switch(buffer.buffer[1] & 0x0f) { // Parse command type
        case (0x2): {
            setTargetAngle(((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3], buffer.buffer[2] >> 4u);
        }
        break;
    }
}

bool dataReady {false};
uint16_t offset {0};

    // Temperature calibration values
//    uint8_t tempR = NVMTEMP[0] & 0xff;
//    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
//    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
//    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

uint16_t measureAngle() {
    static uint16_t angle;
    
    dataReady = false;
    auto startTime {util::getTime()};
    
    as5600::getAngle([](bool success, const dma::I2CTransfer& transfer) {
        angle = reinterpret_cast<const uint16_t*>(transfer.buf)[0];
        dataReady = true;
    });
    
    while (!dataReady && util::getTime() - startTime < 5) {
        __WFI();
    }
    
    return util::switchEndianness(angle);
}

void calibrate() {
    uint16_t angle {0};
    bldc::applyTorque(0, 255);
    
    do {
        util::sleep(20);
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

int16_t getDifference(uint16_t angleA, uint16_t angleB) {
    int16_t diff = static_cast<int16_t>(angleA) - static_cast<int16_t>(angleB);
    
    if (diff > 2048) {
        return diff - 4096;
    } else if (diff < -2048) {
        return diff + 4096;
    } else {
        return diff;
    }
}

void applyTorque(uint16_t angle, uint8_t power, bool counterclockwise = true) {
    // Calculate electrical angle from encoder reading
    uint16_t eAngleCW = (data::options.polePairs * (fullRotation + angle - offset)) % fullRotation;
    // Flip the angle if the motor polarity is reversed
    uint16_t eAngle = data::options.direction < 0? fullRotation - eAngleCW : eAngleCW;
    
    bldc::applyTorque(counterclockwise == (data::options.direction > 0)? eAngle + 1024: eAngle + 3072, power);
}

int main() {
    util::init();

    uart::init();
    dma::init();
    i2c::init();
    as5600::init();
    bldc::init();
    
    calibrate();
    srand(SysTick->VAL);
    
    //uart::setCallback(processCommand);
    
    uart::print("Hello uart!\n");
    printf("Hello printf!\n");
    
    util::setInterval(8000, []() -> void {
        targetAngle = (rand() % 4096) - 2048;
        if (targetAngle < 0) {targetAngle = -targetAngle;}
    });
    
    uint16_t angle {0};
    float v {0};
    float a {0};
    
    while (1) {
//        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
//        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
//        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));

        // Calculate difference between current and set angle
//        uint32_t startTime {SysTick->VAL};
        int16_t dAngle = getDifference(targetAngle, angle);
        
        uint16_t prevAngle {angle};
        angle = measureAngle();
        
        float newV = v + (getDifference(angle, prevAngle) - v) / 30.0f;
        float newA {newV - v};
        v = newV;
        
        if (dAngle < -128 || dAngle > 128) { // Far from target, using time-optimal control
            float D = v * v - 2.0f * a * dAngle;

            if (D <= 2) {
                a += (newA - a) / 20.0f;
            }
            if (dAngle > 0 == a < 0) {
                a = -a;
            }
        
            if (-0.005 < a && a < 0.005) { // Acceleration isn't measured yet, applying full power to measure
                applyTorque(angle, 255, dAngle > 0); // Accelerating towards destination
            } else if (util::abs(D) > 2) { // Acceleration measured, using it to predict the stopping distance
                applyTorque(angle, 255, dAngle > 0 == D <= 0); // Accelerating or decelerating based on prediction
            }
            if (util::getTime() % 10 < 1) {
                printf("v:% 07.3f, a:% 07.4f, D:% 08.1f, dA:% " PRId16 "\n", v, a, D, dAngle);
            }
        } else { // Close to target, using sliding mode control
            a = 0.0f;
            float torque {dAngle - 100 * v};
            applyTorque(angle, util::min(util::abs(static_cast<int>(torque / 3)) + 150, 255), torque > 0);
            if (util::getTime() % 10 < 1) {
                printf("v:% 07.3f, t:% 04.0f, dA:% " PRId16 "\n", v, torque, dAngle);
            }
        }
        
//        if (util::getTime() % 10 < 1) {
//            printf("t: %" PRIu32 "\n", (startTime - SysTick->VAL) / 48);
//        }
        util::runTasks();
        util::sleep(1);
    }

    return 1;
}
