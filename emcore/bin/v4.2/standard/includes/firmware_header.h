////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_header/includes/firmware_header.h
///
/// @project    T9305
///
/// @brief      Firmware header types and info
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023, EM Microelectronic
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#ifndef __FIRMWARE_HEADER_H
#define __FIRMWARE_HEADER_H

#include "types.h"

/******************************************************************************\
 *  Symbolic Constants
\******************************************************************************/
// Add extra salt for non emcore builds.
#if !defined(EMCORE_FLAVOR_ID)
#define EMCORE_FLAVOR_ID 0x00000000U
#endif // EMCORE_FLAVOR_ID

// Determine the stack type based on the flavor. !THIS ASSUMES EMCORE_FLAVORS
// IS DEFINED LIKE SO!:
// SET(EMCORE_FLAVORS
//     "alpw_ble_ll_42"       # Flavor_ID = 1
//     "alpw_ble_ll_50"       # Flavor_ID = 2
//     "alpw_ble_ll_hs_42"    # Flavor_ID = 3
//     "alpw_ble_ll_hs_50"    # Flavor_ID = 4
//     "emb_ble_ll_42"        # Flavor_ID = 5
//     "emb_ble_ll_52"        # Flavor_ID = 6
//     "emb_ble_ll_hs_42"     # Flavor_ID = 7
//     "emb_ble_ll_hs_52"     # Flavor_ID = 8
//
//     CACHE INTERNAL "Versions of EMCORE"
// )
#if (EMCORE_FLAVOR_ID != 0 && EMCORE_FLAVOR_ID < 5)
#define EMCORE_STACK_TYPE 1U
#elif (EMCORE_FLAVOR_ID != 0 && EMCORE_FLAVOR_ID > 4)
#define EMCORE_STACK_TYPE 2U
#else
#define EMCORE_STACK_TYPE 0U
#endif // EMCORE_FLAVOR_ID != 0 && EMCORE_FLAVOR_ID < 5

// symbolic constants
#define MAGIC_HEADER_IDENTIFIER_32       0x52444846U // 'FHDR' in ASCII little endian
#define HEADER_VERSION                   0x00U
#define HEADER_FIELD_EMPTY               0xFFFFFFFFU

#define FIRMWARE_UPDATER_SECTION_CODE    0x00U
#define EMCORE_SECTION_CODE              0x01U
#define USERAPP_SECTION_CODE             0x03U
#define CUSTOMER_BOOTLOADER_SECTION_CODE 0x04U
typedef uint8_t FwHeader_SectionCode_t;

#define EMCORE_STACK_TYPE_OFFSET 4U
#define CRC_CHECK_BIT_OFFSET     8U
// #define MONOLITHIC_SECTION_CODE 0x05U // TODO(anyone): MONOLITHIC_SECTION_CODE
// was not defined in the spec. For now MONOLITHIC_SECTION_CODEs will be USERAPP_SECTION_CODEs


/******************************************************************************\
 *  Macros
\******************************************************************************/
#define FIRMWARE_OPTIONS_FLAVOR_ID_MSK(x) (x & 0x0000000FU)
#define FIRMWARE_OPTIONS_STACK_TYPE_MSK(x) (x & 0x000000F0U)
#define FIRMWARE_OPTIONS_CRC_CHECK_MSK(x) (x & 0x00000100U)

#define GET_FIRMWARE_OPTIONS_FLAVOR_ID(x) FIRMWARE_OPTIONS_FLAVOR_ID_MSK(x)
#define GET_FIRMWARE_OPTIONS_STACK_TYPE(x) (FIRMWARE_OPTIONS_STACK_TYPE_MSK(x) >> EMCORE_STACK_TYPE_OFFSET)
#define GET_FIRMWARE_OPTIONS_CRC_CHECK(x) (FIRMWARE_OPTIONS_CRC_CHECK_MSK(x) >> CRC_CHECK_BIT_OFFSET)

#define SET_FIRMWARE_OPTIONS_FLAVOR_ID(x) FIRMWARE_OPTIONS_FLAVOR_ID_MSK(x)
#define SET_FIRMWARE_OPTIONS_STACK_TYPE(x) FIRMWARE_OPTIONS_STACK_TYPE_MSK((x << EMCORE_STACK_TYPE_OFFSET))
#define SET_FIRMWARE_OPTIONS_CRC_CHECK(x) FIRMWARE_OPTIONS_CRC_CHECK_MSK((x << CRC_CHECK_BIT_OFFSET))


/******************************************************************************\
 *  Types
\******************************************************************************/
typedef void (*NVM_EntryFn_t)(void);

/// Firmware header structure
typedef struct
{
    /// magic number corresponding to 'FHDR' in ASCII
    uint32_t magicNumber;
    /// Version of the header
    uint8_t headerVersion;
    /// length of the header in number of bytes
    uint8_t headerLength;
    /// Section code: Firmware Updater = 0x00, EM-Core = 0x01, Customer App = 0x03, Bootloader = 0x04
    uint8_t sectionCode;
    /// reserved
    uint8_t rfu;
    /// Start address of the firmware segment
    void *firmwareStartAddress;
    /// Size of the firmware
    uint32_t firmwareSize;
    /// CRC of the firmware
    uint32_t firmwareCRC;
    /// Reserved
    uint32_t emcoreCRC;
    /// Additionnal information of the firmware
    uint32_t firmwareOptions;
    /// Version of the frmware
    uint16_t firmwareVersion;
    /// Execution address of the firmware
    NVM_EntryFn_t firmwareExecutionAddress;
    /// CRC of the header
    uint32_t headerCRC;
} __attribute__((aligned(4))) __attribute__((packed)) firmwareHeaderV0_t;
#define FIRMWARE_HEADER_T firmwareHeaderV0_t
#define NEG_OFFSET_HDR_CRC 6U // This must always be the number of bytes from
                              // the end of headerCRC field to the end of the
                              // structure. If you change FIRMWARE_HEADER_T,
                              // please verify this NEG_OFFSET_HDR_CRC.

/******************************************************************************\
 *  External
\******************************************************************************/
extern firmwareHeaderV0_t __attribute__((section(".programHeader")))
programHeader;

extern void NVM_Entry(void);

extern uint32_t PAYLOAD_SIZE[];
extern uint32_t PAYLOAD_START[];
extern uint32_t PAYLOAD_END[];

#endif // __FIRMWARE_HEADER_H
