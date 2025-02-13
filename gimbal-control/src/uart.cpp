#include "uart.hpp"


static uart::DefaultQueue                   motorOutQueue {};
static uart::DefaultCallback::buffer_type   motorInBuffer {};
static uart::DefaultCallback::callback_type motorCallback {nullptr};
static uint32_t                             motorReceiveTime {0};

constexpr static float    baudRate {115200};
constexpr static uint32_t clockFrequency {48000000};

static void initSERCOM(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1)  // Enable receiver
#if !UART_HALF_DUPLEX
	                             | SERCOM_USART_INT_CTRLB_TXEN(1)  // Enable transmitter
#else
	                             | SERCOM_USART_INT_CTRLB_COLDEN(1)  // Enable collision detection
#endif
	                             | SERCOM_USART_INT_CTRLB_PMODE_ODD              // Enable odd parity
	                             | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT           // Use 1 stop bit
	                             | SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT;          // Use 8 data bits
	regs->USART_INT.SERCOM_BAUD = 65536 * (1 - 16 * baudRate / clockFrequency);  // Set baud rate
	regs->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_LSB               // Send LSB first
	                             | SERCOM_USART_INT_CTRLA_CMODE_ASYNC
	                             | SERCOM_USART_INT_CTRLA_SAMPR_16X_ARITHMETIC          // Set clock mode
	                             | SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_WITH_PARITY  // Enable parity
	                             | SERCOM_USART_INT_CTRLA_RXPO_PAD0                     // Set receive pin
	                             | SERCOM_USART_INT_CTRLA_TXPO_PAD0                     // Set transmit pin
	                             | SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK            // Use internal clock
	                             | SERCOM_USART_INT_CTRLA_ENABLE(1);                    // Enable peripheral

	regs->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC(1) | SERCOM_USART_INT_INTENSET_TXC(1);
}

#if UART_HALF_DUPLEX
static void disableTx(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN(1);
	while (regs->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}

static void enableTx(sercom_registers_t* regs) {
	regs->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN(1);
	while (regs->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk);
}
#endif

static void startTransfer(sercom_registers_t* regs, uart::DefaultQueue& outQueue, bool force = false) {
	if (motorOutQueue.size()) {
		if (motorOutQueue.front().remaining) {
			return;  // Another tranfer ongoing
		}
	}

	if (outQueue.empty()) {  // No pending transactions
		return;
	}

#if UART_HALF_DUPLEX
	enableTx(regs);
#endif
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
		if (regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) {  // Transmit complete (outgoing transfer)
			--outQueue.front().remaining;
			if (!outQueue.front().remaining) {  // Transmitted last byte, turning off transmitter
				outQueue.pop_front();
				if (outQueue.empty()) {
#if UART_HALF_DUPLEX
					disableTx(regs);
#endif
				} else {
					startTransfer(regs, outQueue, true);
				}
			} else {
				regs->USART_INT.SERCOM_DATA = outQueue.front().buffer[++outQueue.front().transferred];
			}
		} else if (regs->USART_INT.SERCOM_INTFLAG
		           & SERCOM_USART_INT_INTFLAG_RXC_Msk) {                    // Receive complete (incoming transfer)
			if (!inBuffer.remaining || util::getTime() - prevByteTime > 1) {  // Received first byte, set up new transfer
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
			prevByteTime = util::getTime();
		}
	} else {
		regs->USART_INT.SERCOM_STATUS = SERCOM_USART_INT_STATUS_FERR(1);
	}

	(void)regs->USART_INT.SERCOM_DATA;  // Clear the RXC interrupt flag
	regs->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_Msk;
}

extern "C" {
	void SERCOM3_Handler() {
		SERCOM_Handler(SERCOM3_REGS, motorOutQueue, motorInBuffer, motorCallback, motorReceiveTime);
	}
}

void uart::init() {
	// Clock setup
	GCLK_REGS->GCLK_PCHCTRL[SERCOM1_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1)     // Enable SERCOM clock
	                                              | GCLK_PCHCTRL_GEN_GCLK1;  // Set GCLK as a clock source

	// Pin setup
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 14u | 0x1 << 15u)  // Select pins
	                                  | PORT_WRCONFIG_PMUXEN(1)                         // Enable multiplexing
	                                  | PORT_WRCONFIG_PMUX(MUX_PA30D_SERCOM1_PAD2)      // Multiplex to SERCOM
	                                  | PORT_WRCONFIG_WRPMUX(1)                         // Write pin multiplex settings
	                                  | PORT_WRCONFIG_WRPINCFG(1)                       // Write pin config settings
	                                  | PORT_WRCONFIG_HWSEL(1);                         // Select pin range

	// SERCOM setup
	initSERCOM(SERCOM1_REGS);
	NVIC_EnableIRQ(SERCOM1_IRQn);
}

uint8_t uart::print(const char* buf) {
	if (motorOutQueue.full()) {
		return 0;
	}
	uint8_t len {0};
	for (; buf[len] && len < 32; ++len);

	motorOutQueue.push_back({{}, 0, len});
	util::copy(motorOutQueue.back().buffer, reinterpret_cast<const uint8_t*>(buf), len);
	startTransfer(SERCOM3_REGS, motorOutQueue);
	return len;
}

void uart::sendToMotors(const uint8_t* buf, uint8_t len) {
	if (motorOutQueue.full()) {
		return;
	}

	motorOutQueue.push_back({{}, 0, len});
	util::copy(motorOutQueue.back().buffer, buf, len);
	startTransfer(SERCOM3_REGS, motorOutQueue);
}

void uart::setMotorCallback(uart::DefaultCallback::callback_type cb) {
	motorCallback = cb;
}
