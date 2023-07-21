#include "lib/inc/servo.hpp"


static tcc_registers_t* getTimer(uint8_t channel);
static uint8_t getTimerChannel(uint8_t channel);
static uint8_t getPin(uint8_t channel);

static tcc_registers_t* getTimer(uint8_t channel) {
    if (channel < 4) {
        return TCC0_REGS;
    } else if (channel < 6) {
        return TCC1_REGS;
    } else {
        return TCC2_REGS;
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
    } else if (channel < 6) {
        return channel + 2;
    } else {
        return channel - 6;
    }
}

void servo::init() {
    // GLCK config
    GCLK_REGS->GCLK_PCHCTRL[25] = GCLK_PCHCTRL_CHEN(1) // Enable TCC[0:1] clock
            | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source
    GCLK_REGS->GCLK_PCHCTRL[26] = GCLK_PCHCTRL_CHEN(1) // Enable TCC2 clock
            | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source
}

void servo::enable(uint8_t channel) {
    tcc_registers_t* timer {getTimer(channel)};

    // TCC config
    timer->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV8;
    timer->TCC_DBGCTRL = TCC_DBGCTRL_DBGRUN(1); // Run while debugging
    timer->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM; // PWM generation
    timer->TCC_PER = 20000; // 20ms * GCLK_TCC
	timer->TCC_CC[getTimerChannel(channel)] = 1500; // 1.5ms * GCLK_TCC
    timer->TCC_CTRLA |= TCC_CTRLA_ENABLE(1); // Enable timer

    // PORT config
    uint8_t pin {getPin(channel)};
    PORT_REGS->GROUP[0].PORT_PINCFG[pin] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin
    if (pin & 0x1) { // Odd pin
        PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] = (PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] & 0xf)
                | (channel == 3? PORT_PMUX_PMUXO_F : PORT_PMUX_PMUXO_E);
    } else { // Even pin
        PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] = (channel == 2? PORT_PMUX_PMUXE_F : PORT_PMUX_PMUXE_E)
                | (PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] & 0xf0);
    }
}

void servo::disable(uint8_t channel) {
    uint8_t pin {getPin(channel)};
    PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << pin;
    PORT_REGS->GROUP[0].PORT_PINCFG[pin] = PORT_PINCFG_PMUXEN(0); // Enable mux on pin
}

void servo::setChannel(uint8_t channel, int16_t angle) {
    // Range: [1ms * GCLK_TC; 2ms * GCLK_TCC]
    angle = (angle * 1000) >> 10u;
    getTimer(channel)->TCC_CCBUF[getTimerChannel(channel)] = CLAMP(-1500, angle, 1500) / 2 + 1500;
}
