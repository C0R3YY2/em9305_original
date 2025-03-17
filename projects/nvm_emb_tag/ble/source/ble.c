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

#include <my_app_signals.h>
#include <app_api.h>
#include <app_main.h>
#include <att_api.h>
#include <app_ui.h>
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
#include <util/bstream.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <string.h>
#include <hci_api.h>
#include <hci_handler.h>

#include <svc_px.h>

#include <gap/gap_api.h>
#include <fmpl/fmpl_api.h>

#ifdef PRINT_LTK
#include <printf.h>
#endif

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// WSF handler ID.
wsfHandlerId_t gBLE_SubsysHandlerId;

/******************************************************************************\
 *  Local Variables
\******************************************************************************/

/*! proximity reporter control block */
static struct
{
    uint16_t          hdlList[APP_DB_HDL_LIST_LEN];
    uint8_t           discState;
    wsfTimer_t        rssiTimer;                    /* Read RSSI value timer */
    bool              inProgress;                   /* Read RSSI value in progress */
    bdAddr_t          peerAddr;                     /* Peer address */
    uint8_t           addrType;                     /* Peer address type */
    appDbHdl_t        dbHdl;                        /* Peer device database record handle */
    appDbHdl_t        resListRestoreHdl;            /*! Resolving List last restored handle */
} gAppCb;

#ifdef TAG_USE_RPA
/// Local IRK
static const uint8_t gcLocalIrk[LL_KEY_LEN] = {0x95, 0xC8, 0xEE, 0x6F, 0xC5, 0x0D, 0xEF, 0x93, 0x35, 0x4E, 0x7C, 0x57, 0x08, 0xE2, 0xA3, 0x85};
#endif

/// Persistent BB runtime configuration.
static BbRtCfg_t gBbRtCfg;

/// Persistent LL runtime configuration.
static LlRtCfg_t gLlRtCfg;


/******************************************************************************\
 *  ATT Server Configuration.
\******************************************************************************/

/// Enumeration of client characteristic configuration descriptors.
enum
{
    TAG_DISC_IAS_SVC,       /* Immediate Alert service */
    TAG_DISC_GATT_SVC,      /* GATT service */
    TAG_DISC_GAP_SVC,       /* GAP service */
    TAG_DISC_SVC_MAX        /* Discovery complete */
};

/* enumeration of client characteristic configuration descriptors used in local ATT server */
enum
{
    BLE_GATT_SC_CCC_IDX,        /*! GATT service, service changed characteristic */
    BLE_NUM_CCC_IDX             /*! Number of ccc's */
};

/* client characteristic configuration descriptors settings, indexed by ccc enumeration */
static const attsCccSet_t gcCccSet[BLE_NUM_CCC_IDX] =
{
    /* cccd handle         value range                 security level */
    {GATT_SC_CH_CCC_HDL,   ATT_CLIENT_CFG_INDICATE,    DM_SEC_LEVEL_ENC}    /* TAG_GATT_SC_CCC_IDX */
};

/* the Client handle list, gAppCb.hdlList[], is set as follows:
 *
 *  ------------------------------- <- TAG_DISC_IAS_START
 *  | IAS alert level handle      |
 *  ------------------------------- <- TAG_DISC_GATT_START
 *  | GATT svc changed handle     |
 *  -------------------------------
 *  | GATT svc changed ccc handle |
 *  ------------------------------- <- TAG_DISC_GAP_START
 *  | GAP central addr res handle |
 *  -------------------------------
 *  | GAP RPA Only handle         |
 *  -------------------------------
 */

/* Start of each service's handles in the the handle list */
#define TAG_DISC_IAS_START        0
#define TAG_DISC_GATT_START       (TAG_DISC_IAS_START + FMPL_IAS_HDL_LIST_LEN)
#define TAG_DISC_GAP_START        (TAG_DISC_GATT_START + GATT_HDL_LIST_LEN)
#define TAG_DISC_HDL_LIST_LEN     (TAG_DISC_GAP_START + GAP_HDL_LIST_LEN)

/* Pointers into handle list for each service's handles */
static uint16_t *pTagIasHdlList  = &gAppCb.hdlList[TAG_DISC_IAS_START];
static uint16_t *pTagGattHdlList = &gAppCb.hdlList[TAG_DISC_GATT_START];
static uint16_t *pTagGapHdlList  = &gAppCb.hdlList[TAG_DISC_GAP_START];


/******************************************************************************\
 * ATT Client Data
\******************************************************************************/

/* Default value for GATT service changed ccc descriptor */
static const uint8_t gcAppGattScCccVal[] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_INDICATE)}; //lint !e572 Grandfathered Packetcraft macro

/* Default value for Client Supported Features (enable Robust Caching) */
const uint8_t gcAppCsfVal[1] = { ATTS_CSF_ROBUST_CACHING };

/* List of characteristics to configure */
static const attcDiscCfg_t gcAppDiscCfgList[] =
{
    /* Write:  GATT service changed ccc descriptor */
    {gcAppGattScCccVal, sizeof(gcAppGattScCccVal), (GATT_SC_CCC_HDL_IDX + TAG_DISC_GATT_START)},

    /* Write:  GATT client supported features */
    {gcAppCsfVal, sizeof(gcAppCsfVal), (GATT_CSF_HDL_IDX + TAG_DISC_GATT_START) },

    /* Read: GAP central address resolution attribute */
    {NULL, 0, (GAP_CAR_HDL_IDX + TAG_DISC_GAP_START)},
};

/* Characteristic configuration list length */
#define TAG_DISC_CFG_LIST_LEN   (sizeof(gcAppDiscCfgList) / sizeof(attcDiscCfg_t))

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not
// the user application.

/// Maximum number of connections.
/// Must be smaller than LL_MAX_CONN.
#define BLE_CONN_MAX    (1u)

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

/// Advertising data, discoverable mode.
static const uint8_t gcAdvDataDisc[] =
{
    // Flags.
    2,                          // Length.
    DM_ADV_TYPE_FLAGS,          // AD type.
    DM_FLAG_LE_LIMITED_DISC |
    DM_FLAG_LE_BREDR_NOT_SUP,

    // manufacturer specific data */
    3,                                      /*! length */
    DM_ADV_TYPE_MANUFACTURER,               /*! AD type */
    (uint8_t)HCI_ID_EMMICROELECTRONIC,      /*! company ID */
    0x00,

    // TX Power.
    2,                          // Length.
    DM_ADV_TYPE_TX_POWER,       // AD type.
    0,

    // Service UUID list.
    7,                                      /*! length */
    DM_ADV_TYPE_16_UUID,                    /*! AD type */
    UINT16_TO_BYTES(ATT_UUID_LINK_LOSS_SERVICE),
    UINT16_TO_BYTES(ATT_UUID_IMMEDIATE_ALERT_SERVICE),
    UINT16_TO_BYTES(ATT_UUID_TX_POWER_SERVICE)
};

/// Scan data, discoverable mode.
static const uint8_t gcScanDataDisc[] =
{
    // Device name.
    11,                         // Length.
    DM_ADV_TYPE_LOCAL_NAME,     // AD type.
    'E', 'M', '9', '3', '0', '5', '_', 'T', 'A', 'G'
};

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
    .auth           = DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG, // Authentication and bonding flags.
    .iKeyDist       = 0,                                   // Initiator key distribution flags.
    .rKeyDist       = DM_KEY_DIST_LTK | DM_KEY_DIST_IRK,   // Responder key distribution flags.
    .oob            = false,                               // true if Out-of-band pairing data is present.
    .initiateSec    = true                                 // true to initiate security upon connection.
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

static const appCfg_t gcAfTagAppCfg =
{
    .abortDisc = true,                           /*! True to abort service discovery if service not found */
    .disconnect = true                           /*! True to disconnect if ATT transaction times out */
};

/// Buffer Pool runtime configuration.
/// @details Buffers will be allocated with the first possible match.
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

/*! Read RSSI interval in seconds */
#define TAG_READ_RSSI_INTERVAL      3

/* Change RPA every 5 mins */
#define RPA_TIMEOUT_SEC 300

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void BLE_SetupAdv(void);

static void BLE_StartAdv(void);

static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg);

static void BLE_ProcRssiTimer();

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
        pMsg->pValue = (uint8_t *)(pMsg + 1);
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
    attsCccEvt_t *pMsg;
    appDbHdl_t dbHdl;

    // If CCC not set from initialization and there's a device record and currently bonded.
    if ((pEvt->handle != ATT_HANDLE_NONE) &&
        ((dbHdl = AppDbGetHdl((dmConnId_t)pEvt->hdr.param)) != APP_DB_HDL_NONE) &&
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
 *  @brief  ATTS write callback for Immediate Alert service.
 *
 *  @return ATT status.
 */
static uint8_t BLE_IasWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                                 uint16_t offset, uint16_t len, uint8_t *pValue,
                                 attsAttr_t *pAttr)
{
    /* sanity check on length */
    if (len < CH_ALERT_LVL_LEN)
    {
        return ATT_ERR_LENGTH;
    }

    // Handle IAS alert action here

    return ATT_SUCCESS;
}

/**
*  @brief  Perform actions on connection open.
*
*  @param  pMsg    Pointer to DM callback event message.
*
*  @return None.
*/
static void BLE_Open(dmEvt_t *pMsg)
{
#ifdef TAG_USE_RPA
    /* Request security from master */
    AppSlaveSecurityReq((dmConnId_t)pMsg->hdr.param);
#endif

    /* Update peer address info */
    gAppCb.addrType = DmHostAddrType(pMsg->connOpen.addrType);
    BdaCpy(gAppCb.peerAddr, pMsg->connOpen.peerAddr);

    /* Start RSSI timer*/
    BLE_ProcRssiTimer();
}

/**
 * @brief Perform actions on connection close.
 * @param pMsg Pointer to message.
 */
static void BLE_Close()
{
    /* if read RSSI in progress, stop timer */
    if (gAppCb.inProgress)
    {
        gAppCb.inProgress = false;

        /* stop timer */
        WsfTimerStop(&gAppCb.rssiTimer);
    }
}

/**
 *  @brief  Get peer key from a device database record.
 *
 *  @param  dbHdl   Device database record handle.
 *
 *  @return Pointer to peer key if key is valid or NULL if not valid.
 */
static dmSecKey_t *BLE_GetPeerKey(appDbHdl_t dbHdl)
{
    /* if database record handle valid */
    if (dbHdl != APP_DB_HDL_NONE)
    {
        return AppDbGetKey(dbHdl, DM_KEY_IRK, NULL);
    }

    return NULL;
}

/**
 *  @brief  Handle add device to resolving list indication.
 *
 *  @param  dbHdl   Device DB record handle.
 *
 *  @return None.
 */
static void BLE_PrivAddDevToResListInd(appDbHdl_t dbHdl)
{
    dmSecKey_t *pPeerKey;

    /* if peer IRK present */
    if ((pPeerKey = BLE_GetPeerKey(dbHdl)) != NULL)
    {
        /* set advertising peer address */
        AppSetAdvPeerAddr(pPeerKey->irk.addrType, pPeerKey->irk.bdAddr);
    }
}

/**
 *  @brief  Handle remove device from resolving list indication.
 *
 *  @param  pMsg    Pointer to DM callback event message.
 *
 *  @return None.
 */
static void BLE_PrivRemDevFromResListInd(dmEvt_t *pMsg)
{
    if (pMsg->hdr.status == HCI_SUCCESS)
    {
        if (AppDbGetHdl((dmConnId_t)pMsg->hdr.param) != APP_DB_HDL_NONE)
        {
            uint8_t addrZeros[BDA_ADDR_LEN] = {0};

            /* clear advertising peer address and its type */
            AppSetAdvPeerAddr(HCI_ADDR_TYPE_PUBLIC, addrZeros);
        }
    }
}

/**
 * @brief Set up advertising.
 */
static void BLE_SetupAdv(void)
{
    // Set advertising and scan response data for discoverable mode.
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(gcAdvDataDisc), (uint8_t *)gcAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(gcScanDataDisc), (uint8_t *)gcScanDataDisc);

    // Set advertising and scan response data for connectable mode.
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
    AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);
}

/**
 * @brief Start advertising.
 */
static void BLE_StartAdv(void)
{
    // Start advertising; automatically set connectable/discoverable mode and bondable mode.
    AppAdvStart(APP_MODE_AUTO_INIT);
}

/**
 *  @brief  Process a received ATT indication.
 *
 *  @param  pMsg    Pointer to ATT callback event message.
 *
 *  @return None.
 */
static void BLE_ValueUpdate(attEvt_t *pMsg)
{
    if (pMsg->hdr.status == ATT_SUCCESS)
    {
        /* determine which profile the handle belongs to */

        /* GATT */
        if (GattValueUpdate(pTagGattHdlList, pMsg) == ATT_SUCCESS)
        {
            return;
        }

        /* GAP */
        if (GapValueUpdate(pTagGapHdlList, pMsg) == ATT_SUCCESS)
        {
            return;
        }
    }
}

/**
 *  @brief  GAP service discovery has completed.
 *
 *  @param  connId    Connection identifier.
 *
 *  @return None.
 */
static void BLE_DiscGapCmpl(dmConnId_t connId)
{
    appDbHdl_t dbHdl;

    /* if RPA Only attribute found on peer device */
    if ((pTagGapHdlList[GAP_RPAO_HDL_IDX] != ATT_HANDLE_NONE) &&
        ((dbHdl = AppDbGetHdl(connId)) != APP_DB_HDL_NONE))
    {
        /* update DB */
        AppDbSetPeerRpao(dbHdl, true);
    }
}

/**
 *  @brief  This function is called when the periodic read RSSI timer expires.
 *
 *  @param  pMsg    Pointer to message.
 *
 *  @return None.
 */
static void BLE_ProcRssiTimer()
{
    dmConnId_t connId;

    /* if still connected */
    if ((connId = AppConnIsOpen()) != DM_CONN_ID_NONE)
    {
        /* read RSSI value for the active connection */
        DmConnReadRssi(connId);

        /* restart timer */
        WsfTimerStartSec(&gAppCb.rssiTimer, TAG_READ_RSSI_INTERVAL);
    }
}

/**
 *  @brief  Discovery callback.
 *
 *  @param  connId    Connection identifier.
 *  @addtogroup  status    Service or configuration status.
 *
 *  @return None.
 */
static void BLE_DiscCback(dmConnId_t connId, uint8_t status)
{
    switch (status)
    {
    case APP_DISC_INIT:
        /* set handle list when initialization requested */
        AppDiscSetHdlList(connId, TAG_DISC_HDL_LIST_LEN, gAppCb.hdlList);
        break;

    case APP_DISC_READ_DATABASE_HASH:
        /* Read peer's database hash */
        AppDiscReadDatabaseHash(connId);
        break;

    case APP_DISC_SEC_REQUIRED:
        /* request security */
        AppSlaveSecurityReq(connId);
        break;

    case APP_DISC_START:
        /* initialize discovery state */
        gAppCb.discState = TAG_DISC_IAS_SVC;

        /* discover immediate alert service */
        FmplIasDiscover(connId, pTagIasHdlList);
        break;

    case APP_DISC_FAILED:
        if (pAppCfg->abortDisc)
        {
            /* if immediate alert service not found */
            if (gAppCb.discState == TAG_DISC_IAS_SVC)
            {
                /* discovery failed */
                AppDiscComplete(connId, APP_DISC_FAILED);
                break;
            }
        }
        /* Else falls through. */

    case APP_DISC_CMPL:
        /* next discovery state */
        gAppCb.discState++;

        if (gAppCb.discState == TAG_DISC_GATT_SVC)
        {
            /* discover GATT service */
            GattDiscover(connId, pTagGattHdlList);
        }
        else if (gAppCb.discState == TAG_DISC_GAP_SVC)
        {
            /* discover GAP service */
            GapDiscover(connId, pTagGapHdlList);
        }
        else
        {
            /* discovery complete */
            AppDiscComplete(connId, APP_DISC_CMPL);

            /* GAP service discovery completed */
            BLE_DiscGapCmpl(connId);

            /* start configuration */
            AppDiscConfigure(connId, APP_DISC_CFG_START, TAG_DISC_CFG_LIST_LEN,
                             (attcDiscCfg_t *)gcAppDiscCfgList, TAG_DISC_HDL_LIST_LEN, gAppCb.hdlList);
        }
        break;

    case APP_DISC_CFG_START:
        /* start configuration */
        AppDiscConfigure(connId, APP_DISC_CFG_START, TAG_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *)gcAppDiscCfgList, TAG_DISC_HDL_LIST_LEN, gAppCb.hdlList);
        break;

    case APP_DISC_CFG_CMPL:
        AppDiscComplete(connId, APP_DISC_CFG_CMPL);
        break;

    case APP_DISC_CFG_CONN_START:
        /* no connection setup configuration for this application */
        break;

    default:
        break;
    }
}

/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pMsg Pointer to message.
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
            /* process discovery-related ATT messages */
            AppDiscProcAttMsg((attEvt_t *)pEvt);

            // Process server-related ATT messages.
            AppServerProcAttMsg(pEvt);
        }
        // Process DM messages.
        else if ((pEvt->event >= DM_CBACK_START) && (pEvt->event <= DM_CBACK_END))
        {
            // Process advertising and connection-related messages.
            AppSlaveProcDmMsg((dmEvt_t *)pEvt);

            // Process security-related messages.
            AppSlaveSecProcDmMsg((dmEvt_t *)pEvt);

            /* process server-related DM messages */
            AppServerProcDmMsg(pEvt, DM_ROLE_SLAVE);

            /* process discovery-related messages */
            AppDiscProcDmMsg((dmEvt_t *)pEvt);
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
    switch (pMsg->event)
    {

    case BLE_START_ADV_SIG:
        /* Restore first device to resolving list in Controller. */
        gAppCb.resListRestoreHdl = AppAddNextDevToResList(APP_DB_HDL_NONE);

        if (gAppCb.resListRestoreHdl == APP_DB_HDL_NONE)
        {
            /* No device to restore.  Setup application. */
            BLE_SetupAdv();
            BLE_StartAdv();
        }
        break;

    case ATTC_READ_RSP:
    //lint -fallthrough
    case ATTC_HANDLE_VALUE_IND:
        BLE_ValueUpdate((attEvt_t *)pMsg);
        break;

    case DM_RESET_CMPL_IND:
        AttsCalculateDbHash();
        DmSecGenerateEccKeyReq();
        break;

    case DM_ADV_START_IND:
        break;

    case DM_ADV_STOP_IND:
        break;

    case DM_CONN_OPEN_IND:
        BLE_Open((dmEvt_t *)pMsg);
        break;

    case DM_CONN_CLOSE_IND:
        BLE_Close();
        break;

    case DM_SEC_PAIR_CMPL_IND:
        gAppCb.dbHdl = AppDbGetHdl((dmConnId_t)pMsg->param);
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

    case DM_SEC_AUTH_REQ_IND:
        AppHandlePasskey(&((dmEvt_t *)pMsg)->authReq);
        break;

    case DM_SEC_ECC_KEY_IND:
        DmSecSetEccKey(&((dmEvt_t *)pMsg)->eccMsg.data.key);
        break;

    case DM_SEC_COMPARE_IND:
        AppHandleNumericComparison(&((dmEvt_t *)pMsg)->cnfInd);
        break;

    case DM_PRIV_ADD_DEV_TO_RES_LIST_IND:
        if (gAppCb.resListRestoreHdl == APP_DB_HDL_NONE)
        {
            BLE_PrivAddDevToResListInd(AppDbGetHdl((dmConnId_t)pMsg->param));
        }
        else
        {
            /* Set the advertising peer address. */
            BLE_PrivAddDevToResListInd(gAppCb.resListRestoreHdl);

            /* Retore next device to resolving list in Controller. */
            gAppCb.resListRestoreHdl = AppAddNextDevToResList(gAppCb.resListRestoreHdl);

            if (gAppCb.resListRestoreHdl == APP_DB_HDL_NONE)
            {
                /* No additional device to restore. Setup application. */
                BLE_SetupAdv();
                BLE_StartAdv();
            }
        }
#ifdef TAG_USE_RPA
        /* enable device privacy -- start generating local RPAs every RPA_TIMEOUT_SEC seconds */
        DmDevPrivStart(RPA_TIMEOUT_SEC);
#endif
        break;

    case DM_PRIV_REM_DEV_FROM_RES_LIST_IND:
        BLE_PrivRemDevFromResListInd((dmEvt_t *)pMsg);
        break;

    case DM_ADV_NEW_ADDR_IND:
        break;

    case TAG_RSSI_TIMER_IND:
        BLE_ProcRssiTimer();
        break;

    case DM_CONN_READ_RSSI_IND:
        /* if successful */
        if (pMsg->status == HCI_SUCCESS)
        {
            /* display RSSI value */
            AppUiDisplayRssi(((dmEvt_t *)pMsg)->readRssi.rssi);
        }
        break;

    case DM_PRIV_CLEAR_RES_LIST_IND:
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
            .freeMemAvail = WsfHeapCountAvailable()};

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
    DmDevPrivInit();
    DmDevVsInit(0);
    DmConnInit();
    DmAdvInit();
    DmConnSlaveInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);
    // Register Callbacks for DM.
    DmRegister(BLE_DmCback);
    DmConnRegister(DM_CLIENT_ID_APP, BLE_DmCback);

    // Logical Link Control and Adaptation Layer Protocol (L2CAP).
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    // Attribute Protocol.
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    pAttCfg = (attCfg_t *)&gcAttCfg;
    AttsInit();
    AttsIndInit();
    AttcInit();
    // Register Callbacks for ATT.
    AttRegister(BLE_AttCback);
    AttConnRegister(AppServerConnCback);
    AttsCccRegister(BLE_NUM_CCC_IDX, (attsCccSet_t *)gcCccSet, BLE_AttsCccCback);

    // Security Manager.
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    pSmpCfg = (smpCfg_t *)&gcSmpCfg;
    SmprInit();
    SmprScInit();

    // Register for app framework discovery callbacks
    AppDiscRegister(BLE_DiscCback);

    // Application Framework (AF).
    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);
    AppSlaveInit();
    AppServerInit();
    pAppAdvCfg = (appAdvCfg_t *)&gcAfAdvCfg;
    pAppSlaveCfg = (appSlaveCfg_t *)&gcAfSlaveCfg;
    pAppSecCfg = (appSecCfg_t *)&gcAfSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t *)&gcAfUpdateCfg;
    pAppCfg = (appCfg_t *)&gcAfTagAppCfg;

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gBLE_SubsysHandlerId = handlerId;

    /* initialize rssi timer */
    gAppCb.rssiTimer.handlerId = handlerId;
    gAppCb.rssiTimer.msg.event = TAG_RSSI_TIMER_IND;
    gAppCb.inProgress = false;

    // Initialize attribute server database.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();
    SvcPxCbackRegister(NULL, BLE_IasWriteCback);
    SvcPxAddGroup();

    // Set Service Changed CCCD index.
    GattSetSvcChangedIdx(BLE_GATT_SC_CCC_IDX);

#ifdef TAG_USE_RPA
    // Set local IRK for device
    DmSecSetLocalIrk((uint8_t *)gcLocalIrk);
#endif
    // Reset the device.
    DmDevReset();
}
