#include "host.hpp"


static host::DefaultQueue                   outQueue {};
static host::DefaultCallback::buffer_type   inBuffer {};
static host::DefaultCallback::callback_type callback {nullptr};
static uint32_t                             lastReceiveTime {0};

static void initSERCOM(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)                       // Enable receiver
	                             | SERCOM_USART_INT_CTRLB_TXEN(1)                       // Enable receiver
	                             | SERCOM_USART_INT_CTRLB_PMODE_ODD                     // Set odd parity mode
	                             | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT                  // Set 1 stop bit
	                             | SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;                 // Set frame to 8 bits
	regs->USART_INT.SERCOM_BAUD = 65536.0f * (1 - 16.0f * 115200.0f / 48000000.0f);     // Set baud rate
	regs->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB                      // Send LSB first
	                             | SERCOM_USART_INT_CTRLA_CMODE_ASYNC                   // Asynchronous smode
	                             | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC          // 16x oversampling
	                             | SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY  // Enable parity
	                             | SERCOM_USART_INT_CTRLA_RXPO_PAD3                     // Receive on pad 3
	                             | SERCOM_USART_INT_CTRLA_TXPO_PAD1                     // Transmit on pad 2
	                             | SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK            // Use internal clock
	                             | SERCOM_USART_INT_CTRLA_ENABLE(1);                    // Enable USART

	regs->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1)   // Enable receive complete interrupt
	                                | SERCOM_USART_INT_INTENSET_TXC(1);  // Enable transmit complete interrupt
}

bool host::busy() {
	if (outQueue.size()) {
		if (outQueue.front().transferred) {  // Another transfer in progress
			return true;
		}
	}

	return false;
}

static void startTransfer(sercom_registers_t* regs, host::DefaultQueue& outQueue) {
	if (host::busy()) {
		return;
	}

	regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[0];
	++outQueue.front().transferred;
}

static void SERCOM_Handler(
    sercom_registers_t*                  regs,
    host::DefaultQueue&                  outQueue,
    host::DefaultCallback::buffer_type&  inBuffer,
    host::DefaultCallback::callback_type callback,
    uint32_t&                            prevByteTime
) {
	if (!(regs->USART_INT.SERCOM_STATUS & SERCOM_USART_INT_STATUS_FERR_Msk)) {  // Not a framing error
		if (regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) {  // Outgoing transfer
			if (!outQueue.size()) {
				(void)regs->USART_INT.SERCOM_DATA;
				regs->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
				return;
			}

			--outQueue.front().remaining;
			if (!outQueue.front().remaining) {  // Transmitted last byte, turning off transmitter
				if (outQueue.front().callback) {
					outQueue.front().callback();
				}
				outQueue.pop_front();
				if (outQueue.size()) {
					startTransfer(regs, outQueue);
				}
			} else {
				regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[outQueue.front().transferred++];
			}
		}
		if (regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) {  // Incoming transfer
			if (!inBuffer.remaining || util::getTime() - prevByteTime > 3) {  // Received first byte, set up new transfer
				inBuffer.buffer[0] = regs->USART_INT.SERCOM_DATA;
				inBuffer.remaining = inBuffer.buffer[0];
				inBuffer.transferred = 1;
			} else {  // Continued transfer
				inBuffer.buffer[inBuffer.transferred++] = regs->USART_INT.SERCOM_DATA;
				--inBuffer.remaining;
			}
			if (!inBuffer.remaining && callback) {  // Received last byte, ready to process
				callback(inBuffer);
			}
			prevByteTime = util::getTime();
		}
	} else {
		regs->USART_INT.SERCOM_STATUS = SERCOM_USART_INT_STATUS_FERR(1);
	}

	(void)regs->USART_INT.SERCOM_DATA;  // Clear the RXC interrupt flag
	regs->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

extern "C" {
	void SERCOM1_Handler() {
		SERCOM_Handler(SERCOM1_REGS, outQueue, inBuffer, callback, lastReceiveTime);
	}
}

void host::init() {
	// Clock setup
	GCLK_REGS->GCLK_PCHCTRL[SERCOM1_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1)     // Enable SERCOM3 clock
	                                              | GCLK_PCHCTRL_GEN_GCLK1;  // Set GCLK1 as a clock source

	// Pin setup
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x3 << 14u)           // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                     // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA30D_SERCOM1_PAD2)  // Multiplex to SERCOM
	                                  | PORT_WRCONFIG_WRPMUX(1)                     // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1)                   // Write pin config settings
	                                  | PORT_WRCONFIG_HWSEL(1);                     // Select pin range

	// SERCOM setup
	initSERCOM(SERCOM1_REGS);
	NVIC_EnableIRQ(SERCOM1_IRQn);
}

void host::enable() {
	SERCOM1_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE(1);  // Enable USART
}

void host::disable() {
	SERCOM1_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE(1);  // Enable USART
}

void host::send(const uint8_t* buf, const uint8_t len, void (*cb)()) {
	if (outQueue.full()) {
		return;
	}

	outQueue.push_back({{}, 0, len, cb});
	util::copy(outQueue.back().buffer, buf, len);
	startTransfer(SERCOM1_REGS, outQueue);
}

void host::setCallback(host::DefaultCallback::callback_type cb) {
	callback = cb;
}
