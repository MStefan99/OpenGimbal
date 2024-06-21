#include "dma.hpp"

#define I2C_REGS  SERCOM2_REGS
#define UART_REGS SERCOM3_REGS

static dmac_descriptor_registers_t __attribute__((section(".lpram"))) DESCRIPTOR_TABLE[DMA_CH_COUNT];
static dmac_descriptor_registers_t __attribute__((section(".lpram"))) WRITE_BACK_DESCRIPTOR_TABLE[DMA_CH_COUNT];


static RingBuffer<dma::I2CTransfer, uint8_t, 3> pendingI2CTransfers {};


static void nextTransfer();
static void nextI2CTransfer();


static void I2CStreamOut(const dma::I2CTransfer& transfer);
static void I2CStreamIn(const dma::I2CTransfer& transfer);


static void completeI2CTransfer(bool success);


// Interrupt handlers
extern "C" {
	void DMAC_Handler() {
		while (DMAC_REGS->DMAC_INTSTATUS) {
			DMAC_REGS->DMAC_CHID = DMAC_REGS->DMAC_INTPEND & DMAC_INTPEND_ID_Msk;
			DMAC_REGS->DMAC_CHINTFLAG = DMAC_CHINTFLAG_Msk;

			switch (DMAC_REGS->DMAC_CHID) {
				case DMA_CH_I2C_TX:
				case DMA_CH_I2C_RX:
					completeI2CTransfer(true);
					break;
			}
		}

		nextTransfer();
	}

	void SERCOM2_Handler() {
		dma::I2CTransfer& transfer {pendingI2CTransfers.front()};

		if (I2C_REGS->I2CM.SERCOM_INTFLAG & SERCOM_I2CM_INTFLAG_ERROR_Msk
		    || (I2C_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_RXNACK_Msk)) {
			I2C_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_CMD(3);
			DMAC_REGS->DMAC_CHID = DMA_CH_I2C_TX;
			DMAC_REGS->DMAC_CHCTRLA = DMAC_CHCTRLA_ENABLE(0);
			DMAC_REGS->DMAC_CHID = DMA_CH_I2C_RX;
			DMAC_REGS->DMAC_CHCTRLA = DMAC_CHCTRLA_ENABLE(0);
			completeI2CTransfer(false);
			nextTransfer();
		} else {
			static bool regAddrWritten {false};

			if (!regAddrWritten) {
				I2C_REGS->I2CM.SERCOM_DATA = transfer.regAddr;
				regAddrWritten = true;
			} else {
				I2CStreamIn(transfer);
				I2C_REGS->I2CM.SERCOM_INTENCLR = SERCOM_I2CM_INTENSET_MB(1);
				regAddrWritten = false;
			}
		}

		I2C_REGS->I2CM.SERCOM_INTFLAG = SERCOM_I2CM_INTFLAG_Msk;
	}
}

// Initialization

void dma::init() {
	DMAC_REGS->DMAC_BASEADDR = (uint32_t)DESCRIPTOR_TABLE;
	DMAC_REGS->DMAC_WRBADDR = (uint32_t)WRITE_BACK_DESCRIPTOR_TABLE;
	DMAC_REGS->DMAC_CTRL = DMAC_CTRL_LVLEN0(1) | DMAC_CTRL_DMAENABLE(1);

	// Interrupt setup
	NVIC_EnableIRQ(DMAC_IRQn);
}

void dma::initI2C() {
	// Tx setup
	DMAC_REGS->DMAC_CHID = DMA_CH_I2C_TX;
	DMAC_REGS->DMAC_CHCTRLB = DMAC_CHCTRLB_TRIGACT_BEAT | DMAC_CHCTRLB_TRIGSRC(SERCOM2_DMAC_ID_TX);
	DMAC_REGS->DMAC_CHINTENSET = DMAC_CHINTENSET_TCMPL(1) | DMAC_CHINTENSET_TERR(1);

	DESCRIPTOR_TABLE[DMA_CH_I2C_TX].DMAC_BTCTRL =
	    DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_SRCINC(1) | DMAC_BTCTRL_VALID(1);

	// Rx setup
	DMAC_REGS->DMAC_CHID = DMA_CH_I2C_RX;
	DMAC_REGS->DMAC_CHCTRLB = DMAC_CHCTRLB_TRIGACT_BEAT | DMAC_CHCTRLB_TRIGSRC(SERCOM2_DMAC_ID_RX);
	DMAC_REGS->DMAC_CHINTENSET = DMAC_CHINTENSET_TCMPL(1) | DMAC_CHINTENSET_TERR(1);

	DESCRIPTOR_TABLE[DMA_CH_I2C_RX].DMAC_BTCTRL =
	    DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_DSTINC(1) | DMAC_BTCTRL_VALID(1);
}

static void nextTransfer() {
	if (pendingI2CTransfers.size()) {
		nextI2CTransfer();
	}
}

// I2C Transfers
void dma::startTransfer(const I2CTransfer& transfer) {
	__disable_irq();
	pendingI2CTransfers.push_back(transfer);
	__enable_irq();
	nextTransfer();
}

static void nextI2CTransfer() {
	dma::I2CTransfer& transfer {pendingI2CTransfers.front()};

	if ((I2C_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) != SERCOM_I2CM_STATUS_BUSSTATE(1)) {
		return;  // SERCOM busy, cannot start another transfer
	}

	switch (transfer.type) {
		case dma::I2CTransferType::Read:
			I2CStreamIn(transfer);
			break;
		case dma::I2CTransferType::Write:
			I2CStreamOut(transfer);
			break;
		case dma::I2CTransferType::WriteRead:
			I2C_REGS->I2CM.SERCOM_INTENSET = SERCOM_I2CM_INTENSET_MB(1);
			I2C_REGS->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR(transfer.devAddr << 1u);
			break;
	}
}

static void I2CStreamOut(const dma::I2CTransfer& transfer) {
	DMAC_REGS->DMAC_CHID = DMA_CH_I2C_TX;
	DESCRIPTOR_TABLE[DMA_CH_I2C_TX].DMAC_BTCNT = transfer.len;
	DESCRIPTOR_TABLE[DMA_CH_I2C_TX].DMAC_SRCADDR = reinterpret_cast<uint32_t>(&transfer.buf) + transfer.len;
	DESCRIPTOR_TABLE[DMA_CH_I2C_TX].DMAC_DSTADDR = reinterpret_cast<uint32_t>(&I2C_REGS->I2CM.SERCOM_DATA);
	DMAC_REGS->DMAC_CHCTRLA = DMAC_CHCTRLA_ENABLE(1);

	I2C_REGS->I2CM.SERCOM_ADDR =
	    SERCOM_I2CM_ADDR_ADDR(transfer.devAddr << 1u) | SERCOM_I2CM_ADDR_LEN(transfer.len) | SERCOM_I2CM_ADDR_LENEN(1);
}

static void I2CStreamIn(const dma::I2CTransfer& transfer) {
	DMAC_REGS->DMAC_CHID = DMA_CH_I2C_RX;
	DESCRIPTOR_TABLE[DMA_CH_I2C_RX].DMAC_BTCNT = transfer.len;
	DESCRIPTOR_TABLE[DMA_CH_I2C_RX].DMAC_SRCADDR = reinterpret_cast<uint32_t>(&I2C_REGS->I2CM.SERCOM_DATA);
	DESCRIPTOR_TABLE[DMA_CH_I2C_RX].DMAC_DSTADDR = reinterpret_cast<uint32_t>(transfer.buf) + transfer.len;
	DMAC_REGS->DMAC_CHCTRLA = DMAC_CHCTRLA_ENABLE(1);

	I2C_REGS->I2CM.SERCOM_ADDR = SERCOM_I2CM_ADDR_ADDR(transfer.devAddr << 1u | 0x1) | SERCOM_I2CM_ADDR_LEN(transfer.len)
	                           | SERCOM_I2CM_ADDR_LENEN(1);
}

static void completeI2CTransfer(bool success) {
	if (pendingI2CTransfers.empty()) {
		return;
	}
	dma::I2CTransfer transfer {pendingI2CTransfers.front()};
	if (transfer.cb) {
		transfer.cb(success, transfer);
	}
	pendingI2CTransfers.pop_front();
}
