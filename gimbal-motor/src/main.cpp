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

void calibrate() {
    bldc::applyTorque(0, 100);
    util::sleep(1000);
    bldc::applyTorque(1200, 100);
    util::sleep(1000);
    bldc::applyTorque(2400, 100);
    util::sleep(500);
    bldc::applyTorque(0, 0);
}

int main() {
    util::init();

    dma::init();
    i2c::init();
    bldc::init();

    PORT_REGS->GROUP[0].PORT_DIRSET = 1;
    
    if (data::poles == 0) {
        //calibrate();
    }

    // Temperature calibration values
    uint8_t tempR = NVMTEMP[0] & 0xff;
    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

    while (1) {
        PORT_REGS->GROUP[0].PORT_OUTSET = 1;

        uint16_t angle;
        as5600::getAngle(angle, [](bool success) {
            dataReady = true;
        });
        
//        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
//        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
//        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));
        
        while (!dataReady) __WFI();
        dataReady = false;
        
        PORT_REGS->GROUP[0].PORT_OUTCLR = 1;
        util::sleep(1);
    }

    return 1;
}
