#include "lib/inc/util.hpp"


static uint32_t ticks
{
    0
};


extern "C" {

    void SysTick_Handler() {
        ++ticks;
    }
}

void util::init() { // TODO: move away
    /* Clock distribution
     *
     * OSC16M @ 8MHz
     * |
     * `--> GCLK1 @ 500KHz
     *      |
     *      `--> ADC @ 250KHz
     *
     * DFLL48M @ 48MHz
     * |
     * `--> GCLK0 @ 48MHz
     *      |
     *      |--> MCLK @ 48MHz
     *      |
     *      `--> TCC @ 48MHz
     */


    uint32_t calibration = *((uint32_t*)0x00806020);

    // SUPC config
    SUPC_REGS->SUPC_VREF = SUPC_VREF_TSEN(1) // Enable temperature sensor
            | SUPC_VREF_SEL_1V0; // Set 1.0V as a reference
    SUPC_REGS->SUPC_VREG = SUPC_VREG_SEL_BUCK | SUPC_VREG_ENABLE(1);
    while (!(SUPC_REGS->SUPC_STATUS & SUPC_INTFLAG_VREGRDY_Msk));

    // NVM setup
    NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_MANW(1) // Use NVM in manual write mode
            | NVMCTRL_CTRLB_RWS(4); // Use 4 wait states for NVM
    
    // PM config
    PM_REGS->PM_PLCFG = PM_PLCFG_PLSEL_PL2; // Enter PL2
    while (!(PM_REGS->PM_INTFLAG & PM_INTFLAG_PLRDY_Msk)); // Wait for the transition to complete

    // OSCCTRL config
    OSCCTRL_REGS->OSCCTRL_OSC16MCTRL = OSCCTRL_OSC16MCTRL_ENABLE(1) // Enable OSC16M
            | OSCCTRL_OSC16MCTRL_FSEL_8; // Set frequency to 8MHz
    OSCCTRL_REGS->OSCCTRL_DFLLVAL = OSCCTRL_DFLLVAL_COARSE(calibration >> 26u) // Load calibration value
            | OSCCTRL_DFLLVAL_FINE(512); // Middle value for FINE (0-1023) is a good starting point
    OSCCTRL_REGS->OSCCTRL_DFLLCTRL = OSCCTRL_DFLLCTRL_ENABLE(1) // Enable DFLL48M
            | OSCCTRL_DFLLCTRL_MODE(0); // Run in open-loop mode

    // GLCK config
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_GENEN(1) // Enable GCLK 0
            | GCLK_GENCTRL_SRC_DFLL48M; // Set DFLL48M as a source
    
    GCLK_REGS->GCLK_GENCTRL[1] = GCLK_GENCTRL_GENEN(1) // Enable GCLK 1
            | GCLK_GENCTRL_SRC_OSC16M // Set OSC16M as a source
            | GCLK_GENCTRL_DIVSEL_DIV2 // Set division mode (2^(x+1))
            | GCLK_GENCTRL_DIV(3); // Divide by 16 (2^(3+1))
    
    // SysTick config
    SysTick_Config(48000);

    // NVIC config
    __DMB();
    __enable_irq();
    NVIC_EnableIRQ(SysTick_IRQn);
}

// Fast inverse square root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float util::invSqrt(float x) {
  float halfx = 0.5f * x;
  union {
    float f;
    long i;
  } conv = {x};
  conv.i = 0x5f3759df - (conv.i >> 1);
  conv.f *= 1.5f - (halfx * conv.f * conv.f);
  conv.f *= 1.5f - (halfx * conv.f * conv.f);
  return conv.f;
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
