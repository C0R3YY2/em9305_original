////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app.c
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

#include "my_app.h"

#include <ble_task.h>
#include <fwu_cmd.h>
#include <fwu_crypto.h>
#include <fwu_error_code.h>
#include <my_app_signals.h>
#include <my_app_task.h>
#include <types.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

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

MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp_EvtStates_t ret = MY_APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_INIT_SIG:
        {
            /* Write your application initialization code here. */

            // Instruct the BLE task to advertise.
            BleTask_PostEvent(BLE_START_ADV_SIG, NULL);
        }
        break;

        case MY_APP_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_EVT_ST_ERROR;
        }
        break;

        case MY_APP_VERIFY_SIGNATURE_SIG:
        {
            FWU_ErrorCode_t fwuErrorCode;

            FWU_SignatureVerif_t *material = (FWU_SignatureVerif_t *)pParams;

            fwuErrorCode =
                    FWUCrypto_VerifyFirmwareSignature(&material->signature,
                                                      &material->publicKey,
                                                      &material->digest[0]);

            // Post an event to the BLE task.
            BleTask_PostEvent(BLE_SIGNATURE_VERIFIED_SIG,
                              (void *)((uint32_t)fwuErrorCode));
        }
        break;

        /* Add other cases to handle other application signals here. */

        default:
        {
            // Unknown event!
            ret = MY_APP_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
