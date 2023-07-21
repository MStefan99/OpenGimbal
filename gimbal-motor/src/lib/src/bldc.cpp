#include "lib/inc/bldc.hpp"

static tcc_registers_t* getTimer(uint8_t channel) {
    if (channel < 4) {
        return TCC0_REGS;
    } else {
        return TCC1_REGS;
    }
}

static uint8_t getTimerChannel(uint8_t channel) {
    if (channel < 4) {
        return channel;
    } else {
        return channel & 0x01;
    }
}

static uint8_t getPin(uint8_t channel) {
    if (channel < 2) {
        return channel + 4;
    } else if (channel < 4) {
        return channel + 8;
    } else {
        return channel + 2;
    }
}

void bldc::init() {
    // GLCK config
    GCLK_REGS->GCLK_PCHCTRL[25] = GCLK_PCHCTRL_CHEN(1) // Enable TCC[0:1] clock
            | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source
    GCLK_REGS->GCLK_PCHCTRL[26] = GCLK_PCHCTRL_CHEN(1) // Enable TCC2 clock
            | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source

    // TCC config
    TCC0_REGS->TCC_DBGCTRL = TCC_DBGCTRL_DBGRUN(1); // Run while debugging
    TCC0_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM | TCC_WAVE_POL1(1) | TCC_WAVE_POL3(1); // PWM generation
    TCC0_REGS->TCC_PER = 250;
    TCC0_REGS->TCC_CC[0] = 0;
    TCC0_REGS->TCC_CC[1] = 0;
    TCC0_REGS->TCC_CC[2] = 0;
    TCC0_REGS->TCC_CC[3] = 0;
    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE(1); // Enable timer

    TCC1_REGS->TCC_DBGCTRL = TCC_DBGCTRL_DBGRUN(1); // Run while debugging
    TCC1_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM | TCC_WAVE_POL1(1); // PWM generation
    TCC1_REGS->TCC_PER = 250;
    TCC1_REGS->TCC_CC[0] = 0;
    TCC1_REGS->TCC_CC[1] = 0;
    TCC1_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE(1); // Enable timer

    // PORT config
    for (uint8_t i{0}; i < 6; ++i) {
        uint8_t pin{getPin(i)};
        PORT_REGS->GROUP[0].PORT_PINCFG[pin] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin
        if (pin & 0x1) { // Odd pin
            PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] = (PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] & 0xf)
                    | (i == 3 ? PORT_PMUX_PMUXO_F : PORT_PMUX_PMUXO_E);
        } else { // Even pin
            PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] = (i == 2 ? PORT_PMUX_PMUXE_F : PORT_PMUX_PMUXE_E)
                    | (PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] & 0xf0);
        }
    }
}

static constexpr float SQRT3 = sqrtf(3);

void bldc::setAngle(int16_t angle) {
    float fangle = angle / 477.4648292757;
    
    float va = sinf(fangle);
    float vb = cosf(fangle);
    
    TCC0_REGS->TCC_CCBUF[0] = TCC0_REGS->TCC_CCBUF[1] = (va + 1) / 2 * 250;
    TCC0_REGS->TCC_CCBUF[2] = TCC0_REGS->TCC_CCBUF[3] = ((-va + SQRT3 * vb) / 2 + 1) / 2 * 250;
    TCC1_REGS->TCC_CCBUF[0] = TCC1_REGS->TCC_CCBUF[1] = ((-va - SQRT3 * vb) / 2 + 1) / 2 * 250;
}

void bldc::beep(uint16_t frequency) {
    uint16_t period = 8000000 / frequency;
    
    TCC0_REGS->TCC_PER = period;
    TCC1_REGS->TCC_PER = period;
}
