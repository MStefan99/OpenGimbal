#include "pwm.hpp"

void pwm::init() {
	// Clock setup
	GCLK_REGS->GCLK_PCHCTRL[TCC0_GCLK_ID] = GCLK_PCHCTRL_CHEN(1)     // Enable clock
	                                      | GCLK_PCHCTRL_GEN_GCLK0;  // Set GCLK0 as a clock source

	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 8u | 0x1 << 9u)    // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                         // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA08E_TCC0_WO0)          // Connect to TC
	                                  | PORT_WRCONFIG_WRPMUX(1)                         // Write multiplexing bits
	                                  | PORT_WRCONFIG_WRPINCFG(1);                      // Write pin config bits
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 10u | 0x1 << 11u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                         // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA10F_TCC0_WO2)          // Connect to TC
	                                  | PORT_WRCONFIG_WRPMUX(1)                         // Write multiplexing bits
	                                  | PORT_WRCONFIG_WRPINCFG(1);                      // Write pin config bits

	TCC0_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;  // Use NPWM mode
	TCC0_REGS->TCC_PER = 65536;
	TCC0_REGS->TCC_CTRLA = TCC_CTRLA_ENABLE(1)        // Enable TC
	                     | TCC_CTRLA_PRESCALER_DIV1;  // Divide clock by 1
}

void pwm::setDuty(uint8_t channel, uint16_t duty) {
	if (channel > 3) {
		return;
	}

	TCC0_REGS->TCC_CC[channel] = duty;
	TCC0_REGS->TCC_CTRLBSET = TCC_CTRLBSET_CMD_RETRIGGER;
}

void pwm::setBrightness(uint8_t channel, uint16_t brightness) {
	setDuty(channel, powf(brightness / 65535.0f, gamma) * 65535);
}
