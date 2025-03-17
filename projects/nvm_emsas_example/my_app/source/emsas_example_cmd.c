////////////////////////////////////////////////////////////////////////////////
///
/// @file       emsas_example_cmd.c
///
/// @project    T9305
///
/// @brief      EMSAS example command parser use NVM SW.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic
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

#include "emsas_example_cmd.h"
#include <em_transport_manager.h>
#include <my_app_signals.h>
#include <my_app_task.h>

/******************************************************************************\
 *  Protected function definitions
\******************************************************************************/

/**
 * @brief Command parser - EMSAS Timer enable/disable Command.
 * @param opcode Operation code of the command that trigger this handler.
 * @param cmdParamsLength Length of the command parameters.
 * @param pCmdParams Pointer to the command parameters.
 * @param maxEventParamsLength Maximum length of the event parameters.
 * @param pEventParams Pointer to a buffer for the event parameters.
 */
void EMSAS_TimerCmd(const uint16_t opcode,
        const uint16_t cmdParamsLength, const uint8_t *pCmdParams,
        const uint16_t maxEventParamsLength, uint8_t *pEventParams)
{
    // assume success
    uint8_t errorCode = ERROR_CODE_SUCCESS;
    uint8_t returnParamsLength = 1u;

    // Double check if the security is sufficient to execute the command.
    if (EMSystem_IsSecuritySufficient(EMSAS_AUTHMIN_TIMER_TEST))
    {
        // Check parameters.
        if (cmdParamsLength != 1u)
        {
            // Invalid parameters.
            errorCode = ERROR_CODE_INVALID_HCI_COMMAND_PARAMETERS;
        }
        else
        {
            // Set the timer active state
            switch (pCmdParams[0u])
            {
                case 0:
                    // deactivate the timer event notifications
                    MyAppTask_PostEvent(EVENT_STOP_SIG, NULL);
                break;
                case 1:
                    // activate the timer event notifications
                    MyAppTask_PostEvent(EVENT_START_SIG, NULL);
                break;
                default:
                    // Invalid parameters.
                    errorCode = ERROR_CODE_INVALID_HCI_COMMAND_PARAMETERS;
                break;
            }
        }
    }
    else
    {
        // Insufficient Security.
        errorCode = ERROR_CODE_INSUFFICIENT_SECURITY;
    }

    // Send the response event.
    pEventParams[EVENT_COMMAND_COMPLETE_OFFSET_PARAMETER_0] = errorCode;
    (void) EMTransportManager_SendCommandCompleteEvent(
            opcode,
            returnParamsLength,
            maxEventParamsLength,
            pEventParams,
            NULL);
}
