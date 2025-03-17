////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/nvm/includes/nvm.h
///
/// @project    T9305
///
/// @brief      NVM drivers
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


#ifndef NVM_H
#define NVM_H

#include <arc.h>
#include "memory_map.h"
#include "t9305_nvm.h"
#include "t9305_pml.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup nvm NVM driver
 * @brief Driver for writing or erasing the NVM.
 *
 * This module contains all the code required to write or erase the NVM.
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// Size of a page in byte.
#define NVM_PAGE_SIZE_BYTE      (8192)
/// Size of a page in word.
#define NVM_PAGE_SIZE_WORD      (2048)

/// Size of a row in byte.
#define NVM_ROW_SIZE_BYTE       (512)
/// Size of a row in word.
#define NVM_ROW_SIZE_WORD       (128)

/// Number of pages in the main area.
#define NVM_PAGE_COUNT_MAIN     (64)
/// Number of pages in the information area.
#define NVM_PAGE_COUNT_INFO     (4)

/// Number of words in the main area.
#define NVM_SIZE_MAIN_WORD      (NVM_PAGE_COUNT_MAIN * NVM_PAGE_SIZE_WORD)
/// Number of words in the info area.
#define NVM_SIZE_INFO_WORD      (NVM_PAGE_COUNT_INFO * NVM_PAGE_SIZE_WORD)

/// Value when the NVM is erased.
#define NVM_ERASED_VAL          (0xFFFFFFFF)


/******************************************************************************\
 *  MACROS
\******************************************************************************/

/// AHB address to NVM address conversion for main area.
#define AHB_TO_NVM_ADDR_MAIN_AREA(x) \
    (((x) - (uint32_t)MEMORY_NVM_START) / sizeof(uint32_t))

/// AHB address to NVM address conversion for info area.
#define AHB_TO_NVM_ADDR_INFO_AREA(x) \
    (((x) - (uint32_t)MEMORY_NVMINFO_START) / sizeof(uint32_t))

/// NVM address to AHB address conversion for main area.
#define NVM_TO_AHB_ADDR_MAIN_AREA(x) \
    ((x) * sizeof(uint32_t) + (uint32_t)MEMORY_NVM_START)

/// NVM address to AHB address conversion for info area.
#define NVM_TO_AHB_ADDR_INFO_AREA(x) \
    ((x) * sizeof(uint32_t) + (uint32_t)MEMORY_NVMINFO_START)


/******************************************************************************\
 *  STRUCTURES
\******************************************************************************/

/// NVM Config.
typedef struct
{
    /// Maximum burst size in external-up mode.
    uint8_t maxBurstSizeExtUp;
    /// Delay(us) between burst in external-up mode.
    uint32_t delayUsBetweenBurstExtUp;
} NVM_Config_t;


/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/// NVM Status.
typedef enum
{
    /// Success, the operation was successfull.
    NVM_SUCCESS = 0,
    /// The operation failed because a region was locked.
    NVM_FAILED_LOCK,
    /// The operation failed because of a brown out.
    NVM_FAILED_BROWNOUT,
    /// The operation failed because of a wrong parameter.
    NVM_FAILED_PARAMETER,
} NVM_Status_t;


/// NVM Lock Page type.
typedef enum
{
    /// The page is unlocked.
    NVM_PAGE_UNLOCKED = 0,
    /// The page is locked.
    NVM_PAGE_LOCKED,
    /// Function call parameter error.
    NVM_PAGE_FAILED_PARAMETER,
} NVM_LockPage_t;


/// NVM Lock type.
typedef enum
{
    /// No lock.
    NVM_LOCK_NONE                   = 0,
    /// Lock for Erase main operation.
    NVM_LOCK_OPERATION_ERASE_MAIN   = NVM_LOCK_ERASE_MAIN(1),
    /// Lock for Erase full operation.
    NVM_LOCK_OPERATION_ERASE_FULL   = NVM_LOCK_ERASE_FULL(1),
    /// Lock for Redundancy pages configuration.
    NVM_LOCK_REDUND_CONFIG          = NVM_LOCK_REDUND(1),
    /// Lock for all particular lock bits (Master lock bit).
    NVM_LOCK_MASTER_BIT             = NVM_LOCK_MASTER(1),
} NVM_Lock_t;


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Get the lock of the NVM.
 * @return The lock(s).
 */
static ALWAYS_INLINE NVM_Lock_t NVM_GetLock(void)
{
    return PML->RegNvmLockMaster.r32;
}


/**
 * @brief Lock additional NVM operations.
 *
 * This function is used to lock following operations:
 * - Erase main area @ref NVM_EraseMain().
 * - Erase full (main + information area) @ref NVM_EraseFull().
 * - NVM redundacy pages mapping.
 *
 * This function is also used to lock all particular lock bits to prevent their
 * further modification.
 *
 * @note Once a lock is set it cannot be reverted. Only a POR will unlock it.
 */
static ALWAYS_INLINE void NVM_SetLock(NVM_Lock_t lock)
{
    PML->RegNvmLockMaster.r32 = lock;
}


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Erase main and information areas.
 * @return The status of the operation.
 */
NVM_Status_t NVM_EraseFull(void);

/**
 * @brief Erase main area.
 * @return The status of the operation.
 */
NVM_Status_t NVM_EraseMain(void);

/**
 * @brief Erase a page.
 * @param infoArea The area where the page to erase is located (0 = main area,
 * 1 = info area).
 * @param page The page to erase. Range is 0 to 63 for main area and 0 to 3 for
 * info area.
 * @return The status of the operation.
 */
NVM_Status_t NVM_ErasePage(bool infoArea, uint8_t page);

#if defined(HW_DI) && (HW_DI >= 4)
/**
 * @brief Erase NVM-Main or NMV-Info page(s) based on a physical address and length.
 * @param addr Physical address to start (no adj needed).
 * @param length Number of bytes to write, errors if not 4 byte aligned (no adj needed).
 * @param *pData Pointer to source data.
 * @return The status of the operation.
 */
NVM_Status_t NVM_ErasePages_PhysAddr(uint32_t addr, uint32_t length);

/**
 * @brief Write data to NVM-Main or NMV-Info using only physical address and length
 * @param addr Physical address to start (no adj needed, must be 4 byte aligned).
 * @param length Number of bytes to write, errors if not 4 byte aligned (no adj needed).
 * @param *pData Pointer to source data.
 * @return The status of the operation.
 *
 */
NVM_Status_t NVM_Write_PhysAddr( uint32_t addr, uint32_t length, uint32_t* pData);
#endif

/**
 * @brief Write data to NVM.
 * @param infoArea The area where the page to write is located (0 = main area,
 * 1 = info area).
 * @param addr The address where the data will be written (addressed in word).
 * @param pData A pointer to the data to write.
 * @param length The length of the data to write in word (32 bits). The max
 * length is 2048.
 * @return The status of the operation.
 *
 * @note The address starts at 0x00 and corresponds to 0x300000 for
 * NVM main area and 0x400000 for NVM info area.
 *
 * @note The address is in word so a write to 0x100 corresponds to physical
 * address 0x300400 (main area).
 *
 * @warning It is forbidden to write more than 1 time on the same region (word)
 * if the region is not erased. This function doesn't check if the region was
 * erased before writing.
 *
 * Example:
 * @code
 * // Write 8 words (32 bytes) to address 0x300400 (main area)
 * NVM_Write(0, 0x100, (uint32_t*)buffer, 8);
 * @endcode
 */
NVM_Status_t NVM_Write(bool infoArea,
                       uint32_t addr,
                       uint32_t* pData,
                       uint32_t length);

/**
 * @brief Get the lock of a page.
 * @param infoArea The area where the page is located (0 = main area,
 * 1 = info area).
 * @param page The page number.
 * @return The lock of the page.
 */
NVM_LockPage_t NVM_GetLockPage(bool infoArea, uint8_t page);

/**
 * @brief Lock the write/erase operation for a given page.
 * @param infoArea The area where the page is located (0 = main area,
 * 1 = info area).
 * @param page The page number.
 * @return The status of the operation.
 * @note Once lock is set it cannot be reverted. Only a POR will unlock the page.
 */
NVM_Status_t NVM_LockPage(bool infoArea, uint8_t page);

/**
 * @brief Set the write and erase timings.
 * @param eraseTime Erase time from 0 (9.5ms) to 15 (20ms). The default value is
 * 8 (13.5ms).
 * @param writeTime Write time from 0 (8us) to 15 (16us). The default value is
 * 8 (12us).
 */
void NVM_SetTimings(uint8_t eraseTime, uint8_t writeTime);

/**
 * @brief Set and enable the redundancy page.
 * @param redundancyPage The redundancy page (0 or 1).
 * @param page The page to be replaced by the redundancy page.
 * @param enable Enable the redundancy page.
 */
void NVM_SetRedundancyPageMapping(uint8_t redundancyPage,
                                  uint8_t page,
                                  bool enable);

/**
 * @brief Get the page that is replaced by the redundancy page.
 * @param redundancyPage The redundancy page (0 or 1).
 * @return page The page that is replaced by the redundancy page.
 */
uint8_t NVM_GetRedundancyPageMapping(uint8_t redundancyPage);


/** @} */ // End of group nvm
/** @} */ // End of addtogroup drivers


#endif /* NVM_H */
