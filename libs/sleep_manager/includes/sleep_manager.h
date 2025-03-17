////////////////////////////////////////////////////////////////////////////////
///
/// @file       sleep_manager.h
///
/// @project    T9305
///
/// @brief      Sleep Manager
///
/// @classification  Internal Use
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021, EM Microelectronic
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


#ifndef _SLEEP_MANAGER_H
#define _SLEEP_MANAGER_H


/**
 * @addtogroup libraries
 * @{
 * @defgroup sleep_manager Sleep Manager
 * @brief Sleep Manager library.
 *
 * This module contains all the code required to put the device in sleep mode.
 * @{
*/


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief This function can be called when there is nothing to do to put the
 * device in sleep mode. The device will enter sleep mode if it is enabled and
 * if there is enough time before the next planned event or if it is explicitly
 * required by an EMS command.
 * @param savedInterrupts IRQ threshold returned from the
 * IRQ_DisableAndSaveInterrupts function.
 * @note This function does all the required checks before going to sleep. It
 * also saves all the registered modules. If the sleep mode cannot be entered,
 * the device will enter in CPU sleep mode.
 * @note This function shall be called only by an application that doesn't have
 * a RTOS. Otherwise, it shall be called by the IDLE task of the RTOS.
 * @note This function must be called with interrupts disabled and return with
 * interrupts disabled.
 * @return IRQ threshold.
 */
uint32_t SLEEP_MANAGER_GoToSleep(uint32_t savedInterrupts);


/** @} */ // End of group sleep_manager
/** @} */ // End of addtogroup libraries


#endif /* _SLEEP_MANAGER_H */
