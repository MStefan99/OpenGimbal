#include "lib/inc/data.hpp"

const uint8_t data::NVM_DATA[FLASH_PAGE_SIZE * 4] __attribute__((aligned(FLASH_PAGE_SIZE * 4),keep,space(prog))) = {0};
const data::Options& data::options {*reinterpret_cast<const data::Options*>(data::NVM_DATA)};
static uint8_t* nvmCopy {nullptr};

static void nvmWaitUntilReady();
static void nvmRowErase(uint32_t address);
static void nvmPageWrite();

static void nvmWaitUntilReady() {
    while ((NVMCTRL_REGS->NVMCTRL_INTFLAG & NVMCTRL_INTFLAG_READY_Msk)!= NVMCTRL_INTFLAG_READY_Msk);
}

static void nvmRowErase(const uint8_t* address) {
    NVMCTRL_REGS->NVMCTRL_ADDR = reinterpret_cast<uint32_t>(address) >> 1u;
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMD_ER_Val | NVMCTRL_CTRLA_CMDEX_KEY;

    nvmWaitUntilReady();
}

static void nvmPageWrite(const uint8_t* address) {
    NVMCTRL_REGS->NVMCTRL_ADDR = reinterpret_cast<uint32_t>(address) >> 1u;
    NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMD_WP_Val | NVMCTRL_CTRLA_CMDEX_KEY;

    nvmWaitUntilReady();
}

void data::edit(const uint8_t& addr, uint8_t& buf, uint8_t len) {
    if (nvmCopy == nullptr) {
        nvmCopy = reinterpret_cast<uint8_t*>(malloc(sizeof (data::NVM_DATA)));
        util::copy(reinterpret_cast<uint32_t*>(nvmCopy), reinterpret_cast<const uint32_t*>(data::NVM_DATA), sizeof data::NVM_DATA / 4);
    }

    util::copy(nvmCopy + (&addr - data::NVM_DATA), &buf, len);
}

void data::write() {
    if (nvmCopy == nullptr) {
        return;
    }

    for (uint8_t row{0}; row < sizeof (data::NVM_DATA) / FLASH_PAGE_SIZE / 4; ++row) {
        uint16_t rowOffset = row * FLASH_PAGE_SIZE * 4;
        nvmRowErase(NVM_DATA + rowOffset);
        
        for (uint8_t i{0}; i < 4; ++i) {
            uint16_t offset = rowOffset + i * FLASH_PAGE_SIZE;
            util::copy((uint32_t*)(data::NVM_DATA + offset), (uint32_t*)(nvmCopy + offset), FLASH_PAGE_SIZE / 4);
            nvmPageWrite(NVM_DATA + offset);
        }
    }

    free(nvmCopy);
}