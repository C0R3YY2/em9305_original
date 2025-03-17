////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/uart/includes/uart_memory.h
///
/// @project    T9305
///
/// @brief      UART driver memory.
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


#ifndef _UART_MEMORY_H
#define _UART_MEMORY_H


#include "uart.h"


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/// Maximum number of entries into the UART RX transaction buffer.
#define UART_RX_TRANSACTION_BUFFER_SIZE (4)


/// Maximum number of entries into the UART TX transaction buffer.
#define UART_TX_TRANSACTION_BUFFER_SIZE (4)


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/**
 * @brief Persistent memory for the UART module.
 */
typedef struct
{
    /// Default configuration bits for the UART.
    UART_Config_t configBits;

    /// Baud rate to set the UART to.
    RegUARTBaudRate_t baudRate;

    /// If the UART module is enabled.
    bool enabled;
} UART_PersistentMemory_t;


/**
 * @brief Non-persistent memory for the UART module.
 */
typedef struct
{
    /// RX transaction buffer.
    UART_Transaction_t rxBuffer[UART_RX_TRANSACTION_BUFFER_SIZE];

    /// TX transaction buffer.
    UART_Transaction_t txBuffer[UART_TX_TRANSACTION_BUFFER_SIZE];
} UART_NonPersistentMemory_t;


/******************************************************************************\
 *  EXTERNAL GLOBAL VARIABLES
\******************************************************************************/


// This is defined as an extern to a uint32_T array to prevent it's use within
// the module. Using this within the module would make the memory address
// hardcoded within the ROM.
#ifndef UART_MEMORY_INTERNAL


/**
 * @brief Reference to the persistent memory for the UART module.
 * @note Do not use this directly.
 * @ingroup uart
 */
extern char gUART_PersistentMemory[];


/**
 * @brief Reference to the non-persistent memory for the UART module.
 * @note Do not use this directly.
 */
extern char gUART_NonPersistentMemory[];


#endif // UART_MEMORY_INTERNAL


#endif /* _UART_MEMORY_H */
