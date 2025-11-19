#include "eic.hpp"

void (*callback)() {nullptr};


extern "C" {
	void EIC_Handler() {
		if (callback) {
			callback();
		}
		EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
	}
}

void eic::init() {
	EIC_REGS->EIC_ASYNCH = 0x1 << 6u;
	EIC_REGS->EIC_INTENSET = 0x1 << 6u;
	EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE6_RISE;
	EIC_REGS->EIC_CTRLA = EIC_CTRLA_ENABLE(1);  // Select pin range

	NVIC_EnableIRQ(EIC_IRQn);
}

void eic::enable() {
	PORT_REGS->GROUP[0].PORT_PINCFG[22] = PORT_PINCFG_PMUXEN(1);                 // Enable mux on pin 22
	PORT_REGS->GROUP[0].PORT_PMUX[11] = PORT_PMUX_PMUXE(MUX_PA22A_EIC_EXTINT6);  // Mux pin 22 to EIC
}

void eic::setCallback(void (*cb)()) {
	callback = cb;
}
