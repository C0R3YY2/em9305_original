////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app_2.c
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

#include <em_transport_manager.h>
#include "my_app_2.h"
#include <my_app_2_task.h>
#include <my_app_signals.h>
#include <types.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

#define EMSAS_COUNTER_VALUE_UPDATE_EVENT_OPCODE (1u)

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

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

MyApp2_EvtStates_t MyApp2_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp2_EvtStates_t ret = MY_APP_2_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_2_INIT_SIG:
        {
            /* Write your application initialization code here. */
        }
        break;

        case COUNTER_VALUE_UPDATED_SIG:
        {
            // Send the counter value.
            uint32_t counter = (uint32_t)pParams;
            uint8_t eventParams[] = { EVENT_VENDOR_CODE_EMSAS,
                (uint8_t)EMSAS_COUNTER_VALUE_UPDATE_EVENT_OPCODE,
                (uint8_t)(counter >> 24),
                (uint8_t)(counter >> 16),
                (uint8_t)(counter >> 8),
                (uint8_t)(counter) };
            (void) EMTransportManager_SendEvent(EVENT_VENDOR_SPECIFIC, sizeof(eventParams), eventParams, NULL);
        }
        break;

        case MY_APP_2_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_2_EVT_ST_ERROR;
        }
        break;

        /* Add other cases to handle other application signals here. */

        default:
        {
            // Unknown event!
            ret = MY_APP_2_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
