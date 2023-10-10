#include "lib/inc/uart.hpp"


#define SERCOM_REGS SERCOM3_REGS
#define ADDR 0

static uint8_t buf[3] {0};
static uint8_t remainingBytes {0};

extern "C" {
    void SERCOM3_Handler() {
        uint8_t d = SERCOM_REGS->USART_INT.SERCOM_DATA;
        
        if (remainingBytes == 0) {
            if (d == (0x50 | ADDR)) {
                buf[0] = d;
                remainingBytes = 2;
            } else {
                onInput(buf[1] << 8u | buf[2]);
            }
        } else {
            buf[3 - remainingBytes] = d;
            --remainingBytes;
        }
    }
}


void uart::init() {
	GCLK_REGS->GCLK_PCHCTRL[SERCOM3_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1) // Enable SERCOM3 clock
					| GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source

	// PORT config
	PORT_REGS->GROUP[0].PORT_PINCFG[22] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 22
	PORT_REGS->GROUP[0].PORT_PINCFG[23] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 23
	PORT_REGS->GROUP[0].PORT_PMUX[11] = PORT_PMUX_PMUXE(MUX_PA22C_SERCOM3_PAD0) // Mux pin 22 to SERCOM3
					| PORT_PMUX_PMUXO(MUX_PA23C_SERCOM3_PAD1); // Mux pin 23 to SERCOM3

	// DMA config
	//dma::initUART();

	// SERCOM config
	SERCOM_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)
					//| SERCOM_USART_INT_CTRLB_TXEN(1)
					| SERCOM_USART_INT_CTRLB_PMODE_EVEN
					| SERCOM_USART_INT_CTRLB_SBMODE_1_BIT
					| SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;
	SERCOM_REGS->USART_INT.SERCOM_BAUD = 63018; // 115200 baud
	SERCOM_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB
					| SERCOM_USART_INT_CTRLA_CMODE_ASYNC
                    | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC
					| SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY
					| SERCOM_USART_INT_CTRLA_RXPO_PAD1
					| SERCOM_USART_INT_CTRLA_TXPO_PAD0
					| SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK
					| SERCOM_USART_INT_CTRLA_ENABLE(1);
    
	SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1); 
    NVIC_EnableIRQ(SERCOM3_IRQn);
}
