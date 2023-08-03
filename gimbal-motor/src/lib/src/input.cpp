#include "lib/inc/input.hpp"

#define TC_REGS TC4_REGS

extern "C" {
    void Input_Handler() {
        uint16_t c0 {TC_REGS->COUNT16.TC_CC[0]};
        uint16_t c1 {TC_REGS->COUNT16.TC_CC[1]};
        PORT_REGS->GROUP[0].PORT_OUTTGL = 1;
        TC_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
    }
}

void input::init() {
    // GCLK config
    GCLK_REGS->GCLK_PCHCTRL[29] = GCLK_PCHCTRL_CHEN(1) // Enable TC4 clock
            | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source
    
    // NVIC config
    NVIC_EnableIRQ(TC4_IRQn);
    
    // PORT config
    PORT_REGS->GROUP[0].PORT_PINCFG[14] = PORT_PINCFG_PMUXEN(1);
    PORT_REGS->GROUP[0].PORT_PINCFG[15] = PORT_PINCFG_PMUXEN(1);
    PORT_REGS->GROUP[0].PORT_PMUX[7] = PORT_PMUX_PMUXE_E | PORT_PMUX_PMUXO_E;
    
    // TC config
    TC_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_MCEO0(1)
            | TC_EVCTRL_MCEO1(1)
            | TC_EVCTRL_EVACT_PW;
    TC_REGS->COUNT16.TC_INTENSET = TC_INTENSET_MC1(1);
//    TC_REGS->COUNT16.TC_DBGCTRL = TC_DBGCTRL_DBGRUN(1);
    TC_REGS->COUNT16.TC_CTRLA = TC_CTRLA_COPEN0(1)
            | TC_CTRLA_COPEN1(1)
            | TC_CTRLA_CAPTEN0(1)
            | TC_CTRLA_CAPTEN1(1)
            | TC_CTRLA_PRESCALER_DIV16
            | TC_CTRLA_MODE_COUNT16;
    
    TC_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE(1);
    TC_REGS->COUNT16.TC_CTRLBSET = TC_CTRLBSET_CMD_RETRIGGER;
}
