#include "main.hpp"

/* Pinouts
 * 
 * PA02 - Joystick analog input 1
 * PA04 - Joystick analog input 2
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

#define DV_OUT 1


void setPower(bool enabled) {
    if (enabled) {
        PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 5u;
    } else {
        PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 5u;
    }
}


#if DV_OUT
struct DVData {        
    uint8_t header {0x03};
    uint16_t dt {};
    uint16_t joystickX {};
    uint16_t joystickY {};
    uint8_t footer {0xfc};
} __attribute__((packed));
#endif


int main() {
    util::init();
    buttons::init();
    pwm::init();
    uart::init();

    PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 << 5u;

    while (1) {
        #if DV_OUT
            auto startMs = util::getTime();
            auto startUs = SysTick->VAL;
        #endif

        ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND // Set GND as negative input
            | ADC_INPUTCTRL_MUXPOS_PIN0; // Set temperature sensor as positive input
        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
        uint16_t joystickY = 4095 - ADC_REGS->ADC_RESULT;
        
        ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND // Set GND as negative input
            | ADC_INPUTCTRL_MUXPOS_PIN2; // Set temperature sensor as positive input
        ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
        while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
        uint16_t joystickX = ADC_REGS->ADC_RESULT;
    
        #if DV_OUT
            DVData data {};

            data.dt = (util::getTime() - startMs) * 1000 + (startUs - SysTick->VAL) / 2;
            data.joystickX = joystickX;
            data.joystickY = joystickY;
            uart::send(reinterpret_cast<uint8_t*>(&data), sizeof(data));
        #endif

        buttons::update();
        
        util::sleep(5);
    }
}
