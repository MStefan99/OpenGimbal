#include "lib/inc/adc.hpp"

static void (*measureCallback)(uint16_t);


extern "C" {
    void ADC_Handler() {
        if (measureCallback) {
            measureCallback(ADC_REGS->ADC_RESULT);
        }
        ADC_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;
    }
}

void adc::init() {
    // GCLK config
    GCLK_REGS->GCLK_PCHCTRL[ADC_GCLK_ID] = GCLK_PCHCTRL_CHEN(1) // Enable ADC clock
            | GCLK_PCHCTRL_GEN_GCLK1; //Set GCLK1 as a clock source

    // ADC config
    ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC2; // Set ADC reference voltage
    ADC_REGS->ADC_CALIB = ADC_CALIB_BIASREFBUF((OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC_BIASREFBUF_Msk) >> FUSES_OTP5_WORD_0_ADC_BIASREFBUF_Pos) 
            | ADC_CALIB_BIASCOMP((OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC_BIASCOMP_Msk) >> FUSES_OTP5_WORD_0_ADC_BIASCOMP_Pos);
    ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND // Set GND as negative input
            | ADC_INPUTCTRL_MUXPOS_AIN6; // Set battery pin as positive input
    ADC_REGS->ADC_SAMPCTRL = ADC_SAMPCTRL_OFFCOMP(1); // Enable offset compensation
    ADC_REGS->ADC_INTENSET = ADC_INTFLAG_RESRDY(1); // Enable result ready interrupt
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_ENABLE(1); // Enable ADC
    
    NVIC_EnableIRQ(ADC_IRQn);
}

void adc::measureBattery(void(*cb)(uint16_t)) {
    measureCallback = cb;
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}
