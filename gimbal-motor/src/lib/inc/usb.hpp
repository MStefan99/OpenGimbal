/* 
 * File:   usb.h
 * Author: mikha
 *
 * Created on October 31, 2021, 1:27 PM
 */

#ifndef USB_HPP
#define	USB_HPP

#include "device.h"
#include <cstring>

#include "data.hpp"

namespace usb {

    enum class REQ_DIRECTION : uint8_t {
        HOST_TO_DEVICE = 0x00,
        DEVICE_TO_HOST = 0x01
    };

    enum class REQ_TYPE : uint8_t {
        STANDARD = 0x00,
        CLASS = 0x01,
        VENDOR = 0x02
    };

    enum class REQ_RECIPIENT : uint8_t {
        DEVICE = 0x00,
        INTERFACE = 0x01,
        ENDPOINT = 0x02,
        OTHER = 0x03
    };

    enum class DEVICE_REQ : uint8_t {
        GET_STATUS = 0x00,
        CLEAR_FEATURE = 0x01,
        SET_FEATURE = 0x03,
        SET_ADDRESS = 0x05,
        GET_DESCRIPTOR = 0x06,
        SET_DESCRIPTOR = 0x07,
        GET_CONFIGURATION = 0x08,
        SET_CONFIGURATION = 0x09
    };

    enum class INTERFACE_REQ : uint8_t {
        GET_STATUS = 0x00,
        CLEAR_FEATURE = 0x01,
        SET_FEATURE = 0x03,
        GET_INTERFACE = 0x0A,
        SET_INTERFACE = 0x11
    };

    enum class ENDPOINT_REQ : uint8_t {
        GET_STATUS = 0x00,
        CLEAR_FEATURE = 0x01,
        SET_FEATURE = 0x03,
        SYNCH_FRAME = 0x12
    };

    enum class DESCRIPTOR_TYPE : uint8_t {
        DEVICE = 0x01,
        CONFIGURATION = 0x02,
        STRING = 0x03,
        INTERFACE = 0x04,
        ENDPOINT = 0x05,
        DEVICE_QUALIFIER = 0x06,
        BOS = 0x0F,
        DEVICE_CAPABILITY = 0x10
    };

    typedef struct __attribute__((packed)) {
        uint8_t bLength; // 18
        uint8_t bDescriptorType; // 0x01
        uint16_t bcdUSB; // 0x0210
        uint8_t bDeviceClass;
        uint8_t bDeviceSubclass;
        uint8_t bDeviceProtocol;
        uint8_t bMaxPacketSize;
        uint16_t idVendor;
        uint16_t idProduct;
        uint16_t bcdDevice;
        uint8_t iManufacturer;
        uint8_t iProduct;
        uint8_t iSerialNumber;
        uint8_t bNumConfigurations;
    } usb_descriptor_device;

    typedef struct __attribute__((packed)) {
        uint8_t bLength; // 7
        uint8_t bDescriptorType; // 0x05
        uint8_t bEndpointAddress;
        uint8_t bmAttributes;
        uint16_t wMaxPacketSize;
        uint8_t bInterval;
    }
    usb_descriptor_endpoint;

    typedef struct __attribute__((packed)) {
        uint8_t bLength; // 9
        uint8_t bDescriptorType; // 0x04
        uint8_t bInterfaceNumber;
        uint8_t bAlternateSetting;
        uint8_t bNumEndpoints;
        uint8_t bInterfaceClass;
        uint8_t bInterfaceSubclass;
        uint8_t bInterfaceProtocol;
        uint8_t iInterface;
        usb_descriptor_endpoint ENDPOINTS[2]; // TODO: make dynamic size
    }
    usb_descriptor_interface;

    typedef struct __attribute__((packed)) {
        uint8_t bLength; // 9
        uint8_t bDescritptorType; // 0x02
        uint16_t wTotalLength;
        uint8_t bNumInterfaces;
        uint8_t bConfigurationValue;
        uint8_t iConfiguration;
        uint8_t bmAttributes;
        uint8_t bMaxPower;
        usb_descriptor_interface INTERFACES[1]; // TODO: make dynamic size
    }
    usb_descriptor_configuration;

    typedef struct __attribute__((packed)) {
        uint8_t bLength;
        uint8_t bDescriptorType; // 0x03
        const char16_t bString[18];
    }
    usb_descriptor_string;

    typedef struct __attribute__((packed)) {
        uint8_t bLength; // 20
        uint8_t bDescriptorType;
        uint8_t bDevCapabilityType;
        uint8_t bReserved;
        uint8_t PlatformCapabilityUUID[16];
        uint32_t dwWindowsVersion;
        uint16_t wMSOSDescriptorSetTotalLength;
        uint8_t bMS_VendorCode;
        uint8_t bAltEnumCode;
    }
    usb_dev_capability;

    typedef struct __attribute__((packed)) {
        uint8_t bLength; // 5
        uint8_t bDescriptorType;
        uint16_t wTotalLength;
        uint8_t bNumDeviceCaps;
        usb_dev_capability dev_capability;
    }
    usb_descriptor_bos;

    typedef struct __attribute__((packed)) {
        uint16_t wLength; // 20
        uint16_t wDescriptorType; // 0x03 for MS_OS_20_FEATURE_COMPATBLE_ID
        uint8_t wCompatibleID[8];
        uint8_t wSubCompatibleID[8];
    }
    usb_descriptor_compatible_id;

    typedef struct __attribute__((packed)) {
        uint16_t wLength;
        uint16_t wDescriptorType; // 0x04
        uint16_t wPropertyDataType; // 0x01
        uint16_t wPropertyNameLength;
        const char16_t PropertyName[20];
        uint16_t wPropertyDataLength;
        const char16_t PropertyData[39];
    }
    usb_descriptor_registry_property;

    typedef struct __attribute__((packed)) {
        uint16_t wLength; // 10
        uint16_t wDescriptorType; // 0x00
        uint32_t dwWindowsVersion; // 0x06030000 for Windows 8.1
        uint16_t wTotalLength;
        usb_descriptor_compatible_id compatible_id;
        usb_descriptor_registry_property registry_property;
    }
    usb_descriptor_ms_os_20;

    extern usb_descriptor_device_registers_t EPDESCTBL[];

    extern usb_descriptor_device DESCRIPTOR_DEVICE;
    extern usb_descriptor_configuration DESCRIPTOR_CONFIGURATION[];
    extern usb_descriptor_string DESCRIPTOR_STRING[];
    extern usb_descriptor_bos DESCRIPTOR_BOS;
    extern usb_descriptor_ms_os_20 DESCRIPTOR_MS_OS_20;


    void init();
    void writeDefault(uint8_t* data, uint8_t len);
    void write(uint8_t* data, uint8_t len);
    void read(uint8_t* data, uint8_t len);
}

#endif	/* USB_HPP */
