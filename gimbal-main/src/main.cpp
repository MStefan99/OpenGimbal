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
    lsm6dso::init();
    uart::init();

    PORT_REGS->GROUP[0].PORT_DIRSET = (0x1 << 17u);
    
    int16_t setpoint {0};
    uint8_t buf[4] = {
        0x41,
        0x02
    };
    
    while (1) {
        setpoint += lsm6dso::getRot()[1] / 20.0f;
        if (setpoint < 0) {
            setpoint = setpoint + 4096;
        } else if (setpoint > 4096) {
            setpoint = setpoint - 4096;
        }
        buf[2] = 0xf0 | setpoint >> 8u;
        buf[3] = setpoint & 0xff;
        uart::sendToMotors(buf, 4);
        util::sleep(5);
        lsm6dso::update();
    }

    return 1;
}
