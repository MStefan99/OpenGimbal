#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/i2c.hpp"
#include "lib/inc/lsm6dso.hpp"
#include "lib/inc/uart.hpp"


int main() {
    util::init();
    dma::init();
    i2c::init();
    //lsm6dso::init();
    uart::init();
    
    PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_STANDBY;
    while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_STANDBY);
    __WFI();
    uart::print("Wake up!\n");
    
    while (1) {
        __WFI();
    }

    return 1;
}
