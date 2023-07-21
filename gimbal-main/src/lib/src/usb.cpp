#include "lib/inc/usb.hpp"
#include "lib/inc/servo.hpp"

using namespace usb;


static void controlHandler();
static void standardRequestHandler();
static void vendorRequestHandler();
static void enableEndpoints(uint8_t configurationNumber);
static void endpoint1Handler();


#define MIN(a, b) ((a < b)? (a) : (b))


#define BMREQUESTTYPE_DPTD(bmRequestType) ((bmRequestType & 0x80) >> 7)
#define BMREQUESTTYPE_TYPE(bmRequestType) ((bmRequestType & 0x60) >> 5)
#define BMREQUESTTYPE_RECIPIENT(bmRequestType) (bmRequestType & 0x1f)

#define WVALUE_TYPE(wValue) ((wValue & 0xff00) >> 8)
#define WVALUE_IDX(wValue) (wValue & 0xff)
#define WVALUE_ADD(wValue) (wValue & 0x7f)

typedef struct __attribute__((packed)) {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
}
usb_device_endpoint0_request;

typedef struct __attribute__((packed)) {
    uint8_t bRequest;
    uint8_t bValue;
    uint8_t bData[128];
}
usb_device_endpoint1_request;

usb_descriptor_device_registers_t usb::EPDESCTBL[2];

static usb_device_endpoint0_request EP0REQ;
static usb_device_endpoint1_request EP1REQ;

static uint8_t* defaultData{nullptr};
static uint8_t defaultLen{0};

extern "C" {

    void USB_Handler() {
        if (USB_REGS->DEVICE.USB_INTFLAG & USB_DEVICE_INTFLAG_EORST_Msk) { // Process USB reset
        USB_REGS->DEVICE.USB_INTFLAG = USB_DEVICE_INTFLAG_EORST(1); // Clear pending interrupt
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_RXSTP(1) // Enable SETUP endpoint interrupt
                | USB_DEVICE_EPINTENSET_TRCPT0(1) // Enable OUT endpoint interrupt
                | USB_DEVICE_EPINTENSET_TRCPT1(1); // Enable IN endpoint interrupt
        EPDESCTBL[0].DEVICE_DESC_BANK[0].USB_ADDR = (uint32_t) & EP0REQ;
    }
        
        if (USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & (USB_DEVICE_EPINTFLAG_RXSTP_Msk | USB_DEVICE_EPINTFLAG_TRCPT1_Msk | USB_DEVICE_EPINTFLAG_TRCPT0_Msk)) {
            controlHandler();
        } else if (USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPINTFLAG & (USB_DEVICE_EPINTFLAG_TRCPT1_Msk | USB_DEVICE_EPINTFLAG_TRCPT0_Msk)) {
            endpoint1Handler();
            USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUS_BK0RDY(1);
            USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPINTFLAG = USB_DEVICE_EPINTFLAG_Msk; // Clear all pending endpoint interrupts
        }
    }
}

static void controlHandler() {
    if (USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_RXSTP_Msk) { // Process SETUP transfers
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG = USB_DEVICE_EPINTFLAG_RXSTP(1); // Clear pending interrupt
        uint8_t type = BMREQUESTTYPE_TYPE(EP0REQ.bmRequestType);
        switch (type) {
            case (uint8_t)REQ_TYPE::STANDARD:
                standardRequestHandler();
                break;
            case (uint8_t)REQ_TYPE::VENDOR:
                vendorRequestHandler();
                break;
        }
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUS_BK1RDY(1);
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUS_BK0RDY(1);
    }

    if (USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT0_Msk) { // Process OUT transfers
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG = USB_DEVICE_EPINTFLAG_TRCPT0(1); // Clear pending interrupt
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUS_BK1RDY(1);
    }

    if (USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT1_Msk) { // Process IN transfers
        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG = USB_DEVICE_EPINTFLAG_TRCPT1(1); // Clear pending interrupt
        if (USB_REGS->DEVICE.USB_DADD && !(USB_REGS->DEVICE.USB_DADD & USB_DEVICE_DADD_ADDEN_Msk)) {
            USB_REGS->DEVICE.USB_DADD |= USB_DEVICE_DADD_ADDEN(1);
        }
    }
}

static void standardRequestHandler() {
    uint8_t descType = WVALUE_TYPE(EP0REQ.wValue);
    switch (EP0REQ.bRequest) {
        case (uint8_t)DEVICE_REQ::GET_DESCRIPTOR:
            switch (descType) {
                case (uint8_t)DESCRIPTOR_TYPE::DEVICE:
                    EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t) & DESCRIPTOR_DEVICE;
                    EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_BYTE_COUNT(MIN(DESCRIPTOR_DEVICE.bLength, EP0REQ.wLength))
                            | USB_DEVICE_PCKSIZE_SIZE(0x3)
                            | USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
                    break;
                case (uint8_t)DESCRIPTOR_TYPE::BOS:
                    EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t) & DESCRIPTOR_BOS;
                    EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_BYTE_COUNT(MIN(DESCRIPTOR_BOS.wTotalLength, EP0REQ.wLength))
                            | USB_DEVICE_PCKSIZE_SIZE(0x3)
                            | USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
                    break;
                case (uint8_t)DESCRIPTOR_TYPE::CONFIGURATION:
                    EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t) & DESCRIPTOR_CONFIGURATION + WVALUE_IDX(EP0REQ.wValue);
                    EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE =
                            USB_DEVICE_PCKSIZE_BYTE_COUNT(MIN(DESCRIPTOR_CONFIGURATION[WVALUE_IDX(EP0REQ.wValue)].wTotalLength, EP0REQ.wLength))
                            | USB_DEVICE_PCKSIZE_SIZE(0x3)
                            | USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
                    break;
                case (uint8_t)DESCRIPTOR_TYPE::STRING:
                {
                    uint8_t idx = WVALUE_IDX(EP0REQ.wValue);
                    if (idx <= 2) {
                        EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t) & (DESCRIPTOR_STRING[idx]);
                        EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_BYTE_COUNT(MIN(DESCRIPTOR_STRING[idx].bLength, EP0REQ.wLength))
                                | USB_DEVICE_PCKSIZE_SIZE(0x3)
                                | USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
                    } else {
                        USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSCLR_STALLRQ1(1);
                    }
                }
                    break;
                case (uint8_t)DESCRIPTOR_TYPE::DEVICE_QUALIFIER:
                    USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSCLR_STALLRQ1(1);
                    break;
            }
            USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUS_BK1RDY(1);
            break;
        case (uint8_t)DEVICE_REQ::SET_ADDRESS:
            USB_REGS->DEVICE.USB_DADD = WVALUE_ADD(EP0REQ.wValue);
            EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
            break;
        case (uint8_t)DEVICE_REQ::SET_CONFIGURATION:
            EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
            enableEndpoints(EP0REQ.wValue);
            break;
    }
}

static void vendorRequestHandler() {
    if (EP0REQ.bRequest == 0x01) {
        EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t) & DESCRIPTOR_MS_OS_20;
        EPDESCTBL[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_BYTE_COUNT(MIN(DESCRIPTOR_MS_OS_20.wTotalLength, EP0REQ.wLength))
                | USB_DEVICE_PCKSIZE_SIZE(0x3)
                | USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
    }
}

void endpoint1Handler() {
    if (USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT0_Msk) { // OUT transfer
        if (EP1REQ.bRequest == static_cast<uint8_t>(data::DATA_REQUEST::READ)) {
            switch (EP1REQ.bValue) {
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::SETTINGS):
                    write(reinterpret_cast<uint8_t*>(&data::SETTINGS_DESCRIPTOR), sizeof (data::SETTINGS_DESCRIPTOR));
                    break;
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::INPUTS):
                    write(reinterpret_cast<uint8_t*>(&data::INPUTS_DESCRIPTOR), sizeof (data::INPUTS_DESCRIPTOR));
                    break;
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::MUX):
                    write(reinterpret_cast<uint8_t*>(&data::MUX_DESCRIPTOR), sizeof (data::MUX_DESCRIPTOR));
                    break;
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::TRIMS):
                    write(reinterpret_cast<uint8_t*>(&data::TRIMS_DESCRIPTOR), sizeof (data::TRIMS_DESCRIPTOR));
                    break;
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::OUTPUTS):
                    write(reinterpret_cast<uint8_t*>(&data::OUTPUTS_DESCRIPTOR), sizeof (data::OUTPUTS_DESCRIPTOR));
                    break;
            }
        } else {
            switch (EP1REQ.bValue) {
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::MUX):
                    util::copy(data::MUX_DESCRIPTOR.wMux, reinterpret_cast<int16_t*>(EP1REQ.bData), data::muxLength);
                    data::save();
                    break;
                case static_cast<uint8_t>(data::DATA_DESCRIPTOR_TYPE::TRIMS):
                    util::copy(data::TRIMS_DESCRIPTOR.wTrims, reinterpret_cast<int16_t*>(EP1REQ.bData), data::outputChannelCount);
                    data::save();
                    break;
            }
        }
        EPDESCTBL[1].DEVICE_DESC_BANK[0].USB_PCKSIZE = USB_DEVICE_PCKSIZE_MULTI_PACKET_SIZE(sizeof(EP1REQ)) | USB_DEVICE_PCKSIZE_SIZE(0x3);
        return;
    }

    if (defaultLen) {
        write(defaultData, defaultLen);
    }
}

static void enableEndpoints(uint8_t configurationNumber) {
    USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPCFG = USB_DEVICE_EPCFG_EPTYPE0(0x4) // Configure endpoint 1 bank 0 as interrupt out
            | USB_DEVICE_EPCFG_EPTYPE1(0x4); // Configure endpoint 1 bank 1 as interrupt in
    EPDESCTBL[1].DEVICE_DESC_BANK[0].USB_ADDR = (uint32_t) & EP1REQ;
    EPDESCTBL[1].DEVICE_DESC_BANK[0].USB_PCKSIZE = USB_DEVICE_PCKSIZE_MULTI_PACKET_SIZE(sizeof(EP1REQ)) | USB_DEVICE_PCKSIZE_SIZE(0x3);
    USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0(1) // Enable OUT endpoint interrupt
            | USB_DEVICE_EPINTENSET_TRCPT1(1); // Enable IN endpoint interrupt

    writeDefault(reinterpret_cast<uint8_t*>(&data::STATUS_DESCRIPTOR), sizeof(data::STATUS_DESCRIPTOR));
}

void usb::init() {
    uint32_t calibration = *((uint32_t*)0x00806020);

    NVIC_SetPriority(USB_IRQn, 3);
    NVIC_EnableIRQ(USB_IRQn);

    PORT_REGS->GROUP[0].PORT_PINCFG[24] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 24
    PORT_REGS->GROUP[0].PORT_PINCFG[25] = PORT_PINCFG_PMUXEN(1); // Enable mux on pin 25
    PORT_REGS->GROUP[0].PORT_PMUX[12] = PORT_PMUX_PMUXE_G // Mux pin 24 to USB
            | PORT_PMUX_PMUXO_G; // Mux pin 25 to USB

    USB_REGS->DEVICE.USB_PADCAL = USB_PADCAL_TRANSN(calibration >> 13u & 0x1f)
            | USB_PADCAL_TRANSP(calibration >> 18u & 0x1f)
            | USB_PADCAL_TRIM(calibration >> 23u & 0x7); // USB pad calibration
    USB_REGS->DEVICE.USB_CTRLA = USB_CTRLA_ENABLE(1) // Enable USB
            | USB_CTRLA_MODE_DEVICE; // Enable in device mode
    USB_REGS->DEVICE.DEVICE_ENDPOINT[0].USB_EPCFG = USB_DEVICE_EPCFG_EPTYPE0(0x1) // Configure endpoint 0 as setup out
            | USB_DEVICE_EPCFG_EPTYPE1(0x1); // Configure endpoint 0 as setup in
    USB_REGS->DEVICE.USB_DESCADD = (uint32_t) & EPDESCTBL; // Setting endpoint descriptor address
    USB_REGS->DEVICE.USB_CTRLB = USB_DEVICE_CTRLB_DETACH(0); // Attach to host
    USB_REGS->DEVICE.USB_INTENSET = USB_DEVICE_INTENSET_EORST(1); // Enable end-of-reset interrupt
}

void usb::writeDefault(uint8_t* data, uint8_t len) {
    defaultData = data;
    defaultLen = len;

    if (!(USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPSTATUS & USB_DEVICE_EPSTATUS_BK1RDY(0))) {
        write(data, len);
    }
}

void usb::write(uint8_t* data, uint8_t len) {
    EPDESCTBL[1].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t)data;
    EPDESCTBL[1].DEVICE_DESC_BANK[1].USB_PCKSIZE = USB_DEVICE_PCKSIZE_BYTE_COUNT(len) | USB_DEVICE_PCKSIZE_SIZE(0x3) | USB_DEVICE_PCKSIZE_AUTO_ZLP(1);
    USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPSTATUSSET = USB_DEVICE_EPSTATUS_BK1RDY(1);
}

void usb::read(uint8_t* data, uint8_t len) {
    //    memcpy(data, outBuf1, MIN(len, sizeof (outBuf1)));
    USB_REGS->DEVICE.DEVICE_ENDPOINT[1].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUS_BK0RDY(1);
}
