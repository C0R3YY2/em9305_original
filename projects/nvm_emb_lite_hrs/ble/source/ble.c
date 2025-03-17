////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble.c
///
/// @project    T9305
///
/// @brief      Bluetooth LE management file.
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

#include <att_api.h>
#include <att_handler.h>
#include <bas/bas_api.h>
#include <dm_api.h>
#include <gatt/gatt_api.h>
#include <hrps/hrps_api.h>
#include <svc_batt.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <svc_dis.h>
#include <svc_hrs.h>
#include <util/bstream.h>
#include <wsf_msg.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <string.h>
#include <emb_lite_api.h>
#include <my_app_signals.h>
#include <my_app_task.h>

#include "ble.h"
#include "embLite_config.h"

/******************************************************************************\
 *  Global variables
\******************************************************************************/
/// WSF handler ID.
wsfHandlerId_t gBLE_SubsysHandlerId;

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

/// Heart rate measurement configuration.
static const hrpsCfg_t gcHrpsCfg =
{
    .period = 1000      // Measurement timer expiration period in ms.
};

/// Battery measurement configuration.
static const basCfg_t gcBasCfg =
{
    .period    = 30,    // Battery measurement timer expiration period in seconds.
    .count     = 1,     // Perform battery measurement after this many timer periods.
    .threshold = 100    // Send battery level notification to peer when below this level.
};


/******************************************************************************\
 *  ATT Server Configuration.
\******************************************************************************/

/// Enumeration of client characteristic configuration descriptors.
enum
{
    BLE_GATT_SC_CCC_IDX,    // GATT service, service changed characteristic.
    BLE_HRS_HRM_CCC_IDX,    // Heart rate service, heart rate monitor characteristic.
    BLE_BATT_LVL_CCC_IDX,   // Battery service, battery level characteristic.
    BLE_NUM_CCC_IDX
};

/// Client characteristic configuration descriptors settings, indexed by above enumeration.
static const attsCccSet_t gcCccSet[BLE_NUM_CCC_IDX] =
{
    [BLE_GATT_SC_CCC_IDX]  = { .handle     = GATT_SC_CH_CCC_HDL,
                               .valueRange = ATT_CLIENT_CFG_INDICATE,
                               .secLevel   = DM_SEC_LEVEL_NONE },
    [BLE_HRS_HRM_CCC_IDX]  = { .handle     = HRS_HRM_CH_CCC_HDL,
                               .valueRange = ATT_CLIENT_CFG_NOTIFY,
                               .secLevel   = DM_SEC_LEVEL_NONE },
    [BLE_BATT_LVL_CCC_IDX] = { .handle     = BATT_LVL_CH_CCC_HDL,
                               .valueRange = ATT_CLIENT_CFG_NOTIFY,
                               .secLevel   = DM_SEC_LEVEL_NONE }
};

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/
static void BLE_ProcessMsg(EMBLite_Msg_t *pMsg);
static void BLE_ProcAttsCccState(attsCccEvt_t *pMsg);
static void BLE_AttsCccCback(attsCccEvt_t *pEvt);
static void BLE_SubsysHandler(wsfEventMask_t event, wsfMsgHdr_t *pEvt);

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/
/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pMsg Pointer to message.
 */
static void BLE_SubsysHandler(wsfEventMask_t event, wsfMsgHdr_t *pEvt)
{
    // Process BLE ATT and task related messages
    switch (pEvt->event)
    {
        // External signals and timer signals.
        case BLE_START_ADV_SIG:
        {
            // Instruct the BLE task to advertise.
            EMBLite_StartAdvertising(&gMyEMBLite_Config);
        }
        break;

        case ATTS_CCC_STATE_IND:
        {
            BLE_ProcAttsCccState((attsCccEvt_t *)pEvt);
        }
        break;

        case BLE_HR_TIMER_SIG:
        {
            HrpsProcMsg(pEvt);
        }
        break;

        case BLE_BATT_TIMER_SIG:
        {
            BasProcMsg(pEvt);
        }
        break;

        default:
            break;
    }

}

/**
 * @brief ATTS client characteristic configuration callback.
 * @param pEvt ATTS CCC event.
 */
static void BLE_AttsCccCback(attsCccEvt_t *pEvt)
{
    attsCccEvt_t  *pMsg;
    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attsCccEvt_t))) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attsCccEvt_t));
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief Process messages from profiles, subsystems and other tasks.
 * @param pMsg Pointer to message.
 */
static void BLE_ProcessMsg(EMBLite_Msg_t *pMsg)
{
    // Allow forwarded DM and ATT messages from EMBLite to be processed in desired profiles and services
    HrpsProcMsg((wsfMsgHdr_t *)pMsg);
    BasProcMsg((wsfMsgHdr_t *)pMsg);

    switch (pMsg->hdr.event)
    {
        case DM_CONN_CLOSE_IND:
            // Stop heart rate measurement.
            HrpsMeasStop((dmConnId_t)pMsg->hdr.param);
            // Stop battery measurement.
            BasMeasBattStop((dmConnId_t)pMsg->hdr.param);
            break;

        default:
            break;
    }
}

/**
 * @brief Process ATT CCC state change.
 * @param pMsg Pointer to message.
 */
static void BLE_ProcAttsCccState(attsCccEvt_t *pMsg)
{
    switch (pMsg->idx)
    {
        // Handle heart rate measurement CCC.
        case BLE_HRS_HRM_CCC_IDX:
            if (pMsg->value == ATT_CLIENT_CFG_NOTIFY)
            {
                HrpsMeasStart((dmConnId_t)pMsg->hdr.param, BLE_HR_TIMER_SIG, BLE_HRS_HRM_CCC_IDX);
            }
            else
            {
                HrpsMeasStop((dmConnId_t)pMsg->hdr.param);
            }
            break;

        // Handle battery level CCC.
        case BLE_BATT_LVL_CCC_IDX:
            if (pMsg->value == ATT_CLIENT_CFG_NOTIFY)
            {
                BasMeasBattStart((dmConnId_t)pMsg->hdr.param, BLE_BATT_TIMER_SIG, BLE_BATT_LVL_CCC_IDX);
            }
            else
            {
                BasMeasBattStop((dmConnId_t)pMsg->hdr.param);
            }
            break;

        default:
            break;
    }
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void BLE_Init(void)
{
    wsfHandlerId_t handlerId;

    EMBLite_ConfigureStack(&gMyEMBLite_Config);

    EMBLite_InitializeStack(&gMyEMBLite_Config);

    // User defined handler for ATT events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gBLE_SubsysHandlerId = handlerId;

    // Initialize heart rate profile sensor.
    HrpsInit(gBLE_SubsysHandlerId, (hrpsCfg_t*)&gcHrpsCfg);
    HrpsSetFlags(CH_HRM_FLAGS_VALUE_8BIT | CH_HRM_FLAGS_ENERGY_EXP);

    // Initialize battery service server.
    BasInit(gBLE_SubsysHandlerId, (basCfg_t*)&gcBasCfg);

    // Register CCC Callback for ATT.
    AttsCccRegister(BLE_NUM_CCC_IDX, (attsCccSet_t*)gcCccSet, BLE_AttsCccCback);

    // Initialize attribute server database and register callbacks.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();
    SvcHrsCbackRegister(NULL, HrpsWriteCback);
    SvcHrsAddGroup();
    SvcDisAddGroup();
    SvcBattCbackRegister(BasReadCback, NULL);
    SvcBattAddGroup();

    // Set Service Changed CCCD index.
    GattSetSvcChangedIdx(BLE_GATT_SC_CCC_IDX);

    EMBLite_RegisterBLEProcessor((EMBLite_BLESubsys_Proc_t)BLE_ProcessMsg);
    EMBLite_RegisterAppSignaler((AppSignaler_t)MyAppTask_PostEvent);
    EMBLite_StartStack(&gMyEMBLite_Config);
}
