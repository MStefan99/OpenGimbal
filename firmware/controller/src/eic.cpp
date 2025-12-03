#include "eic.hpp"


static eic::ButtonCallback buttonCallback {nullptr};
static eic::Callback       sensor1Callback {nullptr};
static eic::Callback       sensor2Callback {nullptr};
static eic::Callback       hostCallback {nullptr};

extern "C" {
	void EIC_Handler() {
		if (EIC_REGS->EIC_INTFLAG & (0x1 << 2) && sensor1Callback) {
			sensor1Callback();
		}

		if (EIC_REGS->EIC_INTFLAG & (0x1 << 3) && sensor1Callback) {
			sensor2Callback();
		}

		if ((EIC_REGS->EIC_INTFLAG & (0x3 << 6)) && buttonCallback) {
			bool    primary = EIC_REGS->EIC_INTFLAG & (0x1 << 6);
			uint8_t state = (~(PORT_REGS->GROUP[0].PORT_IN >> 22u)) & 0x3;

			buttonCallback(primary, primary ? state & 0x1 : state & 0x2);
		}

		if (EIC_REGS->EIC_INTFLAG & (0x1 << 11) && hostCallback) {
			hostCallback();
		}

		EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
	}
}

void eic::init() {
	// Clock setup
	GCLK_REGS->GCLK_PCHCTRL[EIC_GCLK_ID] = GCLK_PCHCTRL_CHEN(1)     // Enable EIC clock
	                                     | GCLK_PCHCTRL_GEN_GCLK3;  // Set GCLK2 as a clock source

	// Set up pins 18 and 19
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 2u | 0x1 << 3u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                       // Enable multiplexing
	                                  | PORT_WRCONFIG_INEN(1)                         // Enable inputs
	                                  | PORT_WRCONFIG_PMUX(MUX_PA18A_EIC_EXTINT2)     // Multiplex to EIC
	                                  | PORT_WRCONFIG_WRPMUX(1)                       // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1)                     // Write pin config settings
	                                  | PORT_WRCONFIG_HWSEL(1);                       // Select pin range

	// Set up pins 24 and 25
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 6u | 0x1 << 7u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                       // Enable multiplexing
	                                  | PORT_WRCONFIG_INEN(1)                         // Enable inputs
	                                  | PORT_WRCONFIG_PULLEN(1)                       // Enable pull resistors
	                                  | PORT_WRCONFIG_PMUX(MUX_PA22A_EIC_EXTINT6)     // Multiplex to EIC
	                                  | PORT_WRCONFIG_WRPMUX(1)                       // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1)                     // Write pin config settings
	                                  | PORT_WRCONFIG_HWSEL(1);                       // Select pin range

	// Set up pin 31
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 15u)           // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                     // Enable multiplexing
	                                  | PORT_WRCONFIG_INEN(1)                       // Enable inputs
	                                  | PORT_WRCONFIG_PMUX(MUX_PA31A_EIC_EXTINT11)  // Multiplex to EIC
	                                  | PORT_WRCONFIG_WRPMUX(1)                     // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1)                   // Write pin config settings
	                                  | PORT_WRCONFIG_HWSEL(1);                     // Select pin range

	// Enable pull-ups on button inputs
	PORT_REGS->GROUP[0].PORT_OUTSET = (0x1 << 22u | 0x1 << 23u);

	EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE2_RISE | EIC_CONFIG_SENSE3_RISE | EIC_CONFIG_SENSE6_BOTH
	                        | EIC_CONFIG_SENSE7_BOTH | EIC_CONFIG_FILTEN6(1) | EIC_CONFIG_FILTEN7(1);
	EIC_REGS->EIC_CONFIG[1] = EIC_CONFIG_SENSE3_RISE | EIC_CONFIG_FILTEN3(1);
	EIC_REGS->EIC_INTENSET = ((0x1 << 2u) | (0x1 << 3u) | (0x1 << 6u) | (0x1 << 7u) | (0x1 << 11u));  // Enable interrupts
	EIC_REGS->EIC_CTRLA = EIC_CTRLA_ENABLE(1);                                                        // Enable EIC

	NVIC_EnableIRQ(EIC_IRQn);
}

void eic::setButtonCallback(ButtonCallback cb) {
	buttonCallback = cb;
}

void eic::setSensor1Callback(Callback cb) {
	sensor1Callback = cb;
}

void eic::setSensor2Callback(Callback cb) {
	sensor2Callback = cb;
}

void eic::setHostCallback(Callback cb) {
	hostCallback = cb;
}
