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

#include "ble.h"

#include <my_app_signals.h>
#include <app_api.h>
#include <app_main.h>
#include <att_api.h>
#include <att_handler.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <gatt/gatt_api.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <ll_init_api.h>
#include <pal_cfg.h>
#include <pal_sys.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <svc_dis.h>
#include <svc_txp.h>
#include <util/bstream.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <string.h>
#include <emb_dm_api.h>
#include <pal_radio2.h>
#include <hci_api.h>
#include <hci_handler.h>

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not
// the user application.

/// Maximum number of connections.
/// Must be smaller than LL_MAX_CONN.
#define BLE_CONN_MAX    (2u)

// Sanity check on the BLE_CONN_MAX definition.
_Static_assert(BLE_CONN_MAX <= LL_MAX_CONN, "BLE_CONN_MAX cannot be larger than LL_MAX_CONN.");

/// ATT Desired MTU
#define ATT_ACCEPTABLE_MTU          (247)

/// Link layer runtime configuration - TX buffers.
#define LL_RT_CFG_NUM_TX_BUFS       (2u)
/// Link layer runtime configuration - RX buffers.
#define LL_RT_CFG_NUM_RX_BUFS       (2u)
/// Link layer runtime configuration - Maximum number of pending advertising reports.
#define LL_RT_CFG_MAX_ADV_REPORTS   (3u)

/// TX Power (dbm) for advertising and connected states
#define TX_POW_VAL_ADV  (6)
#define TX_POW_VAL_CONN (-3)


/// Advertising data, discoverable mode.
uint8_t gAdvDataDisc[] =
{
    // Flags.
    2,                          // Length.
    DM_ADV_TYPE_FLAGS,          // AD type.
    DM_FLAG_LE_GENERAL_DISC |
    DM_FLAG_LE_BREDR_NOT_SUP,

    // TX Power.
    2,                          // Length.
    DM_ADV_TYPE_TX_POWER,       // AD type.
    0,

    // Service UUID list.
    5,                          // Length.
    DM_ADV_TYPE_16_UUID,        // AD type.
    UINT16_TO_BYTES(ATT_UUID_TX_POWER_SERVICE),
    UINT16_TO_BYTES(ATT_UUID_DEVICE_INFO_SERVICE),
};


/// Scan data, discoverable mode.
uint8_t gScanRespData[] =
{
    // Device name.
    11,                         // Length.
    DM_ADV_TYPE_LOCAL_NAME,     // AD type.
    'E', 'M', '9', '3', '0', '5', '_', 'T', 'X', 'S'
};

uint8_t gAdvDataDiscLen = sizeof(gAdvDataDisc);
uint8_t gScanRespDataLen = sizeof(gScanRespData);


/// Configurable parameters for advertising (application framework).
static const appAdvCfg_t gcAfAdvCfg =
{
    .advDuration = {   0,  0,  0 }, // Advertising durations in ms (0 = infinite) for state 1, state 2 and state 3.
    .advInterval = { 160,  0,  0 }  // Advertising intervals in 0.625 ms units for state 1, state 2 and state 3.
};

/// Configurable parameters for peripheral (application framework).
static const appSlaveCfg_t gcAfSlaveCfg =
{
    .connMax = BLE_CONN_MAX,        // Maximum connections.
};

/// Configurable parameters for connection parameter update (application framework).
static const appUpdateCfg_t gcAfUpdateCfg =
{
    .idlePeriod      = 0,                   // Connection idle period in ms before attempting
                                            // connection parameter update; set to zero to disable.
    .connIntervalMin = 640,                 // Minimum connection interval in 1.25ms units.
    .connIntervalMax = 800,                 // Maximum connection interval in 1.25ms units.
    .connLatency     = 0,                   // Connection latency.
    .supTimeout      = 900,                 // Supervision timeout in 10ms units.
    .maxAttempts     = 5                    // Number of update attempts before giving up.
};

/// ATT configurable parameters (increase MTU).
static const attCfg_t gcAttCfg =
{
    .discIdleTimeout = 15,                    // ATT server service discovery connection idle timeout in seconds
    .mtu             = ATT_ACCEPTABLE_MTU,    // acceptable ATT MTU
    .transTimeout    = ATT_MAX_TRANS_TIMEOUT, // transaction timeout in seconds
    .numPrepWrites   = 4                      // number of queued prepare writes supported by server
};

/// Configurable parameters for security (application framework).
static const appSecCfg_t gcAfSecCfg =
{
    .auth        = DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG, // Authentication and bonding flags.
    .iKeyDist    = 0,                                   // Initiator key distribution flags.
    .rKeyDist    = (uint8_t)DM_KEY_DIST_LTK,            // Responder key distribution flags.
    .oob         = false,                               // true if Out-of-band pairing data is present.
    .initiateSec = true                                 // true to initiate security upon connection.
};

/// SMP security parameter configuration.
static const smpCfg_t gcSmpCfg =
{
    .attemptTimeout    = 500,                   // 'Repeated attempts' timeout in msec.
    .ioCap             = SMP_IO_NO_IN_NO_OUT,   // I/O Capability.
    .minKeyLen         = 16,                    // Minimum encryption key length. Note: while this value can be reduced to 7, it is recommended to keep at 16 to prevent key entropy reduction attacks.
    .maxKeyLen         = 16,                    // Maximum encryption key length.
    .maxAttempts       = 1,                     // Attempts to trigger 'repeated attempts' timeout.
    .auth              = 0,                     // Device authentication requirements.
    .maxAttemptTimeout = 64000,                 // Maximum repeated attempts timeout in msec.
    .attemptDecTimeout = 64000,                 // Time msec before attemptExp decreases.
    .attemptExp        = 2                      // Repeated attempts multiplier exponent.
};

/// Buffer Pool runtime configuration.
/// @details Buffers will be allocated with the first possible match.
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    { .len = 16,                   .num = 4                                                                         },
    { .len = 32,                   .num = 5 + 2 * BLE_CONN_MAX                                                      },
    { .len = 64,                   .num = 2 * BLE_CONN_MAX                                                          },
    { .len = 112,                  .num = BLE_CONN_MAX                                                              },
    { .len = 272,                  .num = LL_RT_CFG_MAX_ADV_REPORTS + LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS },
    { .len = 288,                  .num = 1                                                                         }
};

/*! \brief Control block */
static struct
{
    /// WSF handler ID.
    wsfHandlerId_t    handlerId;
    int8_t            txOutputPowerAdv;
    int8_t            txOutputPowerConn;
} txpsCb;

/******************************************************************************\
 *  ATT Server Configuration.
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// WSF handler ID.
wsfHandlerId_t gBleHandlerId;

/// Persistent BB runtime configuration.
static BbRtCfg_t gBbRtCfg;

/// Persistent LL runtime configuration.
static LlRtCfg_t gLlRtCfg;

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for Transmit Power service used to read the TX Power level.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t TxpReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                     uint16_t offset, attsAttr_t *pAttr)
{
    /* read the power level level and set attribute value */
    int8_t txPower = PalRadioGetTxPower();
    memcpy(pAttr->pValue, &txPower, 1);
    return ATT_SUCCESS;
}


/**
 * @brief Start Advertising.
 */
static void BLE_StartAdv(void)
{
    // Update tx power in ad data
    // Radio implementation may limit upper and lower bounds as well as available steps.
    DmAdvSetAdValue(DM_ADV_TYPE_TX_POWER, 1, (uint8_t *)&txpsCb.txOutputPowerAdv, (uint16_t *)&gAdvDataDiscLen, gAdvDataDisc, (uint16_t)gAdvDataDiscLen);

    // Set advertising and scan response data for discoverable mode.
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, gAdvDataDiscLen, (uint8_t*)gAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, gScanRespDataLen, (uint8_t*)gScanRespData);

    // Set advertising and scan response data for connectable mode.
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
    AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);

    // Start advertising; automatically set connectable/discoverable mode and bondable mode.
    AppAdvStart(APP_MODE_AUTO_INIT);

    // Set transmit power for the advertising state. Use 0xFF to change for all phys.
    // NOTE:- The Tx power may not use the same output power value for first couple advertisements if changed here.
    EMB_DmLEPCSetTxPower(DM_CONN_ID_NONE, EMB_BLE_STATE_ADVERTISING, 0xFF, txpsCb.txOutputPowerAdv);

}

/**
 * @brief Process messages from the Device Manager (DM).
 * @param pMsg Pointer to message.
 */
static void BLE_ProcDmMsg(dmEvt_t *pMsg)
{
    switch (pMsg->hdr.event)
    {
        // Device manager events.
        case DM_RESET_CMPL_IND:
            AttsCalculateDbHash();
            DmSecGenerateEccKeyReq();
            break;

        case DM_ADV_SET_START_IND:
            break;

        case DM_ADV_SET_STOP_IND:
            break;

        case DM_ADV_START_IND:
            // Update the Ad value to the current Tx power once the advertising has started
           DmAdvSetAdValue(DM_ADV_TYPE_TX_POWER, 1, (uint8_t *)&txpsCb.txOutputPowerAdv, (uint16_t *)&gAdvDataDiscLen, gAdvDataDisc, (uint16_t)gAdvDataDiscLen);
           break;

        case DM_ADV_STOP_IND:
            break;

        case DM_CONN_OPEN_IND:
          // Set transmit power for connected state. Use 0xFF to set tx power for for all phys.
           EMB_DmLEPCSetTxPower((dmConnId_t)pMsg->hdr.param, EMB_BLE_STATE_CONNECTED, 0xFF, txpsCb.txOutputPowerConn);
          break;

        case DM_CONN_CLOSE_IND:
            break;

        case DM_SEC_PAIR_CMPL_IND:
            DmSecGenerateEccKeyReq();
            break;

        case DM_SEC_PAIR_FAIL_IND:
            DmSecGenerateEccKeyReq();
            break;

        case DM_SEC_ENCRYPT_IND:
            break;

        case DM_SEC_ENCRYPT_FAIL_IND:
            break;

        case DM_SEC_AUTH_REQ_IND:
            AppHandlePasskey(&pMsg->authReq);
            break;

        case DM_SEC_ECC_KEY_IND:
            DmSecSetEccKey(&pMsg->eccMsg.data.key);
            break;

        case DM_SEC_COMPARE_IND:
            AppHandleNumericComparison(&pMsg->cnfInd);
            break;

        case DM_PRIV_CLEAR_RES_LIST_IND:
            break;

        case DM_HW_ERROR_IND:
            break;

        default:
            break;
    }
}

/**
 * @brief Device Manager event handler.
 * @param pEvt DM event.
 */
static void BLE_DmHandler(dmEvt_t *pEvt)
{
    if (pEvt != NULL)
    {
        // Process advertising and connection-related messages.
        AppSlaveProcDmMsg(pEvt);

        // Process security-related messages.
        AppSlaveSecProcDmMsg(pEvt);

        BLE_ProcDmMsg(pEvt);
    }
}

/**
 * @brief ATT event handler.
 * @param pEvt ATT event.
 */
static void BLE_AttHandler(attEvt_t *pEvt)
{
    if (pEvt != NULL)
    {
        // Process server-related ATT messages.
        AppServerProcAttMsg((wsfMsgHdr_t *)pEvt);
    }
}

/**
 * @brief Process messages from the event handler.
 * @param pMsg Pointer to message.
 */
static void BLE_ProcMsg(wsfMsgHdr_t *pMsg)
{
    switch (pMsg->event)
    {
        // External signals and timer signals.
        case BLE_START_ADV_SIG:
           BLE_StartAdv();
            break;

        default:
            break;
    }
}

/**
 * @brief BLE Task event handler.
 * @param event Unused.
 * @param pMsg Pointer to message.
 */
static void BLE_TaskHandler(wsfEventMask_t event, wsfMsgHdr_t *pEvt)
{
    // unused
    (void)(event);

    if (pEvt != NULL)
    {
        // Perform profile and user interface-related operations.
        BLE_ProcMsg(pEvt);
    }
}

/**
 * @brief Buffer initialization.
 * @details gcBuffPoolDesc variable can be modified to suit the user's needs.
 */
static void BLE_InitBuffer(void)
{
    uint32_t memUsed;

    const uint8_t numPools = sizeof(gcBuffPoolDesc) / sizeof(gcBuffPoolDesc[0]);

    memUsed = WsfBufInit(numPools, (wsfBufPoolDesc_t *)&gcBuffPoolDesc[0]);

    WsfHeapAlloc(memUsed);
}

/**
 * @brief Get the current Link Layer runtime configuration.
 * @param[out] llRtCfg returned Link Layer runtime configuration.
 */
static void BLE_GetCurrentLlRtConfig(LlRtCfg_t *llRtCfg)
{
    LlGetDefaultRunTimeCfg(llRtCfg);
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_LL_PARAM,
                         &llRtCfg->maxAdvSets,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, maxAdvSets)));

#if BT_VER >= LL_VER_BT_CORE_SPEC_5_0
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BLE_PHY,
                         &llRtCfg->phy2mSup,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, phy2mSup)));
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_CTE,
                         &llRtCfg->cteUnused,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, cteUnused)));
#endif
}

/**
 * @brief Get the current Baseband runtime configuration.
 * @param[out] bbRtCfg returned Baseband runtime configuration.
 */
static inline void BLE_GetCurrentBbRtConfig(BbRtCfg_t *bbRtCfg)
{
    PalBbLoadCfg((PalBbCfg_t*)bbRtCfg);
}

/**
 *  @brief Initialize the stack.
 */
static uint32_t BLE_StackInit(LlRtCfg_t *llRtCfg, BbRtCfg_t *bbRtCfg)
{
    bdAddr_t bdAddr;
    wsfHandlerId_t handlerId;

    BLE_GetCurrentBbRtConfig(bbRtCfg);

    BLE_GetCurrentLlRtConfig(llRtCfg);

    // Custom LL configuration.
    llRtCfg->maxConn       = BLE_CONN_MAX;
    llRtCfg->maxAclLen     = ATT_ACCEPTABLE_MTU + L2C_HDR_LEN;
    llRtCfg->numTxBufs     = LL_RT_CFG_NUM_TX_BUFS;
    llRtCfg->numRxBufs     = LL_RT_CFG_NUM_RX_BUFS;
    llRtCfg->maxAdvReports = LL_RT_CFG_MAX_ADV_REPORTS;

    LlInitRtCfg_t llCfg =
    {
        .pBbRtCfg     = bbRtCfg,
        .wlSizeCfg    = 1, // Whitelist size.
        .rlSizeCfg    = 1, // Resolvable list size.
        .plSizeCfg    = 1, // Periodic list size.
        .pLlRtCfg     = llRtCfg,
        .pFreeMem     = WsfHeapGetFreeStartAddress(),
        .freeMemAvail = WsfHeapCountAvailable()
    };

    // Load the Bluetooth address and use it as the seed for the PRAND.
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));

    // Init manual Power Control
    EMB_ManualPcInit();
    llRtCfg->embSpecLlRtCfg.enhancedPowerControl = FALSE;


    uint32_t memUsed = LlInit(&llCfg);

    // Load the Bluetooth address and use it as the seed for the PRAND.
    LlSetBdAddr((uint8_t*)bdAddr);

    // Security Module.
    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    uint16_t aclLength = ATT_ACCEPTABLE_MTU + L2C_HDR_LEN;

    // Host Controller Interface.
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);
    HciSetMaxRxAclLen(aclLength);

    // Device Manager.
    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);
    DmConnInit();
    DmAdvInit();
    DmConnSlaveInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);

    // Logical Link Control and Adaptation Layer Protocol (L2CAP).
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    // Attribute Protocol.
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();

    // Security Manager.
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmprInit();
    SmprScInit();

    // Application framework (AF).
    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);
    AppSlaveInit();
    AppServerInit();

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_TaskHandler);
    gBleHandlerId = handlerId;

    // Initialize the control block
    memset(&txpsCb, 0, sizeof(txpsCb));

    // Store handler ID
    txpsCb.handlerId = handlerId;

    // Set initial tx power for advertising and connected states
    txpsCb.txOutputPowerAdv =   TX_POW_VAL_ADV;
    txpsCb.txOutputPowerConn =  TX_POW_VAL_CONN;

    // Set configuration pointers.
    pAppAdvCfg = (appAdvCfg_t*)&gcAfAdvCfg;
    pAppSlaveCfg = (appSlaveCfg_t*)&gcAfSlaveCfg;
    pAppSecCfg = (appSecCfg_t*)&gcAfSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t*)&gcAfUpdateCfg;
    pSmpCfg = (smpCfg_t*)&gcSmpCfg;
    pAttCfg = (attCfg_t *)&gcAttCfg;

    return memUsed;
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void BLE_Init(void)
{
    uint32_t memUsed;

    PalSysInit();

    memUsed = BLE_StackInit(&gLlRtCfg, &gBbRtCfg);   // Initialize the stack modules and the handlers.

    WsfHeapAlloc(memUsed);

    BLE_InitBuffer();

    // Initialize OS timer.
    WsfTimerInit();

    // Register for DM callbacks.
    DmRegister(BLE_DmHandler);
    DmConnRegister(DM_CLIENT_ID_APP, BLE_DmHandler);
    // Register Callbacks for ATT.
    AttRegister(BLE_AttHandler);
    AttConnRegister(AppServerConnCback);

    // Initialize attribute server database and register callbacks.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();
    SvcTxpCbackRegister(TxpReadCback);
    SvcTxpAddGroup();
    SvcDisAddGroup();

    // Reset the device.
    DmDevReset();
}
