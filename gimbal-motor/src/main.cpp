#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/as5600.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

bool dataReady {false};
uint16_t angle0 {0};
uint16_t angle120 {0};
uint8_t poles {7};
uint16_t rev {585};

    // Temperature calibration values
//    uint8_t tempR = NVMTEMP[0] & 0xff;
//    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
//    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
//    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

void calibrate() {
    uint16_t angle{0};

    bldc::applyTorque(0, 100);
    util::sleep(1000);

    as5600::getAngle(angle, [](bool success) {
        dataReady = true;
    });
    while (!dataReady) {
        __WFI();
    }
    angle0 = util::switchEndianness(angle);
//    angle = 0;
//
//    bldc::applyTorque(1200, 100);
//    util::sleep(1000);
//
//    as5600::getAngle(angle);
//    while (!angle) {
//        __WFI();
//    }
//    angle120 = util::switchEndianness(angle);
//    
//    uint16_t diff = angle120 - angle0;
//    rev = diff * 3;
//    poles = 4096 / rev;

    bldc::applyTorque(0, 0);
}

int main() {
    util::init();

    dma::init();
    i2c::init();
    bldc::init();

    PORT_REGS->GROUP[0].PORT_DIRSET = 1;

    if (data::poles == 0) {
        calibrate();
    }

    uint16_t angle;
    while (1) {
        dataReady = false;
        as5600::getAngle(angle, [](bool success) {
            dataReady = true;
        });

        //        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
        //        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
        //        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));

        while (!dataReady) __WFI();
        PORT_REGS->GROUP[0].PORT_OUTSET = 1;
        angle = util::switchEndianness(angle);
        int16_t pos = angle - angle0;
        if (pos < 0) pos += 4096;
        
        uint16_t eAngle = pos % rev * poles;

        bldc::applyTorque(eAngle + 900, 100);
       
        PORT_REGS->GROUP[0].PORT_OUTCLR = 1;
//        util::sleep(1);
    }

    return 1;
}
