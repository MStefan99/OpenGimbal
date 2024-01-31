#include "lib/inc/i2c.hpp"

#define SERCOM_REGS SERCOM0_REGS


void i2c::init() { // GCLK config
	GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_CORE] = GCLK_PCHCTRL_CHEN(1) // Enable SERCOM0 clock
					| GCLK_PCHCTRL_GEN_GCLK0; //Set GCLK0 as a clock source

	// PORT config
	PORT_REGS->GROUP[0].PORT_PINCFG[4] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 4
	PORT_REGS->GROUP[0].PORT_PINCFG[5] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 5
	PORT_REGS->GROUP[0].PORT_PMUX[2] = PORT_PMUX_PMUXE(MUX_PA04D_SERCOM0_PAD0) // Mux pin 4 to SERCOM0
					| PORT_PMUX_PMUXO(MUX_PA08C_SERCOM0_PAD0); // Mux pin 5 to SERCOM0

	// DMA config
	dma::initI2C();
	SERCOM_REGS->I2CM.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN(1);

	// SERCOM config
	SERCOM_REGS->I2CM.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(9) | SERCOM_I2CM_BAUD_BAUDLOW(19); // 1MHz
	SERCOM_REGS->I2CM.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_LOWTOUTEN(1)
					| SERCOM_I2CM_CTRLA_SPEED_FASTPLUS_MODE
					| SERCOM_I2CM_CTRLA_PINOUT(0)
					| SERCOM_I2CM_CTRLA_MODE_I2C_MASTER
					| SERCOM_I2CM_CTRLA_ENABLE(1);
	SERCOM_REGS->I2CM.SERCOM_INTENSET = SERCOM_I2CM_INTENSET_ERROR(1);
	NVIC_EnableIRQ(SERCOM2_IRQn);

	SERCOM_REGS->I2CM.SERCOM_STATUS |= SERCOM_I2CM_STATUS_BUSSTATE(1);
	while (!(SERCOM_REGS->I2CM.SERCOM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk));
}

void i2c::write(uint8_t devAddr, uint8_t* buf, uint8_t size, void (*cb)(bool, const dma::I2CTransfer&), bool littleEndian) {
	auto transfer {dma::I2CTransfer{
		.devAddr = devAddr,
		.len = size,
		.type = dma::I2CTransferType::Write,
        .cb = cb,
		.littleEndian = littleEndian
	}};
    
	util::copy(transfer.buf, buf, size);
	dma::startTransfer(transfer);
}

void i2c::read(uint8_t devAddr, uint8_t size, void (*cb)(bool, const dma::I2CTransfer&), bool littleEndian) {
    auto transfer {dma::I2CTransfer{
		.devAddr = devAddr,
		.len = size,
		.type = dma::I2CTransferType::Read,
        .cb = cb,
		.littleEndian = littleEndian
	}}; 
    
	dma::startTransfer(transfer);
}

void i2c::writeRegister(uint8_t devAddr, uint8_t regAddr, uint8_t* buf, uint8_t size, void (*cb)(bool, const dma::I2CTransfer&), bool littleEndian) {
	auto transfer {dma::I2CTransfer{
		.devAddr = devAddr,
		.regAddr = regAddr,
		.len = (uint8_t) (size + 1),
		.type = dma::I2CTransferType::Write,
        .cb = cb,
		.littleEndian = littleEndian
	}};

    transfer.buf[0] = regAddr;
	util::copy(transfer.buf + 1, buf, size);
	dma::startTransfer(transfer);
}

void i2c::readRegister(uint8_t devAddr, uint8_t regAddr, uint8_t size, void (*cb)(bool, const dma::I2CTransfer&), bool littleEndian) {
    auto transfer {dma::I2CTransfer{
		.devAddr = devAddr,
		.regAddr = regAddr,
		.len = size,
		.type = dma::I2CTransferType::WriteRead,
        .cb = cb,
		.littleEndian = littleEndian
	}};
    
	dma::startTransfer(transfer);
}
