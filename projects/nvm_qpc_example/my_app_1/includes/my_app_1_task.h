////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app_1_task.h
///
/// @project    T9305
///
/// @brief      Application task 1
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

#ifndef MY_APP_1_TASK_H
#define MY_APP_1_TASK_H

#include <my_app_signals.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Maximum number of events for the application task.
#define MY_APP_1_TASK_EVENTS_QUEUE_SIZE      (4u)

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Create the application task.
 */
void MyApp1Task_Create(void);

/**
 * @brief Start the application task.
 * @note The task priority shall be smaller than the BLE task! The variable
 * gNextAvailablePriority is used to get the next available priority. So this
 * function shall be called after BLE task has been started to be sure the
 * priorities are correct.
 */
void MyApp1Task_Start(void);

/**
 * @brief Post an event to the application task.
 * @param signal Signal number of the event.
 * @param pParams Pointer to the event parameters (can be NULL).
 */
void MyApp1Task_PostEvent(MyApp_Signals_t signal, void *pParams);

#endif // MY_APP_1_TASK_H
