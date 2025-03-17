////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/nvm_info.h
///
/// @project    T9305
///
/// @brief      NVM information area access
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020, EM Microelectronic
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

#ifndef _NVM_INFO_H
#define _NVM_INFO_H

#include "types.h"
#include "memory_map.h"


/******************************************************************************\
 *  CONSTANTS - NVM INFO P3: EM Configuration Data
\******************************************************************************/

#define NVM_INFO_P3_WAFER_AND_LOT_INFORMATION_ADDR          (MEMORY_NVMINFO_START + 0x7F80)

#define NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_ADDR              (MEMORY_NVMINFO_START + 0x7F08)
#define NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_VALUE             (0x6A53B14D)
#define NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_HAMM_DIST_LIMIT   (4)

#define NVM_INFO_P3_NVM_PROGRAMMED_FLAG_ADDR                (MEMORY_NVMINFO_START + 0x7F00)
#define NVM_INFO_P3_NVM_PROGRAMMED_FLAG_VALUE               (0x4EA6D18B)
#define NVM_INFO_P3_NVM_PROGRAMMED_FLAG_HAMM_DIST_LIMIT     (4)

#define NVM_INFO_P3_EM_AUTHENTICATION_PUBLIC_KEY_ADDR       (MEMORY_NVMINFO_START + 0x7DC8)
#define NVM_INFO_P3_EM_PATCH_SIGNATURE_PUBLIC_KEY_ADDR      (MEMORY_NVMINFO_START + 0x7D88)
#define NVM_INFO_P3_EM_PUBLIC_KEYS_BLOCK_ADDR               (MEMORY_NVMINFO_START + 0x7D80)

#define NVM_INFO_P3_EM_HW_LOCK_BITS_BLOCK_ADDR              (MEMORY_NVMINFO_START + 0x7D00)

#define NVM_INFO_P3_EM_TRIMMING_DATA_BLOCK_ADDR             (MEMORY_NVMINFO_START + 0x7C80)

#define NVM_INFO_P3_DEVICE_ID_ADDR                          (MEMORY_NVMINFO_START + 0x7C48)
#define NVM_INFO_P3_DEVICE_ID_BLOCK_ADDR                    (MEMORY_NVMINFO_START + 0x7C40)

#define NVM_INFO_P3_MAC_ADDRESS_ADDR                        (MEMORY_NVMINFO_START + 0x7C08)
#define NVM_INFO_P3_MAC_ADDRESS_BLOCK_ADDR                  (MEMORY_NVMINFO_START + 0x7C00)


/******************************************************************************\
 *  CONSTANTS - NVM INFO P2: USER Configuration Data
\******************************************************************************/

#define NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_ADDR            (MEMORY_NVMINFO_START + 0x5F08)
#define NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_VALUE           (0x6A53B14D)
#define NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_HAMM_DIST_LIMIT (4)

#define NVM_INFO_P2_NVM_READ_LOCK_FLAG_ADDR                 (MEMORY_NVMINFO_START + 0x5F00)
#define NVM_INFO_P2_NVM_READ_LOCK_FLAG_VALUE                (0x4EA6D18B)
#define NVM_INFO_P2_NVM_READ_LOCK_FLAG_HAMM_DIST_LIMIT      (4)

#define NVM_INFO_P2_USER_PATCH_SIGNATURE_PUBLIC_KEY_ADDR    (MEMORY_NVMINFO_START + 0x5D88)
#define NVM_INFO_P2_USER_PUBLIC_KEYS_BLOCK_ADDR             (MEMORY_NVMINFO_START + 0x5D80)

#define NVM_INFO_P2_USER_HW_LOCK_BITS_BLOCK_ADDR            (MEMORY_NVMINFO_START + 0x5D00)

#define NVM_INFO_P2_USER_TRIMMING_DATA_BLOCK_ADDR           (MEMORY_NVMINFO_START + 0x5C80)


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// NVM INFO Status.
typedef enum
{
    /// Success, the operation was successfull.
    NVMINFO_SUCCESS = 0,
    /// The operation failed because of the block is empty.
    NVMINFO_FAILED_EMPTY,
    /// The operation failed because of an invalid length.
    NVMINFO_FAILED_LENGTH,
    /// The operation failed because of an invalid CRC.
    NVMINFO_FAILED_CRC,
} NVMINFO_Status_t;


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * @brief Check a flag validity.
 * @param pFlag Address of the flag.
 * @param expValue Expected value of the flag.
 * @param hammDistLimit Maximum Hamming distance.
 * @return TRUE is the flag is valid, FALSE otherwise.
 */
bool NVMINFO_CheckFlag(uint32_t *pFlag, uint32_t expValue, uint8_t hammDistLimit);

/**
 * @brief Check if a NVM info block is valid.
 * @param pBlock Pointer to the beginning of the block.
 * @return Status of the operation.
 */
NVMINFO_Status_t NVMINFO_CheckBlock(uint32_t *pBlock);

/**
 * @brief Load a NVM info block (type addr+data).
 * @param pBlock Pointer to the beginning of the block.
 * @return Status of the operation.
 */
NVMINFO_Status_t NVMINFO_LoadBlock(uint32_t *pBlock);


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Check the NVM Programmed Flag validity.
 * @return TRUE is the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckNVMProgrammedFlag(void)
{
    return NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P3_NVM_PROGRAMMED_FLAG_ADDR,
            (uint32_t)NVM_INFO_P3_NVM_PROGRAMMED_FLAG_VALUE,
            (uint8_t)NVM_INFO_P3_NVM_PROGRAMMED_FLAG_HAMM_DIST_LIMIT);
}

/**
 * @brief Check the EM Authentication Required Flag validity.
 * @return TRUE is the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckEMAuthenticationRequiredFlag(void)
{
    return NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_ADDR,
            (uint32_t)NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_VALUE,
            (uint8_t)NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_HAMM_DIST_LIMIT);
}

/**
 * @brief Check the USER Authentication Required Flag validity.
 * @return TRUE is the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckUSERAuthenticationRequiredFlag(void)
{
    return NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_ADDR,
            (uint32_t)NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_VALUE,
            (uint8_t)NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_HAMM_DIST_LIMIT);
}

/**
 * @brief Check the NVM Read Lock Flag validity.
 * @return TRUE is the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckNVMReadLockFlag(void)
{
    return NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P2_NVM_READ_LOCK_FLAG_ADDR,
            (uint32_t)NVM_INFO_P2_NVM_READ_LOCK_FLAG_VALUE,
            (uint8_t)NVM_INFO_P2_NVM_READ_LOCK_FLAG_HAMM_DIST_LIMIT);
}


#endif /* _NVM_INFO_H */
