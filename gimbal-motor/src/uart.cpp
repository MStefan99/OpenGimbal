#include "uart.hpp"


static uart::DefaultQueue                   outQueue {};
static uart::DefaultCallback::buffer_type   inBuffer {};
static uart::DefaultCallback::callback_type callback {nullptr};
static uint32_t                             lastReceiveTime {0};

static void initSERCOM(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)                       // Enable receiver
	                             | SERCOM_USART_INT_CTRLB_COLDEN(1)                     // Enable collision detection
	                             | SERCOM_USART_INT_CTRLB_PMODE_ODD                     // Set odd parity mode
	                             | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT                  // Set 1 stop bit
	                             | SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;                 // Set frame to 8 bits
	regs->USART_INT.SERCOM_BAUD = 65536.0f * (1 - 16.0f * 115200.0f / 16000000.0f);     // Set baud rate
	regs->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB                      // Send LSB first
	                             | SERCOM_USART_INT_CTRLA_CMODE_ASYNC                   // Asynchronous smode
	                             | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC          // 16x oversampling
	                             | SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY  // Enable parity
	                             | SERCOM_USART_INT_CTRLA_RXPO_PAD0                     // Receive on pad 0
	                             | SERCOM_USART_INT_CTRLA_TXPO_PAD0                     // Transmit on pad 0
	                             | SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK;           // Use internal clock

	regs->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1)   // Enable receive complete interrupt
	                                | SERCOM_USART_INT_INTENSET_TXC(1);  // Enable transmit complete interrupt
}

static bool busy() {
	if (outQueue.size()) {
		if (outQueue.front().transferred) {  // Another transfer in progress
			return true;
		}
	}

	return false;
}

static void disableTx(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN(1);
	while (regs->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void enableTx(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN(1);
	while (regs->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void startTransfer(sercom_registers_t* regs, uart::DefaultQueue& outQueue) {
	if (busy()) {
		return;
	}

	enableTx(regs);
	++outQueue.front().transferred;
	regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[0];
}

static void SERCOM_Handler(
    sercom_registers_t*                  regs,
    uart::DefaultQueue&                  outQueue,
    uart::DefaultCallback::buffer_type&  inBuffer,
    uart::DefaultCallback::callback_type callback,
    uint32_t&                            prevByteTime
) {
	if (!(regs->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_FERR_Msk)) {  // Not a framing error
		if (regs->USART_INT.SERCOM_CTRLB & SERCOM_USART_INT_CTRLB_TXEN_Msk) {     // Outgoing transfer
			--outQueue.front().remaining;
			if (!outQueue.front().remaining) {  // Transmitted last byte, turning off transmitter
				if (outQueue.front().callback) {
					outQueue.front().callback();
				}
				outQueue.pop_front();
				if (outQueue.empty()) {
					disableTx(regs);
				} else {
					startTransfer(regs, outQueue);
				}
			} else {
				regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[outQueue.front().transferred++];
			}
		} else {  // Incoming transfer
			if (!inBuffer.remaining
			    || RTC_REGS->MODE0.RTC_COUNT - prevByteTime > 64) {  // Received first byte, set up new transfer
				inBuffer.buffer[0] = regs->USART_INT.SERCOM_DATA;
				inBuffer.remaining = (inBuffer.buffer[0] >> 4u);
				inBuffer.transferred = 1;
			} else {  // Continued transfer
				inBuffer.buffer[inBuffer.transferred++] = regs->USART_INT.SERCOM_DATA;
				--inBuffer.remaining;
			}
			if (!inBuffer.remaining && callback) {  // Received last byte, ready to process
				callback(inBuffer);
			}
			prevByteTime = RTC_REGS->MODE0.RTC_COUNT;
		}
	} else {
		regs->USART_INT.SERCOM_STATUS = SERCOM_USART_INT_STATUS_FERR(1);
	}

	(void)regs->USART_INT.SERCOM_DATA;  // Clear the RXC interrupt flag
	regs->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

extern "C" {
	void SERCOM3_Handler() {
		SERCOM_Handler(SERCOM3_REGS, outQueue, inBuffer, callback, lastReceiveTime);
	}
}

void uart::init() {
	GCLK_REGS->GCLK_PCHCTRL[SERCOM3_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1)     // Enable SERCOM3 clock
	                                              | GCLK_PCHCTRL_GEN_GCLK2;  // Set GCLK2 as a clock source

	// SERCOM config
	initSERCOM(SERCOM3_REGS);
	NVIC_EnableIRQ(SERCOM3_IRQn);
}

void uart::enable() {
	SERCOM3_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE(1);  // Enable USART

	PORT_REGS->GROUP[0].PORT_PINCFG[22] = PORT_PINCFG_PMUXEN(1);                  // Enable mux on pin 22
	PORT_REGS->GROUP[0].PORT_PINCFG[23] = PORT_PINCFG_PMUXEN(1);                  // Enable mux on pin 23
	PORT_REGS->GROUP[0].PORT_PMUX[11] = PORT_PMUX_PMUXE(MUX_PA22C_SERCOM3_PAD0)   // Mux pin 22 to SERCOM3
	                                  | PORT_PMUX_PMUXO(MUX_PA23C_SERCOM3_PAD1);  // Mux pin 23 to SERCOM3
}

void uart::disable() {
	SERCOM3_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE(1);  // Enable USART
}

uint8_t uart::print(const char* buf) {
	if (outQueue.full()) {
		return 0;
	}
	uint8_t len {0};
	for (; buf[len] && len < 32; ++len);

	__disable_irq();
	outQueue.push_back({{}, 0, len});
	util::copy(outQueue.back().buffer, reinterpret_cast<const uint8_t*>(buf), len);
	__enable_irq();
	startTransfer(SERCOM3_REGS, outQueue);
	return len;
}

void uart::send(const uint8_t* buf, uint8_t len, void (*cb)()) {
	if (outQueue.full()) {
		return;
	}

	__disable_irq();
	outQueue.push_back({{}, 0, len, cb});
	util::copy(outQueue.back().buffer, buf, len);
	__enable_irq();
	startTransfer(SERCOM3_REGS, outQueue);
}

void uart::setCallback(uart::DefaultCallback::callback_type cb) {
	callback = cb;
}
