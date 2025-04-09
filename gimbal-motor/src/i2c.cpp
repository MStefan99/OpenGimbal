#include "i2c.hpp"

#define SERCOM_REGS SERCOM1_REGS

static bool busy {false};

static RingBuffer<i2c::Transfer, uint8_t, 8> pendingTransfers {};


static void startTransfer(const i2c::Transfer& transfer);
static void nextTransfer();
static void completeTransfer(bool success);


extern "C" {
	void SERCOM1_Handler() {
		auto& transfer {pendingTransfers.front()};

		if (SERCOM_REGS->I2CM.SERCOM_INTFLAG & SERCOM_I2CM_INTFLAG_ERROR_Msk
		    || (SERCOM_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_RXNACK_Msk)) {  // Error
			SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_CMD(3);
			completeTransfer(false);
			nextTransfer();
		} else if (SERCOM_REGS->I2CM.SERCOM_INTFLAG & SERCOM_I2CM_INTFLAG_MB_Msk) {  // Master on bus
			if (!transfer.flags.read) {
				if (transfer.transferred < transfer.length) {
					SERCOM_REGS->I2CM.SERCOM_DATA = SERCOM_I2CM_DATA_DATA(transfer.buf[transfer.transferred++]
					);  // Send bytes (1st - register address, then data)
				} else {
					SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_CMD(3);
					completeTransfer(true);
					nextTransfer();
				}
			} else {
				if (!transfer.flags.regAddrWritten) {
					SERCOM_REGS->I2CM.SERCOM_DATA = SERCOM_I2CM_DATA_DATA(transfer.buf[0]);  // Send register address
					transfer.flags.regAddrWritten = true;
				} else {
					SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_ACKACT(0);  // Send ACK after first received byte
					SERCOM_REGS->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR(transfer.devAddr << 1u | 0x1);  // Repeated start
				}
			}
		} else {  // Slave on bus
			transfer.buf[transfer.transferred++] = SERCOM_REGS->I2CM.SERCOM_DATA;

			uint8_t remaining = transfer.length - transfer.transferred;
			if (remaining > 1) {
				SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_ACKACT(0)  // Send ACK after next byte
				                               | SERCOM_I2CM_CTRLB_CMD(0x2);  // Read next byte
			} else if (remaining == 1) {
				SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_ACKACT(1)  // Send NACK after last byte
				                               | SERCOM_I2CM_CTRLB_CMD(0x2);  // Read next byte
			} else {
				SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_CMD(0x3);  // Stop
				completeTransfer(true);
				nextTransfer();
			}
		}

		SERCOM_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;
	}
}

void i2c::init() {
	// GCLK config
	GCLK_REGS->GCLK_PCHCTRL[SERCOM1_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1)     // Enable SERCOM1 clock
	                                              | GCLK_PCHCTRL_GEN_GCLK1;  // Set GCLK1 as a clock source

	// PORT config
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 | (0x1 << 1u)) | PORT_WRCONFIG_PMUXEN(1)
	                                  | PORT_WRCONFIG_PMUX(MUX_PA16C_SERCOM1_PAD0) | PORT_WRCONFIG_WRPMUX(1)
	                                  | PORT_WRCONFIG_WRPINCFG(1) | PORT_WRCONFIG_HWSEL(1);

	// SERCOM config
	SERCOM_REGS->I2CM.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(30)      // 1.3us
	                              | SERCOM_I2CM_BAUD_BAUDLOW(72);  // 0.6us, 400kHz
	SERCOM_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_SPEED_STANDARD_AND_FAST_MODE | SERCOM_I2CM_CTRLA_PINOUT(0)
	                               | SERCOM_I2CM_CTRLA_MODE_I2C_MASTER | SERCOM_I2CM_CTRLA_SCLSM(1)
	                               | SERCOM_I2CM_CTRLA_ENABLE(1);
	SERCOM_REGS->I2CM.SERCOM_INTENSET =
	    SERCOM_I2CM_INTENSET_MB(1) | SERCOM_I2CM_INTENSET_SB(1) | SERCOM_I2CM_INTENSET_ERROR(1);
	NVIC_EnableIRQ(SERCOM1_IRQn);

	SERCOM_REGS->I2CM.SERCOM_STATUS |= SERCOM_I2CM_STATUS_BUSSTATE(1);
	while (!(SERCOM_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk));
}

void i2c::write(uint8_t devAddr, uint8_t regAddr, uint8_t* buf, uint8_t size, void (*cb)(bool, const i2c::Transfer&)) {
	Transfer transfer {.devAddr = devAddr, .length = (uint8_t)(size + 1), .flags = {.read = false}, .cb = cb};

	transfer.buf[0] = regAddr;  // Write register address into first buffer byte
	util::copy(transfer.buf + 1, buf, util::min(static_cast<unsigned int>(size), sizeof(transfer.buf - 1)));
	startTransfer(transfer);
}

void i2c::read(uint8_t devAddr, uint8_t regAddr, uint8_t size, void (*cb)(bool, const i2c::Transfer&)) {
	Transfer transfer {.devAddr = devAddr, .length = size, .flags = {.read = true}, .cb = cb};

	transfer.buf[0] = regAddr;  // Write register address into first buffer byte
	startTransfer(transfer);
}

void startTransfer(const i2c::Transfer& transfer) {
	__disable_irq();
	pendingTransfers.push_back(transfer);
	__enable_irq();
	nextTransfer();
}

static void nextTransfer() {
	if (pendingTransfers.empty()) {
		return;
	}

	if (busy) {
		return;
	}

	busy = true;
	SERCOM_REGS->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR(pendingTransfers.front().devAddr << 1u);
}

static void completeTransfer(bool success) {
	busy = false;

	if (pendingTransfers.empty()) {
		return;
	}

	auto transfer {pendingTransfers.front()};
	if (transfer.cb) {
		transfer.cb(success, transfer);
	}

	pendingTransfers.pop_front();
}
