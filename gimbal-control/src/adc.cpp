#include "adc.hpp"


static void (*batteryCallback)(uint16_t);
static void (*xCallback)(uint16_t);
static void (*yCallback)(uint16_t);


extern "C" {
	void ADC_Handler() {
		switch ((ADC_REGS->ADC_INPUTCTRL & ADC_INPUTCTRL_MUXPOS_Msk) >> ADC_INPUTCTRL_MUXPOS_Pos) {
			case (ADC_INPUTCTRL_MUXPOS_AIN0_Val): {
				if (xCallback) {
					xCallback(ADC_REGS->ADC_RESULT);
				}
				break;
			}
			case (ADC_INPUTCTRL_MUXPOS_AIN1_Val): {
				if (yCallback) {
					yCallback(ADC_REGS->ADC_RESULT);
				}
				break;
			}
			case (ADC_INPUTCTRL_MUXPOS_AIN6_Val): {
				if (batteryCallback) {
					batteryCallback(ADC_REGS->ADC_RESULT);
				}
				break;
			}
		}

		ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;
	}
}

void adc::init() {
	// Clock setup
	GCLK_REGS->GCLK_PCHCTRL[ADC_GCLK_ID] = GCLK_PCHCTRL_CHEN(1)     // Enable ADC clock
	                                     | GCLK_PCHCTRL_GEN_GCLK2;  // Set GCLK2 as a clock source

	// Voltage reference setup
	SUPC_REGS->SUPC_VREF = SUPC_VREF_TSEN(1)      // Enable temperature sensor
	                     | SUPC_VREF_ONDEMAND(1)  // Only enable if requested
	                     | SUPC_VREF_SEL_1V0;     // Set 1.0V as a reference

	// Pin setup
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 2u | 0x1 << 3u | 0x1 << 6u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                                   // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA02B_ADC_AIN0)                    // Multiplex to ADC
	                                  | PORT_WRCONFIG_WRPMUX(1)     // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1);  // Write pin config settings

	// ADC setup
	// ADC_REGS->ADC_CTRLB = ADC_CTRLB_PRESCALER_DIV2;  // Set prescaler to 2
	ADC_REGS->ADC_CALIB = ADC_CALIB_BIASREFBUF(
	                          (OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC_BIASREFBUF_Msk)
	                          >> FUSES_OTP5_WORD_0_ADC_BIASREFBUF_Pos
	                      )
	                    | ADC_CALIB_BIASCOMP(
	                          (OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC_BIASCOMP_Msk)
	                          >> FUSES_OTP5_WORD_0_ADC_BIASCOMP_Pos
	                    );
	ADC_REGS->ADC_SAMPCTRL = ADC_SAMPCTRL_OFFCOMP(1);    // Enable offset compensation
	ADC_REGS->ADC_INTENSET = ADC_INTENSET_RESRDY(1);     // Enable result ready interrupt
	ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC2;  // Set ADC reference voltage
	ADC_REGS->ADC_CTRLA =                                // ADC_CTRLA_RUNSTDBY(1)           // Enable in standby mode
	    ADC_CTRLA_ONDEMAND(1)                            // Only enable if requested
	    | ADC_CTRLA_ENABLE(1);                           // Enable ADC

	NVIC_EnableIRQ(ADC_IRQn);
}

void adc::measureTemperature(void (*cb)(uint16_t)) {
	batteryCallback = cb;

	// ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTREF;    // Set ADC reference voltage, enable-protected
	ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND    // Set GND as negative input
	                        | ADC_INPUTCTRL_MUXPOS_TEMP;  // Set temperature sensor as positive input

	ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}

void adc::measureBattery(void (*cb)(uint16_t)) {
	batteryCallback = cb;

	ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND    // Set GND as negative input
	                        | ADC_INPUTCTRL_MUXPOS_AIN6;  // Set battery pin as positive input

	ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}

void adc::measureX(void (*cb)(uint16_t)) {
	xCallback = cb;

	ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND    // Set GND as negative input
	                        | ADC_INPUTCTRL_MUXPOS_AIN0;  // Set X axis pin as positive input

	ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}

void adc::measureY(void (*cb)(uint16_t)) {
	yCallback = cb;

	ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND    // Set GND as negative input
	                        | ADC_INPUTCTRL_MUXPOS_AIN1;  // Set Y axis pin as positive input

	ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}
