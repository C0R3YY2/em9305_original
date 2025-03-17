////////////////////////////////////////////////////////////////////////////////
///
/// @file       common/includes/9305/memory_map.h
///
/// @project    T9305
///
/// @brief      Pointers and constants for the memory regions.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2015, EM Microelectronic
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

#ifndef _MEMORY_MAP_H
#define _MEMORY_MAP_H

#include <types.h>

/**
 * @defgroup mem Memory Region Constants
 */

/**
 * @brief Pointer to the start of IROM.
 * @ingroup mem
 */
extern uint8_t MEMORY_IROM_START[];

/**
 * @brief Pointer to the first byte after IROM.
 * @ingroup mem
 */
extern uint8_t MEMORY_IROM_END[];

/**
 * @brief Size of IROM in bytes.
 * @ingroup mem
 */
#define MEMORY_IROM_SIZE ((uint32_t)(MEMORY_IROM_END - MEMORY_IROM_START))

/**
 * @brief Pointer to the start of IRAM1.
 * @ingroup mem
 */
extern uint8_t MEMORY_IRAM1_START[];

/**
 * @brief Pointer to the first byte after IRAM1.
 * @ingroup mem
 */
extern uint8_t MEMORY_IRAM1_END[];

/**
 * @brief Size of IRAM1 in bytes.
 * @ingroup mem
 */
#define MEMORY_IRAM1_SIZE ((uint32_t)(MEMORY_IRAM1_END - MEMORY_IRAM1_START))

/**
 * @brief Pointer to the start of IRAM2.
 * @ingroup mem
 */
extern uint8_t MEMORY_IRAM2_START[];

/**
 * @brief Pointer to the first byte after IRAM2.
 * @ingroup mem
 */
extern uint8_t MEMORY_IRAM2_END[];

/**
 * @brief Size of IRAM2 in bytes.
 * @ingroup mem
 */
#define MEMORY_IRAM2_SIZE ((uint32_t)(MEMORY_IRAM2_END - MEMORY_IRAM2_START))

/**
 * @brief Pointer to the start of IRAM0.
 * @ingroup mem
 */
extern uint8_t MEMORY_IRAM0_START[];

/**
 * @brief Pointer to the first byte after IRAM0.
 * @ingroup mem
 */
extern uint8_t MEMORY_IRAM0_END[];

/**
 * @brief Size of IRAM0 in bytes.
 * @ingroup mem
 */
#define MEMORY_IRAM0_SIZE ((uint32_t)(MEMORY_IRAM0_END - MEMORY_IRAM0_START))

/**
* @brief Pointer to the start of NVMENTRY.
* @ingroup mem
*/
extern uint8_t MEMORY_NVMENTRY_START[];

/**
* @brief Pointer to the first byte after NVM.
* @ingroup mem
*/
extern uint8_t MEMORY_NVMENTRY_END[];

/**
* @brief Size of NVM in bytes.
* @ingroup mem
*/
#define MEMORY_NVMENTRY_SIZE ((uint32_t)(MEMORY_NVMENTRY_END - MEMORY_NVMENTRY_START))

/**
* @brief Pointer to the start of NVMCODE.
* @ingroup mem
*/
extern uint8_t MEMORY_NVMCODE_START[];

/**
* @brief Pointer to the first byte after NVM.
* @ingroup mem
*/
extern uint8_t MEMORY_NVMCODE_END[];

/**
* @brief Size of NVM in bytes.
* @ingroup mem
*/
#define MEMORY_NVMCODE_SIZE ((uint32_t)(MEMORY_NVMCODE_END - MEMORY_NVMCODE_START))

/**
* @brief Pointer to the start of NVM.
* @ingroup mem
*/
extern uint8_t MEMORY_NVM_START[];

/**
* @brief Pointer to the first byte after NVM.
* @ingroup mem
*/
extern uint8_t MEMORY_NVM_END[];

/**
* @brief Size of NVM in bytes.
* @ingroup mem
*/
#define MEMORY_NVM_SIZE ((uint32_t)(MEMORY_NVM_END - MEMORY_NVM_START))

/**
* @brief Pointer to the start of NVMINFO.
* @ingroup mem
*/
extern uint8_t MEMORY_NVMINFO_START[];

/**
* @brief Pointer to the first byte after NVMINFO.
* @ingroup mem
*/
extern uint8_t MEMORY_NVMINFO_END[];

/**
* @brief Size of NVMINFO in bytes.
* @ingroup mem
*/
#define MEMORY_NVMINFO_SIZE ((uint32_t)(MEMORY_NVMINFO_END - MEMORY_NVMINFO_START))

/**
 * @brief Pointer to the start of DRAM0.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM0_START[];

/**
 * @brief Pointer to the first byte after DRAM0.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM0_END[];

/**
 * @brief Size of DRAM0 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM0_SIZE ((uint32_t)(MEMORY_DRAM0_END - MEMORY_DRAM0_START))

/**
 * @brief Pointer to the start of DRAM1.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM1_START[];

/**
 * @brief Pointer to the first byte after DRAM1.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM1_END[];

/**
 * @brief Size of DRAM1 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM1_SIZE ((uint32_t)(MEMORY_DRAM1_END - MEMORY_DRAM1_START))

/**
 * @brief Pointer to the start of DRAM2.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM2_START[];

/**
 * @brief Pointer to the first byte after DRAM2.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM2_END[];

/**
 * @brief Size of DRAM2 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM2_SIZE ((uint32_t)(MEMORY_DRAM2_END - MEMORY_DRAM2_START))

/**
 * @brief Pointer to the start of DRAM3.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM3_START[];

/**
 * @brief Pointer to the first byte after DRAM3.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM3_END[];

/**
 * @brief Size of DRAM3 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM3_SIZE ((uint32_t)(MEMORY_DRAM3_END - MEMORY_DRAM3_START))

/**
 * @brief Pointer to the start of DRAM4.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM4_START[];

/**
 * @brief Pointer to the first byte after DRAM4.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM4_END[];

/**
 * @brief Size of DRAM4 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM4_SIZE ((uint32_t)(MEMORY_DRAM4_END - MEMORY_DRAM4_START))

/**
 * @brief Pointer to the start of DRAM5.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM5_START[];

/**
 * @brief Pointer to the first byte after DRAM5.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM5_END[];

/**
 * @brief Size of DRAM5 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM5_SIZE ((uint32_t)(MEMORY_DRAM5_END - MEMORY_DRAM5_START))

/**
 * @brief Pointer to the start of DRAM6.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM6_START[];

/**
 * @brief Pointer to the first byte after DRAM6.
 * @ingroup mem
 */
extern uint8_t MEMORY_DRAM6_END[];

/**
 * @brief Size of DRAM6 in bytes.
 * @ingroup mem
 */
#define MEMORY_DRAM6_SIZE ((uint32_t)(MEMORY_DRAM6_END - MEMORY_DRAM6_START))

/**
 * @brief Pointer to the start of PRAM.
 * @ingroup mem
 */
extern uint8_t MEMORY_PRAM_START[];

/**
 * @brief Pointer to the first byte after PRAM.
 * @ingroup mem
 */
extern uint8_t MEMORY_PRAM_END[];

/**
 * @brief Size of PRAM in bytes.
 * @ingroup mem
 */
#define MEMORY_PRAM_SIZE ((uint32_t)(MEMORY_PRAM_END - MEMORY_PRAM_START))

/**
 * @brief Pointer to the first byte after actually used PRAM.
 * @ingroup mem
 */
extern uint8_t MEMORY_PRAM_USED_END[];

/**
 * @brief Size of allocated PRAM in bytes used by linker.
 * @ingroup mem
 */
#define MEMORY_PRAM_SIZE_USED ((uint32_t)(MEMORY_PRAM_USED_END - MEMORY_PRAM_START))

/**
 * @brief Pointer to the start of POOL.
 * @ingroup mem
 */
extern uint8_t MEMORY_POOL_START[];

/**
 * @brief Pointer to the first byte after POOL.
 * @ingroup mem
 */
extern uint8_t MEMORY_POOL_END[];

/**
 * @brief Size of POOL in bytes.
 * @ingroup mem
 */
#define MEMORY_POOL_SIZE ((uint32_t)(MEMORY_POOL_END - MEMORY_POOL_START))

/**
 * @brief Pointer to the start of NPRAM.
 * @ingroup mem
 */
extern uint8_t MEMORY_NPRAM_START[];

/**
 * @brief Pointer to the first byte after NPRAM.
 * @ingroup mem
 */
extern uint8_t MEMORY_NPRAM_END[];

/**
 * @brief Size of NPRAM in bytes.
 * @ingroup mem
 */
#define MEMORY_NPRAM_SIZE ((uint32_t)(MEMORY_NPRAM_END - MEMORY_NPRAM_START))

/**
 * @brief Determine if the memory address is within persistent memory.
 * @param pAddr Memory address to check.
 * @returns true if the memory address is within persistent memory, false
 * otherwise.
 * @ingroup mem
 */
static ALWAYS_INLINE bool MEM_AddressInPersistentMemory(void *pAddr)
{
    // The only persistent memory section is PRAM.
    return (uint8_t*)pAddr >= MEMORY_PRAM_START &&
        (uint8_t*)pAddr < MEMORY_PRAM_END;
}

/**
 * @brief Determine if the memory address is within DRAM1 memory.
 * @param pAddr Memory address to check.
 * @returns true if the memory address is within DRAM1 memory, false
 * otherwise.
 * @ingroup mem
 */
static ALWAYS_INLINE bool MEM_AddressInDram1Memory(void *pAddr)
{
    // The only persistent memory section is DRAM0.
    return (uint8_t*)pAddr >= MEMORY_DRAM1_START &&
        (uint8_t*)pAddr < MEMORY_DRAM1_END;
}
// @todo TODO: more similar API can be added for DRAM0-DRAM6, IRAM0-IRAM2, IROM, NVM

/**
 * @brief Determine if the memory address is within DRAM1 memory.
 * @param pAddr Memory address to check.
 * @returns true if the memory address is within DRAM1 memory, false
 * otherwise.
 * @ingroup mem
 */
static ALWAYS_INLINE bool MEM_AddressInIram0Memory(void *pAddr)
{
    // The only persistent memory section is DRAM0.
    return (uint8_t*)pAddr >= MEMORY_IRAM0_START &&
        (uint8_t*)pAddr < MEMORY_IRAM0_END;
}

/**
 * @brief Pointer to the start of the IVT.
 * @ingroup mem
 */
extern uint8_t MEMORY_IVT_START[];

/**
 * @brief Pointer to the first byte after the IVT.
 * @ingroup mem
 */
extern uint8_t MEMORY_IVT_END[];

/**
 * @brief Size of the IVT in bytes.
 * @ingroup mem
 */
#define MEMORY_IVT_SIZE ((uint32_t)(MEMORY_IVT_END - MEMORY_IVT_START))

 /**
 * @brief Pointer to the start of the JLI table (in ROM).
 * @ingroup mem
 */
extern uint8_t MEMORY_JLI_START[];

/**
 * @brief Pointer to the first byte after the JLI table (in rom).
 * @ingroup mem
 */
extern uint8_t MEMORY_JLI_END[];

/**
 * @brief Size of the JLI table (in ROM) in bytes.
 * @ingroup mem
 */
#define MEMORY_JLI_SIZE ((uint32_t)(MEMORY_JLI_END - MEMORY_JLI_START))

#endif /* _MEMORY_MAP_H */
