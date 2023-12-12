/*
 * Header file for ATSAMD09C13A
 *
 * Copyright (c) 2022 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software and any derivatives
 * exclusively with Microchip products. It is your responsibility to comply with third party license
 * terms applicable to your use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
 * EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/* File generated from device description version 2019-11-25T07:00:27Z */
#ifndef _SAMD09C13A_H_
#define _SAMD09C13A_H_

/* Header version uses Semantic Versioning 2.0.0 (https://semver.org/) */
#define HEADER_FORMAT_VERSION "2.1.0"

#define HEADER_FORMAT_VERSION_MAJOR (2)
#define HEADER_FORMAT_VERSION_MINOR (1)
#define HEADER_FORMAT_VERSION_PATCH (0)

/* SAMD09C13A definitions
  This file defines all structures and symbols for SAMD09C13A:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
*/

#ifdef __cplusplus
 extern "C" {
#endif

#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#  include <stdint.h>
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#if !defined(SKIP_INTEGER_LITERALS)
#  if defined(_UINT8_) || defined(_UINT16_) || defined(_UINT32_)
#    error "Integer constant value macros already defined elsewhere"
#  endif

#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
/* Macros that deal with sizes of integer constants for C/C++ */
#  define _UINT8_(x)   ((uint8_t)(x))    /* C code: 8-bits unsigned integer constant value */
#  define _UINT16_(x)  ((uint16_t)(x))   /* C code: 16-bits unsigned integer constant value */
#  define _UINT32_(x)  ((uint32_t)(x))   /* C code: 32-bits unsigned integer constant value */

#else /* Assembler */

#  define _UINT8_(x) x    /* Assembler: 8-bits unsigned integer constant value */
#  define _UINT16_(x) x   /* Assembler: 16-bits unsigned integer constant value */
#  define _UINT32_(x) x   /* Assembler: 32-bits unsigned integer constant value */
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
#endif /* SKIP_INTEGER_LITERALS */

/* ************************************************************************** */
/* CMSIS DEFINITIONS FOR SAMD09C13A                                         */
/* ************************************************************************** */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
/* Interrupt Number Definition */
typedef enum IRQn
{
/******  CORTEX-M0PLUS Processor Exceptions Numbers ******************************/
  Reset_IRQn                = -15, /* -15 Reset Vector, invoked on Power up and warm reset */
  NonMaskableInt_IRQn       = -14, /* -14 Non maskable Interrupt, cannot be stopped or preempted */
  HardFault_IRQn            = -13, /* -13 Hard Fault, all classes of Fault    */
  SVCall_IRQn               =  -5, /* -5  System Service Call via SVC instruction */
  PendSV_IRQn               =  -2, /* -2  Pendable request for system service */
  SysTick_IRQn              =  -1, /* -1  System Tick Timer                   */

/******  SAMD09C13A specific Interrupt Numbers ***********************************/
  PM_IRQn                   =   0, /* 0   Power Manager (PM)                  */
  SYSCTRL_IRQn              =   1, /* 1   System Control (SYSCTRL)            */
  WDT_IRQn                  =   2, /* 2   Watchdog Timer (WDT)                */
  RTC_IRQn                  =   3, /* 3   Real-Time Counter (RTC)             */
  EIC_IRQn                  =   4, /* 4   External Interrupt Controller (EIC) */
  NVMCTRL_IRQn              =   5, /* 5   Non-Volatile Memory Controller (NVMCTRL) */
  DMAC_IRQn                 =   6, /* 6   Direct Memory Access Controller (DMAC) */
  EVSYS_IRQn                =   8, /* 8   Event System Interface (EVSYS)      */
  SERCOM0_IRQn              =   9, /* 9   Serial Communication Interface (SERCOM0) */
  SERCOM1_IRQn              =  10, /* 10  Serial Communication Interface (SERCOM1) */
  TC1_IRQn                  =  13, /* 13  Basic Timer Counter (TC1)           */
  TC2_IRQn                  =  14, /* 14  Basic Timer Counter (TC2)           */
  ADC_IRQn                  =  15, /* 15  Analog Digital Converter (ADC)      */
  PTC_IRQn                  =  18, /* 18  Peripheral Touch Controller (PTC)   */

  PERIPH_MAX_IRQn           =  18  /* Max peripheral ID */
} IRQn_Type;
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct _DeviceVectors
{
  /* Stack pointer */
  void* pvStack;
  /* CORTEX-M0PLUS handlers */
  void* pfnReset_Handler;                        /* -15 Reset Vector, invoked on Power up and warm reset */
  void* pfnNonMaskableInt_Handler;               /* -14 Non maskable Interrupt, cannot be stopped or preempted */
  void* pfnHardFault_Handler;                    /* -13 Hard Fault, all classes of Fault */
  void* pvReservedC12;
  void* pvReservedC11;
  void* pvReservedC10;
  void* pvReservedC9;
  void* pvReservedC8;
  void* pvReservedC7;
  void* pvReservedC6;
  void* pfnSVCall_Handler;                       /*  -5 System Service Call via SVC instruction */
  void* pvReservedC4;
  void* pvReservedC3;
  void* pfnPendSV_Handler;                       /*  -2 Pendable request for system service */
  void* pfnSysTick_Handler;                      /*  -1 System Tick Timer */

  /* Peripheral handlers */
  void* pfnPM_Handler;                           /*   0 Power Manager (PM) */
  void* pfnSYSCTRL_Handler;                      /*   1 System Control (SYSCTRL) */
  void* pfnWDT_Handler;                          /*   2 Watchdog Timer (WDT) */
  void* pfnRTC_Handler;                          /*   3 Real-Time Counter (RTC) */
  void* pfnEIC_Handler;                          /*   4 External Interrupt Controller (EIC) */
  void* pfnNVMCTRL_Handler;                      /*   5 Non-Volatile Memory Controller (NVMCTRL) */
  void* pfnDMAC_Handler;                         /*   6 Direct Memory Access Controller (DMAC) */
  void* pvReserved7;
  void* pfnEVSYS_Handler;                        /*   8 Event System Interface (EVSYS) */
  void* pfnSERCOM0_Handler;                      /*   9 Serial Communication Interface (SERCOM0) */
  void* pfnSERCOM1_Handler;                      /*  10 Serial Communication Interface (SERCOM1) */
  void* pvReserved11;
  void* pvReserved12;
  void* pfnTC1_Handler;                          /*  13 Basic Timer Counter (TC1) */
  void* pfnTC2_Handler;                          /*  14 Basic Timer Counter (TC2) */
  void* pfnADC_Handler;                          /*  15 Analog Digital Converter (ADC) */
  void* pvReserved16;
  void* pvReserved17;
  void* pfnPTC_Handler;                          /*  18 Peripheral Touch Controller (PTC) */
} DeviceVectors;

#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if !defined DONT_USE_PREDEFINED_CORE_HANDLERS
/* CORTEX-M0PLUS exception handlers */
void Reset_Handler                 ( void );
void NonMaskableInt_Handler        ( void );
void HardFault_Handler             ( void );
void SVCall_Handler                ( void );
void PendSV_Handler                ( void );
void SysTick_Handler               ( void );
#endif /* DONT_USE_PREDEFINED_CORE_HANDLERS */

#if !defined DONT_USE_PREDEFINED_PERIPHERALS_HANDLERS
/* Peripherals interrupt handlers */
void PM_Handler                    ( void );
void SYSCTRL_Handler               ( void );
void WDT_Handler                   ( void );
void RTC_Handler                   ( void );
void EIC_Handler                   ( void );
void NVMCTRL_Handler               ( void );
void DMAC_Handler                  ( void );
void EVSYS_Handler                 ( void );
void SERCOM0_Handler               ( void );
void SERCOM1_Handler               ( void );
void TC1_Handler                   ( void );
void TC2_Handler                   ( void );
void ADC_Handler                   ( void );
void PTC_Handler                   ( void );
#endif /* DONT_USE_PREDEFINED_PERIPHERALS_HANDLERS */
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

/* Configuration of the CORTEX-M0PLUS Processor and Core Peripherals */
#define __CM0PLUS_REV                 0x0001 /* Cortex-M0+ Core Revision                                                  */
#define __MPU_PRESENT                      0 /* MPU present or not                                                        */
#define __NVIC_PRIO_BITS                   2 /* Number of Bits used for Priority Levels                                   */
#define __VTOR_PRESENT                     1 /* Vector Table Offset Register present or not                               */
#define __Vendor_SysTickConfig             0 /* Set to 1 if different SysTick Config is used                              */
#define __ARCH_ARM                         1
#define __ARCH_ARM_CORTEX_M                1

/* CMSIS includes */
#include "core_cm0plus.h"
#if defined USE_CMSIS_INIT
#include "system_samd09.h"
#endif /* USE_CMSIS_INIT */

/* ************************************************************************** */
/*   SOFTWARE PERIPHERAL API DEFINITION FOR SAMD09C13A                        */
/* ************************************************************************** */
#include "component/adc.h"
#include "component/dmac.h"
#include "component/dsu.h"
#include "component/eic.h"
#include "component/evsys.h"
#include "component/fuses.h"
#include "component/gclk.h"
#include "component/hmatrixb.h"
#include "component/mtb.h"
#include "component/nvmctrl.h"
#include "component/pac.h"
#include "component/pm.h"
#include "component/port.h"
#include "component/ptc.h"
#include "component/rtc.h"
#include "component/sercom.h"
#include "component/sysctrl.h"
#include "component/tc.h"
#include "component/wdt.h"

/* ************************************************************************** */
/*   INSTANCE DEFINITIONS FOR SAMD09C13A */
/* ************************************************************************** */
#include "instance/adc.h"
#include "instance/dmac.h"
#include "instance/dsu.h"
#include "instance/eic.h"
#include "instance/evsys.h"
#include "instance/fuses.h"
#include "instance/gclk.h"
#include "instance/mtb.h"
#include "instance/nvmctrl.h"
#include "instance/pac0.h"
#include "instance/pac1.h"
#include "instance/pac2.h"
#include "instance/pm.h"
#include "instance/port.h"
#include "instance/ptc.h"
#include "instance/rtc.h"
#include "instance/sbmatrix.h"
#include "instance/sercom0.h"
#include "instance/sercom1.h"
#include "instance/sysctrl.h"
#include "instance/tc1.h"
#include "instance/tc2.h"
#include "instance/wdt.h"

/* ************************************************************************** */
/*  PERIPHERAL ID DEFINITIONS FOR SAMD09C13A                                  */
/* ************************************************************************** */
#define ID_PAC0          (  0) /* Peripheral Access Controller (PAC0) */
#define ID_PM            (  1) /* Power Manager (PM) */
#define ID_SYSCTRL       (  2) /* System Control (SYSCTRL) */
#define ID_GCLK          (  3) /* Generic Clock Generator (GCLK) */
#define ID_WDT           (  4) /* Watchdog Timer (WDT) */
#define ID_RTC           (  5) /* Real-Time Counter (RTC) */
#define ID_EIC           (  6) /* External Interrupt Controller (EIC) */
#define ID_PAC1          ( 32) /* Peripheral Access Controller (PAC1) */
#define ID_DSU           ( 33) /* Device Service Unit (DSU) */
#define ID_NVMCTRL       ( 34) /* Non-Volatile Memory Controller (NVMCTRL) */
#define ID_PORT          ( 35) /* Port Module (PORT) */
#define ID_DMAC          ( 36) /* Direct Memory Access Controller (DMAC) */
#define ID_MTB           ( 38) /* Cortex-M0+ Micro-Trace Buffer (MTB) */
#define ID_SBMATRIX      ( 39) /* HSB Matrix (SBMATRIX) */
#define ID_PAC2          ( 64) /* Peripheral Access Controller (PAC2) */
#define ID_EVSYS         ( 65) /* Event System Interface (EVSYS) */
#define ID_SERCOM0       ( 66) /* Serial Communication Interface (SERCOM0) */
#define ID_SERCOM1       ( 67) /* Serial Communication Interface (SERCOM1) */
#define ID_TC1           ( 70) /* Basic Timer Counter (TC1) */
#define ID_TC2           ( 71) /* Basic Timer Counter (TC2) */
#define ID_ADC           ( 72) /* Analog Digital Converter (ADC) */
#define ID_PTC           ( 75) /* Peripheral Touch Controller (PTC) */

#define ID_PERIPH_MAX    ( 75) /* Number of peripheral IDs */

/* ************************************************************************** */
/*   REGISTER STRUCTURE ADDRESS DEFINITIONS FOR SAMD09C13A                    */
/* ************************************************************************** */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#define ADC_REGS                         ((adc_registers_t*)0x42002000)                /* ADC Registers Address        */
#define DMAC_REGS                        ((dmac_registers_t*)0x41004800)               /* DMAC Registers Address       */
#define DSU_REGS                         ((dsu_registers_t*)0x41002000)                /* DSU Registers Address        */
#define EIC_REGS                         ((eic_registers_t*)0x40001800)                /* EIC Registers Address        */
#define EVSYS_REGS                       ((evsys_registers_t*)0x42000400)              /* EVSYS Registers Address      */
#define GCLK_REGS                        ((gclk_registers_t*)0x40000c00)               /* GCLK Registers Address       */
#define SBMATRIX_REGS                    ((hmatrixb_registers_t*)0x41007000)           /* SBMATRIX Registers Address   */
#define MTB_REGS                         ((mtb_registers_t*)0x41006000)                /* MTB Registers Address        */
#define NVMCTRL_REGS                     ((nvmctrl_registers_t*)0x41004000)            /* NVMCTRL Registers Address    */
#define OTP4_FUSES_REGS                  ((fuses_otp4_fuses_registers_t*)0x00806020)   /* FUSES Registers Address      */
#define TEMP_LOG_FUSES_REGS              ((fuses_temp_log_fuses_registers_t*)0x00806030) /* FUSES Registers Address      */
#define USER_FUSES_REGS                  ((fuses_user_fuses_registers_t*)0x00804000)   /* FUSES Registers Address      */
#define PAC0_REGS                        ((pac_registers_t*)0x40000000)                /* PAC0 Registers Address       */
#define PAC1_REGS                        ((pac_registers_t*)0x41000000)                /* PAC1 Registers Address       */
#define PAC2_REGS                        ((pac_registers_t*)0x42000000)                /* PAC2 Registers Address       */
#define PM_REGS                          ((pm_registers_t*)0x40000400)                 /* PM Registers Address         */
#define PORT_REGS                        ((port_registers_t*)0x41004400)               /* PORT Registers Address       */
#define PORT_IOBUS_REGS                  ((port_registers_t*)0x60000000)               /* PORT Registers Address       */
#define PTC_REGS                         ((ptc_registers_t*)0x42002c00)                /* PTC Registers Address        */
#define RTC_REGS                         ((rtc_registers_t*)0x40001400)                /* RTC Registers Address        */
#define SERCOM0_REGS                     ((sercom_registers_t*)0x42000800)             /* SERCOM0 Registers Address    */
#define SERCOM1_REGS                     ((sercom_registers_t*)0x42000c00)             /* SERCOM1 Registers Address    */
#define SYSCTRL_REGS                     ((sysctrl_registers_t*)0x40000800)            /* SYSCTRL Registers Address    */
#define TC1_REGS                         ((tc_registers_t*)0x42001800)                 /* TC1 Registers Address        */
#define TC2_REGS                         ((tc_registers_t*)0x42001c00)                 /* TC2 Registers Address        */
#define WDT_REGS                         ((wdt_registers_t*)0x40001000)                /* WDT Registers Address        */
#endif /* (defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAMD09C13A                                  */
/* ************************************************************************** */
#define ADC_BASE_ADDRESS                 _UINT32_(0x42002000)                          /* ADC Base Address */
#define DMAC_BASE_ADDRESS                _UINT32_(0x41004800)                          /* DMAC Base Address */
#define DSU_BASE_ADDRESS                 _UINT32_(0x41002000)                          /* DSU Base Address */
#define EIC_BASE_ADDRESS                 _UINT32_(0x40001800)                          /* EIC Base Address */
#define EVSYS_BASE_ADDRESS               _UINT32_(0x42000400)                          /* EVSYS Base Address */
#define GCLK_BASE_ADDRESS                _UINT32_(0x40000c00)                          /* GCLK Base Address */
#define SBMATRIX_BASE_ADDRESS            _UINT32_(0x41007000)                          /* SBMATRIX Base Address */
#define MTB_BASE_ADDRESS                 _UINT32_(0x41006000)                          /* MTB Base Address */
#define NVMCTRL_BASE_ADDRESS             _UINT32_(0x41004000)                          /* NVMCTRL Base Address */
#define OTP4_FUSES_BASE_ADDRESS          _UINT32_(0x00806020)                          /* FUSES Base Address */
#define TEMP_LOG_FUSES_BASE_ADDRESS      _UINT32_(0x00806030)                          /* FUSES Base Address */
#define USER_FUSES_BASE_ADDRESS          _UINT32_(0x00804000)                          /* FUSES Base Address */
#define PAC0_BASE_ADDRESS                _UINT32_(0x40000000)                          /* PAC0 Base Address */
#define PAC1_BASE_ADDRESS                _UINT32_(0x41000000)                          /* PAC1 Base Address */
#define PAC2_BASE_ADDRESS                _UINT32_(0x42000000)                          /* PAC2 Base Address */
#define PM_BASE_ADDRESS                  _UINT32_(0x40000400)                          /* PM Base Address */
#define PORT_BASE_ADDRESS                _UINT32_(0x41004400)                          /* PORT Base Address */
#define PORT_IOBUS_BASE_ADDRESS          _UINT32_(0x60000000)                          /* PORT Base Address */
#define PTC_BASE_ADDRESS                 _UINT32_(0x42002c00)                          /* PTC Base Address */
#define RTC_BASE_ADDRESS                 _UINT32_(0x40001400)                          /* RTC Base Address */
#define SERCOM0_BASE_ADDRESS             _UINT32_(0x42000800)                          /* SERCOM0 Base Address */
#define SERCOM1_BASE_ADDRESS             _UINT32_(0x42000c00)                          /* SERCOM1 Base Address */
#define SYSCTRL_BASE_ADDRESS             _UINT32_(0x40000800)                          /* SYSCTRL Base Address */
#define TC1_BASE_ADDRESS                 _UINT32_(0x42001800)                          /* TC1 Base Address */
#define TC2_BASE_ADDRESS                 _UINT32_(0x42001c00)                          /* TC2 Base Address */
#define WDT_BASE_ADDRESS                 _UINT32_(0x40001000)                          /* WDT Base Address */

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAMD09C13A                                           */
/* ************************************************************************** */
#include "pio/samd09c13a.h"

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAMD09C13A                                */
/* ************************************************************************** */
#define FLASH_SIZE                     _UINT32_(0x00002000)    /*    8kB Memory segment type: flash */
#define FLASH_PAGE_SIZE                _UINT32_(        64)
#define FLASH_NB_OF_PAGES              _UINT32_(       128)

#define CAL_SIZE                       _UINT32_(0x00000008)    /*    0kB Memory segment type: fuses */
#define LOCKBIT_SIZE                   _UINT32_(0x00000004)    /*    0kB Memory segment type: fuses */
#define OTP1_SIZE                      _UINT32_(0x00000008)    /*    0kB Memory segment type: fuses */
#define OTP2_SIZE                      _UINT32_(0x00000008)    /*    0kB Memory segment type: fuses */
#define OTP4_SIZE                      _UINT32_(0x000000e0)    /*    0kB Memory segment type: fuses */
#define OTP4_PAGE_SIZE                 _UINT32_(        64)
#define OTP4_NB_OF_PAGES               _UINT32_(         3)

#define TEMP_LOG_SIZE                  _UINT32_(0x00000008)    /*    0kB Memory segment type: fuses */
#define USER_PAGE_SIZE                 _UINT32_(0x00000100)    /*    0kB Memory segment type: user_page */
#define USER_PAGE_PAGE_SIZE            _UINT32_(        64)
#define USER_PAGE_NB_OF_PAGES          _UINT32_(         4)

#define HMCRAMC0_SIZE                  _UINT32_(0x00001000)    /*    4kB Memory segment type: ram */
#define HPB0_SIZE                      _UINT32_(0x00010000)    /*   64kB Memory segment type: io */
#define HPB1_SIZE                      _UINT32_(0x00010000)    /*   64kB Memory segment type: io */
#define HPB2_SIZE                      _UINT32_(0x00010000)    /*   64kB Memory segment type: io */
#define PPB_SIZE                       _UINT32_(0x00100000)    /* 1024kB Memory segment type: io */
#define SCS_SIZE                       _UINT32_(0x00001000)    /*    4kB Memory segment type: io */
#define PERIPHERALS_SIZE               _UINT32_(0x20000000)    /* 524288kB Memory segment type: io */

#define FLASH_ADDR                     _UINT32_(0x00000000)    /* FLASH base address (type: flash)*/
#define CAL_ADDR                       _UINT32_(0x00800000)    /* CAL base address (type: fuses)*/
#define LOCKBIT_ADDR                   _UINT32_(0x00802000)    /* LOCKBIT base address (type: fuses)*/
#define OTP1_ADDR                      _UINT32_(0x00806000)    /* OTP1 base address (type: fuses)*/
#define OTP2_ADDR                      _UINT32_(0x00806008)    /* OTP2 base address (type: fuses)*/
#define OTP4_ADDR                      _UINT32_(0x00806020)    /* OTP4 base address (type: fuses)*/
#define TEMP_LOG_ADDR                  _UINT32_(0x00806030)    /* TEMP_LOG base address (type: fuses)*/
#define USER_PAGE_ADDR                 _UINT32_(0x00804000)    /* USER_PAGE base address (type: user_page)*/
#define HMCRAMC0_ADDR                  _UINT32_(0x20000000)    /* HMCRAMC0 base address (type: ram)*/
#define HPB0_ADDR                      _UINT32_(0x40000000)    /* HPB0 base address (type: io)*/
#define HPB1_ADDR                      _UINT32_(0x41000000)    /* HPB1 base address (type: io)*/
#define HPB2_ADDR                      _UINT32_(0x42000000)    /* HPB2 base address (type: io)*/
#define PPB_ADDR                       _UINT32_(0xe0000000)    /* PPB base address (type: io)*/
#define SCS_ADDR                       _UINT32_(0xe000e000)    /* SCS base address (type: io)*/
#define PERIPHERALS_ADDR               _UINT32_(0x40000000)    /* PERIPHERALS base address (type: io)*/

/* ************************************************************************** */
/*   DEVICE SIGNATURES FOR SAMD09C13A                                         */
/* ************************************************************************** */
#define CHIP_DSU_DID                   _UINT32_(0X10040107)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR SAMD09C13A                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Event Generator IDs for SAMD09C13A */
/* ************************************************************************** */
#define EVENT_ID_GEN_RTC_CMP_0                            1 /* ID for RTC event generator CMP_0 */
#define EVENT_ID_GEN_RTC_CMP_1                            2 /* ID for RTC event generator CMP_1 */
#define EVENT_ID_GEN_RTC_OVF                              3 /* ID for RTC event generator OVF */
#define EVENT_ID_GEN_RTC_PER_0                            4 /* ID for RTC event generator PER_0 */
#define EVENT_ID_GEN_RTC_PER_1                            5 /* ID for RTC event generator PER_1 */
#define EVENT_ID_GEN_RTC_PER_2                            6 /* ID for RTC event generator PER_2 */
#define EVENT_ID_GEN_RTC_PER_3                            7 /* ID for RTC event generator PER_3 */
#define EVENT_ID_GEN_RTC_PER_4                            8 /* ID for RTC event generator PER_4 */
#define EVENT_ID_GEN_RTC_PER_5                            9 /* ID for RTC event generator PER_5 */
#define EVENT_ID_GEN_RTC_PER_6                           10 /* ID for RTC event generator PER_6 */
#define EVENT_ID_GEN_RTC_PER_7                           11 /* ID for RTC event generator PER_7 */
#define EVENT_ID_GEN_EIC_EXTINT_0                        12 /* ID for EIC event generator EXTINT_0 */
#define EVENT_ID_GEN_EIC_EXTINT_1                        13 /* ID for EIC event generator EXTINT_1 */
#define EVENT_ID_GEN_EIC_EXTINT_2                        14 /* ID for EIC event generator EXTINT_2 */
#define EVENT_ID_GEN_EIC_EXTINT_3                        15 /* ID for EIC event generator EXTINT_3 */
#define EVENT_ID_GEN_EIC_EXTINT_4                        16 /* ID for EIC event generator EXTINT_4 */
#define EVENT_ID_GEN_EIC_EXTINT_5                        17 /* ID for EIC event generator EXTINT_5 */
#define EVENT_ID_GEN_EIC_EXTINT_6                        18 /* ID for EIC event generator EXTINT_6 */
#define EVENT_ID_GEN_EIC_EXTINT_7                        19 /* ID for EIC event generator EXTINT_7 */
#define EVENT_ID_GEN_DMAC_CH_0                           20 /* ID for DMAC event generator CH_0 */
#define EVENT_ID_GEN_DMAC_CH_1                           21 /* ID for DMAC event generator CH_1 */
#define EVENT_ID_GEN_DMAC_CH_2                           22 /* ID for DMAC event generator CH_2 */
#define EVENT_ID_GEN_DMAC_CH_3                           23 /* ID for DMAC event generator CH_3 */
#define EVENT_ID_GEN_TC1_OVF                             31 /* ID for TC1 event generator OVF */
#define EVENT_ID_GEN_TC1_MC_0                            32 /* ID for TC1 event generator MC_0 */
#define EVENT_ID_GEN_TC1_MC_1                            33 /* ID for TC1 event generator MC_1 */
#define EVENT_ID_GEN_TC2_OVF                             34 /* ID for TC2 event generator OVF */
#define EVENT_ID_GEN_TC2_MC_0                            35 /* ID for TC2 event generator MC_0 */
#define EVENT_ID_GEN_TC2_MC_1                            36 /* ID for TC2 event generator MC_1 */
#define EVENT_ID_GEN_ADC_RESRDY                          37 /* ID for ADC event generator RESRDY */
#define EVENT_ID_GEN_ADC_WINMON                          38 /* ID for ADC event generator WINMON */

/* ************************************************************************** */
/*  Event User IDs for SAMD09C13A */
/* ************************************************************************** */
#define EVENT_ID_USER_DMAC_CH_0                           0 /* ID for DMAC event user CH_0 */
#define EVENT_ID_USER_DMAC_CH_1                           1 /* ID for DMAC event user CH_1 */
#define EVENT_ID_USER_DMAC_CH_2                           2 /* ID for DMAC event user CH_2 */
#define EVENT_ID_USER_DMAC_CH_3                           3 /* ID for DMAC event user CH_3 */
#define EVENT_ID_USER_TC1_EVU                            10 /* ID for TC1 event user EVU */
#define EVENT_ID_USER_TC2_EVU                            11 /* ID for TC2 event user EVU */
#define EVENT_ID_USER_ADC_START                          12 /* ID for ADC event user START */
#define EVENT_ID_USER_ADC_SYNC                           13 /* ID for ADC event user SYNC */

#ifdef __cplusplus
}
#endif

#endif /* _SAMD09C13A_H_ */

