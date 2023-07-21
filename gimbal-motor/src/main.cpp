#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/updates.hpp"
#include "lib/inc/usb.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/dma.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

/*
 * THIS FILE IS FOR INITIALIZATION ONLY!
 * 
 * Use the updates.cpp file for the main program logic
 */

int main() {
    util::init();
    
    data::load();
    dma::init();
//    sbus::init();
    i2c::init();
//    usb::init();

    updates::init();

    // Temperature calibration values
    uint8_t tempR = NVMTEMP[0] & 0xff;
    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;


    while (1) {
        util::sleep(1);

        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
        data::STATUS_DESCRIPTOR.bTemp = tempR + ((ADC_REGS->ADC_RESULT - adcR) * (tempH - tempR) / (adcH - adcR));

        updates::ms();

        if (util::getTickCount() % 20 == 0) {
            updates::fast();

            if (util::getTickCount() % 1000 == 0) {
                updates::slow();

                // TODO: Doesn't work, figure out why
            }
        }
    }

    return 1;
}
