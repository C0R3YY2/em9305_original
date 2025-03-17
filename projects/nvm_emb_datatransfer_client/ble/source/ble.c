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

#include <app_api.h>
#include <app_main.h>
#include <att_api.h>
#include <att_handler.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <hci_api.h>
#include <hci_handler.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <ll_init_api.h>
#include <my_app_signals.h>
#include <pal_cfg.h>
#include <pal_sys.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <string.h>
#include <util/bstream.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <wsf_nvm.h>
#include <printf.h>
#include <emdtp_client.h>
#include <emdt_svc.h>

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not
// the user application.

/** @brief Maximum number of connections.
 *  @note  Must be less than or equal to DM_CONN_MAX.
 */
#define BLE_CONN_MAX (3u)

// Sanity check on the BLE_CONN_MAX definition.
_Static_assert(BLE_CONN_MAX <= DM_CONN_MAX,
               "BLE_CONN_MAX cannot be larger than DM_CONN_MAX.");

/** @brief ATT Desired MTU */
#define ATT_ACCEPTABLE_MTU (247)
/** @brief Link layer runtime configuration - TX buffers. */
#define LL_RT_CFG_NUM_TX_BUFS (2u)
/** @brief Link layer runtime configuration - RX buffers. */
#define LL_RT_CFG_NUM_RX_BUFS (2u)

// Sanity check on the number of runtime buffers.
_Static_assert((LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS) <= UINT8_MAX,
               "Number of runtime buffers has to less than 255.");

/** @brief Link layer runtime configuration - Maximum number of pending advertising reports. */
#define LL_RT_CFG_MAX_ADV_REPORTS (3u)

/* Default value for CCC indications */
const uint8_t datcCccIndVal[2] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_INDICATE)};

/* Default value for CCC notifications */
const uint8_t datcCccNtfVal[2] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_NOTIFY)};

/* Default value for Client Supported Features (enable Robust Caching) */
const uint8_t datcCsfVal[1] = {ATTS_CSF_ROBUST_CACHING};

/* List of characteristics to configure after service discovery */
static const attcDiscCfg_t gcDatcDiscCfgList[] =
{
    /* Write:  GATT service changed ccc descriptor */
    {datcCccIndVal, sizeof(datcCccIndVal), (GATT_SC_CCC_HDL_IDX + DATC_DISC_GATT_START)},

    /* Write:  GATT client supported features */
    {datcCsfVal, sizeof(datcCsfVal), (GATT_CSF_HDL_IDX + DATC_DISC_GATT_START)},

    /* Write:  Proprietary data service changed ccc descriptor */
    {datcCccNtfVal, sizeof(datcCccNtfVal), (EMM_P_NA_CCC_HDL_IDX + DATC_DISC_EMMP_START)},
};

/* Characteristic configuration list length */
#define DATC_DISC_CFG_LIST_LEN (sizeof(gcDatcDiscCfgList) / sizeof(attcDiscCfg_t))

/* sanity check:  make sure configuration list length is <= handle list length */
WSF_CT_ASSERT(DATC_DISC_CFG_LIST_LEN <= DATC_DISC_HDL_LIST_LEN); // lint !e506 Compiler constant check. Okay to always evaluate to true. That means check succeeded :)

/** @brief Configuration for ATT Notifications duration  */
static const BLE_Timer_Cfg_t gcTimerCfg =
{
    .period = 30,   // timer expiration period in seconds */
};

/** @brief configurable parameters for Application (central role)*/
static const appMasterCfg_t gcAfMasterCfg =
{
    .scanInterval = 96,             // The scan interval, in 0.625 ms units
    .scanWindow = 48,               // The scan window, in 0.625 ms units
    .scanDuration = 10000,          // The scan duration in ms
    .discMode = DM_DISC_MODE_NONE,  // The GAP discovery mode
    .scanType = DM_SCAN_TYPE_ACTIVE // The scan type (active or passive)
};

/** @brief ATT configurable parameters (increase MTU). */
static const attCfg_t gcAttCfg =
{
    .discIdleTimeout = 15,                 // ATT server service discovery connection idle timeout in seconds
    .mtu = ATT_ACCEPTABLE_MTU,             // acceptable ATT MTU
    .transTimeout = ATT_MAX_TRANS_TIMEOUT, // transaction timeout in seconds
    .numPrepWrites = 4                     // number of queued prepare writes supported by server
};

/** @brief Configurable parameters for security (application framework). */
static const appSecCfg_t gcAfSecCfg =
{
    .auth = DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,    // Authentication and bonding flags.
    .iKeyDist = DM_KEY_DIST_LTK,                    // Initiator key distribution flags.
    .rKeyDist = 0,                                  // Responder key distribution flags.
    .oob = false,                                   // true if Out-of-band pairing data is present.
    .initiateSec = false                            // true to initiate security upon connection.
};

/** @brief SMP security parameter configuration. */
static const smpCfg_t gcSmpCfg =
{
    .attemptTimeout = 500,        // 'Repeated attempts' timeout in msec.
    .ioCap = SMP_IO_NO_IN_NO_OUT, // I/O Capability.
    .minKeyLen = 16,              // Minimum encryption key length. Note: while this value can be reduced to 7, it is recommended to keep at 16 to prevent key entropy reduction attacks.
    .maxKeyLen = 16,              // Maximum encryption key length.
    .maxAttempts = 1,             // Attempts to trigger 'repeated attempts' timeout.
    .auth = 0,                    // Device authentication requirements.
    .maxAttemptTimeout = 64000,   // Maximum repeated attempts timeout in msec.
    .attemptDecTimeout = 64000,   // Time msec before attemptExp decreases.
    .attemptExp = 2               // Repeated attempts multiplier exponent.
};

/** @brief Buffer Pool runtime configuration.
 *  @details Buffers will be allocated with the first possible match.
 */
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    {.len = 16, .num = 4},
    {.len = 32, .num = 5 + 2 * BLE_CONN_MAX},
    {.len = 64, .num = 2 * BLE_CONN_MAX},
    {.len = 112, .num = BLE_CONN_MAX},
    {.len = ATT_ACCEPTABLE_MTU + 24, .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS},
    {.len = 272, .num = LL_RT_CFG_MAX_ADV_REPORTS},
    {.len = 288, .num = 1}
};

/** @brief Connection parameters */
static const hciConnSpec_t gcConnCfg =
{
    .connIntervalMin = 40,  // Minimum connection interval in 1.25ms units
    .connIntervalMax = 40,  // Maximum connection interval in 1.25ms units
    .connLatency = 0,       // Connection latency
    .supTimeout = 600,      // Supervision timeout in 10ms units
    .minCeLen = 0,          // Unused
    .maxCeLen = 0           // Unused
};

/** @brief Configurable parameters for service and characteristic discovery */
static const appDiscCfg_t gcAfDiscCfg =
{
    .waitForSec = false,    // true to wait for a secure connection before initiating discovery
    .readDbHash = true      // true to fall back on database hash to verify handles when no bond exists.
};

static const appCfg_t gcAfAppCfg =
{
    .abortDisc = false, // true to abort service discovery if service not found
    .disconnect = true  // true to disconnect if ATT transaction times out
};

/******************************************************************************\
 *  EM Data Transfer Profile Client Configuration.
\******************************************************************************/

/** @brief EM data transfer profile server configuration. */
static EmdtpClient_Cfg_t gEmdtpClientCfg =
{
    .pUserData = NULL
};

/******************************************************************************\
 *  Type definition
\******************************************************************************/

/** @brief Connection info structure. */
typedef struct
{
    appDbHdl_t dbHdl;   // Device database record handle type
    uint8_t addrType;   // Type of address of device to connect to
    bdAddr_t addr;      // Address of device to connect to
    bool doConnect;   // true to issue connect on scan complete
} BLE_ConnInfoCb_t;

/** @brief Connection control block structure. */
typedef struct
{
    bool transferEnabled; // true indicates data transfer enabled
    dmConnId_t connId;      // Connection ID
} BLE_ConnCb_t;

/** @brief Application control block */
typedef struct
{
    uint16_t hdlList[BLE_CONN_MAX][APP_DB_HDL_LIST_LEN];    // Cached handle list
    uint8_t hdlListLen;                                     // Cached handle list length
    bool scanning;                                        // true if scanning
    bool autoConnect;                                     // true if auto-connecting
    BLE_ConnInfoCb_t connInfo;                              // Connection information
    uint8_t discState[BLE_CONN_MAX];                        // Service discovery state
    BLE_ConnCb_t conn[BLE_CONN_MAX];                        // Connection control block
    wsfTimer_t perTimer[BLE_CONN_MAX];                      // Periodic timer
    BLE_Timer_Cfg_t cfg;                                    // Configurable parameters
} BLE_AppCb_t;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/** @brief WSF handler ID. */
wsfHandlerId_t gBLE_SubsysHandlerId;

/** @brief Persistent BB runtime configuration. */
static BbRtCfg_t gBbRtCfg;

/** @brief Persistent LL runtime configuration. */
static LlRtCfg_t gLlRtCfg;

/** @brief Application control block. */
static BLE_AppCb_t gAppCb;

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg);

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

#ifdef PRINT_LTK
/**
 * @brief Print the Long-Term Key.
 * @param connId Connection ID.
 */
static void BLE_PrintLTK(dmConnId_t connId)
{
    extern uint8_t *SmpDmGetLtk(dmConnId_t connId);
    uint8_t *pLtk = SmpDmGetLtk(connId);

    printf("LTK: ");

    if (pLtk != NULL)
    {
        for (uint8_t i = 0; i < SEC_CMAC_KEY_LEN; i++)
        {
            printf("%02X", pLtk[i]);
        }
    }
    else
    {
        printf("INVALID");
    }

    printf("\r\n");
}
#endif // PRINT_LTK

/**
 * @brief Device Manager event callback.
 * @param pEvt DM event.
 */
static void BLE_DmCback(dmEvt_t *pEvt)
{
    dmEvt_t *pMsg;
    uint16_t len;
    uint16_t reportLen;

    len = DmSizeOfEvt(pEvt);

    if (pEvt->hdr.event == DM_SCAN_REPORT_IND)
    {
        reportLen = pEvt->scanReport.len;
    }
    else
    {
        reportLen = 0;
    }

    if ((pMsg = WsfMsgAlloc(len + reportLen)) != NULL)
    {
        memcpy(pMsg, pEvt, len);
        if (pEvt->hdr.event == DM_SCAN_REPORT_IND)
        {
            pMsg->scanReport.pData = (uint8_t *)((uint8_t *)pMsg + len);
            memcpy(pMsg->scanReport.pData, pEvt->scanReport.pData, reportLen);
        }
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
        pMsg->pValue = (uint8_t *)(pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief  Process a received ATT notification.
 * @param  pMsg    Pointer to ATT callback event message.
 */
static void BLE_HandleValueNtf(attEvt_t *pMsg)
{
    printf("ConnId:%d Len:%d ", pMsg->hdr.param, pMsg->valueLen);
    for(int i = 0; i < pMsg->valueLen; i++)
    {
        printf("%c ", pMsg->pValue[i]);
    }
    printf("\r\n");
}

/**
 * @brief EM Data Transfer Profile Client callback.
 * @param pEvt EMDTPC event.
 * @param pUserData User-defined parameter passed during configuration @ref EmdtpClient_Cfg_t.
 */
static void BLE_EmdtpClientCback(EmdtpClient_Event_t *pEvt, void *pUserData)
{
    attEvt_t *pMsg = pEvt->params.writeRsp.pEvt;
    dmConnId_t connId;

    switch (pEvt->type)
    {
        case EMDTPCLIENT_TRANSFER_DATA_EVENT:
            BLE_HandleValueNtf(pMsg);
            break;

        case EMDTPCLIENT_TRANSFER_ENABLED_EVENT:
            connId = (dmConnId_t) pMsg->hdr.param;
            gAppCb.conn[connId - 1].transferEnabled = true;
            break;

        case EMDTPCLIENT_TRANSFER_DISABLED_EVENT:
            connId = (dmConnId_t) pMsg->hdr.param;
            gAppCb.conn[connId - 1].transferEnabled = false;
            break;

        default:
            break;
    }
}

/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pEvt Pointer to WSF message.
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
            // Process discovery-related ATT messages
            AppDiscProcAttMsg((attEvt_t *)pEvt);
        }
        // Process DM messages.
        else if ((pEvt->event >= DM_CBACK_START) && (pEvt->event <= DM_CBACK_END))
        {
            // Process advertising and connection-related messages.
            AppMasterProcDmMsg((dmEvt_t *)pEvt);

            // Process security-related messages
            AppMasterSecProcDmMsg((dmEvt_t *)pEvt);

            // Process server-related DM messages
            AppServerProcDmMsg(pEvt, DM_ROLE_MASTER);

            // Process discovery-related messages
            AppDiscProcDmMsg((dmEvt_t *)pEvt);
        }
        else
        {
            // no specific message to process
        }

        // Process EMDT Profile Client messages
        EmdtpClient_ProcMsg(pEvt);

        // Perform profiles, subsystems and other tasks related operations.
        BLE_ProcSubsysMsg(pEvt);
    }
}

/**
 * @brief  Set up Scanning and other procedures that need to be performed after device reset.
 */
static void BLE_StartScan()
{
    gAppCb.autoConnect = true;

    AppScanStart(gcAfMasterCfg.discMode, gcAfMasterCfg.scanType, gcAfMasterCfg.scanDuration);
    DmConnSetConnSpec((hciConnSpec_t *)&gcConnCfg);
}

/**
 * @brief  Perform actions on scan start.
 * @param  pMsg    Pointer to DM callback event message.
 */
static void BLE_ScanDidStart(dmEvt_t *pMsg)
{
    if (pMsg->hdr.status == HCI_SUCCESS)
    {
        gAppCb.scanning = true;
        printf("Scanning ...\r\n");
    }
}

/**
 * @brief  Perform actions on scan stop.
 * @param  pMsg    Pointer to DM callback event message.
 */
static void BLE_ScanDidStop(dmEvt_t *pMsg)
{
    if (pMsg->hdr.status == HCI_SUCCESS)
    {
        gAppCb.scanning = false;
        gAppCb.autoConnect = false;

        /* Open connection */
        if (gAppCb.connInfo.doConnect)
        {
            (void)AppConnOpen(gAppCb.connInfo.addrType, gAppCb.connInfo.addr, gAppCb.connInfo.dbHdl);
            gAppCb.connInfo.doConnect = false;
        }

        printf("Scanning stopped\r\n");
    }
}

/**
 * @brief  Return if we are currently connected to device with database handle.
 * @param  dbHdl    Device database record handle type.
 * @return True if we are connected, false otherwise.
 */
static bool BLE_PeripheralIsConnected(appDbHdl_t dbHdl)
{
    dmConnId_t connIdList[DM_CONN_MAX];
    uint8_t numOpenConn;
    uint8_t i;

    numOpenConn = AppConnOpenList(connIdList);

    for (i = 0; i < numOpenConn; i++)
    {
        if (AppDbGetHdl(connIdList[i]) == dbHdl)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief  Handle a scan report.
 * @param  pMsg    Pointer to DM callback event message.
 */
static void BLE_ScanReport(dmEvt_t *pMsg)
{
    uint8_t *pData;
    appDbHdl_t dbHdl = APP_DB_HDL_NONE;
    bool connect = false;
    const uint8_t emmSvcUuid[ATT_128_UUID_LEN] = {UUID_EM_DATA_TRANSFER_SERVICE};

    /* disregard if not scanning or autoconnecting */
    if (!gAppCb.scanning || !gAppCb.autoConnect)
    {
        return;
    }

    /* if we already have a bond with this device then connect to it */
    if ((dbHdl = AppDbFindByAddr(pMsg->scanReport.addrType, pMsg->scanReport.addr)) != APP_DB_HDL_NONE)
    {
        /* if this is a directed advertisement where the initiator address is an RPA */
        if (DM_RAND_ADDR_RPA(pMsg->scanReport.directAddr, pMsg->scanReport.directAddrType))
        {
            /* resolve direct address to see if it's addressed to us */
            AppMasterResolveAddr(pMsg, dbHdl, APP_RESOLVE_DIRECT_RPA);
        }
        else
        {
            connect = true;
        }
    }

    /* if the peer device uses an RPA */
    else if (DM_RAND_ADDR_RPA(pMsg->scanReport.addr, pMsg->scanReport.addrType))
    {
        /* resolve advertiser's RPA to see if we already have a bond with this device */
        AppMasterResolveAddr(pMsg, APP_DB_HDL_NONE, APP_RESOLVE_ADV_RPA);
    }
    else if ((pData = DmFindAdType(DM_ADV_TYPE_128_UUID, pMsg->scanReport.len,
                                   pMsg->scanReport.pData)) != NULL)
    {
        /* check length and service */
        if ((pData[DM_AD_LEN_IDX] == (ATT_128_UUID_LEN+1)) && (memcmp(&pData[DM_AD_DATA_IDX], emmSvcUuid, ATT_128_UUID_LEN) == 0))
        {
            connect = true;
        }
    }

    if(connect && (BLE_PeripheralIsConnected(dbHdl) == false))
    {
        /* stop scanning and connect */
        gAppCb.autoConnect = false;
        AppScanStop();
        /* Store peer information for connect on scan stop */
        gAppCb.connInfo.addrType = DmHostAddrType(pMsg->scanReport.addrType);
        memcpy(gAppCb.connInfo.addr, pMsg->scanReport.addr, sizeof(bdAddr_t));
        gAppCb.connInfo.dbHdl = dbHdl;
        gAppCb.connInfo.doConnect = true;
    }
}

/**
 * @brief  Initialize the timer for ATT Notifications check.
 * @param  handerId    WSF handler ID of the application using this service.
 * @param  pCfg        service configurable parameters.
 */
void BLE_TimerInit(wsfHandlerId_t handlerId, const BLE_Timer_Cfg_t *pCfg)
{
    uint8_t i;

    for(i = 0; i < BLE_CONN_MAX; i++)
    {
        gAppCb.perTimer[i].handlerId = handlerId;
    }

    gAppCb.cfg = *pCfg;
}

/**
 * @brief  This function starts a timer.
 * @param  connId      DM connection identifier.
 * @param  timerEvt    WSF event designated by the application for the timer.
 */
void BLE_TimerStart(dmConnId_t connId, uint8_t timerEvt)
{
    /* initialize control block */
    gAppCb.perTimer[connId - 1].msg.param = connId;
    gAppCb.perTimer[connId - 1].msg.event = timerEvt;

    /* start timer */
    WsfTimerStartSec(&gAppCb.perTimer[connId - 1], gAppCb.cfg.period);
}

/**
 * @brief This function is called by the application when the periodic timer expires.
 * @param connId Connection identifier.
 */
void BLE_TimerExp(dmConnId_t connId)
{
    if(gAppCb.conn[connId - 1].transferEnabled)
    {
        EmdtpClient_StopDataTransfer(connId);
    }
}

/**
 * @brief GAP service discovery has completed.
 * @param connId Connection identifier.
 */
static void BLE_DiscGapCmpl(dmConnId_t connId)
{
    appDbHdl_t dbHdl;
    uint16_t *pDatcGapHdlList;

    // if RPA Only attribute found on peer device
    pDatcGapHdlList = &gAppCb.hdlList[connId - 1][DATC_DISC_GAP_START];
    if ((pDatcGapHdlList[GAP_RPAO_HDL_IDX] != ATT_HANDLE_NONE) &&
        ((dbHdl = AppDbGetHdl(connId)) != APP_DB_HDL_NONE))
    {
        // Update DB
        AppDbSetPeerRpao(dbHdl, true);
        AppDbNvmStorePeerRpao(dbHdl);
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
    case BLE_START_SCAN_SIG:
        BLE_StartScan();
        break;

    case BLE_TIMER_SIG:
        connId = (dmConnId_t)pMsg->param;
        BLE_TimerExp(connId);
        break;

    case DM_RESET_CMPL_IND:
        DmSecGenerateEccKeyReq();
        break;

    case DM_SCAN_START_IND:
        BLE_ScanDidStart((dmEvt_t *)pMsg);
        break;

    case DM_SCAN_STOP_IND:
        BLE_ScanDidStop((dmEvt_t *)pMsg);
        break;

    case DM_SCAN_REPORT_IND:
        // Filter out the data in scan report
        BLE_ScanReport((dmEvt_t *)pMsg);
        break;

    case DM_CONN_OPEN_IND:
        connId = (dmConnId_t)pMsg->param;

        // Set connection id in application control block.
        gAppCb.conn[connId - 1].connId = connId;

        printf("Connection Successful\r\n");
        break;

    case DM_CONN_CLOSE_IND:
        connId = (dmConnId_t)pMsg->param;

        // Clear connection id in application control block
        gAppCb.conn[connId - 1].connId = DM_CONN_ID_NONE;

        // Disable Timer.
        WsfTimerStop(&gAppCb.perTimer[connId - 1]);

        printf("Disconnected\r\n");
        break;

    case DM_CONN_UPDATE_IND:
        break;

    case DM_SEC_AUTH_REQ_IND:
        AppHandlePasskey(&((dmEvt_t *)pMsg)->authReq);
        break;

    case DM_SEC_ECC_KEY_IND:
        DmSecSetEccKey(&((dmEvt_t *)pMsg)->eccMsg.data.key);
        break;

    case DM_SEC_CALC_OOB_IND:
        break;

    case DM_SEC_PAIR_CMPL_IND:
        DmSecGenerateEccKeyReq();
#ifdef PRINT_LTK
        BLE_PrintLTK((dmConnId_t)pMsg->param);
#endif // PRINT_LTK
        break;

    case DM_SEC_PAIR_FAIL_IND:
        DmSecGenerateEccKeyReq();
        break;

    case DM_SEC_ENCRYPT_IND:
        break;

    case DM_SEC_ENCRYPT_FAIL_IND:
        break;

    case DM_SEC_COMPARE_IND:
        AppHandleNumericComparison(&((dmEvt_t *)pMsg)->cnfInd);
        break;

    case DM_ADV_NEW_ADDR_IND:
        break;

    case DM_PRIV_CLEAR_RES_LIST_IND:
        break;

    case DM_REMOTE_FEATURES_IND:
        break;

    default:
        break;
    }
}

/**
 * @brief  Discovery callback.
 * @param  connId    Connection identifier.
 * @param  status    Service or configuration status.
 */
static void BLE_DiscCback(dmConnId_t connId, uint8_t status)
{
    uint16_t *pDatcGattHdlList;
    uint16_t *pDatcGapHdlList;
    uint16_t *pDatcEmmpHdlList;

    switch (status)
    {
    case APP_DISC_INIT:
        // Set handle list when initialization requested
        AppDiscSetHdlList(connId, gAppCb.hdlListLen, gAppCb.hdlList[connId - 1]);
        break;

    case APP_DISC_READ_DATABASE_HASH:
        // Read peer's database hash
        AppDiscReadDatabaseHash(connId);
        break;

    case APP_DISC_SEC_REQUIRED:
        // Initiate security
        AppMasterSecurityReq(connId);
        break;

    case APP_DISC_START:
        // Initialize discovery state
        gAppCb.discState[connId - 1] = DATC_DISC_GATT_SVC;

        // Store possible change in cache by hash
        AppDbNvmStoreCacheByHash(AppDbGetHdl(connId));

        // Discover GATT service
        pDatcGattHdlList = &gAppCb.hdlList[connId - 1][DATC_DISC_GATT_START];
        GattDiscover(connId, pDatcGattHdlList);
        break;

    case APP_DISC_FAILED:
        if (pAppCfg->abortDisc)
        {
            // If discovery failed for proprietary data service then disconnect
            if (gAppCb.discState[connId - 1] == DATC_DISC_EMMP_SVC)
            {
                AppConnClose(connId);
                break;
            }
        }
        // Else falls through

    case APP_DISC_CMPL:
        // Next discovery state
        gAppCb.discState[connId - 1]++;

        if (gAppCb.discState[connId - 1] == DATC_DISC_GAP_SVC)
        {
            // Discover GAP service
            pDatcGapHdlList = &gAppCb.hdlList[connId - 1][DATC_DISC_GAP_START];
            GapDiscover(connId, pDatcGapHdlList);
        }
        else if (gAppCb.discState[connId - 1] == DATC_DISC_EMMP_SVC)
        {
            // discover proprietary data service
            pDatcEmmpHdlList = &gAppCb.hdlList[connId - 1][DATC_DISC_EMMP_START];
            EmdtpClient_Discover(connId, pDatcEmmpHdlList);
        }
        else
        {
            // Discovery complete
            AppDiscComplete(connId, APP_DISC_CMPL);

            // GAP service discovery completed
            BLE_DiscGapCmpl(connId);

            // Store cached handle list in NVM
            AppDbNvmStoreHdlList(AppDbGetHdl(connId));

            // Configure EMM proprietary data
            AppDiscConfigure(connId, APP_DISC_CFG_START, DATC_DISC_CFG_LIST_LEN,
                    (attcDiscCfg_t *) gcDatcDiscCfgList, DATC_DISC_HDL_LIST_LEN, gAppCb.hdlList[connId-1]);
        }
        break;

    case APP_DISC_CFG_START:
        // Start configuration
        AppDiscConfigure(connId, APP_DISC_CFG_START, DATC_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *)gcDatcDiscCfgList, DATC_DISC_HDL_LIST_LEN, gAppCb.hdlList[connId - 1]);
        printf("EMM Discovery: data configured\r\n");
        break;

    case APP_DISC_CFG_CMPL:
        // Start timer to toggle data transfer service
        BLE_TimerStart(connId, BLE_TIMER_SIG);

        // Set transferEnabled to true as specified in default config for EM DTS Profile
        gAppCb.conn[connId - 1].transferEnabled = true;

        // Set handles for EM Data Transfer Profile
        pDatcEmmpHdlList = &gAppCb.hdlList[connId - 1][DATC_DISC_EMMP_START];
        EmdtpClient_SetHandles(connId, pDatcEmmpHdlList);

        AppDiscComplete(connId, status);

        BLE_StartScan();
        break;

    case APP_DISC_CFG_CONN_START:
        pDatcEmmpHdlList = &gAppCb.hdlList[connId - 1][DATC_DISC_EMMP_START];
        EmdtpClient_SetHandles(connId, pDatcEmmpHdlList);
        break;

    default:
        break;
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
    PalBbLoadCfg((PalBbCfg_t *)bbRtCfg);
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

    llRtCfg->maxConn = BLE_CONN_MAX;
    llRtCfg->maxAclLen = ATT_ACCEPTABLE_MTU + L2C_HDR_LEN;
    llRtCfg->numTxBufs = LL_RT_CFG_NUM_TX_BUFS;
    llRtCfg->numRxBufs = LL_RT_CFG_NUM_RX_BUFS;
    llRtCfg->maxAdvReports = LL_RT_CFG_MAX_ADV_REPORTS;

    LlInitRtCfg_t llCfg =
    {
        .pBbRtCfg = bbRtCfg,
        .wlSizeCfg = 1, // Whitelist size.
        .rlSizeCfg = 1, // Resolvable list size.
        .plSizeCfg = 1, // Periodic list size.
        .pLlRtCfg = llRtCfg,
        .pFreeMem = WsfHeapGetFreeStartAddress(),
        .freeMemAvail = WsfHeapCountAvailable()
    };

    bdAddr_t bdAddr;

    memUsed = LlInit(&llCfg);
    WsfHeapAlloc(memUsed);

    // Load the Bluetooth address and use it as the seed for the PRAND.
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));
    LlSetBdAddr((uint8_t *)bdAddr);
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
    DmDevVsInit(0);
    DmConnInit();
    DmScanInit();
    DmConnMasterInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);
    // Register Callbacks for DM.
    DmRegister(BLE_DmCback);
    DmConnRegister(DM_CLIENT_ID_APP, BLE_DmCback);

    // Logical Link Control and Adaptation Layer Protocol (L2CAP).
    L2cInit();
    L2cMasterInit();

    // Attribute Protocol.
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    pAttCfg = (attCfg_t *)&gcAttCfg;
    AttcInit();
    // Register Callbacks for ATT.
    AttRegister(BLE_AttCback);

    /* Register for app framework discovery callbacks */
    AppDiscRegister(BLE_DiscCback);

    // Security Manager.
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    pSmpCfg = (smpCfg_t *)&gcSmpCfg;
    SmpiInit();
    SmpiScInit();

    // Application Framework (AF).
    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);

    /* Set configuration pointers */
    pAppMasterCfg = (appMasterCfg_t *)&gcAfMasterCfg;
    pAppSecCfg = (appSecCfg_t *)&gcAfSecCfg;
    pAppDiscCfg = (appDiscCfg_t *)&gcAfDiscCfg;
    pAppCfg = (appCfg_t *)&gcAfAppCfg;
    pSmpCfg = (smpCfg_t *)&gcSmpCfg;
    pAttCfg = (attCfg_t *)&gcAttCfg;

    /* Initialize application framework */
    AppMasterInit();
    AppDiscInit();

    // Initialize the control block
    memset(&gAppCb, 0, sizeof(gAppCb));

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    BLE_TimerInit(handlerId, &gcTimerCfg);
    gBLE_SubsysHandlerId = handlerId;

    /* set handle list length */
    gAppCb.hdlListLen = DATC_DISC_HDL_LIST_LEN;

    WsfNvmInit();

    // Proprietary Data Transfer Profile init.
    EmdtpClient_Init(&gEmdtpClientCfg, BLE_EmdtpClientCback);

    printf("EM Microelectronic - EMB DTS Client\r\n-----------------------------------\r\n");

    // Reset the device.
    DmDevReset();
}
