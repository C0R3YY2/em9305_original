////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app_1.c
///
/// @project    T9305
///
/// @brief      Application
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

#include "app_timer.h"
#include "my_app_1.h"
#include <my_app_1_task.h>
#include <my_app_2_task.h>
#include <my_app_signals.h>
#include <types.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Time between events.
#define CONFIG_DELTA_TIME_US (1000000ul) // 1000000us -> 1s

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Test counter.
static uint32_t gCounter = 0;

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

MyApp1_EvtStates_t MyApp1_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp1_EvtStates_t ret = MY_APP_1_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_1_INIT_SIG:
        {
            // Initialize the timer peripheral.
            AppTimer_Init();

            // Start the timer.
            SystemTime_t anchorTime = AppTimer_GetCurrentTimestamp();
            SystemTime_t nextEventTime = AppTimer_GetTimestamp(anchorTime, CONFIG_DELTA_TIME_US);
            AppTimer_StartTimer(nextEventTime);
        }
        break;

        case TIMER_TICK_SIG:
        {
            // Re-start the timer.
            SystemTime_t anchorTime = (SystemTime_t)pParams;
            SystemTime_t nextEventTime = AppTimer_GetTimestamp(anchorTime, CONFIG_DELTA_TIME_US);
            AppTimer_StartTimer(nextEventTime);

            // Send the new counter value to the other task.
            gCounter++;
            MyApp2Task_PostEvent(COUNTER_VALUE_UPDATED_SIG, (void*)gCounter);
        }
        break;

        case MY_APP_1_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_1_EVT_ST_ERROR;
        }
        break;

        /* Add other cases to handle other application signals here. */

        default:
        {
            // Unknown event!
            ret = MY_APP_1_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
