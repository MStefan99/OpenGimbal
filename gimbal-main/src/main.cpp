#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
#include "lib/inc/data.hpp"
#include "lib/inc/usb.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/dma.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

int main() {
    util::init();
    
    dma::init();
    i2c::init();
//    usb::init();

    // Temperature calibration values
    uint8_t tempR = NVMTEMP[0] & 0xff;
    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;


    while (1) {
        util::sleep(1);

        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
    }

    return 1;
}
