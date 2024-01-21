#include "lib/inc/uart.hpp"


#define SERCOM_REGS SERCOM3_REGS


static uart::Buffer<uint8_t, 8> motorInBuffer {};
static uart::Buffer<uint8_t, 16> motorOutBuffer {};

static uart::Buffer<uint8_t, 8> controlInBuffer {};
static uart::Buffer<uint8_t, 8> controlOutBuffer {};

static uart::Callback<uint8_t, 8> motorCb {nullptr};
static uart::Callback<uint8_t, 8> controlCb {nullptr};
        

static void disableTx() {
    SERCOM_REGS->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN(1);
    while (SERCOM_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void enableTx() {
    SERCOM_REGS->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN(1);
    while (SERCOM_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

extern "C" {
    void SERCOM3_Handler() {
        if (!(SERCOM_REGS->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_FERR_Msk) // Not a framing error
                && SERCOM_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) { // Received a byte
            if (SERCOM_REGS->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_TXEN_Msk) { // Transmitter enabled (outgoing transfer)
                SERCOM_REGS->USART_INT.SERCOM_DATA = motorOutBuffer.buffer[motorOutBuffer.transferrred++];
                --motorOutBuffer.remaining;
                if (!motorOutBuffer.remaining) { // Transmitted last byte, turning off transmitter
                    disableTx();
                }
            } else { // Transmitter disabled (incoming transfer)
                if (!motorInBuffer.remaining) { // Received first byte, set up new transfer
                    motorInBuffer.buffer[0] = SERCOM_REGS->USART_INT.SERCOM_DATA;
                    motorInBuffer.remaining = motorInBuffer.buffer[0] >> 4u;
                    motorInBuffer.transferrred = 1;
                } else { // Continued transfer   
                    motorInBuffer.buffer[motorInBuffer.transferrred++] = SERCOM_REGS->USART_INT.SERCOM_DATA;
                    --motorInBuffer.remaining;
                    if (!motorInBuffer.remaining && motorCb) { // Received last byte, ready to process
                        motorCb(motorInBuffer);
                    }
                }
            }
        }
        (void)SERCOM_REGS->USART_INT.SERCOM_DATA; // Clear the RXC interrupt flag
        SERCOM_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
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
    
	SERCOM_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1)
            | SERCOM_USART_INT_INTENSET_TXC(1);
    NVIC_EnableIRQ(SERCOM3_IRQn);
}


void uart::sendToMotors(uint8_t* buf, uint8_t len) {
    util::copy(motorOutBuffer.buffer, buf, len);
    motorOutBuffer.remaining = len;
    motorOutBuffer.transferrred = 0;
    enableTx();
    SERCOM_REGS->USART_INT.SERCOM_DATA = motorOutBuffer.buffer[0];
}

void uart::setMotorCallback(uart::Callback<uint8_t, 8> cb) {
    motorCb = cb;
}
