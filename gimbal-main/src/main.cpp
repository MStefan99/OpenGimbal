#include "device.h"
//#include <xc.h>  // TODO: explore, possibly delete Harmony files

#include "lib/inc/util.hpp"
//#include "lib/inc/data.hpp"
#include "lib/inc/uart.hpp"
//#include "lib/inc/i2c.hpp"
#include "lib/inc/dma.hpp"

#define NVMTEMP ((uint32_t*)0x00806030)

int main() {
    util::init();
    
    dma::init();
//    i2c::init();
    uart::init();

    // Temperature calibration values
//    uint8_t tempR = NVMTEMP[0] & 0xff;
//    uint16_t adcR = (NVMTEMP[1] & 0xfff00) >> 8u;
//    uint8_t tempH = (NVMTEMP[0] & 0xff0000) >> 12u;
//    uint16_t adcH = (NVMTEMP[1] & 0xfff00000) >> 20u;

    uint16_t angle {2048};
    int8_t da {1};
    uint8_t data[6] = {0x10, 00, 00, 0x11, 0, 0};

    while (1) {
        data[1] = angle >> 8u;
        data[2] = angle;
        uint16_t angle2 = 4096 - angle / 2;
        data[4] = angle2 >> 8u;
        data[5] = angle2;
        
        for (uint8_t i {0}; i < 2; ++i) {
            for (uint8_t j {1}; j < 3; ++j) {
                if (data[3*i + j] >= 0x10 && data[3*i + j] <= 0x11) {
                    data[3*i + j] += 2;
                }
            }
        }
        
        uart::send(data, 6);
        
        angle += da;
        if (angle > 3072) {da = -1;}
        if (angle < 1024) {da = 1;}
        
        util::sleep(1);
//        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
//        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
    }

    return 1;
}
