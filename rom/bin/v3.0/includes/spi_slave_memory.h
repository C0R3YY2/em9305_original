////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/spi_slave/includes/spi_slave_memory.h
///
/// @project    T9305
///
/// @brief      SPI slave driver memory.
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


#ifndef _SPIS_MEMORY_H
#define _SPIS_MEMORY_H


#include "spi_slave.h"


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/// Maximum number of entries into the SPI slave RX transaction buffer.
#define SPIS_RX_TRANSACTION_BUFFER_SIZE (4)


/// Maximum number of entries into the SPI slave TX transaction buffer.
#define SPIS_TX_TRANSACTION_BUFFER_SIZE (4)


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/**
 * @brief Persistent memory for the SPI slave module.
 */
typedef struct
{
    /// Default configuration bits for the SPI slave.
    SPIS_Config_t configBits;

    /// If the SPI slave module is enabled.
    bool enabled;

    /// Value to set the STS1 byte to.
    uint8_t sts1;
} SPIS_PersistentMemory_t;


/**
 * @brief Non-persistent memory for the SPI slave module.
 */
typedef struct
{
    /// RX transaction buffer.
    SPIS_Transaction_t rxBuffer[SPIS_RX_TRANSACTION_BUFFER_SIZE];

    /// TX transaction buffer.
    SPIS_Transaction_t txBuffer[SPIS_TX_TRANSACTION_BUFFER_SIZE];
} SPIS_NonPersistentMemory_t;


/******************************************************************************\
 *  EXTERNAL GLOBAL VARIABLES
\******************************************************************************/


#ifndef SPIS_MEMORY_INTERNAL


/**
 * @brief Reference to the persistent memory for the SPI slave module.
 * @note Do not use this directly.
 */
extern char gSPIS_PersistentMemory[];


/**
 * @brief Reference to the non-persistent memory for the SPI slave module.
 * @note Do not use this directly.
 */
extern char gSPIS_NonPersistentMemory[];


#endif // SPIS_MEMORY_INTERNAL


#endif /* _SPIS_MEMORY_H */
