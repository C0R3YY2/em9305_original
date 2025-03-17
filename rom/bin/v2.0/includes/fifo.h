////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/fifo.h
///
/// @project    T9305
///
/// @brief      Hardware FIFO routines for the UART and SPI slave blocks.
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


#ifndef FIFO_H
#define FIFO_H


#include <types.h>


/**
 * @brief Put data into the TX FIFO.
 * @param pData Pointer to data to be put into FIFO.
 * @param dataLength Length of data (in bytes) to be written.
 */
void FIFO_PutTxData(const void *pData, uint8_t dataLength,
    volatile uint8_t *pFifo1b, volatile uint16_t *pFifo2b,
    volatile uint32_t * pFifo4b);


/**
 * @brief Get data from the RX FIFO.
 * @param pData Pointer to the buffer to store data from the FIFO.
 * @param dataLength Length of data (in bytes) to be read.
 */
void FIFO_GetRxData(void *pData, uint8_t dataLength,
    const volatile uint8_t *pFifo1b, const volatile uint16_t *pFifo2b,
    const volatile uint32_t * pFifo4b);


#endif // FIFO_H
