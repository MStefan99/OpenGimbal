#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/as5600.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

// The total degrees for one full rotation
static constexpr uint16_t fullRotation {4096};

bool dataReady {false};
uint16_t offset {0};
uint8_t polePairs {0};

    // Temperature calibration values
//    uint8_t tempR = NVMTEMP[0] & 0xff;
//    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
//    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
//    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

uint16_t measureAngle() {
    uint16_t angle;
    
    dataReady = false;
    as5600::getAngle(angle, [](bool success) {
        dataReady = true;
    });
    while (!dataReady) {
        __WFI();
    }
    
    return util::switchEndianness(angle);
}

void calibrate() {
    bldc::applyTorque(0, 100);
    util::sleep(1000);
    
    offset = measureAngle();
    uint16_t angle {0};
    uint16_t torqueAngle {0};
    
    do {
        torqueAngle += 10;
        
        if (torqueAngle >= fullRotation) {
            torqueAngle = 0;
            ++polePairs;
        }
        bldc::applyTorque(torqueAngle, 100);
        
        angle = measureAngle();
    } while (ABS(angle - offset) > 10 || polePairs == 0);
    
    bldc::applyTorque(0, 0);
}

int main() {
    util::init();

    dma::init();
    i2c::init();
    bldc::init();

    PORT_REGS->GROUP[0].PORT_DIRSET = 1;
    calibrate();

    while (1) {
        //        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
        //        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
        //        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));

        uint16_t angle = measureAngle();
        PORT_REGS->GROUP[0].PORT_OUTSET = 1;
        uint16_t eAngle = polePairs * (fullRotation + angle - offset) % fullRotation;
        
        bldc::applyTorque(eAngle + 1024, 100);
        
        PORT_REGS->GROUP[0].PORT_OUTCLR = 1;
//        util::sleep(1);
    }

    return 1;
}
