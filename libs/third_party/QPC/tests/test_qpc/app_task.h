////////////////////////////////////////////////////////////////////////////////
///
/// @file       app_task.h
///
/// @project    T9305
///
/// @brief      Application task
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic-Marin SA
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

#ifndef __APP_TASK_H
#define __APP_TASK_H

#include "types.h"
#include "macros.h"
#include "bsp.h"


/******************************************************************************\
 *  Types
\******************************************************************************/

/// Application task signals enum.
typedef enum
{
    APP_INIT_SIG = Q_USER_SIG,
    APP_READ_STATUS32_SIG,
    APP_ERROR_SIG
} APP_TASK_SIG_t;

/// Application task event states enum.
typedef enum
{
    APP_EVT_ST_HANDLED = 0,
    APP_EVT_ST_UNKNOWN,
    APP_EVT_ST_ERROR,
} APP_TASK_EVT_ST_t;


/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Maximum number of events for the application task.
#define APP_TASK_EVENTS_QUEUE_SIZE      (4u)


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Create the application task.
 */
void APP_TASK_Create(void);

/**
 * @brief Start the application task.
 * @note The task priority shall be smaller than the BLE and USB tasks! The
 * variable gNextAvailablePriority is used to get the next available
 * priority. So this function shall be called after BLE and USB tasks have been
 * started to be sure the priorities are correct.
 */
void APP_TASK_Start(void);

/**
 * @brief Post an event to the application task.
 * @param signal Signal number of the event.
 * @param params Pointer to the event parameters (can be NULL).
 */
void APP_TASK_PostEvent(APP_TASK_SIG_t signal, void *pParams);


#endif // __APP_TASK_H
