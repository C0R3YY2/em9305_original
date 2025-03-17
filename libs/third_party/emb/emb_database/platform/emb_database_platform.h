////////////////////////////////////////////////////////////////////////////////
///
/// @file       emb_database_internal.h
///
/// @project    EMB
///
/// @brief      EMB Bonding information database
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic-Marin SA
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

#ifndef _EMB_DATABASE_PLATFORM_H
#define _EMB_DATABASE_PLATFORM_H

#include "emb_database_internal.h"
#include "memory_map.h"

#define EMB_DB_NVM_PAGE     ((uint8_t)1U) // NVM INFO Page 1
#define EMB_DB_NVM_AREA     (1U)
#define EMB_DB_NVM_ADDRESS  ((uint32_t)MEMORY_NVMINFO_START + (EMB_DB_NVM_PAGE * (uint32_t)NVM_PAGE_SIZE_BYTE))

// NOTE: These functions are all internal and not meant to be exposed to the
// customer. For the public API, please see emb_database_api.h.

/**
 * @brief Platform-specific get number of bytes an EMB Database record consumes in NVM memory.
 *
 * @return Number of bytes consumed by EMB Database record.
 */
uint32_t embDb_getRecordSizeInMemory(void);


/**
 * @brief Platform-specific invalidate EMB Database record at given address.
 * @param address Starting address of record to invalidate.
 *
 * @return Error code @ref errno.h.
 */
int32_t embDb_invalidateRecordAtAddress(EMBDB_Record_t *address);

/**
 * @brief Platform-specific write record to NVM.
 * @param record Pointer to EMB Database record to be written.
 * @param storeAddress NVM Main address to start writing.
 *
 * @return Error code @ref errno.h.
 */
int32_t embDb_writeRecordToNVMInfoP1AtAddress(const EMBDB_Record_t *record, const void *storeAddress);

/**
 * @brief Platform-specific erase NVM page.
 *
 * @return Error code @ref errno.h.
 */
int32_t embDb_eraseNVMInfoP1(void);

/**
 * @brief Platform-specific Calculate CRC over a given record.
 * @param record EMB Database to calculate CRC on.
 *
 * @return CRC32 of given EMB Database record.
 */
uint32_t embDb_calculateRecordCrc(const EMBDB_Record_t *record);

#endif
