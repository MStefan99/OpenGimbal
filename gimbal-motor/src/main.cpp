#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/bldc.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/dma.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

bool dataReady {false};


void calibrate() {
    bldc::setAngle(0, 100);
    util::sleep(200);
    bldc::setAngle(1000, 100);
    util::sleep(200);
    bldc::setAngle(0, 0);
}


int main() {
    util::init();
    
    dma::init();
    i2c::init();
    bldc::init();
    
    if (data::poles == 0) {
        calibrate();
    }

    PORT_REGS->GROUP[0].PORT_DIRSET = 1;

    // Temperature calibration values
    uint8_t tempR = NVMTEMP[0] & 0xff;
    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

    while (1) {
        PORT_REGS->GROUP[0].PORT_OUTSET = 1;

//        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
//        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
//        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));
        
        uint16_t angle;
//        i2c::readRegister(0x36, 0x0e, reinterpret_cast<uint8_t*>(&angle), 2, [](bool success) {
//            dataReady = true;
//        });
        
        while (!dataReady) __WFI();
        dataReady = false;
        
        PORT_REGS->GROUP[0].PORT_OUTCLR = 1;
    }

    return 1;
}
