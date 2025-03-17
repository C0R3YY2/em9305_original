////////////////////////////////////////////////////////////////////////////////
///
/// @file       bl_app.c
///
/// @project    T9305
///
/// @brief      Bootloader - Application
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic-Marin SA
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

#include "bl_app_task.h"


/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

APP_TASK_EVT_ST_t APP_HandleEvent(APP_TASK_SIG_t signal, void *pParams);


/******************************************************************************\
 *  Protected functions declarations
\******************************************************************************/

APP_TASK_EVT_ST_t APP_HandleEvent(APP_TASK_SIG_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    APP_TASK_EVT_ST_t ret = APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case APP_INIT_SIG:
        {
            // Nothing to do.
        }
        break;

        case APP_ERROR_SIG:
        {
            // Error!
            ret = APP_EVT_ST_ERROR;
        }
        break;

        default:
        {
            // Unknown event!
            ret = APP_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
