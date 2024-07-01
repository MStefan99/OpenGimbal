#include "buttons.hpp"


static buttons::Callback callback {nullptr};

extern "C" {
	void EIC_Handler() {
		if (callback) {
			bool    left = EIC_REGS->EIC_INTFLAG & EIC_INTFLAG_EXTINT4_Msk;
			uint8_t state = (~(PORT_REGS->GROUP[0].PORT_IN >> 24u)) & 0x3;

			callback(left, left ? state & 0x1 : state & 0x2);
		}

		EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
	}
}

void buttons::init() {
	GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_EIC      // Enable EIC clock
	                        | GCLK_CLKCTRL_CLKEN(1)    // Enable clock
	                        | GCLK_CLKCTRL_GEN_GCLK1;  // Set GCLK1 as a clock source

	// Set up pins 24 and 25
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 8u | 0x1 << 9u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                       // Enable multiplexing
	                                  | PORT_WRCONFIG_INEN(1)                         // Enable inputs
	                                  | PORT_WRCONFIG_PULLEN(1)                       // Enable pull resistors
	                                  | PORT_WRCONFIG_PMUX(MUX_PA24A_EIC_EXTINT4)     // Multiplex to EIC
	                                  | PORT_WRCONFIG_WRPMUX(1)                       // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1)                     // Write pin config settings
	                                  | PORT_WRCONFIG_HWSEL(1);                       // Select pin range

	// Enable pull-ups on button inputs
	PORT_REGS->GROUP[0].PORT_OUTSET = (0x1 << 24u | 0x1 << 25u);

	EIC_REGS->EIC_INTENSET = EIC_INTENSET_EXTINT4(1) | EIC_INTENSET_EXTINT5(1);  // Enable interrupts
	EIC_REGS->EIC_WAKEUP = EIC_WAKEUP_WAKEUPEN4(1) | EIC_WAKEUP_WAKEUPEN5(1);    // Wake on interrupts
	EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE4_BOTH | EIC_CONFIG_SENSE5_BOTH;   // Generate interrupts on both edges
	EIC_REGS->EIC_CTRL = EIC_CTRL_ENABLE(1);                                     // Enable EIC

	NVIC_EnableIRQ(EIC_IRQn);
}

void buttons::setCallback(Callback cb) {
	callback = cb;
}
