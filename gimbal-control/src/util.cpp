#include "util.hpp"


static uint32_t ticks {0};


extern "C" {
    void SysTick_Handler() {
        ++ticks;
    }
}

void util::init() {
    // Oscillator setup
    SYSCTRL_REGS->SYSCTRL_OSC8M = SYSCTRL_REGS->SYSCTRL_OSC8M & ~SYSCTRL_OSC8M_PRESC_Msk | SYSCTRL_OSC8M_PRESC_2;
    
    // GCLK setup
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_ID(1)
            | GCLK_GENCTRL_SRC_OSCULP32K
            | GCLK_GENCTRL_GENEN(1);
    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_ID(1)
            | GCLK_GENDIV_DIV(32);
    
    // APB clock setup
    PM_REGS->PM_APBCMASK = PM_APBCMASK_SERCOM1(1) // Enable APB SERCOM1 clock
            | PM_APBCMASK_TC1(1) // Enable APB TC1 clock
            | PM_APBCMASK_TC2(1) // Enable APB TC2 clock
            | PM_APBCMASK_ADC(1); // Enable APB ADC clock

    // SysTick setup
    SysTick_Config(2000);

    // NVIC setup
    __DMB();
    __enable_irq();
    NVIC_EnableIRQ(SysTick_IRQn);
    
    // ADC setup
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_ADC // Enable ADC clock
            | GCLK_CLKCTRL_CLKEN(1) // Enable clock
			| GCLK_CLKCTRL_GEN_GCLK0; //Set GCLK0 as a clock source
    ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC0; // Set ADC reference voltage
    ADC_REGS->ADC_INTENSET = ADC_INTFLAG_RESRDY(1); // Enable result ready interrupt
    ADC_REGS->ADC_CTRLB = ADC_CTRLB_PRESCALER_DIV8; // Divide input clock by 8
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_ENABLE(1); // Enable ADC
    
    PORT_REGS->GROUP[0].PORT_PINCFG[2] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 2
    PORT_REGS->GROUP[0].PORT_PMUX[1] = PORT_PMUX_PMUXE(MUX_PA02B_ADC_AIN0); // Mux pin 2 to ADC
    
    PORT_REGS->GROUP[0].PORT_PINCFG[4] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 4
    PORT_REGS->GROUP[0].PORT_PMUX[2] = PORT_PMUX_PMUXE(MUX_PA04B_ADC_AIN2); // Mux pin 4 to ADC
}

uint32_t util::getTime() {
    return ticks;
}

void util::sleep(uint32_t ms) {
    uint32_t t{ticks + ms};

    while (ticks < t) {
        __WFI();
    }
}
