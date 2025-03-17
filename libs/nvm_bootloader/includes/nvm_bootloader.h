////////////////////////////////////////////////////////////////////////////////
///
/// @file       nvm_bootloader/includes/nvm_bootloader.h
///
/// @project    T9305
///
/// @brief      NVM bootloader function
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic
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


#ifndef _NVM_BOOTLOADER_H
#define _NVM_BOOTLOADER_H


#include "types.h"

#define NUM_HDR_ENTRIES (10) // This is more than necessary...

/******************************************************************************\
 *  NVM NVM_BOOTLOADER FUNCTION PROTOTYPES
\******************************************************************************/

void nvm_bootloader(void);

/**
 * @brief Function called during the NVM boot sequence, this is the entry point
 * for all code jumped to by this bootloader.
 * @note This function must be implemented in the application code.
 */
void NVM_Entry(void);

void initHeaderEntryValues(uint32_t hdr_entry[NUM_HDR_ENTRIES]);
void saveHeaderEntry(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t addr);
uint32_t getNextPageIfHeaderAddress(uint32_t addr);
void getAllHeaderEntries(uint32_t hdr_entry[NUM_HDR_ENTRIES]);
bool runChecksum(uint16_t gPmlResetFlags);
void checkCRCHeader(uint32_t* hdr_entry);
void checkCRC(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint16_t gPmlResetFlags);

bool getBootLoaderFunction(uint32_t function);
bool getEmCoreFunction(uint32_t function);
bool getCustomerBootLoader(uint32_t hdr_entry[NUM_HDR_ENTRIES],uint32_t function);
bool getCustomerApplication(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t function);
bool getEmCoreApplication(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t function);
bool getFirmwareUpdater(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t function);

void loadBootLoaderFunction(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t* function);
void loadEmCoreFunction(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t* function);
void loadCustomerApplication(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t* function);
void loadCustomerBootLoader(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t* function);
void loadEmCoreApplication(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t* function);
void getMainFunction(uint32_t hdr_entry[NUM_HDR_ENTRIES], uint32_t* function);
void jumpToFunction(uint32_t function);



#endif // _NVM_BOOTLOADER_H
