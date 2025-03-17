////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/nvm_info/includes/nvm_info.h
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

#define NVM_INFO_P3_ADC_CAL_SOURCE3_SEL_VHI_ADDR             (MEMORY_NVMINFO_START + 0x7BBC)
#define NVM_INFO_P3_ADC_CAL_BLOCK7_ADDR                      (MEMORY_NVMINFO_START + 0x7BB4)
#define NVM_INFO_P3_ADC_CAL_SOURCE3_ADDR                     (MEMORY_NVMINFO_START + 0x7BAC)
#define NVM_INFO_P3_ADC_CAL_BLOCK6_ADDR                      (MEMORY_NVMINFO_START + 0x7BA4)
#define NVM_INFO_P3_ADC_CAL_SOURCE2_ADDR                     (MEMORY_NVMINFO_START + 0x7B9C)
#define NVM_INFO_P3_ADC_CAL_BLOCK5_ADDR                      (MEMORY_NVMINFO_START + 0x7B94)
#define NVM_INFO_P3_ADC_CAL_SOURCE1_ADDR                     (MEMORY_NVMINFO_START + 0x7B8C)
#define NVM_INFO_P3_ADC_CAL_BLOCK4_ADDR                      (MEMORY_NVMINFO_START + 0x7B84)
#define NVM_INFO_P3_ADC_CAL_SOURCE0_RANGE2_ADDR              (MEMORY_NVMINFO_START + 0x7784)
#define NVM_INFO_P3_ADC_CAL_BLOCK3_ADDR                      (MEMORY_NVMINFO_START + 0x777C)
#define NVM_INFO_P3_ADC_CAL_SOURCE0_RANGE1_ADDR              (MEMORY_NVMINFO_START + 0x737C)
#define NVM_INFO_P3_ADC_CAL_BLOCK2_ADDR                      (MEMORY_NVMINFO_START + 0x7374)
#define NVM_INFO_P3_ADC_CAL_SOURCE0_RANGE0_ADDR              (MEMORY_NVMINFO_START + 0x6F74)
#define NVM_INFO_P3_ADC_CAL_BLOCK1_ADDR                      (MEMORY_NVMINFO_START + 0x6F6C)

#define NVM_INFO_P3_TI_CALIBRATION_ADDR                      (MEMORY_NVMINFO_START + 0x6F64)
#define NVM_INFO_P3_TI_CALIBRATION_BLOCK_ADDR                (MEMORY_NVMINFO_START + 0x6F5C)

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

#define NVM_INFO_P2_MAC_ADDRESS_ADDR                        (MEMORY_NVMINFO_START + 0x5C08)
#define NVM_INFO_P2_MAC_ADDRESS_BLOCK_ADDR                  (MEMORY_NVMINFO_START + 0x5C00)

#define NVM_INFO_P2_PML_CONFIGURATION_ADDR                  (MEMORY_NVMINFO_START + 0x5BFC)
#define NVM_INFO_P2_PML_CONFIGURATION_BLOCK_ADDR            (MEMORY_NVMINFO_START + 0x5BF4)

#define NVM_INFO_P2_ADC_CAL_SOURCE3_SEL_VHI_ADDR            (MEMORY_NVMINFO_START + 0x5BB0)
#define NVM_INFO_P2_ADC_CAL_BLOCK7_ADDR                     (MEMORY_NVMINFO_START + 0x5BA8)
#define NVM_INFO_P2_ADC_CAL_SOURCE3_ADDR                    (MEMORY_NVMINFO_START + 0x5BA0)
#define NVM_INFO_P2_ADC_CAL_BLOCK6_ADDR                     (MEMORY_NVMINFO_START + 0x5B98)
#define NVM_INFO_P2_ADC_CAL_SOURCE2_ADDR                    (MEMORY_NVMINFO_START + 0x5B90)
#define NVM_INFO_P2_ADC_CAL_BLOCK5_ADDR                     (MEMORY_NVMINFO_START + 0x5B88)
#define NVM_INFO_P2_ADC_CAL_SOURCE1_ADDR                    (MEMORY_NVMINFO_START + 0x5B80)
#define NVM_INFO_P2_ADC_CAL_BLOCK4_ADDR                     (MEMORY_NVMINFO_START + 0x5B78)
#define NVM_INFO_P2_ADC_CAL_SOURCE0_RANGE2_ADDR             (MEMORY_NVMINFO_START + 0x5778)
#define NVM_INFO_P2_ADC_CAL_BLOCK3_ADDR                     (MEMORY_NVMINFO_START + 0x5770)
#define NVM_INFO_P2_ADC_CAL_SOURCE0_RANGE1_ADDR             (MEMORY_NVMINFO_START + 0x5370)
#define NVM_INFO_P2_ADC_CAL_BLOCK2_ADDR                     (MEMORY_NVMINFO_START + 0x5368)
#define NVM_INFO_P2_ADC_CAL_SOURCE0_RANGE0_ADDR             (MEMORY_NVMINFO_START + 0x4F68)
#define NVM_INFO_P2_ADC_CAL_BLOCK1_ADDR                     (MEMORY_NVMINFO_START + 0x4F60)

#define NVM_INFO_P2_TI_CALIBRATION_ADDR                     (MEMORY_NVMINFO_START + 0x4F58)
#define NVM_INFO_P2_TI_CALIBRATION_BLOCK_ADDR               (MEMORY_NVMINFO_START + 0x4F50)

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// NVM INFO Status.
typedef enum
{
    /// Success, the operation was successful.
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

/**
 * @brief Get the MAC address.
 * @return Pointer to the MAC address (6 bytes).
 * @note Priority: USER MAC address - EM MAC address - NULL.
 */
uint8_t* NVMINFO_GetMacAddress(void);

/**
 * @brief Check VBAT1 at 1.7V in Step Up Mode validity.
 * @return TRUE if the flag is valid, FALSE otherwise.
 */
bool NVMINFO_CheckVBAT1_1p7v_StepUpModeFlag(void);

/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Check the NVM Programmed Flag validity.
 * @return TRUE if the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckNVMProgrammedFlag(void)
{
    bool var = NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P3_NVM_PROGRAMMED_FLAG_ADDR,
            (uint32_t)NVM_INFO_P3_NVM_PROGRAMMED_FLAG_VALUE,
            (uint8_t)NVM_INFO_P3_NVM_PROGRAMMED_FLAG_HAMM_DIST_LIMIT);
    return var;
}

/**
 * @brief Check the EM Authentication Required Flag validity.
 * @return TRUE if the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckEMAuthenticationRequiredFlag(void)
{
    bool var = NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_ADDR,
            (uint32_t)NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_VALUE,
            (uint8_t)NVM_INFO_P3_EM_AUTH_REQUIRED_FLAG_HAMM_DIST_LIMIT);
    return var;
}

/**
 * @brief Check the USER Authentication Required Flag validity.
 * @return TRUE if the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckUSERAuthenticationRequiredFlag(void)
{
    bool var = NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_ADDR,
            (uint32_t)NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_VALUE,
            (uint8_t)NVM_INFO_P2_USER_AUTH_REQUIRED_FLAG_HAMM_DIST_LIMIT);
    return var;
}

/**
 * @brief Check the NVM Read Lock Flag validity.
 * @return TRUE if the flag is valid, FALSE otherwise.
 */
static ALWAYS_INLINE bool NVMINFO_CheckNVMReadLockFlag(void)
{
    bool var = NVMINFO_CheckFlag(
            (uint32_t*)NVM_INFO_P2_NVM_READ_LOCK_FLAG_ADDR,
            (uint32_t)NVM_INFO_P2_NVM_READ_LOCK_FLAG_VALUE,
            (uint8_t)NVM_INFO_P2_NVM_READ_LOCK_FLAG_HAMM_DIST_LIMIT);
    return var;
}

#endif /* _NVM_INFO_H */
