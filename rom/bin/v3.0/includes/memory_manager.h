////////////////////////////////////////////////////////////////////////////////
///
/// @file       memory_manager.h
///
/// @project    T9305
///
/// @brief      The memory manager provides functionality to allocate memory
///             and manage enabling and disabling the physical memories to
///             efficiently accomodate the system's memory requirements.
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

#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include <types.h>
#include <t9305_periph.h>
#include "memory_map.h"


/**
 * @defgroup memory Memory Manager
 * @brief Memory manager.
 */

/**
 * @addtogroup memory
 * @{
 */


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * @brief Initialize the memory manager memory pointers.  This function should
 * be called early in the boot process before any memory allocation requests
 * would be made.  This function should only be called during boot, and not
 * when resuming from sleep.
 * @param pramSize Number of bytes statically allocated in PRAM.
 * @param npramSize Number of bytes statically allocated in NPRAM.
 * @ingroup memory
 */
void Memory_InitEx( uint32_t pramSize, uint32_t npramSize );

/**
 * @brief Return the total number of bytes free in the memory pool.
 * @returns Size in bytes of the available memory in memory pool.
 * @ingroup memory
 */
uint32_t Memory_GetFreeSize( void );

/**
 * @brief Return the total size of the memory pool in bytes.
 * @returns Size of the memory pool in bytes.
 * @ingroup memory
 */
uint32_t Memory_GetPoolSize( void );

/**
 * @brief Allocate the specified number of bytes of persistent memory if available.
 * @param numberOfBytes Number of bytes to reserve.
 * @returns Pointer to the reserved memory block or NULL if the memory
 * could not be allocated.
 * @ingroup memory
 */
void * Memory_AllocatePersistent( uint32_t numberOfBytes );

/**
 * @brief Allocate the specified number of bytes of persistent memory if available.
 * The memory reserved is 32-bit aligned.
 * @param numberOfBytes Number of bytes to reserve.
 * @returns Pointer to the reserved memory block or NULL if the memory
 * could not be allocated.
 * @ingroup memory
 */
void * Memory_AllocatePersistentAligned( uint32_t numberOfBytes );

/**
 * @brief Allocate the specified number of bytes of nonpersistent memory if available.
 * @param numberOfBytes Number of bytes to reserve.
 * @returns Pointer to the reserved memory block or NULL if the memory
 * could not be allocated.
 * @ingroup memory
 */
void * Memory_AllocateNonpersistent( uint32_t numberOfBytes );

/**
 * @brief Allocate the specified number of bytes of nonpersistent memory if available.
 * The memory reserved is 32-bit aligned.
 * @param numberOfBytes Number of bytes to reserve.
 * @returns Pointer to the reserved memory block or NULL if the memory
 * could not be allocated.
 * @ingroup memory
 */
void * Memory_AllocateNonpersistentAligned( uint32_t numberOfBytes );

/**
 * @brief Return the size in bytes of the persistent memory currently allocated.
 * @returns Size in bytes of the allocated persistent memory.
 * @ingroup memory
 */
uint32_t Memory_GetPersistentSize( void );

/**
 * @brief Return the size in bytes of the total amount of persistent memory
 * currently available.  Persistent memory is reserved in blocks.  When the
 * reserved persistent memory is fully allocated, additional persistent memory will
 * be reserved from the memory pool, which decreases the available nonpersistent
 * memory.
 * @returns Size in bytes of the reserved persistent memory.
 * @ingroup memory
 */
uint32_t Memory_GetPersistentReservedSize( void );

/**
 * @brief Return the total size in bytes of the nonpersistent memory currently allocated.
 * @returns Size in bytes of the allocated nonpersistent memory.
 * @ingroup memory
 */
uint32_t Memory_GetNonpersistentSize( void );

/**
 * @brief Return the total size in bytes of the total amount of nonpersistent memory
 * currently available.  Nonpersistent memory is reserved in blocks.  Since the
 * persistent and nonpersistent memory are reserved from the same memory pool, the
 * amount of available nonpersistent memory is reduced as additional persistent
 * memory is reserved.
 * @returns Size in bytes of the reserved persistent memory.
 * @ingroup memory
 */
uint32_t Memory_GetNonpersistentReservedSize( void );


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Initialize the memory manager memory pointers.  This function should
 * be called early in the boot process before any memory allocation requests
 * would be made.  This function should only be called during boot, and not
 * when resuming from sleep.
 * @ingroup memory
 */
static ALWAYS_INLINE void Memory_Init( void )
{
    Memory_InitEx( MEMORY_PRAM_SIZE_USED, MEMORY_NPRAM_SIZE );
}

/** @} (end addtogroup memory) */

#endif  // _MEMORY_MANAGER_H
