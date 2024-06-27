#include "adc.hpp"


static void (*xCallback)(uint16_t);
static void (*yCallback)(uint16_t);


extern "C" {
	void ADC_Handler() {
		switch ((ADC_REGS->ADC_INPUTCTRL & ADC_INPUTCTRL_MUXPOS_Msk) >> ADC_INPUTCTRL_MUXPOS_Pos) {
			case (ADC_INPUTCTRL_MUXPOS_PIN2_Val): {
				if (xCallback) {
					xCallback(ADC_REGS->ADC_RESULT);
				}
				break;
			}
			case (ADC_INPUTCTRL_MUXPOS_PIN0_Val): {
				if (yCallback) {
					yCallback(ADC_REGS->ADC_RESULT);
				}
				break;
			}
		}

		ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;
	}
}

void adc::init() {
	// Clock setup
	GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_ADC      // Enable ADC clock
	                        | GCLK_CLKCTRL_CLKEN(1)    // Enable clock
	                        | GCLK_CLKCTRL_GEN_GCLK0;  // Set GCLK0 as a clock source

	// Pin setup
	PORT_REGS->GROUP[0].PORT_PINCFG[2] = PORT_PINCFG_PMUXEN(1);              // Enable mux on pin 2
	PORT_REGS->GROUP[0].PORT_PMUX[1] = PORT_PMUX_PMUXE(MUX_PA02B_ADC_AIN0);  // Mux pin 2 to ADC

	PORT_REGS->GROUP[0].PORT_PINCFG[4] = PORT_PINCFG_PMUXEN(1);              // Enable mux on pin 4
	PORT_REGS->GROUP[0].PORT_PMUX[2] = PORT_PMUX_PMUXE(MUX_PA04B_ADC_AIN2);  // Mux pin 4 to ADC

	// ADC setup
	ADC_REGS->ADC_CTRLB = ADC_CTRLB_PRESCALER_DIV8;      // Set prescaler to 8
	ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC0;  // Set ADC reference voltage
	ADC_REGS->ADC_CALIB =
	    ADC_CALIB_LINEARITY_CAL(
	        ((OTP4_FUSES_REGS->FUSES_OTP4_WORD_0 & FUSES_OTP4_WORD_0_ADC_LINEARITY_0_Msk)
	         >> FUSES_OTP4_WORD_0_ADC_LINEARITY_0_Pos)
	        | ((OTP4_FUSES_REGS->FUSES_OTP4_WORD_1 & FUSES_OTP4_WORD_1_ADC_LINEARITY_1_Msk) << 4u)
	    )
	    | ADC_CALIB_BIAS_CAL(
	        (OTP4_FUSES_REGS->FUSES_OTP4_WORD_1 & FUSES_OTP4_WORD_1_ADC_BIASCAL_Msk) >> FUSES_OTP4_WORD_1_ADC_BIASCAL_Pos
	    );
	ADC_REGS->ADC_INTENSET = ADC_INTENSET_RESRDY(1);  // Enable result ready interrupt
	ADC_REGS->ADC_CTRLA = ADC_CTRLA_RUNSTDBY(1)       // Enable in standby mode
	                    | ADC_CTRLA_ENABLE(1);        // Enable ADC

	NVIC_EnableIRQ(ADC_IRQn);
}

void adc::getX(void (*cb)(uint16_t)) {
	xCallback = cb;

	ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND    // Set GND as negative input
	                        | ADC_INPUTCTRL_MUXPOS_PIN2;  // Set battery pin as positive input

	ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}

void adc::getY(void (*cb)(uint16_t)) {
	yCallback = cb;

	ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND    // Set GND as negative input
	                        | ADC_INPUTCTRL_MUXPOS_PIN0;  // Set battery pin as positive input

	ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}
