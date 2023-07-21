#include "lib/inc/usb.hpp"


usb::usb_descriptor_device usb::DESCRIPTOR_DEVICE = {
    .bLength = 18,
    .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::DEVICE,
    .bcdUSB = 0x0201,
    .bDeviceClass = 0x00,
    .bDeviceSubclass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize = 64,
    .idVendor = 0x0424,
    .idProduct = 0x0001,
    .bcdDevice = 0x0002,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 0,
    .bNumConfigurations = 1
};


usb::usb_descriptor_endpoint endpoint1 = {
    .bLength = 7,
    .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::ENDPOINT,
    .bEndpointAddress = 0x01,
    .bmAttributes = 0x3,
    .wMaxPacketSize = 64,
    .bInterval = 200
};


usb::usb_descriptor_endpoint endpoint2 = {
    .bLength = 7,
    .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::ENDPOINT,
    .bEndpointAddress = 0x81,
    .bmAttributes = 0x3,
    .wMaxPacketSize = 64,
    .bInterval = 200
};


usb::usb_descriptor_interface interface0 = {
    .bLength = 9,
    .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::INTERFACE,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = 0xff,
    .bInterfaceSubclass = 0xff,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .ENDPOINTS =
    {endpoint1, endpoint2}
};


usb::usb_descriptor_configuration usb::DESCRIPTOR_CONFIGURATION[] = {
    {
        .bLength = 9,
        .bDescritptorType = (uint8_t) usb::DESCRIPTOR_TYPE::CONFIGURATION,
        .wTotalLength = 32,
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = 0x80,
        .bMaxPower = 75,
        .INTERFACES =
        {interface0}
    }
};


usb::usb_descriptor_string usb::DESCRIPTOR_STRING[] = {
    {
        .bLength = 4,
        .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::STRING,
        .bString =
        {
            0x0409
        }
    },
    {
        20,
        (uint8_t) usb::DESCRIPTOR_TYPE::STRING,
        u"MStefan99"
    },
    {
        36,
        (uint8_t) usb::DESCRIPTOR_TYPE::STRING,
        u"Flight Controller"
    }
};


usb::usb_descriptor_bos usb::DESCRIPTOR_BOS = {
    .bLength = 5,
    .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::BOS,
    .wTotalLength = 33,
    .bNumDeviceCaps = 1,
    .dev_capability =
    {
        .bLength = 28,
        .bDescriptorType = (uint8_t) usb::DESCRIPTOR_TYPE::DEVICE_CAPABILITY,
        .bDevCapabilityType = 0x05, // Platform capability descriptor type
        .bReserved = 0,
        .PlatformCapabilityUUID =
        { 0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C, 0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F},
        .dwWindowsVersion = 0x06030000,
        .wMSOSDescriptorSetTotalLength = 158,
        .bMS_VendorCode = 0x01,
        .bAltEnumCode = 0
    },
};

usb::usb_descriptor_compatible_id compatible_id = {
    20,
    0x03, // MS OS 2.0 compatible ID feature
    "WINUSB",
    {0, 0, 0, 0, 0, 0, 0, 0}
};

usb::usb_descriptor_registry_property registry_property = {
    128,
    0x04,
    0x01,
    40,
    u"DeviceInterfaceGUID",
    78,
    u"{2AAC9F9E-0EED-4860-8FD6-4A372570696D}"
};

usb::usb_descriptor_ms_os_20 usb::DESCRIPTOR_MS_OS_20 = {
    .wLength = 10,
    .wDescriptorType = 0x00, // MS OS 2.0 set header descriptor
    .dwWindowsVersion = 0x06030000, // Windows 8.1
    .wTotalLength = 158,
    .compatible_id = compatible_id,
    .registry_property = registry_property
};
