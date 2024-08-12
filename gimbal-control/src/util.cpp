#include "util.hpp"


static uint32_t ticks {0};


extern "C" {
	void SysTick_Handler() {
		++ticks;
	}
}

void util::init() {
	// Oscillator setup
	SYSCTRL_REGS->SYSCTRL_OSC8M = (SYSCTRL_REGS->SYSCTRL_OSC8M & (SYSCTRL_OSC8M_FRANGE_Msk | SYSCTRL_OSC8M_CALIB_Msk))
	                            | SYSCTRL_OSC8M_ENABLE(1)    // Enable OSC8M
	                            | SYSCTRL_OSC8M_RUNSTDBY(1)  // Enable in standby
	                            | SYSCTRL_OSC8M_ONDEMAND(1)  // Enable only when requested
	                            | SYSCTRL_OSC8M_PRESC_2;     // Divide by 4

	// NVM setup
	NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_MANW(1);  // Change NVM writes to manual

	// GCLK setup
	GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_ID(1)          // GCLK 1
	                        | GCLK_GENCTRL_SRC_OSCULP32K  // Select OSCULP32K as a clock source
	                        | GCLK_GENCTRL_DIVSEL_DIV2    // Select division mode 2^(x+1)
	                        | GCLK_GENCTRL_GENEN(1);      // Enable clock
	GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_ID(1)            // GCLK 1
	                       | GCLK_GENDIV_DIV(6);          // Divide by 128 (2^(6+1))

	// APB clock setup
	PM_REGS->PM_APBCMASK = PM_APBCMASK_SERCOM1(1)  // Enable APB SERCOM1 clock
	                     | PM_APBCMASK_TC1(1)      // Enable APB TC1 clock
	                     | PM_APBCMASK_TC2(1)      // Enable APB TC2 clock
	                     | PM_APBCMASK_ADC(1);     // Enable APB ADC clock

	// SysTick setup
	SysTick_Config(2000);

	// NVIC setup
	__DMB();
	__enable_irq();
	NVIC_EnableIRQ(SysTick_IRQn);
}

uint32_t util::getTime() {
	return ticks;
}

void util::sleep(uint32_t ms) {
	uint32_t start {ticks};

	while (ticks - start < ms) {
		__WFI();
	}
}
