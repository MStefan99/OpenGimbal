#include "lib/inc/data.hpp"

const uint8_t data::NVM_DATA[FLASH_PAGE_SIZE * 4] __attribute__((aligned(FLASH_PAGE_SIZE * 4),keep,space(prog))) = {0};
const uint8_t& data::polePairs {data::NVM_DATA[0]};

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

void data::write(const uint8_t& addr, uint8_t& buf, uint8_t len) {
    uint8_t nvmCopy[sizeof data::NVM_DATA] {};
    
    util::copy(nvmCopy, data::NVM_DATA, sizeof data::NVM_DATA);
    util::copy(nvmCopy + (&addr - data::NVM_DATA), &buf, len);
    
    nvmRowErase(NVM_DATA);
    
    for (uint8_t i {0}; i < 4; ++i) {
        uint16_t offset = i * FLASH_PAGE_SIZE;
        util::copy((uint32_t*)(data::NVM_DATA + offset), (uint32_t*)(nvmCopy + offset), FLASH_PAGE_SIZE / 4);
        nvmPageWrite(NVM_DATA + offset);
    }
}
