/* 
 * File:   data.hpp
 * Author: Mikhail
 *
 * Created on June 22, 2023, 17:58 PM
 */

#ifndef DATA_HPP
#define	DATA_HPP

#include "device.h"

#include "lib/inc/Matrix.hpp"
#include "lib/inc/util.hpp"


namespace data {
    constexpr uint8_t inputChannelCount{2};
    constexpr uint8_t outputChannelCount{2};
    constexpr uint8_t muxLength {inputChannelCount * outputChannelCount};
    
    constexpr uint8_t NVM_MUX_PAGES {(inputChannelCount * outputChannelCount * sizeof(int16_t) + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE};
    constexpr uint8_t NVM_TRIM_PAGES {outputChannelCount * (sizeof(uint16_t) + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE};
    constexpr uint8_t NVM_PAGE_COUNT {NVM_MUX_PAGES + NVM_TRIM_PAGES};

    enum class DATA_REQUEST : uint8_t {
        READ = 0x00,
        WRITE = 0x01,
    };

    enum class DATA_DESCRIPTOR_TYPE : uint8_t {
        STATUS = 0x00,
        SETTINGS = 0x01,
        INPUTS = 0x02,
        MUX = 0x03,
        TRIMS = 0x04,
        OUTPUTS = 0x05
    };

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; // 0x00
        int8_t bTemp;
        uint8_t bReserved;
        int16_t wAcc[3];
        int16_t wRot[3];
        int16_t wRoll;
        int16_t wPitch;
    } usb_data_status_descriptor;

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; // 0x01
        uint8_t bInputChannels;
        uint8_t bOutputChannels;
        uint8_t bmActiveSensors;
    } usb_data_settings_descriptor;

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; // 0x02
        int16_t wInputs[inputChannelCount];
    } usb_data_inputs_descriptor;

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; // 0x03
        int16_t wMux[muxLength];
    } usb_data_mux_descriptor;

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; // 0x04
        int16_t wTrims[outputChannelCount];
    } usb_data_trims_descriptor;

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; //0x05
        int16_t wOutputs[outputChannelCount];
    } usb_data_outputs_descriptor;
    
    extern const uint8_t NVM_DATA[FLASH_PAGE_SIZE * 4];

    extern usb_data_status_descriptor STATUS_DESCRIPTOR;
    extern usb_data_settings_descriptor SETTINGS_DESCRIPTOR;
    extern usb_data_inputs_descriptor INPUTS_DESCRIPTOR;
    extern usb_data_mux_descriptor MUX_DESCRIPTOR;
    extern usb_data_trims_descriptor TRIMS_DESCRIPTOR;
    extern usb_data_outputs_descriptor OUTPUTS_DESCRIPTOR;

    extern uint8_t& activeSensors;
    extern Matrix<int16_t> inputs;
    extern Matrix<int16_t> mux;
    extern Matrix<int16_t> trims;
    extern Matrix<int16_t> outputs;
    
    void load();
    void save();
}

#endif	/* DATA_HPP */
