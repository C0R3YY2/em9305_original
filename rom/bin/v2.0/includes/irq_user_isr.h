////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/irq_user_isr.h
///
/// @project    T9305
///
/// @brief      IRQ User interrupt servise routine
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

#ifndef _IRQ_USER_IRQ_H
#define _IRQ_USER_IRQ_H


/******************************************************************************\
 *  Globals
\******************************************************************************/

/******************************************************************************\
 *  Public functions
\******************************************************************************/

/**
 * @brief Set the User IRQ SW1 Handler function pointer.
 * @param pIRQUserHandlerGPIO function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerSW1(void* pIRQUserHandlerSW1);

/**
 * @brief Set the User IRQ GPIO Handler function pointer.
 * @param pIRQUserHandlerGPIO function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerGPIO(void* pIRQUserHandlerGPIO);

/**
 * @brief Set the User IRQ Sleep Timer Out Cmp Handler function pointer.
 * @param pIRQUserHandler function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerSleepTimerOutCmp(void* pIRQUserHandler);

/**
 * @brief Set the User IRQ Sleep Timer Full Value Handler function pointer.
 * @param pIRQUserHandler function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerSleepTimerFullValue(void* pIRQUserHandler);

/**
 * @brief Set the User IRQ Proto Timer Out Cmp Handler function pointer.
 * @param pIRQUserHandler function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerProtoTimerOutCmp(void* pIRQUserHandler);

/**
 * @brief Set the User IRQ Proto Timer Sync Handler function pointer.
 * @param pIRQUserHandler function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerProtoTimerSync(void* pIRQUserHandler);

/**
 * @brief Set the User IRQ ARC Timer 0 Handler function pointer.
 * @param pIRQUserHandler function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerARCTimer0(void* pIRQUserHandler);

/**
 * @brief Set the User IRQ ARC Timer 1 Handler function pointer.
 * @param pIRQUserHandler function pointer of the user callback
 */
void IRQ_SetIRQUserHandlerARCTimer1(void* pIRQUserHandler);

/******************************************************************************\
 *  USR ISR
\******************************************************************************/

#endif // _IRQ_USER_ISR_H
