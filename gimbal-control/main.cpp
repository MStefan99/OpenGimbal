#include "main.hpp"

/* Pinouts
 * 
 * PA02 - Joystick analog input 1
 * PA03 - Joystick analog input 2
 * 
 * PA05 - Supply, enables joystick and backlight
 * 
 * PA08 - UART
 * 
 * PA14 - LED 3
 * PA15 - LED 4
 * 
 * PA24 - Button 1
 * PA25 - Button 2
 * 
 * PA30 - LED 1 (shared with SWCLK)
 * PA31 - LED 2 (shared with SWDIO)
 * 
 */

void setPower(bool enabled) {
    if (enabled) {
        PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 5u;
    } else {
        PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 5u;
    }
}


int main() {
    util::init();
    buttons::init();
    pwm::init();
    uart::init();
    
    PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 << 5u;

    while (1) {
        buttons::update();
        util::sleep(1);
    }
}
