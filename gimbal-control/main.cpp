#include "device.h"
#include "lib/inc/util.hpp"
#include "lib/inc/uart.hpp"

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
uint8_t leds[] = {30, 31, 14, 15};
uint8_t state{0};

int main() {
    util::init();
    
    // Setting LED pins as outputs
    PORT_REGS->GROUP[0].PORT_DIRSET = 0xC000C020;
    // Disabling SWD pins
    PORT_REGS->GROUP[0].PORT_WRCONFIG = 0xD000C000;
    // Enabling pull on button inputs
    PORT_REGS->GROUP[0].PORT_WRCONFIG = 0xC0060300;
    // Enabling pull-ups on button inputs
    PORT_REGS->GROUP[0].PORT_OUTSET = 0x3000000;

    PORT_REGS->GROUP[0].PORT_OUTSET = 0xC000C020;
    util::sleep(500);    
    PORT_REGS->GROUP[0].PORT_OUTCLR = 0xC000C020;

    util::setInterval(200, [] {
        if (!(PORT_REGS->GROUP[0].PORT_IN & 0x1 << 24u)) {
            switch (state) {
                case 0:
                case 1:
                case 2:
                case 3:
                    PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << leds[state++];
                    break;
                case 4:
                    PORT_REGS->GROUP[0].PORT_OUTCLR = 0xC000C020;
                    ++state;
                    break;
                case 5:
                    PORT_REGS->GROUP[0].PORT_OUTSET = 0xC000C020;
                    state = 15;
                    break;
                    
                case 15:
                case 14:
                case 13:
                case 12:
                    PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << leds[(state--) - 12];
                    break;
                case 11:
                    PORT_REGS->GROUP[0].PORT_OUTSET = 0xC000C020;
                    --state;
                    break;
                case 10:
                    PORT_REGS->GROUP[0].PORT_OUTCLR = 0xC000C020;
                    state = 0;
                    break;    
            }
        } else {
            if (state != 0 && state <= 5) {
                PORT_REGS->GROUP[0].PORT_OUTCLR = 0xC000C020;
                state = 0;
            } else if (state >= 10 && state != 15) {
                PORT_REGS->GROUP[0].PORT_OUTSET = 0xC000C020;
                state = 15;
            }
        }
    });

    while (1) {
        util::runTasks();
        util::sleep(1);
    }
}
