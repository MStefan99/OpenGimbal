#include "lib/inc/buttons.hpp"

extern "C" {
    void EIC_Handler() {
        EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
    }
}


void buttons::init() {    
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_EIC // Enable EIC clock
            | GCLK_CLKCTRL_CLKEN(1) // Enable clock
			| GCLK_CLKCTRL_GEN_GCLK0; //Set GCLK1 as a clock source
    
    // Setting up pins 8 and 9
    PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 8u | 0x1 << 9u)
            | PORT_WRCONFIG_PMUXEN(1)
            | PORT_WRCONFIG_INEN(1)
            | PORT_WRCONFIG_PULLEN(1)
            | PORT_WRCONFIG_PMUX(MUX_PA24A_EIC_EXTINT4)
            | PORT_WRCONFIG_WRPMUX(1)
            | PORT_WRCONFIG_WRPINCFG(1)
            | PORT_WRCONFIG_HWSEL(1);
    
    // Enabling pull-ups on button inputs
    PORT_REGS->GROUP[0].PORT_OUTSET = (0x1 << 24u | 0x1 << 25u);
    
    EIC_REGS->EIC_INTENSET = EIC_INTENSET_EXTINT4(1)
            | EIC_INTENSET_EXTINT5(1);
    EIC_REGS->EIC_WAKEUP = EIC_WAKEUP_WAKEUPEN4(1)
            | EIC_WAKEUP_WAKEUPEN5(1);
    EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE4_BOTH
            | EIC_CONFIG_FILTEN4(1)
            | EIC_CONFIG_SENSE5_BOTH
            | EIC_CONFIG_FILTEN5(1);
    EIC_REGS->EIC_CTRL = EIC_CTRL_ENABLE(1);
    
    NVIC_EnableIRQ(EIC_IRQn);
}
