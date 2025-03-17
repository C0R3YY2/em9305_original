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
#include <wsf_nvm.h>
#include <string.h>
#include <hci_api.h>
#include <hci_handler.h>
#include <svc_px.h>
#include <my_app_task.h>
#include <gap/gap_api.h>
#include <fmpl/fmpl_api.h>
#include "ble.h"
#include "..\..\my_app\source\my_app.h"

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/******************************************************************************\
 *  Local Variables
\******************************************************************************/

// Application control block
static struct
{
    uint16_t          handleList[APP_DB_HDL_LIST_LEN];  // This is 21 per record
    uint8_t           discState;
    bdAddr_t          peerAddr;                         // Peer address
    uint8_t           addrType;                         // Peer address type
    appDbHdl_t        dbHandle;                         // Peer device database record handle
    appDbHdl_t        resListRestoreHandle;             // Resolving List last restored handle
} gAppCb;

// Control block structure - defined in my_app.h
 MyApp_Cb_t gCb = {
    .connId             = DM_CONN_ID_NONE,
    .connUpdateAttempts = 0,
    .doConnUpdate       = false,
    .bondable           = true,
    .bonded             = false,
    .peerAddr           = { 0 },
    .peerAddrType       = LL_ADDR_PUBLIC,
    .connected          = false,
    .callState          = MY_APP_CS_IDLE,
    .msgUID             = EMPTY_UID
};

// Local IRK
static const uint8_t gcLocalIrk[LL_KEY_LEN] =
    { 0x95, 0xC8, 0xEE, 0x6F, 0xC5, 0x0D, 0xEF, 0x93, 0x35, 0x4E, 0x7C, 0x57, 0x08, 0xE2, 0xA3, 0x85 };


// ANCS Primary Service
static const uint8_t gcAttAncsPriSvcUuid[ATT_128_UUID_LEN] =
{
    // 7905F431-B5CE-4E99-A40F-4B1E122D00D0
    0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4,
    0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79
};

// Enums that index into the handle array for the ANCS characteristics.
// These MUST match, in order, the characteristics enumerated below.
enum
{
    ANCS_CTRL_POINT,
    ANCS_NOTIF_SOURCE,
    ANCS_NOTIF_SOURCE_CCC,
    ANCS_DATA_SOURCE,
    ANCS_DATA_SOURCE_CCC,  /* <- iOS doesn't like this */
    ANCS_NUM_CHARS
};

// ANCS Notification Source (notifiable)
static const uint8_t gcAttAncsNotifSrcUuid[ATT_128_UUID_LEN] =
{
    // 9FBF120D-6301-42D9-8C58-25E699A21DBD
    0xBD, 0x1D, 0xA2, 0x99, 0xE6, 0x25, 0x58, 0x8C,
    0xD9, 0x42, 0x01, 0x63, 0x0D, 0x12, 0xBF, 0x9F
};

// ANCS Control Point (writeable with response) (optional)
static const uint8_t gcAttAncsCtrlPointUuid[ATT_128_UUID_LEN] =
{
    // 69d1d8f3-45e1-49a8-9821-9BBDFDAAD9D9
    0xd9, 0xd9, 0xaa, 0xfd, 0xbd, 0x9b, 0x21, 0x98,
    0xa8, 0x49, 0xe1, 0x45, 0xf3, 0xd8, 0xd1, 0x69
};

// ANCS Data Source (notifiable) (optional)
static const uint8_t gcAttAncsDataSrcUuid[ATT_128_UUID_LEN] =
{
    // 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB
    0xfb, 0x7b, 0x7c, 0xce, 0x6a, 0xb3, 0x44, 0xbe,
    0xb5, 0x4b, 0xd6, 0x24, 0xe9, 0xc6, 0xea, 0x22
};

// Characteristics under the primary ANCS service to be discovered
const attcDiscChar_t ANCSNotifChar =
{
    gcAttAncsNotifSrcUuid, ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

const attcDiscChar_t ANCSCtrlPointChar =
{
    gcAttAncsCtrlPointUuid, ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

const attcDiscChar_t ANCSDataSrcChar =
{
    gcAttAncsDataSrcUuid, ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

const attcDiscChar_t gcANCSDataCCC =
{
    .pUuid = attCliChCfgUuid,
    .settings = ATTC_SET_REQUIRED | ATTC_SET_DESCRIPTOR
};

// List of characteristics to be discovered; order matches handle index enums above
//   ANCS_CTRL_POINT, ANCS_NOTIF_SOURCE, ANCS_NOTIF_SOURCE_CCC etc.
//   iOS doesn't like the last one, the CCC for the Data Source Characteristic so we
//   receive a "discovery failure" indication. But the discovery still works and returns
//   a usable handle to write the CCC to enable notifications. This allows the extraction
//   of additional event data (like caller ID).
static const attcDiscChar_t *ANCSDiscCharList[] =
{
  &ANCSCtrlPointChar,

  &ANCSNotifChar,
  &gcANCSDataCCC,

  &ANCSDataSrcChar,
  &gcANCSDataCCC     // <- iOS doesn't like this
};


// Persistent BB runtime configuration.
static BbRtCfg_t gBbRtCfg;

// Persistent LL runtime configuration.
static LlRtCfg_t gLlRtCfg;


/******************************************************************************\
 *  ATT Server Configuration.
\******************************************************************************/

// This is the sequence of the service discovery process. It is tightly coupled
//   to the states in BLE_DiscCback() so you must alter both since
//   in the function it relies on "fall through" in the case statements.
enum
{
    BLE_DISC_ANCS_SVC,      // ANCS service falls through to...
    BLE_DISC_GATT_SVC,      // GATT service
    BLE_DISC_GAP_SVC,       // GAP service
    BLE_DISC_SVC_MAX        // Discovery complete
};

// Enumeration of client characteristic configuration descriptors used in local ATT server
enum
{
    BLE_GATT_SC_CCC_IDX,        // GATT service, service changed characteristic
    BLE_NUM_CCC_IDX             // Number of CCC's
};

// Client characteristic configuration descriptors settings, indexed by CCC enumeration
static const attsCccSet_t gcCccSet[BLE_NUM_CCC_IDX] =
{
    // ccc handle          value range                 security level
    {GATT_SC_CH_CCC_HDL,   ATT_CLIENT_CFG_INDICATE,    DM_SEC_LEVEL_ENC}    // BLE_GATT_SC_CCC_IDX
};

#define ANCS_HANDLE_LIST_LEN ANCS_NUM_CHARS

/* the Client handle list, gAppCb.handleList[], is set as follows:
 *
 *  ------------------------------- <- BLE_DISC_ANCS_START
 *  | ANCS char handles           |
 *  ------------------------------- <- BLE_DISC_GATT_START
 *  | GATT svc changed handle     |
 *  -------------------------------
 *  | GATT svc changed ccc handle |
 *  ------------------------------- <- BLE_DISC_GAP_START
 *  | GAP central addr res handle |
 *  -------------------------------
 */

/* Start of each service's handles in the the handle list */
#define BLE_DISC_ANCS_START       0
#define BLE_DISC_GATT_START       (BLE_DISC_ANCS_START + ANCS_HANDLE_LIST_LEN)
#define BLE_DISC_GAP_START        (BLE_DISC_GATT_START + GATT_HDL_LIST_LEN)
#define BLE_DISC_HANDLE_LIST_LEN  (BLE_DISC_GAP_START + GAP_HDL_LIST_LEN)

/* Pointers into handle list for each service's handles */
static uint16_t *pBLE_ANCSHandleList  = &gAppCb.handleList[BLE_DISC_ANCS_START];
static uint16_t *pBLE_GattHandleList = &gAppCb.handleList[BLE_DISC_GATT_START];
static uint16_t *pBLE_GapHandleList  = &gAppCb.handleList[BLE_DISC_GAP_START];


/******************************************************************************\
 * ATT Client Data
\******************************************************************************/

/* Default value for GATT service changed ccc descriptor */
static const uint8_t gcAppGattScCccIndVal[] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_INDICATE)}; //lint !e572 Grandfathered Packetcraft macro
static const uint8_t gcAppGattScCccNotifVal[] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_NOTIFY)}; //lint !e572 Grandfathered Packetcraft macro

// List of characteristics to configure - provide the indices into the handle table and the processing function
//   moves to the next handle value which is the CCC
static const attcDiscCfg_t gcAppDiscCfgList[] =
{
    // Write:  Enable indications on the GATT service changed CCC Descriptor
    {gcAppGattScCccIndVal, sizeof(gcAppGattScCccIndVal), (GATT_SC_CCC_HDL_IDX + BLE_DISC_GATT_START)},

    // Write:  Enable notifications on the ANCS characteristics
    {gcAppGattScCccNotifVal, sizeof(gcAppGattScCccNotifVal), (BLE_DISC_ANCS_START + ANCS_NOTIF_SOURCE_CCC) },
    {gcAppGattScCccNotifVal, sizeof(gcAppGattScCccNotifVal), (BLE_DISC_ANCS_START + ANCS_DATA_SOURCE_CCC) },
};

/* Characteristic configuration list length */
#define BLE_DISC_CFG_LIST_LEN   (sizeof(gcAppDiscCfgList) / sizeof(attcDiscCfg_t))

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
    // Flags
    2,                          // Length
    DM_ADV_TYPE_FLAGS,          // AD frame type
    DM_FLAG_LE_GENERAL_DISC | DM_FLAG_LE_BREDR_NOT_SUP,

    // Short local name
    ANCS_GAP_DEVICE_NAME_LEN+1, // Length
    DM_ADV_TYPE_LOCAL_NAME,     // AD frame type
    ANCS_GAP_DEVICE_NAME_CHARS,

    // HID device - causes our device to show up under OTHER DEVICES
    // on the Bluetooth screen of iOS
    3,
    DM_ADV_TYPE_16_UUID,
    UINT16_TO_BYTES(ATT_UUID_HID_SERVICE)

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
    .rKeyDist       = (uint8_t)DM_KEY_DIST_LTK,            // Responder key distribution flags.
    .oob            = false,                               // TRUE if Out-of-band pairing data is present.
    .initiateSec    = true                                 // TRUE to initiate security upon connection.
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

static const appCfg_t gcAfBLE_AppCfg =
{
    .abortDisc = true,                          // TRUE to abort service discovery if service not found
    .disconnect = true                          // TRUE to disconnect if ATT transaction times out
};

/// Buffer Pool runtime configuration.
/// @details Buffers will be allocated with the first possible match.
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    { .len = 16,                        .num = 8                                             },
    { .len = 32,                        .num = 5 + 2 * BLE_CONN_MAX                          },
    { .len = 64,                        .num = 2 * BLE_CONN_MAX                              },
    { .len = 112,                       .num = BLE_CONN_MAX                                  },
    { .len = ATT_ACCEPTABLE_MTU + 24,   .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS },
    { .len = 272,                       .num = LL_RT_CFG_MAX_ADV_REPORTS                     },
    { .len = 288,                       .num = 1                                             }
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
        WsfMsgSend(gCb.BLE_SubsysHandlerId, pMsg);
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
        WsfMsgSend(gCb.BLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief ATTS client characteristic configuration callback.
 * @param pEvt ATTS CCC event.
 */
static void BLE_AttsCccCback(attsCccEvt_t *pEvt)
{
    attsCccEvt_t *pMsg;
    appDbHdl_t dbHandle;

    // If CCC not set from initialization and there's a device record and currently bonded.
    if ((pEvt->handle != ATT_HANDLE_NONE) &&
        ((dbHandle = AppDbGetHdl((dmConnId_t)pEvt->hdr.param)) != APP_DB_HDL_NONE) &&
        AppCheckBonded((dmConnId_t)pEvt->hdr.param))
    {
        // Store value in device database.
        AppDbSetCccTblValue(dbHandle, pEvt->idx, pEvt->value);
    }

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attsCccEvt_t))) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attsCccEvt_t));
        WsfMsgSend(gCb.BLE_SubsysHandlerId, pMsg);
    }
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
    /* Request security from master */
    AppSlaveSecurityReq((dmConnId_t)pMsg->hdr.param);

    /* Update peer address info */
    gAppCb.addrType = DmHostAddrType(pMsg->connOpen.addrType);
    BdaCpy(gAppCb.peerAddr, pMsg->connOpen.peerAddr);
    gCb.connected = true;
    gCb.callState = MY_APP_CS_IDLE;
}

/**
 * @brief Perform actions on connection close.
 * @param pMsg Pointer to message.
 */
static void BLE_Close()
{
    gCb.connected = false;
    gCb.callState = MY_APP_CS_IDLE;
}

/**
 * @brief Set up advertising.
 */
static void BLE_SetupAdv(void)
{
    // Set advertising data for discoverable mode.
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(gcAdvDataDisc), (uint8_t *)gcAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(gcAdvDataDisc), (uint8_t *)gcAdvDataDisc);

    // Set advertising and scan response data for connectable mode.
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
    AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);

    // Set the name everyone can see
    (void)AttsSetAttr(GAP_DN_HDL, ANCS_GAP_DEVICE_NAME_LEN, (uint8_t *)ANCS_GAP_DEVICE_NAME_STR);

    // Undirected advertising mode; no specified recipient
    AppSetAdvType(DM_ADV_CONN_UNDIRECT);
}

/**
 * @brief Start advertising.
 */
static void BLE_StartAdv(void)
{
    // Start advertising. From the documentation:
    // The APP_MODE_AUTO_INIT mode uses bonding information in the AF device database to determine
    // if the device is connectable. If the device has a bond, then the advertisement indicates
    // the device is discoverable but not connectable. If there is no bond,
    // the advertisement indicates the device is connectable.
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
        if (GattValueUpdate(pBLE_GattHandleList, pMsg) == ATT_SUCCESS)
        {
            return;
        }

        /* GAP */
        if (GapValueUpdate(pBLE_GapHandleList, pMsg) == ATT_SUCCESS)
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
    // Add your code here
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
        AppDiscSetHdlList(connId, BLE_DISC_HANDLE_LIST_LEN, gAppCb.handleList);
        break;

    case APP_DISC_READ_DATABASE_HASH:
        /* Read peer's database hash */
        AppDiscReadDatabaseHash(connId);
        break;

    case APP_DISC_SEC_REQUIRED:
        break;

    case APP_DISC_START:
        // Obtain handles of the ANCS characteristics we want to use, which are
        // beneath the primary ANCS Service. They are listed in ANCSDiscCharList[]
        // by their 128-bit UUIDs.
        // Then fall through to GAP and GATT - it follows the enumeration.
        gAppCb.discState = BLE_DISC_ANCS_SVC;
        AppDiscFindService(connId,
            ATT_128_UUID_LEN, (uint8_t *)gcAttAncsPriSvcUuid,   /* This is the ANCS primary Service UUID */
            ANCS_NUM_CHARS,                                     /* Number of characteristics in the service */
            (attcDiscChar_t **)ANCSDiscCharList,                /* The UUID list of those characteristics */
            pBLE_ANCSHandleList);                               /* Destination of the characteristic handles */
        break;

    case APP_DISC_FAILED:
        if (pAppCfg->abortDisc)
        {
            /* See if we were looking for ANCS */
            if (gAppCb.discState == BLE_DISC_ANCS_SVC)
            {
                /* Discovery failed, the central doesn't have ANCS */
                AppDiscComplete(connId, APP_DISC_FAILED);
                break;
            }
        }
        /* Else falls through. */

    case APP_DISC_CMPL:
        /* next discovery state */
        gAppCb.discState++;

        if (gAppCb.discState == BLE_DISC_GATT_SVC)
        {
            /* discover GATT service */
            GattDiscover(connId, pBLE_GattHandleList);
        }
        else if (gAppCb.discState == BLE_DISC_GAP_SVC)
        {
            /* discover GAP service */
            GapDiscover(connId, pBLE_GapHandleList);
        }
        else
        {
            /* discovery complete */
            AppDiscComplete(connId, APP_DISC_CMPL);

            /* GAP service discovery completed */
            BLE_DiscGapCmpl(connId);
        }
        break;

    case APP_DISC_CFG_START:
        /* start configuration */
        AppDiscConfigure(connId, APP_DISC_CFG_START, BLE_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *)gcAppDiscCfgList, BLE_DISC_HANDLE_LIST_LEN, gAppCb.handleList);
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
        else if(pEvt->event == MY_APP_WRITE_ANCS_USR_ACT)
        {
            // Write a user action to the control point characteristic
            uint8_t cmdLen = 6;
            ANCS_notif_action_t *p = (ANCS_notif_action_t *)pEvt;
            memcpy(gCb.txBuff, &p->commandID, cmdLen);
            AttcWriteReq(gCb.connId, pBLE_ANCSHandleList[ANCS_CTRL_POINT], cmdLen, gCb.txBuff);
        }
        else if(pEvt->event == MY_APP_WRITE_ANCS_ATTR_REQ)
        {
            // Write a request for information to the control point characteristic
            uint8_t cmdLen = 8;
            ANCS_notif_action_t *p = (ANCS_notif_action_t *)pEvt;
            memcpy(gCb.txBuff, &p->commandID, cmdLen);
            AttcWriteReq(gCb.connId, pBLE_ANCSHandleList[ANCS_CTRL_POINT], cmdLen, gCb.txBuff);
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
        gAppCb.resListRestoreHandle = AppAddNextDevToResList(APP_DB_HDL_NONE);

        if (gAppCb.resListRestoreHandle == APP_DB_HDL_NONE)
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
        gCb.connId = (dmConnId_t)pMsg->param;
        BLE_Open((dmEvt_t *)pMsg);
        break;

    case DM_CONN_CLOSE_IND:
        gCb.connId = DM_CONN_ID_NONE;
        BLE_Close();
        break;

    case DM_SEC_PAIR_IND:
        break;

    case DM_SEC_PAIR_CMPL_IND:
        gAppCb.dbHandle = AppDbGetHdl((dmConnId_t)pMsg->param);
        DmSecGenerateEccKeyReq();

        // Start configuration of ANCS characteristics - MUST come after pairing is completed.
        AppDiscConfigure((dmConnId_t)pMsg->param, APP_DISC_CFG_START, BLE_DISC_CFG_LIST_LEN,
                            (attcDiscCfg_t *)gcAppDiscCfgList, BLE_DISC_HANDLE_LIST_LEN, gAppCb.handleList);
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
        break;

    case DM_ADV_NEW_ADDR_IND:
        break;

    // See if it is a GATT message from the ANCS notification characteristic
    case ATTC_HANDLE_VALUE_NTF:
    {
        attEvt_t *pNotify = (attEvt_t *)pMsg;
        if(pNotify->handle == pBLE_ANCSHandleList[ANCS_NOTIF_SOURCE])
        {
            // It is an async ANCS message. Allocate a memory block and pass it up to the app layer for procressing.
            uint8_t *pBuff = WsfBufAlloc(sizeof(ANCS_Evt_t));
            if(pBuff  != NULL)
            {
                memcpy(pBuff, pNotify->pValue, sizeof(ANCS_Evt_t));
                MyAppTask_PostEvent(MY_APP_ANCS_MSG, pBuff);
            }
        }

        if(pNotify->handle == pBLE_ANCSHandleList[ANCS_DATA_SOURCE])
        {
            // It is an ANCS info response message that we requested

            // pNotify->valueLen is the length of the entire response to a Get Notification Attributes command
            uint8_t *pBuff = WsfBufAlloc(pNotify->valueLen);
            if(pBuff  != NULL)
            {
                // Copy the payload excluding the first byte, commandID, which is always zero
                memcpy(pBuff, pNotify->pValue+1, pNotify->valueLen-1);
                MyAppTask_PostEvent(MY_APP_ANCS_INFO_RSP, pBuff);
            }

        }
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
    LlMathSetSeed((uint32_t *)bdAddr);
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
    DmSecInit();      // Support legacy pairing only
    DmSecLescInit();    // Support legacy pairing and LE Secure Connections
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

    // Security Manager
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    pSmpCfg = (smpCfg_t *)&gcSmpCfg;
    SmprInit();
    SmprScInit();   // Initialize SMP manager for Responder pairing

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
    pAppCfg = (appCfg_t *)&gcAfBLE_AppCfg;

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gCb.BLE_SubsysHandlerId = handlerId;

    // Initialize attribute server database.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();

    // Set Service Changed CCC index.
    GattSetSvcChangedIdx(BLE_GATT_SC_CCC_IDX);

    // Set local IRK for device
    DmSecSetLocalIrk((uint8_t *)gcLocalIrk);

    // Reset the device.
    DmDevReset();
}
