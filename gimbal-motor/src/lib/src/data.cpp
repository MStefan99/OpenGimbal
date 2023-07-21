#include "lib/inc/data.hpp"

const uint8_t data::NVM_DATA[FLASH_PAGE_SIZE * 4] __attribute__((aligned(FLASH_PAGE_SIZE * 4),keep,space(prog))) = {0};

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

data::usb_data_status_descriptor data::STATUS_DESCRIPTOR = {
    .bLength = sizeof (data::usb_data_status_descriptor),
    .bDescriptorType = static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::STATUS)
};

data::usb_data_settings_descriptor data::SETTINGS_DESCRIPTOR = {
    .bLength = sizeof (data::usb_data_settings_descriptor),
    .bDescriptorType = static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::SETTINGS),
    .bInputChannels = data::inputChannelCount,
    .bOutputChannels = data::outputChannelCount,
    .bmActiveSensors = 0
};

data::usb_data_inputs_descriptor data::INPUTS_DESCRIPTOR = {
    .bLength = sizeof (data::usb_data_inputs_descriptor),
    .bDescriptorType = static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::INPUTS),
    .wInputs =
    {}
};

data::usb_data_mux_descriptor data::MUX_DESCRIPTOR = {
    .bLength = sizeof (data::usb_data_mux_descriptor),
    .bDescriptorType = static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::MUX),
    .wMux =
    {}
};

data::usb_data_trims_descriptor data::TRIMS_DESCRIPTOR = {
    .bLength = sizeof (data::usb_data_trims_descriptor),
    .bDescriptorType = static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::TRIMS),
    .wTrims =
    {}
};

data::usb_data_outputs_descriptor data::OUTPUTS_DESCRIPTOR = {
    .bLength = sizeof (data::usb_data_outputs_descriptor),
    .bDescriptorType = static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::OUTPUTS),
    .wOutputs =
    {}
};

uint8_t& data::activeSensors{data::SETTINGS_DESCRIPTOR.bmActiveSensors};

Matrix<int16_t> data::inputs{data::inputChannelCount, 1, data::INPUTS_DESCRIPTOR.wInputs};
Matrix<int16_t> data::mux{data::outputChannelCount, data::inputChannelCount, MUX_DESCRIPTOR.wMux};
Matrix<int16_t> data::trims{1, data::outputChannelCount, data::TRIMS_DESCRIPTOR.wTrims};
Matrix<int16_t> data::outputs{1, data::outputChannelCount, data::OUTPUTS_DESCRIPTOR.wOutputs};

void data::load() {
    nvmWaitUntilReady();
    
    util::copy(MUX_DESCRIPTOR.wMux, reinterpret_cast<const int16_t*>(NVM_DATA), muxLength);
    util::copy(TRIMS_DESCRIPTOR.wTrims, reinterpret_cast<const int16_t*>(NVM_DATA + sizeof(MUX_DESCRIPTOR.wMux)), outputChannelCount);
}

void data::save() {
    nvmRowErase(NVM_DATA);
    
    util::copy((int16_t*)NVM_DATA, MUX_DESCRIPTOR.wMux, muxLength);
    util::copy((int16_t*)NVM_DATA + muxLength, TRIMS_DESCRIPTOR.wTrims, outputChannelCount);
    uint8_t len {1 + (sizeof(MUX_DESCRIPTOR.wMux) + sizeof(TRIMS_DESCRIPTOR.wTrims) - 1) / FLASH_PAGE_SIZE};
    
    for (uint8_t i {0}; i < len; ++i) {
        nvmPageWrite(NVM_DATA + i * FLASH_PAGE_SIZE);
    }
}
