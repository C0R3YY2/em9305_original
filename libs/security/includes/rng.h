////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/security/includes/rng.h
///
/// @project    T9305
///
/// @brief      RNG drivers.
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

#ifndef _RNG_H
#define _RNG_H

#include <types.h>
#include <t9305_periph.h>

/**
* @defgroup rng RNG Driver
*/

/******************************************************************************\
*  CONSTANTS
\******************************************************************************/

/// Number of RNG cores.
#define RNG_NUM_CORES   (4)


/******************************************************************************\
 *  MACROS
\******************************************************************************/


/******************************************************************************\
*  TYPE DEFINITIONS
\******************************************************************************/


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/


/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/

/**
* @brief Set the RNG source (RNG core) of the random data.
* @param core RNG core to be used. Valid range is 0..3.
* @ingroup rng
*/
void RNG_SetSource(uint8_t core);

/**
* @brief Get an 8-bit random value.
* @returns A random value.
* @note This method blocks/polls.
* @ingroup rng
*/
uint8_t RNG_Random8(void);

/**
* @brief Get an 16-bit random value.
* @returns A random value.
* @note This method blocks/polls.
* @ingroup rng
*/
uint16_t RNG_Random16(void);

/**
* @brief Get an 32-bit random value.
* @returns A random value.
* @note This method blocks/polls.
* @ingroup rng
*/
uint32_t RNG_Random32(void);

/**
* @brief Fill a buffer with random data.
* @param pData Buffer to fill with random data.
* @param size Number of bytes in buffer to fill with random data.
* @note This method blocks/polls.
* @sa RNG_RandomStart
* @ingroup rng
*/
void RNG_Random(void *pData, uint32_t size);

#endif // _RNG_H
