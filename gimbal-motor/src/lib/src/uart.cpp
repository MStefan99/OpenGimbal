#include "lib/inc/uart.hpp"


#define SERCOM_REGS SERCOM3_REGS
static constexpr uint8_t deviceAddress {1};

static uint8_t setupByte {0};
static void processCommand(bool success, const dma::UARTTransfer& transfer);

extern "C" {
    void SERCOM3_Handler() {
        if (!(SERCOM_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_FERR_Msk)) {
            if ((SERCOM_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) &&
                    !(SERCOM_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk)) {
                
                SERCOM_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_RXC(1);
                setupByte = SERCOM_REGS->USART_INT.SERCOM_DATA;

                dma::UARTTransfer t {
                    .len = static_cast<uint8_t>((setupByte >> 4u) - 1),
                    .type = dma::UARTTransferType::In,
                    .cb = processCommand
                };
                dma::startTransfer(t);
            }
        }
        (void)SERCOM_REGS->USART_INT.SERCOM_DATA; // Clear the RXC interrupt flag
        SERCOM_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
    }
}

void disableTx(bool success, const dma::UARTTransfer& transfer) {
    SERCOM_REGS->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN(1);
    while (SERCOM_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
    (void)SERCOM_REGS->USART_INT.SERCOM_DATA; // Clear the RXC interrupt flag
    SERCOM_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
    SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1);
}

void processCommand(bool success, const dma::UARTTransfer& transfer) {
    uint8_t len = (setupByte >> 4u) - 1;
    
    if ((setupByte & 0x0f) != deviceAddress) {
        SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1);
        return;
    } // Command intended for another device
    
    switch(transfer.buf[0] & 0x0f) { // Parse command type
        case 0: { // Ping command
            SERCOM_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN(1);
            while (SERCOM_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
            dma::UARTTransfer newTransfer {
                .len = 3,
                .type = dma::UARTTransferType::Out,
                .cb = disableTx
            };
            newTransfer.buf[0] = 0x30;
            newTransfer.buf[1] = deviceAddress << 4u;
            newTransfer.buf[2] = transfer.buf[1];
            dma::startTransfer(newTransfer);
        }
        break;
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
	dma::initUART();

	// SERCOM config
	SERCOM_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)
                    | SERCOM_USART_INT_CTRLB_COLDEN(1)
					| SERCOM_USART_INT_CTRLB_PMODE_ODD
					| SERCOM_USART_INT_CTRLB_SBMODE_1_BIT
					| SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;
	SERCOM_REGS->USART_INT.SERCOM_BAUD = 63020; // 115200 baud
    SERCOM_REGS->USART_INT.SERCOM_DBGCTRL = SERCOM_USART_INT_DBGCTRL_DBGSTOP(1);
	SERCOM_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB
					| SERCOM_USART_INT_CTRLA_CMODE_ASYNC
                    | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC
					| SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY
					| SERCOM_USART_INT_CTRLA_RXPO_PAD0
					| SERCOM_USART_INT_CTRLA_TXPO_PAD0
					| SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK
					| SERCOM_USART_INT_CTRLA_ENABLE(1);
    
	SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1);
    NVIC_EnableIRQ(SERCOM3_IRQn);
}
