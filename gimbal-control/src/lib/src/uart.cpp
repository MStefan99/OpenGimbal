#include "lib/inc/uart.hpp"


#define SERCOM_REGS SERCOM1_REGS

static uart::Buffer<uint8_t, 8> inBuffer {};
static RingBuffer<uart::Buffer<uint8_t, 32>, uint8_t, 4> outQueue {};

static uart::Callback<uint8_t, 8> callback {nullptr};
        

static void disableTx() {
    SERCOM_REGS->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN(1);
    while (SERCOM_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void enableTx() {
    SERCOM_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN(1);
    while (SERCOM_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void startTransfer(bool force = false) {
    if (SERCOM_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_TXEN_Msk && !force) { // SERCOM busy
        return;
    }
    
    if (outQueue.empty()) { // No pending transactions
        return;
    }
    
    enableTx();
    SERCOM_REGS->USART_INT.SERCOM_DATA = outQueue.front().buffer[0];
}

extern "C" {
    void SERCOM1_Handler() {
        if (!(SERCOM_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_FERR_Msk)){// Not a framing error
            if (SERCOM_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_TXEN_Msk) { // Transmitter enabled (outgoing transfer)
                --outQueue.front().remaining;
                if (!outQueue.front().remaining) { // Transmitted last byte, turning off transmitter
                    outQueue.pop_front();
                    if (outQueue.empty()) {
                        disableTx();
                    } else {
                        startTransfer(true);
                    }
                } else {
                    SERCOM_REGS->USART_INT.SERCOM_DATA = outQueue.front().buffer[++outQueue.front().transferred];
                }
            } else { // Transmitter disabled (incoming transfer)
                if (!inBuffer.remaining) { // Received first byte, set up new transfer
                    inBuffer.buffer[0] = SERCOM_REGS->USART_INT.SERCOM_DATA;
                    inBuffer.remaining = (inBuffer.buffer[0] >> 4u) - 1;
                    inBuffer.transferred = 1;
                } else { // Continued transfer   
                    inBuffer.buffer[inBuffer.transferred++] = SERCOM_REGS->USART_INT.SERCOM_DATA;
                    --inBuffer.remaining;
                    if (!inBuffer.remaining && callback) { // Received last byte, ready to process
                        callback(inBuffer);
                    }
                }
            }
        }
        (void)SERCOM_REGS->USART_INT.SERCOM_DATA; // Clear the RXC interrupt flag
        SERCOM_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
    }
}

void uart::init() {    
	GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_SERCOM1_CORE // Setting up SERCOM1 clock
            | GCLK_CLKCTRL_CLKEN(1) // Enable clock
			| GCLK_CLKCTRL_GEN_GCLK0; //Set GCLK0 as a clock source

	// PORT config
	PORT_REGS->GROUP[0].PORT_PINCFG[8] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 8
    //PORT_REGS->GROUP[0].PORT_PMUX[4] = PORT_PMUX_PMUXE_H;
	PORT_REGS->GROUP[0].PORT_PMUX[4] = PORT_PMUX_PMUXE(MUX_PA08C_SERCOM1_PAD2); // Mux pin 8 to SERCOM3

	// SERCOM config
	SERCOM_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)
                    | SERCOM_USART_INT_CTRLB_COLDEN(1)
					| SERCOM_USART_INT_CTRLB_PMODE_ODD
					| SERCOM_USART_INT_CTRLB_SBMODE_1_BIT
					| SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;
	SERCOM_REGS->USART_INT.SERCOM_BAUD = 5138; // 115200 baud
    SERCOM_REGS->USART_INT.SERCOM_DBGCTRL = SERCOM_USART_INT_DBGCTRL_DBGSTOP(1);
	SERCOM_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB
					| SERCOM_USART_INT_CTRLA_CMODE_ASYNC
                    | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC
					| SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY
					| SERCOM_USART_INT_CTRLA_RXPO_PAD2
					| SERCOM_USART_INT_CTRLA_TXPO_PAD1
					| SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK
					| SERCOM_USART_INT_CTRLA_ENABLE(1);
    
	SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1)
            | SERCOM_USART_INT_INTENSET_TXC(1);
    
    NVIC_EnableIRQ(SERCOM1_IRQn);
}


void uart::send(const uint8_t* buf, uint8_t len) {
    if (outQueue.full()) {
        return;
    }
    
    outQueue.push_back({{}, 0, len});
    util::copy(outQueue.back().buffer, buf, len);
    startTransfer();
}

uint8_t uart::print(const char* buf) {
    if (outQueue.full()) {
        return 0;
    }
    uint8_t len {0};
    for (; buf[len] && len < 32; ++len); 
    
    outQueue.push_back({{}, 0, len});
    util::copy(outQueue.back().buffer, reinterpret_cast<const uint8_t*>(buf), len);
    startTransfer();
    return len;
}

void uart::setCallback(uart::Callback<uint8_t, 8> cb) {
    callback = cb;
}