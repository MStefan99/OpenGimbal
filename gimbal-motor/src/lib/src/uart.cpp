#include "lib/inc/uart.hpp"


static uart::DefaultQueue outQueue {};
static uart::DefaultCallback::buffer_type inBuffer {};
static uart::DefaultCallback::callback_type callback {nullptr};


static void initSERCOM(sercom_registers_t* regs) {
    regs->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)
                    | SERCOM_USART_INT_CTRLB_COLDEN(1)
					| SERCOM_USART_INT_CTRLB_PMODE_ODD
					| SERCOM_USART_INT_CTRLB_SBMODE_1_BIT
					| SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;
	regs->USART_INT.SERCOM_BAUD = 63020; // 115200 baud
    regs->USART_INT.SERCOM_DBGCTRL = SERCOM_USART_INT_DBGCTRL_DBGSTOP(1);
	regs->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB
					| SERCOM_USART_INT_CTRLA_CMODE_ASYNC
                    | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC
					| SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY
					| SERCOM_USART_INT_CTRLA_RXPO_PAD0
					| SERCOM_USART_INT_CTRLA_TXPO_PAD0
					| SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK
					| SERCOM_USART_INT_CTRLA_ENABLE(1);
    
	regs->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1)
            | SERCOM_USART_INT_INTENSET_TXC(1);
}


static void disableTx(sercom_registers_t* regs) {
    regs->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN(1);
    while (regs->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void enableTx(sercom_registers_t* regs) {
    regs->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN(1);
    while (regs->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}


static void startTransfer(sercom_registers_t* regs, uart::DefaultQueue& outQueue, bool force = false) {
    if (regs->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_TXEN_Msk && !force) { // SERCOM busy
        return;
    }
    
    if (outQueue.empty()) { // No pending transactions
        return;
    }
    
    enableTx(regs);
    regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[0];
}


static void SERCOM_Handler(sercom_registers_t* regs, 
        uart::DefaultQueue& outQueue, 
        uart::DefaultCallback::buffer_type& inBuffer, 
        uart::DefaultCallback::callback_type callback) {
    if (!(regs->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_FERR_Msk)){// Not a framing error
            if (regs->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_TXEN_Msk) { // Transmitter enabled (outgoing transfer)
                --outQueue.front().remaining;
                if (!outQueue.front().remaining) { // Transmitted last byte, turning off transmitter
                    outQueue.pop_front();
                    if (outQueue.empty()) {
                        disableTx(regs);
                    } else {
                        startTransfer(regs, outQueue, true);
                    }
                } else {
                    regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[++outQueue.front().transferred];
                }
            } else { // Transmitter disabled (incoming transfer)
                if (!inBuffer.remaining) { // Received first byte, set up new transfer
                    inBuffer.buffer[0] = regs->USART_INT.SERCOM_DATA;
                    inBuffer.remaining = (inBuffer.buffer[0] >> 4u) - 1;
                    inBuffer.transferred = 1;
                } else { // Continued transfer   
                    inBuffer.buffer[inBuffer.transferred++] = regs->USART_INT.SERCOM_DATA;
                    --inBuffer.remaining;
                    if (!inBuffer.remaining && callback) { // Received last byte, ready to process
                        callback(inBuffer);
                    }
                }
            }
        }
        (void)regs->USART_INT.SERCOM_DATA; // Clear the RXC interrupt flag
        regs->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

extern "C" {
    void SERCOM3_Handler() {
        SERCOM_Handler(SERCOM3_REGS, outQueue, inBuffer, callback);
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

	// SERCOM config
    initSERCOM(SERCOM3_REGS);
    NVIC_EnableIRQ(SERCOM3_IRQn);
}

uint8_t uart::print(const char* buf) {
    if (outQueue.full()) {
        return 0;
    }
    uint8_t len {0};
    for (; buf[len] && len < 32; ++len); 
    
    outQueue.push_back({{}, 0, len});
    util::copy(outQueue.back().buffer, reinterpret_cast<const uint8_t*>(buf), len);
    startTransfer(SERCOM3_REGS, outQueue);
    return len;
}

void uart::send(const uint8_t* buf, uint8_t len) {
    if (outQueue.full()) {
        return;
    }
    
    outQueue.push_back({{}, 0, len});
    util::copy(outQueue.back().buffer, buf, len);
    startTransfer(SERCOM3_REGS, outQueue);
}

void uart::setCallback(uart::DefaultCallback::callback_type cb) {
    callback = cb;
}
