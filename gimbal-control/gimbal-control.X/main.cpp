#include "device.h"

static uint32_t ticks = 0;

extern "C" {
    void SysTick_Handler() {
        ++ticks;
    }
}

void sleep(uint32_t ms) {
    uint32_t t = ticks + ms;

    while (ticks < t) {
        __WFI();
    }
}


uint8_t pins[] = {5, 30, 31, 14, 15};

int main () {
    // SysTick config
    SysTick->CTRL = 0;
    SysTick->LOAD = 1000 - 1;
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk
            | SysTick_CTRL_CLKSOURCE_Msk
            | SysTick_CTRL_ENABLE_Msk;
    
    // NVIC config
    __DMB();
    __enable_irq();
    NVIC_EnableIRQ(SysTick_IRQn);

    PORT_REGS->GROUP[0].PORT_DIRSET = 0xC000C020;
    PORT_REGS->GROUP[0].PORT_PINCFG[30] = 0;
    PORT_REGS->GROUP[0].PORT_PINCFG[31] = 0;
    
    PORT_REGS->GROUP[0].PORT_OUT = 0xC000C020;
    sleep(1000);
    PORT_REGS->GROUP[0].PORT_OUT = 0;
    sleep(1000);
    
    while (1) {
        for (uint8_t i = 0; i < sizeof(pins); ++i) {   
            PORT_REGS->GROUP[0].PORT_OUT = 1 << pins[i];
            sleep(1000);
        }
        
        PORT_REGS->GROUP[0].PORT_OUT = 0xC000C020;
        sleep(1000);
        PORT_REGS->GROUP[0].PORT_OUT = 0;
        sleep(1000);
    }
}
