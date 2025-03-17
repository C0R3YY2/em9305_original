////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/emdt/profile/client/source/emdtp_client.c
///
/// @project    T9305
///
/// @brief      EM Data Transfer Profile - Client side - source file.
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

#include <emdtp_client.h>
#include <emdt_svc.h>
#include <app_api.h>

/******************************************************************************\
 *  Types
\******************************************************************************/

#define EMDTP_OPCODE_NONE       0x0
#define EMDTP_OPCODE_DISABLE    0x1
#define EMDTP_OPCODE_ENABLE     0x2

typedef struct
{
    uint8_t lastOpcode;
    uint16_t* pHdlList;
} EmdtpClient_ConnCb_t;

/** @brief Control block type definition */
typedef struct
{
    EmdtpClient_Cfg_t   cfg;   /**< Configurable parameters */
    EmdtpClient_Cback_t cback; /**< Registered user callback */
    EmdtpClient_ConnCb_t conn[DM_CONN_MAX];
} EmdtpClient_Cb_t;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/** @brief Control block */
static EmdtpClient_Cb_t gEmdtpClient_Cb;

/******************************************************************************\
 *  Local variables
\******************************************************************************/

/* UUIDs */
static const uint8_t gcEmmPSvcUuid[] = {UUID_EM_DATA_TRANSFER_SERVICE}; // EMM Proprietary service
static const uint8_t gcEmmDChUuid[] = {UUID_EM_DATA_TRANSFER_DATA};     // EMM Proprietary data

/** @brief Proprietary data characteristic */
static const attcDiscChar_t gcEmmDataChar =
{
    .pUuid = gcEmmDChUuid,
    .settings = ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

/** @brief Proprietary data descriptor */
static const attcDiscChar_t gcEmmDataCCC =
{
    .pUuid = attCliChCfgUuid,
    .settings = ATTC_SET_REQUIRED | ATTC_SET_DESCRIPTOR
};

/** @brief List of characteristics to be discovered; order matches handle index enumeration  */
static const attcDiscChar_t *gcEmmCharList[] =
{
    &gcEmmDataChar, // Proprietary data
    &gcEmmDataCCC   // Proprietary data descriptor
};

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

static void EmdtpClient_ProcAttcWriteRsp(attEvt_t *pMsg)
{
    dmConnId_t connId = (dmConnId_t)pMsg->hdr.param;
    EmdtpClient_Event_t evt;

    // If the handle list is not null
    // and the handle is for the EMDT client characteristic configuration characteristic
    // and last opcode is not EMDTP_OPCODE_NONE
    if((gEmdtpClient_Cb.conn[connId - 1].pHdlList != NULL) &&
        (pMsg->handle == gEmdtpClient_Cb.conn[connId - 1].pHdlList[EMM_P_NA_CCC_HDL_IDX]) &&
        (gEmdtpClient_Cb.conn[connId - 1].lastOpcode != EMDTP_OPCODE_NONE))
    {
        if(pMsg->hdr.status == ATT_SUCCESS)
        {
            // Set the event type based on opcode written to server
            if(gEmdtpClient_Cb.conn[connId - 1].lastOpcode == EMDTP_OPCODE_ENABLE)
            {
                evt.type =  EMDTPCLIENT_TRANSFER_ENABLED_EVENT;
            }
            else if (gEmdtpClient_Cb.conn[connId - 1].lastOpcode == EMDTP_OPCODE_DISABLE)
            {
                evt.type =  EMDTPCLIENT_TRANSFER_DISABLED_EVENT;
            }

            // Set event
            evt.params.writeRsp.pEvt = pMsg;

            // Callback to user application
            if (gEmdtpClient_Cb.cback != NULL)
            {
                gEmdtpClient_Cb.cback(&evt, gEmdtpClient_Cb.cfg.pUserData);
            }
        }
        // Reset last opcode
        gEmdtpClient_Cb.conn[connId - 1].lastOpcode = EMDTP_OPCODE_NONE;
    }
}

static void EmdtpClient_ProcAttcValueNtf(attEvt_t *pMsg)
{
    dmConnId_t connId = (dmConnId_t)pMsg->hdr.param;
    EmdtpClient_Event_t evt;

    if(pMsg->handle == gEmdtpClient_Cb.conn[connId - 1].pHdlList[EMM_P_DAT_HDL_IDX])
    {
        if(pMsg->hdr.status == ATT_SUCCESS)
        {
            // Set event type
            evt.type = EMDTPCLIENT_TRANSFER_DATA_EVENT;

            // Set event
            evt.params.notif.pEvt = pMsg;

            // Call back to user application
            if (gEmdtpClient_Cb.cback != NULL)
            {
                gEmdtpClient_Cb.cback(&evt, gEmdtpClient_Cb.cfg.pUserData);
            }
        }
    }
}

static void EmdtpClient_ToggleNotifications(dmConnId_t connId, bool enable)
{
    uint16_t notifCfg;
    uint16_t handle = gEmdtpClient_Cb.conn[connId - 1].pHdlList[EMM_P_NA_CCC_HDL_IDX];

    if (handle != ATT_HANDLE_NONE)
    {
        if(enable)
        {
            notifCfg = ATT_CLIENT_CFG_NOTIFY;
            gEmdtpClient_Cb.conn[connId - 1].lastOpcode = EMDTP_OPCODE_ENABLE;
        }
        else
        {
            notifCfg = 0U;
            gEmdtpClient_Cb.conn[connId - 1].lastOpcode = EMDTP_OPCODE_DISABLE;
        }

        AttcWriteReq(connId, handle, sizeof(notifCfg), (uint8_t *)&notifCfg);
    }
}

/******************************************************************************\
 *  Public functions definitions
\******************************************************************************/

/**
 *  @brief Set service and characteristic handles.
 *  @param connId Connection identifier.
 *  @param pHdlList Characteristic handle list.
 *  @note This function can set restored handles after restablishing a link with a bonded device.
 */
void EmdtpClient_SetHandles(dmConnId_t connId, uint16_t* pHdlList)
{
    WSF_ASSERT(connId <= DM_CONN_MAX);

    gEmdtpClient_Cb.conn[connId - 1].pHdlList = pHdlList;
}

/**
 *  @brief Enable EM Data Transfer notifications.
 *  @param connId Connection identifier.
 */
void EmdtpClient_StartDataTransfer(dmConnId_t connId)
{
    WSF_ASSERT(connId <= DM_CONN_MAX);

    // If the handle list is not null
    // and there is not an opcode in progress
    if((gEmdtpClient_Cb.conn[connId - 1].pHdlList != NULL) &&
        (gEmdtpClient_Cb.conn[connId - 1].lastOpcode == EMDTP_OPCODE_NONE))
    {
        EmdtpClient_ToggleNotifications(connId, true);
    }
}

/**
 *  @brief Disable EM Data Transfer notifications.
 *  @param connId Connection identifier.
 */
void EmdtpClient_StopDataTransfer(dmConnId_t connId)
{
    WSF_ASSERT(connId <= DM_CONN_MAX);

    // If the handle list is not null
    // and there is not an opcode in progress
    if((gEmdtpClient_Cb.conn[connId - 1].pHdlList != NULL) &&
        (gEmdtpClient_Cb.conn[connId - 1].lastOpcode == EMDTP_OPCODE_NONE))
    {
        EmdtpClient_ToggleNotifications(connId, false);
    }
}

/**
 * @brief Initialize the EM Data Transfer Profile, client side.
 * @param pCfg Configuration of the module.
 * @param callback Callback function provided by the user.
 */

void EmdtpClient_Init(EmdtpClient_Cfg_t *cfg, EmdtpClient_Cback_t callback)
{
    gEmdtpClient_Cb.cback = callback;
    gEmdtpClient_Cb.cfg = *cfg;
}

/**
 * @brief Perform service and characteristic discovery for EM Datatransfer service.
 *        Parameter pHdlList must point to an array of length \ref EMM_P_HDL_LIST_LEN.
 *        If discovery is successful the handles of discovered characteristics and
 *        descriptors will be set in pHdlList.
 * @param pMsg Pointer to message.
 */

void EmdtpClient_Discover(dmConnId_t connId, uint16_t *pHdlList)
{
    AppDiscFindService(connId, ATT_128_UUID_LEN, (uint8_t *) gcEmmPSvcUuid,
                        EMM_P_HDL_LIST_LEN, (attcDiscChar_t **) gcEmmCharList, pHdlList);
}

/**
 * @brief  Called by application to notify the EMDTP of System Events.
 * @param pMsg   Pointer to the Event
 */

void EmdtpClient_ProcMsg(wsfMsgHdr_t *pEvt)
{
    dmConnId_t connId;

    switch (pEvt->event)
    {
        case DM_CONN_OPEN_IND:
            connId = (dmConnId_t)pEvt->param;
            // Reset opcode for connection
            gEmdtpClient_Cb.conn[connId - 1].lastOpcode = EMDTP_OPCODE_NONE;
            break;

        case ATTC_HANDLE_VALUE_NTF:
            EmdtpClient_ProcAttcValueNtf((attEvt_t *)pEvt);
            break;

        case ATTC_WRITE_RSP:
            EmdtpClient_ProcAttcWriteRsp((attEvt_t *)pEvt);
            break;

        default:
            break;
    }
}
