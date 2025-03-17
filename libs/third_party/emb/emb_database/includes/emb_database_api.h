////////////////////////////////////////////////////////////////////////////////
///
/// @file       emb_database_api.h
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

#ifndef _EMB_DATABASE_API_H
#define _EMB_DATABASE_API_H

#include "dm_api.h"
#include "util/bda.h"
#include "wsf_types.h"


/**
 * @defgroup emb_database EMB Database
 * @brief Libary for persistantly storing BLE keys in NVM.
 *
 * This module contains all the code required to load and store BLE keys as well
 * as some information about the peer device exchanged during pairing.
 * @{
 */

// Public Type Definitions
/**
 * @brief Metadata structure for discovery and compatability in NVM. Do not change.
 */
typedef struct
{
    uint8_t magicNumber;
    uint8_t recordVersionNumber;
    uint8_t recordValid;
} EMBDB_RecordMetadata_t;

/**
 * @brief Record of peer device pairing information to be stored in NVM.
 */
typedef struct
{
    EMBDB_RecordMetadata_t metadata; /*!< Internal metadata for record. Not to be manipulated by the user. */

    bdAddr_t peerAddress;     /*!< Peer's Bluetooth Address (BD_ADDR) */
    uint8_t  peerAddressType; /*!< Peer's Bluetooth Address Type */

    dmSecLtk_t localLtk;         /*!< Local Device LTK (Long term key) */
    uint8_t    localLtkSecLevel; /*!< Local Device LTK security level */

    dmSecLtk_t peerLtk;         /*!< Peer Device LTK (Long term key) */
    uint8_t    peerLtkSecLevel; /*!< Peer Device LTK security level */

    dmSecIrk_t peerIrk; /*!< Peer's IRK (Identity Resolving Key) */

    dmSecCsrk_t peerCsrk;         /*!< Peer's CSRK (Connection Signature Resolving Key) */
    uint8_t     peerCsrkSecLevel; /*!< Peer's CSRK security level */

    union
    {
        uint8_t keyValidMask; /*!< Union of bit flags signally key validity */
        struct
        {
            uint8_t localLtkValid : 1; /*!< 1 if the local LTK (Long-term Key) is valid for this record */
            uint8_t peerLtkValid : 1;  /*!< 1 if the peer LTK (Long-term Key) is valid for this record */
            uint8_t peerIrkValid : 1;  /*!< 1 if the peer IRK (Identity Resolving Key) is valid for this record */
            uint8_t peerCsrkValid : 1; /*!< 1 if the peer CSRK (Connection Signature Resolving Key) is valid for this record */

            uint8_t rfu : 4; /*!< Field reserved for future use. */
        };
    };

    uint32_t recordCrc; /*!< CRC32 for full record + metadata */
} EMBDB_Record_t;

typedef int32_t (*EMBDB_WriteRecordAtAddress_t)(const EMBDB_Record_t *record, const void *storeAddress); /*!< Function to write a record to a memory address. */
typedef int32_t (*EMBDB_InvalidateRecordAtAddress_t)(EMBDB_Record_t *address);                           /*!< Function to invalidate a record given a memory address. */
typedef int32_t (*EMBDB_EraseDB_t)(void);                                                                /*!< Function to erase database memory. */

/**
 * @brief Storage backend to be used by the database library.
 */
typedef struct
{
    EMBDB_EraseDB_t                   eraseDatabase;             /*!< Function to erase database memory. */
    EMBDB_InvalidateRecordAtAddress_t invalidateRecordAtAddress; /*!< Function to invalidate a record given a memory address. */
    EMBDB_WriteRecordAtAddress_t      writeRecordAtAddress;      /*!< Function to write a record to a memory address. */
    uint32_t                          erasedValue;               /*!< Erased value of storage medium. 0xFF for NVM, or 0x00 for DRAM. */
    void                             *storageStartAddress;       /*!< Starting address of desired storage location. Should be page aligned for NVM locations. */
    uint32_t                          databaseSize;              /*!< Desired size of the database in bytes. */
    uint8_t                           maxRecords;                /*!< Maximum number of records that can be stored in the database. */
} EMBDB_StorageBackend_t;

/**
 * @brief Record retrieval status codes.
 */
enum EMBDB_GetRecordResult_e
{
    EMB_DB_GET_RECORD_SUCCESS = 0U,   /*!< Record retrieved successfully. */
    EMB_DB_GET_RECORD_NOT_FOUND,      /*!< No valid record was found. */
    EMB_DB_GET_RECORD_CHECKSUM_FAIL,  /*!< Record was found, but failed the CRC32 checksum .*/
    EMB_DB_GET_RECORD_RECORD_CORRUPT, /*!< Record was found, but corrupted in NVM. */
    EMB_DB_NO_REGISTERED_BACKEND,     /*!< Operation could not be performed because no storage backend was registered. */
};
typedef uint8_t EMBDB_GetRecordResult_t;


// Public Constant Definitions
#define EMB_DB_RECORD_SIZE (sizeof(EMBDB_Record_t))

/**
 * @brief Validate an embDb_storageBackend_t to be registered in the database.
 * Returns 0 if the backend is useable for the EMB database.
 * Note: if a storage device other than DRAM, NVM Info, or NVM Main is to be
 * validated, this function will return -ENXIO status.
 * @param pStorageBackend read/write backend to use for storage.
 *
 * @return Error code @ref errno.h.
 */
int32_t EMBDB_ValidateBackend(EMBDB_StorageBackend_t *pStorageBackend);

/**
 * @brief Register a storage backend with EMB database. This is used to change
 * the read/write location of the database. This can be changed from the default
 * location of NVM Info page 1 to DRAM, or anywhere in NVM Main.
 * @param pStorageBackend read/write backend to use for storage.
 *
 * @return Error code @ref errno.h.
 */
int32_t EMBDB_RegisterBackend(EMBDB_StorageBackend_t *pStorageBackend);

/**
 * @brief Initialize pStorageBackend with functionality to use NVM Info P1 as a
 * storage location for database records.
 * @param pStorageBackend read/write backend to use for storage.
 */
void EMBDB_LoadNVMInfoP1Backend(EMBDB_StorageBackend_t *pStorageBackend);

/**
 * @brief Store given record into the database. Invalidates all existing records
 * with matching peer address.
 * @param record pointer to the record to store
 *
 * @return Error code @ref errno.h.
 */
int32_t EMBDB_StoreRecord(EMBDB_Record_t *record);

/**
 * @brief Invalidate all records found with the given peer address.
 * @param peerAddress BD_ADDR to match.
 *
 * @note This is only supported on devices without ECC flash.
 *
 * @return Number of records invalidated.
 */
uint16_t EMBDB_InvalidateAllRecordsWithPeerAddress(bdAddr_t peerAddress);

/**
 * @brief Read database and return how many records are found, includes both valid and invalid records
 * @return Number of records found
 */
uint16_t EMBDB_GetNumberOfRecords();

/**
 * @brief Read database and return how many records marked as valid are found
 * @return Number of records found
 */
uint16_t EMBDB_GetNumberOfValidRecords();

/**
 * @brief Searches database for the last record added, returns via passed in record pointer
 * @param returnRecord Pointer to a record structure instance to copy the found record into
 * @return Enum signifying the result of the search
 */
EMBDB_GetRecordResult_t EMBDB_GetLastRecordStored(EMBDB_Record_t *returnRecord);

/**
 * @brief Searches database for a record with a given peerAddress.
 * @param peerAddress Peer Address to search with.
 * @param returnRecord Pointer to a record structure instance to copy the found record into.
 * @return Enum signifying the result of the search.
 */
EMBDB_GetRecordResult_t EMBDB_GetRecordByAddress(bdAddr_t peerAddress, EMBDB_Record_t *returnRecord);

/**
 * @brief memsets the record passed in to 0.
 * @param record Record to initialize.
 */
void EMBDB_InitializeRecord(EMBDB_Record_t *record);

/**
 * @brief Determines if the database is full.
 *
 * @return True if database is full or no backend has been registered, False otherwise.
 */
bool EMBDB_IsDatabaseFull(void);

/**
 * @brief Executes a page erase operation on the memory location containing the database.
 *
 * @return Error code @ref errno.h.
 */
int32_t EMBDB_EraseDatabaseMemory(void);

/**
 * @brief Invalidates all records in the database
 *
 * @note This is only supported on devices without ECC flash.
 *
 * @return Number of records invalidated
 */
uint16_t EMBDB_InvalidateAllRecords(void);

/** @} */ // End of group EMB Database

#endif
