////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble_task.c
///
/// @project    T9305
///
/// @brief      Bluetooth task inteface source file.
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

#include "ble.h"

#include <ble_task.h>
#include <interrupts.h>
#include <my_app_signals.h>
#include <types.h>
#include <wsf_msg.h>
#include <wsf_os.h>

/******************************************************************************\
 *  Public functions definition
\******************************************************************************/

void BleTask_Create(void)
{
    WsfOsInitOnly();
}

void BleTask_Start(void)
{
    WsfOsRegisterTaskInitCback(BLE_Init);

    WsfOsStartOnly();
}

void BleTask_PostEvent(MyApp_Signals_t signal, void *pParams)
{
    BleTask_Evt_t *pMsg;
    uint32_t irqThreshold;

    // Critical section since it can be called from another task.
    irqThreshold = IRQ_DisableAndSaveInterrupts();

    if ((pMsg = WsfMsgAlloc(sizeof(BleTask_Evt_t))) != NULL)
    {
        // Make sure the header is properly initialized.
        memset(pMsg, 0, sizeof(wsfMsgHdr_t));
        pMsg->hdr.event = (uint8_t)signal;
        pMsg->pParams = pParams;
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }

    IRQ_RestoreInterrupts(irqThreshold);
}
