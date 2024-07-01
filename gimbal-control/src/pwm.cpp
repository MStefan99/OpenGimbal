#include "pwm.hpp"

void pwm::init() {
	GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_TC1_TC2  // Enable TC1/TC2 clock
	                        | GCLK_CLKCTRL_CLKEN(1)    // Enable clock
	                        | GCLK_CLKCTRL_GEN_GCLK0;  // Set GCLK0 as a clock source

	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 14u | 0x1 << 15u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                         // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA14E_TC1_WO0)           // Connect to TC
	                                  | PORT_WRCONFIG_WRPMUX(1)                         // Write multiplexing bits
	                                  | PORT_WRCONFIG_WRPINCFG(1);                      // Write pin config bits
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 14u | 0x1 << 15u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                         // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA30E_TC2_WO0)           // Connect to TC
	                                  | PORT_WRCONFIG_WRPMUX(1)                         // Write multiplexing bits
	                                  | PORT_WRCONFIG_WRPINCFG(1)                       // Write config bits
	                                  | PORT_WRCONFIG_HWSEL(1);                         // Select top 16 bits

	TC2_REGS->COUNT8.TC_CTRLA = TC1_REGS->COUNT8.TC_CTRLA = TC_CTRLA_ENABLE(1)        // Enable TC
	                                                      | TC_CTRLA_MODE_COUNT8      // Set 8-bit mode
	                                                      | TC_CTRLA_WAVEGEN_NPWM     // Set Normal PWM mode
	                                                      | TC_CTRLA_RUNSTDBY(1)      // Run in standby mode
	                                                      | TC_CTRLA_PRESCALER_DIV1;  // Divide clock by 1
}

void pwm::setDuty(uint8_t channel, uint8_t duty) {
	if (channel < 2) {
		TC2_REGS->COUNT8.TC_CC[channel % 2] = duty;
	} else if (channel < 4) {
		TC1_REGS->COUNT8.TC_CC[channel % 2] = duty;
	}
}

void pwm::setBrightness(uint8_t channel, uint8_t brightness) {
	uint8_t duty = brightness * brightness / 268 + brightness / 20;

	if (channel < 2) {
		TC2_REGS->COUNT8.TC_CC[channel % 2] = duty;
	} else if (channel < 4) {
		TC1_REGS->COUNT8.TC_CC[channel % 2] = duty;
	}
}
