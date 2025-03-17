////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/firmware_header/includes/firmware_header_symbol.h
/// @project    T9305
///
/// @brief      Resolve information to go into firmware header
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

#ifndef __FIRMWARE_HEADER_SYMBOL_H
#define __FIRMWARE_HEADER_SYMBOL_H

#include "firmware_header.h"


// Check firmware version is defined.
#if defined(FIRMWARE_VERSION)
#define FIRMWARE_VERSION_NUMBER (uint16_t) FIRMWARE_VERSION
#else
#error "FIRMWARE_VERSION must be defined."
#endif

// Check which section it is.
#if defined(EMCORE_BUILD)
#define SECTION_CODE EMCORE_SECTION_CODE
#elif defined(BOOTLOADER_BUILD)
#define SECTION_CODE CUSTOMER_BOOTLOADER_SECTION_CODE
#elif defined(USERAPP_BUILD)
#define SECTION_CODE USERAPP_SECTION_CODE
#elif defined(MONOLITHIC_BUILD)
#define SECTION_CODE USERAPP_SECTION_CODE
#elif defined(FW_UPDATER_BUILD)
#define SECTION_CODE FIRMWARE_UPDATER_SECTION_CODE
#else
#error "EMCORE_BUILD, BOOTLOADER_BUILD, USERAPP_BUILD nor MONOLITHIC_BUILD defined."
#endif

// Check the flavor to determine if we need to add extra salt.
#if (defined(EMCORE_BUILD) && !defined(EMCORE_FLAVOR_ID))
#error "Must define EMCORE_FLAVOR_ID for emcore builds"
#endif

// Compiler define for whether or not bootloader performs crc check on this
// firmware.
// *** Inverse logic is used here ***
// Before this change, the default value of this bit was 0. Per specification,
// we want the nvm_bootloader to perform CRC check with default value. For
// legacy images built before this change, the behavior will be the same. To
// disable the CRC check, a new image must be built.
#if defined(IMAGE_CRC_CHECK)
#define CRC_CHECK_BIT 0
#else
#define CRC_CHECK_BIT 1
#endif


/******************************************************************************\
 *  Firmware Header Data
\******************************************************************************/
firmwareHeaderV0_t __attribute__((used, section(".programHeader")))
programHeader
    = {
          .magicNumber = MAGIC_HEADER_IDENTIFIER_32,
          .headerVersion = HEADER_VERSION,
          .headerLength = (uint8_t)sizeof(programHeader),
          .sectionCode = SECTION_CODE,
          .firmwareStartAddress = (&programHeader + 1U),
          .firmwareSize = (uint32_t)PAYLOAD_SIZE,
          .firmwareCRC = HEADER_FIELD_EMPTY, // CRC's are computed externally.
          .emcoreCRC = HEADER_FIELD_EMPTY,
          .firmwareOptions =
            SET_FIRMWARE_OPTIONS_FLAVOR_ID((uint32_t)EMCORE_FLAVOR_ID) |
            SET_FIRMWARE_OPTIONS_STACK_TYPE((uint32_t)EMCORE_STACK_TYPE) |
            SET_FIRMWARE_OPTIONS_CRC_CHECK((uint32_t)CRC_CHECK_BIT),
          .firmwareVersion = FIRMWARE_VERSION_NUMBER,
          .firmwareExecutionAddress = &NVM_Entry,
          .headerCRC = HEADER_FIELD_EMPTY,
      };

#endif // __FIRMWARE_HEADER_SYMBOL_H
