#include "device.h"

#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"
#include "lib/inc/i2c.hpp"


int main() {
    util::init();
    dma::init();
    i2c::init();

    while (1) {__WFI();}

    return 1;
}
