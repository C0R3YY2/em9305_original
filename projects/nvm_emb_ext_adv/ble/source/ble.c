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

#include <att_defs.h>
#include <app_api.h>
#include <app_main.h>
#include <att_api.h>
#include <att_handler.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <gatt/gatt_api.h>
#include <hci_api.h>
#include <hci_handler.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <ll_init_api.h>
#include <macros.h>
#include <my_app_signals.h>
#include <pal_cfg.h>
#include <pal_sys.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <string.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <util/bstream.h>
#include <wsf_bufio.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <printf.h>

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not
// the user application.

/** @brief Maximum number of connections.
 *  @note  Must be smaller than LL_MAX_CONN.
 */
#define BLE_CONN_MAX    (1u)

// Sanity check on the BLE_CONN_MAX definition.
_Static_assert(BLE_CONN_MAX <= LL_MAX_CONN,
               "BLE_CONN_MAX cannot be larger than LL_MAX_CONN.");

/** @brief ATT Desired MTU */
#define ATT_ACCEPTABLE_MTU          (251u)

/** @brief Link layer runtime configuration - TX buffers. */
#define LL_RT_CFG_NUM_TX_BUFS       (3 * BLE_CONN_MAX)
/** @brief Link layer runtime configuration - RX buffers. */
#define LL_RT_CFG_NUM_RX_BUFS       (3u)

// Sanity check on the number of runtime buffers.
_Static_assert((LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS) <= UINT8_MAX,
               "Number of runtime buffers has to be less than 255.");

/** @brief Link layer runtime configuration - Maximum number of pending advertising reports. */
#define LL_RT_CFG_MAX_ADV_REPORTS   (3u)

/** @brief Extended Advertising Handle */
#define EXT_ADV_HANDLE              0

/** @brief Number of advertising sets */
#define DM_NUM_ADV_SETS_DEF         1

/******************************************************************************\
 *  Type definition
\******************************************************************************/

/******************************************************************************\
 *  Global/Static Variables.
\******************************************************************************/
/** @brief WSF handler ID. */
wsfHandlerId_t gBLE_SubsysHandlerId;

/** @brief Persistent BB runtime configuration. */
static BbRtCfg_t gBbRtCfg;

/** @brief Persistent LL runtime configuration. */
static LlRtCfg_t gLlRtCfg;

static uint8_t gAdvDataDisc[10 * HCI_ADV_DATA_LEN];

static uint8_t gAdvDataFlags[] =
{
    /* flags */
    DM_FLAG_LE_GENERAL_DISC |      // General discoverable flag
    DM_FLAG_LE_BREDR_NOT_SUP,      // Basic Rate/Enhanced Data Rate not supported flag. Low Energy (LE) only is supported.
};

static uint8_t gAdvDataTxpower[] =
{
    0, // TX Power.
};

/* device name */
static uint8_t gAdvDevName[] =
{
    'E',
    'M',
    'B',
    ' ',
    'E',
    'X',
    'T',
    ' ',
    'A',
    'D',
    'V',
};

/* Manufacturer Data */
static uint8_t gAdvMfgData[] =
{
    (HCI_ID_EMMICROELECTRONIC & 0xFF),
    (HCI_ID_EMMICROELECTRONIC>>8),
    0x95,
    0xf1,
    0x95,
    0xf1,
    0x95,
    0x7b,
    0xd4,
    0xfe,
    0xd4,
    0xfe,
    0xd4,
    0xfe,
    0xe4,
    0xaa,
    0x1e,
    0xaa,
    0x1e,
    0xaa,
    0x1e,
    0x2c,
    0xc6,
    0x38,
    0xc6,
    0x38,
    0xc6,
    0x38,
    0x78,
    0x99,
    0xdf,
    0x99,
    0xdf,
    0x99,
    0xdf,
    0xbd,
    0x60,
    0xbd,
    0x60,
    0xbd,
    0x60,
    0xfb,
    0x4a,
    0xfb,
    0x4a,
    0xfb,
    0x4a,
    0x92,
    0x37,
    0x92,
    0x37,
    0x92,
    0x37,
    0xd4,
    0xfe,
    0xd4,
    0xfe,
    0xd4,
    0xfe,
    0xaa,
    0x1e,
    0xaa,
    0x1e,
    0xaa,
    0x1e,
    0xc6,
    0x38,
    0xc6,
    0x38,
    0xc6,
    0x38,
    0x99,
    0xdf,
    0x99,
    0xdf,
    0x99,
    0xdf,
    0xbd,
    0x60,
    0xbd,
    0x60,
    0xbd,
    0x60,
    0xfb,
    0x4a,
    0xfb,
    0x4a,
    0xfb,
    0x4a,
    0x92,
    0x37,
    0x92,
    0x37,
    0x92,
    0x37,
    0x0a,
    0x4b,
    0x0a,
    0x4b,
    0x0a,
    0x4b
};

/// Configurable parameters for extended advertising (application framework).
static const appExtAdvCfg_t gcAfExtAdvCfg =
{
    .advDuration    = { 0},     // Advertising durations in ms (0 = infinite)
    .advInterval    = { 160},   // Advertising intervals in 0.625 ms units
    .maxEaEvents    = { 0},     // Max events that Controller will send prior to connection termination
    .useLegacyPdu   = {false},  // If used with extended Advertisement then Advertising length cannot be greater than 31 bytes.
    .perAdvInterval = {160}     // Advertising intervals for periodic advertising in 1.25 ms units
};

/** @brief Configurable parameters for peripheral (application framework). */
static const appSlaveCfg_t gcAfSlaveCfg =
{
    .connMax = BLE_CONN_MAX,        // Maximum connections.
};

/** @brief Configurable parameters for connection parameter update (application framework). */
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

/** @brief ATT configurable parameters (increase MTU). */
static const attCfg_t gcAttCfg =
{
    .discIdleTimeout = 15,                    // ATT server service discovery connection idle timeout in seconds
    .mtu             = ATT_ACCEPTABLE_MTU,    // acceptable ATT MTU
    .transTimeout    = ATT_MAX_TRANS_TIMEOUT, // transaction timeout in seconds
    .numPrepWrites   = 4                      // number of queued prepare writes supported by server
};

/** @brief Configurable parameters for security (application framework). */
static const appSecCfg_t gcAfSecCfg =
{
    .auth        = DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG, // Authentication and bonding flags.
    .iKeyDist    = 0,                                   // Initiator key distribution flags.
    .rKeyDist    = (uint8_t)DM_KEY_DIST_LTK,            // Responder key distribution flags.
    .oob         = false,                               // true if Out-of-band pairing data is present.
    .initiateSec = false                                // true to initiate security upon connection.
};

/** @brief SMP security parameter configuration. */
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

/** @brief Buffer Pool runtime configuration.
 *  @details Buffers will be allocated with the first possible match.
 */
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    { .len = 16,                        .num = 4                                             },
    { .len = 32,                        .num = 5 + 2 * BLE_CONN_MAX                          },
    { .len = 64,                        .num = 2 * BLE_CONN_MAX                              },
    { .len = 112,                       .num = BLE_CONN_MAX                                  },
    { .len = ATT_ACCEPTABLE_MTU + 24,   .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS },
    { .len = 272,                       .num = LL_RT_CFG_MAX_ADV_REPORTS                     },
    { .len = 288,                       .num = 1                                             }
};

/******************************************************************************\
 *  ATT Server Configuration.
\******************************************************************************/

/// Enumeration of client characteristic configuration descriptors.
enum
{
    BLE_GATT_SC_CCC_IDX,    // GATT service, service changed characteristic.
    BLE_NUM_CCC_IDX
};

/** @brief Client characteristic configuration descriptors settings, indexed by above enumeration. */
static const attsCccSet_t gcCccSet[BLE_NUM_CCC_IDX] = {
    [BLE_GATT_SC_CCC_IDX]         = { .handle     = GATT_SC_CH_CCC_HDL,
                                      .valueRange = ATT_CLIENT_CFG_INDICATE,
                                      .secLevel   = DM_SEC_LEVEL_NONE }
};

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void BLE_SetupAdv(void);

static void BLE_StartAdv(void);

static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg);

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

/**
 * @brief Device Manager event callback.
 * @param pEvt DM event.
 */
static void BLE_DmCback(dmEvt_t *pEvt)
{
    dmEvt_t *pMsg;
    uint16_t len;

    len = DmSizeOfEvt(pEvt);

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
        memcpy(pMsg, pEvt, len);
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief ATT event callback.
 * @param pEvt ATT event.
 */
static void BLE_AttCback(attEvt_t *pEvt)
{
    attEvt_t *pMsg;

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t *) (pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief ATTS client characteristic configuration callback.
 * @param pEvt ATTS CCC event.
 */
static void BLE_AttsCccCback(attsCccEvt_t *pEvt)
{
    attsCccEvt_t  *pMsg;
    appDbHdl_t    dbHdl;

    // If CCC not set from initialization and there's a device record and currently bonded.
    if ((pEvt->handle != ATT_HANDLE_NONE) &&
        ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE) &&
        AppCheckBonded((dmConnId_t)pEvt->hdr.param))
    {
        // Store value in device database.
        AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
    }

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attsCccEvt_t))) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attsCccEvt_t));
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pEvt Pointer to the event.
 */
static void BLE_SubsysHandler(wsfEventMask_t event, wsfMsgHdr_t *pEvt)
{
    // unused
    (void)(event);

    if (pEvt != NULL)
    {
        // Process ATT messages.
        if ((pEvt->event >= ATT_CBACK_START) && (pEvt->event <= ATT_CBACK_END))
        {
            // Process server-related ATT messages.
            AppServerProcAttMsg(pEvt);
        }
        // Process DM messages.
        else if ((pEvt->event >= DM_CBACK_START) && (pEvt->event <= DM_CBACK_END))
        {
            // Process advertising and connection-related messages.
            AppSlaveProcDmMsg((dmEvt_t*)pEvt);

            // Process security-related messages.
            AppSlaveSecProcDmMsg((dmEvt_t*)pEvt);
        }
        else
        {
            // no specific message to process
        }

        // Perform profiles, subsystems and other tasks related operations.
        BLE_ProcSubsysMsg(pEvt);
    }
}

/**
 * @brief Process messages from profiles, subsystems and other tasks.
 * @param pMsg Pointer to message.
 */
static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg)
{
    dmConnId_t connId;
    switch (pMsg->event)
    {
        // External signals and timer signals.
        case BLE_START_ADV_SIG:
            BLE_SetupAdv();
            BLE_StartAdv();
            printf("Starting Extended Advertising\r\n");
            break;

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
            break;

        case DM_ADV_STOP_IND:
            break;

        case DM_CONN_OPEN_IND:
            connId = (dmConnId_t) pMsg->param;
            printf("Connected with Connection Handle 0x%02x\r\n", connId);
            break;

        case DM_CONN_CLOSE_IND:
            //Connection closed, restart advertising
            BLE_StartAdv();
            connId = (dmConnId_t) pMsg->param;
            printf("Connection Closed for Connection Handle 0x%02x\r\n", connId);
            printf("Restarting Extended Advertising\r\n");
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
            AppHandlePasskey(&((dmEvt_t *)pMsg)->authReq);
            break;

        case DM_SEC_ECC_KEY_IND:
            DmSecSetEccKey(&((dmEvt_t *)pMsg)->eccMsg.data.key);
            break;

        case DM_SEC_COMPARE_IND:
            AppHandleNumericComparison(&((dmEvt_t *)pMsg)->cnfInd);
            break;

        case DM_PRIV_CLEAR_RES_LIST_IND:
            break;

        case DM_HW_ERROR_IND:
            break;

        // ATT server events.
        case ATTS_HANDLE_VALUE_CNF:
            break;

        case ATTS_CCC_STATE_IND:
            break;

        default:
            break;
    }
}

/**
 * @brief Set up advertising.
 */
static void BLE_SetupAdv(void)
{
    // Set the extended Advertisement discoverable data
    (void) AppExtAdvSetData((uint8_t) DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, 0 ,(uint8_t *) gAdvDataDisc, sizeof(gAdvDataDisc));

    // Set the Advertise Value for ADV type: AD Flags(LE General Discoverable flag, LE BR_EDR not supported)
    (void) AppExtAdvSetAdValue((uint8_t) DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, DM_ADV_TYPE_FLAGS,
                      sizeof(gAdvDataFlags), (uint8_t*)gAdvDataFlags);

    // Set the Advertise Value for ADV type: Local Name
    (void) AppExtAdvSetAdValue((uint8_t) DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, DM_ADV_TYPE_LOCAL_NAME,
                      sizeof(gAdvDevName), (uint8_t *)gAdvDevName);

    // Set the Advertise Value for ADV type: TX Power
    (void) AppExtAdvSetAdValue((uint8_t) DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, DM_ADV_TYPE_TX_POWER,
                      sizeof(gAdvDataTxpower), (uint8_t *)gAdvDataTxpower);

    // Set the Advertise Value for ADV type: Manufacturer Data.
    (void) AppExtAdvSetAdValue((uint8_t) DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_DISCOVERABLE, DM_ADV_TYPE_MANUFACTURER,
                      sizeof(gAdvMfgData), (uint8_t *)gAdvMfgData);

    // Set extended advertisement data for connectable mode */
    AppExtAdvSetData((uint8_t) DM_ADV_HANDLE_DEFAULT, APP_ADV_DATA_CONNECTABLE, sizeof(gAdvDataDisc), (uint8_t *) gAdvDataDisc, sizeof(gAdvDataDisc));

    // Set the extended advertisement type for connectable undirected advertisement type
    AppExtSetAdvType((uint8_t) DM_ADV_HANDLE_DEFAULT, DM_EXT_ADV_CONN_UNDIRECT);
}

/**
 * @brief Start advertising.
 */
static void BLE_StartAdv(void)
{
    uint8_t advHdl = EXT_ADV_HANDLE;

    // Start the extended Advertisement for discoverable mode
    AppExtAdvStart(DM_NUM_ADV_SETS_DEF, &advHdl, APP_MODE_DISCOVERABLE);
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
 * @brief Link Layer initialization function.
 * @param[in] llRtCfg Link Layer runtime configuration.
 * @param[in] bbRtCfg Baseband runtime configuration.
 */
static void BLE_InitLl(LlRtCfg_t *llRtCfg, BbRtCfg_t *bbRtCfg)
{
    uint32_t memUsed;

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

    bdAddr_t bdAddr;

    memUsed = LlInit(&llCfg);
    WsfHeapAlloc(memUsed);

    // Load the Bluetooth address and use it as the seed for the PRAND.
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));
    LlSetBdAddr((uint8_t*)bdAddr);
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void BLE_Init(void)
{
    wsfHandlerId_t handlerId;

    PalSysInit();

    // Initialize the Link Layer.
    BLE_InitLl(&gLlRtCfg, &gBbRtCfg);

    // Initialize the buffers.
    BLE_InitBuffer();

    // Initialize OS timer.
    WsfTimerInit();

    // Initialize the stack modules and the handlers.

    // Security Module.
    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    // Host Controller Interface.
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);
    HciSetMaxRxAclLen(ATT_ACCEPTABLE_MTU + L2C_HDR_LEN);

    // Device Manager.
    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);             // Initialize the Controller
    DmConnInit();               // Initialize DM connection manager
    DmExtAdvInit();             // Initialize extended advertisement
    DmExtConnSlaveInit();       // Initialize DM connection manager for operation as extended slave.
    DmPrivInit();               // Initialize DM privacy module
    DmHandlerInit(handlerId);   // Initialize DM handler

    // Register Callbacks for DM.
    DmRegister(BLE_DmCback);                            // Register callback for scan and advertising events.
    DmConnRegister(DM_CLIENT_ID_APP, BLE_DmCback);      // DM connection register for Client Application.

    // Logical Link Control and Adaptation Layer Protocol (L2CAP).
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);     // Initialization of Event handler for L2C in slave role.
    L2cInit();                          // Initialize L2C subsystem.
    L2cSlaveInit();                     // Initialize L2C for operation as a Bluetooth LE slave.

    // Attribute Protocol.
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);          // Initialization of ATT handler init
    pAttCfg = (attCfg_t *)&gcAttCfg;
    AttsInit();                         // Initialize ATT server
    AttsIndInit();                      // Initialize ATT server for indications/notifications
    AttsCsfInit();                      // Initialize ATTS client supported features module.

    // Register Callbacks for ATT.
    AttRegister(BLE_AttCback);          // Register ATT callback (mandatory) . This will process all the ATT event messages with ATT handle, value, data etc.
    AttConnRegister(AppServerConnCback); // Register ATT connection callback for ATT server
    AttsCccRegister(BLE_NUM_CCC_IDX, (attsCccSet_t*)gcCccSet, BLE_AttsCccCback); // Register the utility service for managing client characteristic configuration descriptors.

    // Security Manager.
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    pSmpCfg = (smpCfg_t*)&gcSmpCfg;
    SmprInit();                         // Initialize SMP for responder role.
    SmprScInit();                       // Initialize SMP secure connection in initiator role.

    // Application Framework (AF).
    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);
    AppSlaveInit();
    AppServerInit();

    pAppExtAdvCfg = (appExtAdvCfg_t *) &gcAfExtAdvCfg;
    pAppSlaveCfg = (appSlaveCfg_t*)&gcAfSlaveCfg;
    pAppSecCfg = (appSecCfg_t*)&gcAfSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t*)&gcAfUpdateCfg;

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gBLE_SubsysHandlerId = handlerId;

    // Initialize attribute server database and register callbacks.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();

    // Set Service Changed CCCD index.
    GattSetSvcChangedIdx(BLE_GATT_SC_CCC_IDX);

    // Reset the device.
    DmDevReset();
}
