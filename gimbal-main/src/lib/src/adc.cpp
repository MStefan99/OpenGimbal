#include "lib/inc/adc.hpp"
#include "lib/inc/util.hpp"

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
    // Clock setup
    GCLK_REGS->GCLK_PCHCTRL[ADC_GCLK_ID] = GCLK_PCHCTRL_CHEN(1) // Enable ADC clock
            | GCLK_PCHCTRL_GEN_GCLK2; // Set GCLK2 as a clock source
    
    // Voltage reference setup
    SUPC_REGS->SUPC_VREF = SUPC_VREF_TSEN(1) // Enable temperature sensor
            | SUPC_VREF_RUNSTDBY(1) // Enable in standby mode
            | SUPC_VREF_ONDEMAND(1) // Only enable if requested
            | SUPC_VREF_SEL_1V0; // Set 1.0V as a reference

    // ADC setup
    ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC2; // Set ADC reference voltage
    ADC_REGS->ADC_CALIB = ADC_CALIB_BIASREFBUF((OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC_BIASREFBUF_Msk) >> FUSES_OTP5_WORD_0_ADC_BIASREFBUF_Pos) 
            | ADC_CALIB_BIASCOMP((OTP5_FUSES_REGS->FUSES_OTP5_WORD_0 & FUSES_OTP5_WORD_0_ADC_BIASCOMP_Msk) >> FUSES_OTP5_WORD_0_ADC_BIASCOMP_Pos);
    ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND // Set GND as negative input
            | ADC_INPUTCTRL_MUXPOS_AIN6; // Set battery pin as positive input
    ADC_REGS->ADC_SAMPCTRL = ADC_SAMPCTRL_OFFCOMP(1); // Enable offset compensation
    ADC_REGS->ADC_INTENSET = ADC_INTENSET_RESRDY(1); // Enable result ready interrupt
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_RUNSTDBY(1) // Enable in standby mode
            | ADC_CTRLA_ONDEMAND(1) // Only enable if requested
            | ADC_CTRLA_ENABLE(1); // Enable ADC
    
    NVIC_EnableIRQ(ADC_IRQn);
}

void adc::measureBattery(void(*cb)(uint16_t)) {
    measureCallback = cb;
    
    // Workaround to start ADC after exiting sleep, needs to be fixed
    uint16_t i = 0;
    while (i++ < 100);
    
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1);
}
