////////////////////////////////////////////////////////////////////////////////
///
/// @file       app.c
///
/// @project    EM9305
///
/// @brief      Application
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023-24, EM Microelectronic-Marin SA
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

#include "att_api.h"
#include "att_handler.h"
#include "dm_api.h"
#include "dm_handler.h"
#include "emb_database_api.h"
#include "gatt/gatt_api.h"
#include "hci_api.h"
#include "hci_handler.h"
#include "l2c_api.h"
#include "l2c_handler.h"
#include "ll_api.h"
#include "smp_api.h"
#include "smp_handler.h"
#include "rpa.h"
#include "svc_batt.h"
#include "svc_core.h"
#include "svc_dis.h"
#include "wsf_os.h"

/******************************************************************************\
 *  External Variables
\******************************************************************************/

extern hciConnSpec_t gcRPAUpdateCfg;

/******************************************************************************\
 *  Global Variable Definitions
\******************************************************************************/

/* RPA Control block structure definition */
rpacb_t gRpaCb = {
    .connId             = 0,
    .connUpdateAttempts = 0,
    .doConnUpdate       = false,
    .bondable           = true,
    .bonded             = false,
    .peerAddr           = {0},
    .peerAddrType       = LL_ADDR_PUBLIC,
};

EMBDB_Record_t gRpaBondRec;
EMBDB_StorageBackend_t gRpaStorageBackend;

// Irk for example purposes.
static const uint8_t localIrk[LL_KEY_LEN] = {0xA6, 0xD9, 0xFF, 0x70, 0xD6, 0x1E, 0xF0, 0xA4, 0x46, 0x5F, 0x8D, 0x68, 0x19, 0xF3, 0xB4, 0x96};

/*! \brief Client characteristic configuration descriptors settings, indexed by above enumeration. */
static const attsCccSet_t gcRPACccSet[RPA_NUM_CCC_IDX] =
{
    // CCCD handle          Value range                 Security level
    {GATT_SC_CH_CCC_HDL, ATT_CLIENT_CFG_INDICATE, DM_SEC_LEVEL_NONE},                 // RPA_GATT_SC_CCC_IDX
    {BATT_LVL_CH_CCC_HDL, ATT_CLIENT_CFG_NOTIFY, DM_SEC_LEVEL_NONE},                  // RPA_BATT_LVL_CCC_IDX
};

/******************************************************************************/
/*!
 *  \brief  Initialize RPA procedure
 *
 *  \param  None
 *  \return None
 */
/******************************************************************************/
void RPA_Init(void)
{
    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    wsfHandlerId_t handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);
    DmConnInit();
    DmConnSlaveInit();
    hciConnSpec_t connUpdate = gcRPAUpdateCfg;
    DmConnSetConnSpec(&connUpdate);

    DmAdvInit();

    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmDevPrivInit();
    DmSecSetLocalIrk((uint8_t *)localIrk);

    DmHandlerInit(handlerId);
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();

    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmprInit();
    SmprScInit();
    HciSetMaxRxAclLen(100);

    DmRegister(RPA_DmCback);
    DmConnRegister(DM_CLIENT_ID_APP, RPA_DmCback);
    AttRegister(RPA_AttCback);
    AttConnRegister(RPA_ServerConnCback);
    AttsCccRegister(RPA_NUM_CCC_IDX, (attsCccSet_t *)gcRPACccSet, RPA_CccCback);

    // Initialize attribute server database.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();
    SvcDisAddGroup();
    SvcBattCbackRegister(BasReadCback, NULL);
    SvcBattAddGroup();

    // Set Service Changed CCCD index.
    GattSetSvcChangedIdx(RPA_GATT_SC_CCC_IDX);

    // The SPA handler processes ATT messages, DM messages (adv, conn, sec), and profile/user-related messages
    handlerId = WsfOsSetNextHandler(RPA_Handler);
    RPA_HandlerInit(handlerId);

    // Handle bonding.
    EMBDB_LoadNVMInfoP1Backend(&gRpaStorageBackend);
    (void)EMBDB_RegisterBackend(&gRpaStorageBackend);
    EMBDB_GetRecordResult_t status = EMBDB_GetLastRecordStored(&gRpaBondRec);
    if (EMB_DB_GET_RECORD_SUCCESS == status)
    {
        if (gRpaBondRec.peerIrkValid == 1)
        {
            gRpaCb.bonded = true;
            BdaCpy(gRpaCb.peerAddr, gRpaBondRec.peerIrk.bdAddr);
            gRpaCb.peerAddrType = gRpaBondRec.peerIrk.addrType;
        }
    }
    else
    {
        EMBDB_InitializeRecord(&gRpaBondRec);
    }

    DmSecGenerateEccKeyReq();
    DmDevReset();
}


/******************************************************************************/
/*!
 *  \brief  Handle QPC events sent via interrupt or init. Responsible for
 * maintaining the app state and receiving QPC messages.
 *
 *  \param  signal QPC event signal
 *  \param  pointer to QPC event parameters (can be NULL).
 *  \return Task state event
 */
/******************************************************************************/
APP_TASK_EVT_ST_t APP_HandleEvent(APP_TASK_SIG_t signal, void *pParams)
{
    (void)pParams;

    // Let's assume the event will be handled successfully.
    APP_TASK_EVT_ST_t ret = APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case APP_INIT_SIG:
        {
            RPA_Init();
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
