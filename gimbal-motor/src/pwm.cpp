#include "pwm.hpp"

#define TC_REGS TC0_REGS


extern "C" {
	void TC0_Handler() {
		uint16_t duration {TC_REGS->COUNT16.TC_CC[1]};
		// setTargetAngle((util::clamp(duration,
		//         static_cast<uint16_t>(3000),
		//         static_cast<uint16_t>(6000)) - 3000) * 4 / 3); // Map to 0-4096 range
		TC_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
	}
}

void pwm::init() {
	// GCLK config
	GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] = GCLK_PCHCTRL_CHEN(1)     // Enable TC0 clock
	                                     | GCLK_PCHCTRL_GEN_GCLK0;  // Set GCLK0 as a clock source
	// GCLK_REGS->GCLK_PCHCTRL[3] = GCLK_PCHCTRL_CHEN(1) // Enable EIC clock
	//         | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source
	// GCLK_REGS->GCLK_PCHCTRL[5] = GCLK_PCHCTRL_CHEN(1) // Enable EVSYS channel 0 clock
	//         | GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source

	// NVIC config
	NVIC_EnableIRQ(TC0_IRQn);

	// EIC config
	// EIC_REGS->EIC_CONFIG[1] = EIC_CONFIG_SENSE6_HIGH;
	// EIC_REGS->EIC_EVCTRL = 0x1 << 14u;
	// EIC_REGS->EIC_CTRLA = EIC_CTRLA_ENABLE(1);

	// TC config
	TC_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PW;
	TC_REGS->COUNT16.TC_INTENSET = TC_INTENSET_MC1(1);
	// TC_REGS->COUNT16.TC_DBGCTRL = TC_DBGCTRL_DBGRUN(1);
	TC_REGS->COUNT16.TC_CTRLA =
	    TC_CTRLA_CAPTEN1(1) | TC_CTRLA_COPEN1(1) | TC_CTRLA_PRESCALER_DIV16 | TC_CTRLA_MODE_COUNT16;

	TC_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE(1);
	TC_REGS->COUNT16.TC_CTRLBSET = TC_CTRLBSET_CMD_RETRIGGER;

	// EVSYS config
	// EVSYS_REGS->EVSYS_USER[30] = 0x01;
	// EVSYS_REGS->EVSYS_CHANNEL[0] = EVSYS_CHANNEL_EVGEN(0x1a) |
	//         EVSYS_CHANNEL_PATH_ASYNCHRONOUS |
	//         EVSYS_CHANNEL_EDGSEL_BOTH_EDGES;

	// PORT config
	PORT_REGS->GROUP[0].PORT_PINCFG[23] = PORT_PINCFG_PMUXEN(1);
	PORT_REGS->GROUP[0].PORT_PMUX[11] = PORT_PMUX_PMUXO(MUX_PA23E_TC0_WO1);
}
