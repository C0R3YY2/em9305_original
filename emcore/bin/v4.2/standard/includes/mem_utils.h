////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/mem_utils.h
///
/// @project    T9305
///
/// @brief      Utilities for doing memory operations.
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


#ifndef _MEM_UTILS_H
#define _MEM_UTILS_H


#include "types.h"


/**
 * @defgroup mem_utils Memory Utilities
 * @ingroup drivers
 * @brief Utilities for doing memory operations.
 */


/**
 * @addtogroup mem_utils
 * @{
 */


/******************************************************************************\
 *  API FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Read an unaligned 16-bit value.
 * @param pAddress Address to read the value from.
 * @returns The value.
 * @note This is a little endian read.
 */
static ALWAYS_INLINE uint16_t Memory_ReadU16(const void *pAddress)
{
    Pointer_t address;

    // Set the pointer to the given address.
    address.pConstVoid = pAddress;

    // Read the unaligned value.
    return ((uint16_t)(address.pUInt8[1] << 8) | address.pUInt8[0]);
}


/**
 * @brief Write a 16-bit value to unaligned memory.
 * @param value 16-bit value to write.
 * @param pAddress Address to write the value to.
 * @note This is a little endian write.
 */
static ALWAYS_INLINE void Memory_WriteU16(uint16_t value, const void *pAddress)
{
    Pointer_t address;

    // Set the pointer to the given address.
    address.pConstVoid = pAddress;

    // Write the unaligned value.
    address.pUInt8[0] = value & 0xFFu;
    address.pUInt8[1] = (value >> 8) & 0xFFu;
}


/**
 * @brief Read an unaligned 32-bit value.
 * @param pAddress Address to read the value from.
 * @returns The value.
 * @note This is a little endian read.
 */
static ALWAYS_INLINE uint32_t Memory_ReadU32(const void *pAddress)
{
    Pointer_t address;

    // Set the pointer to the given address.
    address.pConstVoid = pAddress;

    // Read the unaligned value.
    return (((uint32_t)address.pUInt8[3]) << 24) |
        (((uint32_t)address.pUInt8[2]) << 16) |
        (((uint32_t)address.pUInt8[1]) << 8) |
        ((uint32_t)address.pUInt8[0]);
}


/**
 * @brief Write a 32-bit value to unaligned memory.
 * @param value 32-bit value to write.
 * @param pAddress Address to write the value to.
 * @note This is a little endian write.
 */
static ALWAYS_INLINE void Memory_WriteU32(uint32_t value, const void *pAddress)
{
    Pointer_t address;

    // Set the pointer to the given address.
    address.pConstVoid = pAddress;

    // Write the unaligned value.
    address.pUInt8[0] = value & 0xFFu;
    address.pUInt8[1] = (value >> 8) & 0xFFu;
    address.pUInt8[2] = (value >> 16) & 0xFFu;
    address.pUInt8[3] = (value >> 24) & 0xFFu;
}


/**
 * @brief Copy 32-bit words to a memory address that is 32-bit aligned.
 * @param pDestination Destination memory address. This must be 32-bit aligned.
 * @param pSource Source memory address.
 * @param numWords Number of 32-bit words to copy.
 * @note pSource does not have to be aligned.
 * @note This produces exactly 1 write for each word.
 */
void CopyToAligned(void *pDestination, const void *pSource,
    uint16_t numWords);


/**
 * @brief Copy 32-bit words from a memory address that is 32-bit aligned.
 * @param pDestination Destination memory address.
 * @param pSource Source memory address. This must be 32-bit aligned.
 * @param numWords Number of 32-bit words to copy.
 * @note pDestination does not have to be aligned.
 * @note This produces exactly 1 read for each word.
 */
void CopyFromAligned(void *pDestination, const void *pSource,
    uint16_t numWords);

/**
 * @brief Copy data from memory area pSource to memory area pDestination with swapped endianness.
 * @param pDestination Destination memory area for the copy.
 * @param pSource Source memory area for the copy.
 * @param numBytes Number of bytes to copy.
 * @note The copy is performed byte per byte.
 */
void memcpy_swape(uint8_t *pDestination, const uint8_t *pSource,
    uint16_t numBytes);

/**
 * @brief Copy data from memory area pSource to memory area pDestination.
 * @param pDestination Destination memory area for the copy.
 * @param pSource Source memory area for the copy.
 * @param numBytes Number of bytes to copy. This must be a multiple of 4.
 * @note The number of bytes must be a multiple of 4.
 * @note The source and destination buffers must be aligned.
 * @note This is optimized to use the loop counter (limited to 65535 bytes).
 */
void memcpy32(uint32_t *pDestination, const uint32_t *pSource,
    uint16_t numBytes);

/**
 * @brief Fill the first numBytes of the memory area pDestination with zero.
 * @param pDestination Memory area to fill. Must be 4 byte aligned.
 * @param numBytes Number of bytes to fill. This must be a multiple of 4.
 * @note The number of bytes must be a multiple of 4.
 * @note The destination buffer must be aligned.
 * @note This is optimized to use the loop counter (limited to 65535 bytes).
 */
void memset32(uint32_t *pDestination, uint16_t numBytes);

/**
 * @}
 */


#endif /* _MEM_UTILS_H */
