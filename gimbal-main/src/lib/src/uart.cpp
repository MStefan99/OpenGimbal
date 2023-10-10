#include "lib/inc/uart.hpp"


#define SERCOM_REGS SERCOM2_REGS


static uint8_t incomingData[8]{};
static uint8_t bytesReceived{0};
static tl::allocator<uint8_t> byteAllocator {};


extern "C" {


	void SBUS_Handler() {
		if (bytesReceived < 8) {
			incomingData[bytesReceived++] = SERCOM_REGS->USART_INT.SERCOM_DATA;
		}
		SERCOM_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTENSET_RXC(1);
	}
}


void uart::init() {
	GCLK_REGS->GCLK_PCHCTRL[SERCOM2_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1) // Enable SERCOM4 clock
					| GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source

	// PORT config
	PORT_REGS->GROUP[0].PORT_PINCFG[8] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 8
	PORT_REGS->GROUP[0].PORT_PINCFG[9] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 9
	PORT_REGS->GROUP[0].PORT_PMUX[4] = PORT_PMUX_PMUXE(MUX_PA08D_SERCOM2_PAD0) // Mux pin 8 to SERCOM2
					| PORT_PMUX_PMUXO(MUX_PA09D_SERCOM2_PAD1); // Mux pin 9 to SERCOM2

	// DMA config
	dma::initUART();

	// SERCOM config
	SERCOM_REGS->USART_INT.SERCOM_CTRLB = 
            //SERCOM_USART_INT_CTRLB_RXEN(1)
					SERCOM_USART_INT_CTRLB_TXEN(1)
					| SERCOM_USART_INT_CTRLB_PMODE_EVEN
					| SERCOM_USART_INT_CTRLB_SBMODE_1_BIT
					| SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;
	SERCOM_REGS->USART_INT.SERCOM_BAUD = 0; // 500KHz
	SERCOM_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB
					| SERCOM_USART_INT_CTRLA_CMODE_ASYNC
                    | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC
					| SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY
					| SERCOM_USART_INT_CTRLA_RXPO_PAD0
					| SERCOM_USART_INT_CTRLA_TXPO_PAD0
					| SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK
					| SERCOM_USART_INT_CTRLA_ENABLE(1);
	//SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1);
}


void uart::send(uint8_t* data, uint8_t size, void (*cb)(bool)) {
	uint8_t* txBuf = byteAllocator.allocate(size);
	util::copy(txBuf, data, size);

	dma::startTransfer(dma::UARTTransfer{
		.buf = txBuf,
		.len = size,
		.sercom = SERCOM_REGS,
        .cb = cb
	});
}


uint8_t uart::dataReceived() {
	return bytesReceived;
}


uint8_t* uart::getData() {
	bytesReceived = 0;
	return incomingData;
}
