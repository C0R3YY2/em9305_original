////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble_task.h
///
/// @project    T9305
///
/// @brief      Bluetooth LE task interface file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
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

#ifndef BLE_TASK_H_
#define BLE_TASK_H_

#include <my_app_signals.h>
#include <wsf_os.h>

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Maximum number of events for the Bluetooth task.
#define BLE_TASK_EVENTS_QUEUE_SIZE     NUM_WSF_TASK_EVENTS

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Create the Bluetooth task.
 */
void BleTask_Create(void);

/**
 * @brief Start the Bluetooth task.
 * @note The task priority shall be higher than the application task. The
 * variable gNextAvailablePriority is used to get the next available priority.
 * So this function shall be called before the application task is started to be
 * sure the priorities are correct.
 */
void BleTask_Start(void);

/**
 * @brief Post an event to the Bluetooth task.
 * @details The signal is retrieved in the function BLE_ProcMsg().
 * @note With the current BLE task implementation, the task cannot subscribe to
 * QPC signals with QActive_subscribe() and cannot publish QPC signals with
 * QF_PUBLISH().
 * @param signal Signal number of the event.
 * @param pParams Pointer to the event parameters (can be NULL).
 */
void BleTask_PostEvent(MyApp_Signals_t signal, void *pParams);

#endif // BLE_TASK_H_
