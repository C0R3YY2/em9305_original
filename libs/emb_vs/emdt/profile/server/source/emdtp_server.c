////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/emdt/profile/server/source/emdtp_server.c
///
/// @project    T9305
///
/// @brief      EM Data Transfer Profile - Server side - source file.
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

#include <att_api.h>
#include <dm_api.h>
#include <emb_dm_api.h>
#include <emdt_svc.h>
#include <emdtp_server.h>
#include <stdint.h>
#include <wsf_os.h>

/******************************************************************************\
 *  Types
\******************************************************************************/

/** @brief Control block type definition */
typedef struct
{
    EmdtpServer_Cfg_t   cfg;   /**< Configurable parameters */
    EmdtpServer_Cback_t cback; /**< Registered user callback */
} EmdtpServer_Cb_t;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/** @brief Control block */
static EmdtpServer_Cb_t EmdtpServer_Cb;

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

static void EmdtpServer_ProcAttsHandleValueCnf(attEvt_t *pMsg)
{
    dmConnId_t connId = (dmConnId_t)pMsg->hdr.param;

    EmdtpServer_Event_t evt =
    {
        .type = EMDTPSERVER_NOTIFICATION_QUEUED_EVENT,
        .params.notifQueued.connId = connId,
        .params.notifQueued.attStatus = pMsg->hdr.status
    };

    EmdtpServer_Cb.cback(&evt, EmdtpServer_Cb.cfg.pUserData);
}

static void EmdtpServer_ProcAttsCccState(attsCccEvt_t *pMsg)
{
    // Handle EM Data Transfer Service CCC.
    if (pMsg->idx == EmdtpServer_Cb.cfg.dataCharCccIdx)
    {
        dmConnId_t connId = (dmConnId_t)pMsg->hdr.param;

        if (pMsg->value == ATT_CLIENT_CFG_NOTIFY)
        {
            EmdtpServer_Event_t evt =
            {
                .type = EMDTPSERVER_NOTIFICATION_STARTED_EVENT,
                .params.notifStarted.connId = connId
            };

            EmdtpServer_Cb.cback(&evt, EmdtpServer_Cb.cfg.pUserData);
        }
        else
        {
            EmdtpServer_Event_t evt =
            {
                .type = EMDTPSERVER_NOTIFICATION_STOPPED_EVENT,
                .params.notifStopped.connId = connId
            };

            EmdtpServer_Cb.cback(&evt, EmdtpServer_Cb.cfg.pUserData);
        }
    }
}

/******************************************************************************\
 *  Public functions definitions
\******************************************************************************/

void EmdtpServer_Init(EmdtpServer_Cfg_t  *pCfg,
                      EmdtpServer_Cback_t callback)
{
    EmdtpServer_Cb.cfg = *pCfg;
    EmdtpServer_Cb.cback = callback;
}

void EmdtpServer_ProcMsg(wsfMsgHdr_t *pMsg)
{
    switch (pMsg->event)
    {
        case ATTS_HANDLE_VALUE_CNF:
            EmdtpServer_ProcAttsHandleValueCnf((attEvt_t *)pMsg);
            break;

        case ATTS_CCC_STATE_IND:
            EmdtpServer_ProcAttsCccState((attsCccEvt_t *)pMsg);
            break;

        default:
            break;
    }
}
