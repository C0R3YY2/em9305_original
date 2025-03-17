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
#include <att_handler.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <eatt_api.h>
#include <gatt/gatt_api.h>
#include <hci_api.h>
#include <hci_handler.h>
#include <hci_core.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <ll_init_api.h>
#include <ll_api.h>
#include <pal_cfg.h>
#include <pal_sys.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <svc_dis.h>
#include <svc_pac.h>
#include <svc_asc.h>
#include <svc_tmas.h>
#include <util/bstream.h>
#include <wsf_buf.h>
#include <wsf_bufio.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_nvm.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <string.h>
#include <mcpc/mcpc_api.h>
#include <ccpc/ccpc_api.h>
#include <tmap/ctu_api.h>
#include <tmap/ctu_medcb.h>
#include <dis_api.h>
#include "ble.h"
#include <ble_task.h>

#ifdef DBG_PRINT
#include <printf.h>
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Audio contexts available for reception */
#define AUDCTUS_AV_SNK_AUD_CTX      (GA_CTX_TYPE_CONVERSAT_BIT | GA_CTX_TYPE_MEDIA_BIT)
/* Audio contexts available for transmission */
#define AUDCTUS_AV_SRC_AUD_CTX      GA_CTX_TYPE_CONVERSAT_BIT

/* Audio contexts supported for reception */
#define AUDCTUS_SNK_SUP_AUD_CTX     (GA_CTX_TYPE_UNSPECIFY_BIT | GA_CTX_TYPE_CONVERSAT_BIT | GA_CTX_TYPE_MEDIA_BIT)
#define AUDCTUS_SRC_SUP_AUD_CTX     (GA_CTX_TYPE_UNSPECIFY_BIT | GA_CTX_TYPE_CONVERSAT_BIT)

/*! Whether to use legacy advertising PDUs with extended advertising */
#define AUDCTUS_USE_LEGACY          false

/// Maximum number of connections.
/// Must be smaller than LL_MAX_CONN.
#define BLE_CONN_MAX    (AUDCTUS_CONN_MAX)

/// ATT Desired MTU
#define ATT_DESIRED_MTU (246)

/// Link layer runtime configuration - TX buffers.
#define LL_RT_CFG_NUM_TX_BUFS       (4u)
/// Link layer runtime configuration - RX buffers.
#define LL_RT_CFG_NUM_RX_BUFS       (4u)
/// Link layer runtime configuration - ISO TX buffers.
#define LL_RT_CFG_NUM_ISO_TX_BUFS   (8u)
/// Link layer runtime configuration - ISO RX buffers.
#define LL_RT_CFG_NUM_ISO_RX_BUFS   (8u)
/// Link layer runtime configuration - Maximum number of pending advertising reports.
#define LL_RT_CFG_MAX_ADV_REPORTS   (3u)

/*! Discovery states:  enumeration of services to be discovered */
enum
{
  AUDCTUS_DISC_GATT_SVC,      /*! GATT service */
  AUDCTUS_DISC_DIS_SVC,       /*! Device Information service */
  AUDCTUS_DISC_MCP_SVC,       /*! Discover Media Control Profile related services */
  AUDCTUS_DISC_CCP_SVC,       /*! Discover Call Control Profile related services */
  AUDCTUS_DISC_SVC_MAX        /*! Discovery complete */
};

enum
{
  AUDCTUS_CFG_DIS_SVC,         /*! DIS services */
  AUDCTUS_CFG_GATT_SVC,        /*! GATT services */
  AUDCTUS_CFG_MCP_SVC,         /*! Configured MCP service */
  AUDCTUS_CFG_CCP_SVC,         /*! Configured CCP service */
  AUDCTUS_CFG_SVC_MAX          /*! Configuration complete */
};

/* Default EATT Channels */
#ifndef AUDCTUS_EATT_CHAN_MAX
#define AUDCTUS_EATT_CHAN_MAX       2
#endif

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not the user application.

// Sanity check on the BLE_CONN_MAX definition.
_Static_assert(BLE_CONN_MAX <= LL_MAX_CONN, "BLE_CONN_MAX cannot be larger than LL_MAX_CONN.");

/// Advertising data, discoverable mode.
static const uint8_t gcAudctusAdvDataDisc[] =
{
    /*! flags */
    2,                                                  /*! length */
    DM_ADV_TYPE_FLAGS,                                  /*! AD type */
    DM_FLAG_LE_GENERAL_DISC |                           /*! flags */
    DM_FLAG_LE_BREDR_NOT_SUP,

    /*! device name */
    7,                                                 /*! length */
    DM_ADV_TYPE_LOCAL_NAME,                             /*! AD type */
    'A',  'u',  'd', 'c', 't', 'l',


    /*! service UUID list */
    19,                                                 /*! length */
    DM_ADV_TYPE_16_UUID,                                /*! AD type */
    UINT16_TO_BYTES(ATT_UUID_COMMON_AUDIO_SERVICE),     /*! CAS service */
    UINT16_TO_BYTES(ATT_UUID_AUDIO_STRM_CTRL_SERVICE),  /*! ASCS service */
    UINT16_TO_BYTES(ATT_UUID_PUB_AUDIO_CAP_SERVICE),    /*! PAC service */
    UINT16_TO_BYTES(ATT_UUID_TMAP_SERVICE),             /*! TMAP service */
    UINT16_TO_BYTES(ATT_UUID_VOLUME_CTRL_SERVICE),      /*! VCS  service */
    UINT16_TO_BYTES(ATT_UUID_DEVICE_INFO_SERVICE),      /*! Device Info service */
    UINT16_TO_BYTES(ATT_UUID_MICROPHONE_CTRL_SERVICE),  /*! MICS service */
    UINT16_TO_BYTES(ATT_UUID_AUDIO_INPUT_CTRL_SERVICE), /*! AICS service */
    UINT16_TO_BYTES(ATT_UUID_COORDINATED_SET_ID_SERVICE), /*! CSIS service */

    /* ASE service data */
    9,                                                  /*! length */
    DM_ADV_TYPE_SERVICE_DATA,                           /*! AD type */
    UINT16_TO_BYTES(ATT_UUID_AUDIO_STRM_CTRL_SERVICE),  /*! ASC service */
    BAP_ANNC_TYPE_GEN,                                  /*! General Announcement */
    UINT16_TO_BYTES(AUDCTUS_AV_SNK_AUD_CTX),            /*! Available Sink Contexts */
    UINT16_TO_BYTES(0x00),                              /*! Not Available to Send Source Contexts */
    0x00,                                               /*! Length of Metadata */
    /*! Metadata -- empty */

    /*! service UUID list */
    5,                                                  /*! length */
    DM_ADV_TYPE_SERVICE_DATA,                           /*! AD type */
    UINT16_TO_BYTES(ATT_UUID_TMAP_SERVICE),             /*! TMAP service */
    (1 << TMAS_ROLE_BIT_CT), 0x00,                      /*! TMAP Role */

};

/*! \brief Peripheral connection specification. */
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
/*! configurable parameters for master */
static const appMasterCfg_t gcAudctusMasterCfg =
{
    .scanInterval =     96,                     /*! The scan interval, in 0.625 ms units */
    .scanWindow =       24,                     /*! The scan window, in 0.625 ms units  */
    .scanDuration  =    1000,                   /*! The scan duration in ms */
    .discMode =         DM_DISC_MODE_NONE,      /*! The GAP discovery mode */
    .scanType =         DM_SCAN_TYPE_ACTIVE     /*! The scan type (active or passive) */
};
#endif //AUDCTUS_PTS_TESTER_ENABLED

/*! \brief Configurable parameters for extended master */
static const appExtMasterCfg_t gcAudctusExtMasterCfg =
{
    .scanInterval = {96        },           /*! The scan interval, in 0.625 ms units */
    .scanWindow =   {48        },           /*! The scan window, in 0.625 ms units. Must be less than or equal to scan interval. */
    .scanDuration  = 4000,                  /*! The scan duration in ms. Set to zero or both duration and period to non-zero to scan until stopped. */
    .scanPeriod    = 0,                     /*! The scan period, in 1.28 sec units. Set to zero to disable periodic scanning. */
    .discMode      = DM_DISC_MODE_NONE,     /*! The GAP discovery mode (general, limited, or none) */
    .scanType      = {DM_SCAN_TYPE_ACTIVE}  /*! The scan type (active or passive) */
};

/*! \brief Configurable parameters for service and characteristic discovery */
static const appDiscCfg_t gcAuductusDiscCfg =
{
    .waitForSec = false,                    /*! true to wait for a secure connection before initiating discovery */
    .readDbHash = true                      /*! true to fall back on database hash to verify handles when no bond exists. */
};

/*! \brief Configurable parameters for advertising */
static const appAdvCfg_t gcAudctusAdvCfg =
{
    .advDuration = {60000, 0, 0},           /*! Advertising durations in ms */
    .advInterval = {  800, 0, 0}            /*! Advertising intervals in 0.625 ms units */
};

/*! \brief Configurable parameters for extended and periodic advertising */
static const appExtAdvCfg_t gcAudctusExtAdvCfg =
{
    .advDuration    = {60000},                /*! Advertising durations for extended advertising in ms */
    .advInterval    = {  800},                /*! Advertising intervals for extended advertising in 0.625 ms
                                              units (20 ms to 10.24 s) */
    .maxEaEvents    = {    0},                /*! Maximum number of extended advertising events Controller
                                              will send prior to terminating extended advertising */
    .useLegacyPdu   = {false },               /*! Whether to use legacy advertising PDUs with extended
                                              advertising. If set to true then length of advertising
                                              data cannot exceed 31 octets. */
    .perAdvInterval = {   80}                 /*! Advertising intervals for periodic advertising in 1.25 ms
                                              units (7.5 ms to 81.91875 s). */
};

/*! \brief Configurable parameters for slave */
static const appSlaveCfg_t gcAudctusSlaveCfg =
{
    .connMax = BLE_CONN_MAX,                        /*! Maximum connections */
};

/*! \brief Configurable parameters for security */
static const appSecCfg_t gcAudctusSecCfg =
{
    .auth = DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,   /*! Authentication and bonding flags */
    .iKeyDist = 0,                                 /*! Initiator key distribution flags */
    .rKeyDist = DM_KEY_DIST_LTK,                   /*! Responder key distribution flags */
    .oob = false,                                  /*! true if Out-of-band pairing data is present */
    .initiateSec = true                            /*! true to initiate security upon connection */
};

/*! \brief Configurable parameters for connection parameter update */
static const appUpdateCfg_t gcAudctusUpdateCfg =
{
    .idlePeriod = 6000,                    /*! Connection idle period in ms before attempting
                                              connection parameter update; set to zero to disable */
    .connIntervalMin = 640,                /*! Minimum connection interval in 1.25ms units */
    .connIntervalMax = 800,                /*! Maximum connection interval in 1.25ms units */
    .connLatency = 0,                      /*! Connection latency */
    .supTimeout = 900,                     /*! Supervision timeout in 10ms units */
    .maxAttempts = 5                       /*! Number of update attempts before giving up */
};

/*! \brief SMP security parameter configuration */
static const smpCfg_t gcAudctusSmpCfg =
{
    .attemptTimeout = 500,                 /*! 'Repeated attempts' timeout in msec */
    .ioCap = SMP_IO_NO_IN_NO_OUT,          /*! I/O Capability */
    .minKeyLen = 7,                        /*! Minimum encryption key length */
    .maxKeyLen = 16,                       /*! Maximum encryption key length */
    .maxAttempts = 1,                      /*! Attempts to trigger 'repeated attempts' timeout */
    .auth = 0,                             /*! Device authentication requirements */
    .maxAttemptTimeout = 64000,            /*! Maximum repeated attempts timeout in msec */
    .attemptDecTimeout = 64000,            /*! Time msec before attemptExp decreases */
    .attemptExp = 2                        /*! Repeated attempts multiplier exponent */
};

/*! \brief ATT configurable parameters (increase MTU) */
static const attCfg_t gcAudctusAttCfg =
{
    .discIdleTimeout = 15,                 /* ATT server service discovery connection idle timeout in seconds */
    .mtu = ATT_DESIRED_MTU,                /* desired ATT MTU */
    .transTimeout = ATT_MAX_TRANS_TIMEOUT, /* transaction timeout in seconds */
    .numPrepWrites = 4                     /* number of queued prepare writes supported by server */
};

/*! \brief Configurable parameters for EATT */
const uint8_t gcAudctusEattPriorityTbl[] = {0, 1, 2, 3, 4, 5, 6, 7};

const eattCfg_t gcAudctusEattCfg =
{
    .mtu = ATT_DESIRED_MTU,                /* MTU */
    .mps = ATT_DESIRED_MTU,                /* MPS */
    .initiateEatt = true,                  /* Initiate EATT on connection open */
    .authoriz = false,                     /* Authorization required */
    .secLevel = DM_SEC_LEVEL_NONE,         /* Security level required */
    .numChans = AUDCTUS_EATT_CHAN_MAX,     /* Number of enhanced l2cap channels per connection */
    gcAudctusEattPriorityTbl,                /* Channel priority table */
};

/// Buffer Pool runtime configuration.
/// @details Buffers will be allocated with the first possible match.
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    { .len = 16,                   .num = 8                                             },
    { .len = 32,                   .num = 2 + 2 * BLE_CONN_MAX                          },
    { .len = 269,                  .num = LL_RT_CFG_MAX_ADV_REPORTS                     },
    { .len = 266,                  .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS },
    { .len = 274,                  .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS + LL_RT_CFG_NUM_ISO_TX_BUFS + LL_RT_CFG_NUM_ISO_RX_BUFS  },
    { .len = 925,                  .num = 1                                             }
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! Application message type */
typedef union
{
    wsfMsgHdr_t     hdr;
    dmEvt_t         dm;
    attsCccEvt_t    ccc;
    attEvt_t        att;
} AudctusMsg_t;


/******************************************************************************\
 *  Global variables
\******************************************************************************/
/// WSF handler ID.
wsfHandlerId_t gBleHandlerId;

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! Application control block */
static struct
{
  uint8_t          audctusAdvHandle;  /*! Advertising set handle */
  bool             useLegacy;         /*! Use legacy advertising */
  dmConnId_t       connId;

  uint16_t         hdlList[APP_DB_HDL_LIST_LEN];   /*! Cached handle list */
  uint8_t          hdlListLen;                     /*! Cached handle list length */
  uint8_t          discState;                      /*! Service discovery state */
  uint8_t          cfgState;                       /*! Service configuration state */
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
  uint8_t          addrType;                     /*! Type of address of device to connect to */
  bdAddr_t         addr;                         /*! Address of device to connect to */
  appDbHdl_t       dbHdl;

  uint8_t          autoConnect;
  uint8_t          doConnect;
#endif
} gAudctusCb;


/// Persistent BB runtime configuration.
static BbRtCfg_t gBbRtCfg;

/// Persistent LL runtime configuration.
static LlRtCfg_t gLlRtCfg;

/* Default value for CCC indications */
const uint8_t audctusCccIndVal[2] = { UINT16_TO_BYTES(ATT_CLIENT_CFG_INDICATE) };

/* Default value for CCC notifications */
const uint8_t audctusCccNtfVal[2] = { UINT16_TO_BYTES(ATT_CLIENT_CFG_NOTIFY) };

/* Default value for Client Supported Features */
const uint8_t audctusCsfVal[1] = { ATTS_CSF_ROBUST_CACHING | ATTS_CSF_EATT_BEARER | ATTS_CSF_MULTI_VAL_NTF };

/* List of DIS characteristics to configure after service discovery */
static const attcDiscCfg_t gcAudctusCfgDisList[] =
{
  /* Read:  DIS Manufacturer name string */
  {NULL, 0, DIS_MFNS_HDL_IDX},

  /* Read:  DIS Model number string */
  {NULL, 0, DIS_MNS_HDL_IDX},

  /* Read:  DIS Serial number string */
  {NULL, 0, DIS_SNS_HDL_IDX},

  /* Read:  DIS Hardware revision string */
  {NULL, 0, DIS_HRS_HDL_IDX},

  /* Read:  DIS Firmware revision string */
  {NULL, 0, DIS_FRS_HDL_IDX},

  /* Read:  DIS Software revision string */
  {NULL, 0, DIS_SRS_HDL_IDX},

  /* Read:  DIS System ID */
  {NULL, 0, DIS_SID_HDL_IDX}
};

/* List of GATT characteristics to configure after service discovery */
static const attcDiscCfg_t gcAudctusCfgGattList[] =
{
  /* Write:  GATT service changed ccc descriptor */
  {audctusCccIndVal, sizeof(audctusCccIndVal), GATT_SC_CCC_HDL_IDX},

  /* Write:  GATT client supported features */
  {audctusCsfVal, sizeof(audctusCsfVal), GATT_CSF_HDL_IDX},
};

/*! Start of each service's handles in the handle list */
#define AUDCTU_DISC_GATT_START        0
#define AUDCTU_DISC_DIS_START         (AUDCTU_DISC_GATT_START + GATT_HDL_LIST_LEN)

/*! Pointers into handle list for each service's handles */
static uint16_t* gpAudctusGattHdlList = &gAudctusCb.hdlList[AUDCTU_DISC_GATT_START];
static uint16_t* gpAudctusDisHdlList = &gAudctusCb.hdlList[AUDCTU_DISC_DIS_START];

/* Characteristic configuration list length */
#define AUDCTUS_CFG_GATT_LIST_LEN   (sizeof(gcAudctusCfgGattList) / sizeof(attcDiscCfg_t))
#define AUDCTUS_CFG_DIS_LIST_LEN   (sizeof(gcAudctusCfgDisList) / sizeof(attcDiscCfg_t))

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/
static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg);
static void BLE_AudctusDiscCback(dmConnId_t connId, uint8_t status);

#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
static void BLE_AudctusScanReport(dmEvt_t* pMsg);
static void BLE_AudctusScanStop(dmEvt_t* pMsg);
static void BLE_AudctusExtScanReport(dmEvt_t* pMsg);
static void BLE_AudctusExtScanStop(dmEvt_t* pMsg);
void BLE_AudctusTestConnect(uint8_t* bd_addr);
#endif // AUDCTUS_PTS_TESTER_ENABLED
/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

#ifdef DBG_PRINT
/**
 * @brief Print the Long-Term Key.
 * @param connId Connection ID.
 * @return None.
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
#endif // DBG_PRINT

/**
 * @brief Device Manager event callback.
 * @param pEvt DM event.
 * @return None.
 */
static void BLE_DmHandler(dmEvt_t *pEvt)
{
    dmEvt_t *pMsg;
    uint16_t len;
    len = DmSizeOfEvt(pEvt);
    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
        memcpy(pMsg, pEvt, len);
        WsfMsgSend(gBleHandlerId, pMsg);
    }
    // Pass the message to TMAP
    TmapCtuDmCback(pEvt);
}

/**
 * @brief ATT event callback.
 * @param pEvt ATT event.
 */
static void BLE_AttHandler(attEvt_t *pEvt)
{
    attEvt_t *pMsg;
    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t *) (pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(gBleHandlerId, pMsg);
    }
}

/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pMsg Pointer to message.
 * @return None.
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
            // process discovery-related ATT messages
            AppDiscProcAttMsg((attEvt_t*)pEvt);

            /* Process server-related ATT messages. */
            AppServerProcAttMsg(pEvt);
        }
        // Process DM messages.
        else if ((pEvt->event >= DM_CBACK_START) && (pEvt->event <= DM_CBACK_END))
        {
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
            AppMasterProcDmMsg((dmEvt_t*)pEvt);

            /* process security-related messages */
            AppMasterSecProcDmMsg((dmEvt_t*)pEvt);

            /* process server-related DM messages */
            AppServerProcDmMsg(pEvt, DM_ROLE_MASTER);
#else
            // Process advertising and connection-related messages.
            AppSlaveProcDmMsg((dmEvt_t*)pEvt);
            // Process security-related messages.
            AppSlaveSecProcDmMsg((dmEvt_t*)pEvt);
            // Process server-related DM messages */
            AppServerProcDmMsg(pEvt, DM_ROLE_SLAVE);
#endif //AUDCTUS_PTS_TESTER_ENABLED

            // process discovery-related messages
            AppDiscProcDmMsg((dmEvt_t*)pEvt);

        }
        else
        {
            // no specific message to process
        }
        // Perform profiles, subsystems and other tasks related operations.
        BLE_ProcSubsysMsg(pEvt);
        TmapProcMsg(pEvt);
    }
}

/**
 *  @brief Set up advertising and other procedures that need to be performed after
 *          device reset.
 *  @param pMsg    Pointer to message.
 *  @return None.
 */
static void BLE_AudctusSetup(dmEvt_t *pMsg)
{
    if (gAudctusCb.useLegacy)
    {
        /* Set advertising and scan response data for discoverable mode */
        AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(gcAudctusAdvDataDisc), (uint8_t *) gcAudctusAdvDataDisc);
        AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(gcAudctusAdvDataDisc), (uint8_t *)gcAudctusAdvDataDisc);

        /* Set advertising and scan response data for connectable mode */
        AppAdvSetData(APP_ADV_DATA_CONNECTABLE, sizeof(gcAudctusAdvDataDisc), (uint8_t *) gcAudctusAdvDataDisc);
        AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, sizeof(gcAudctusAdvDataDisc), (uint8_t *)gcAudctusAdvDataDisc);
#if (AUDCTUS_PTS_TESTER_ENABLED != 1)
        /* Start advertising; automatically set connectable/discoverable mode and bondable mode */
        AppAdvStart(APP_MODE_AUTO_INIT);
#endif
    }
    else
    {
        uint8_t audctusAdvHandle = gAudctusCb.audctusAdvHandle;
        /* Set extended advertising and scan response data for discoverable mode */
        AppExtAdvSetData(audctusAdvHandle, APP_ADV_DATA_DISCOVERABLE, sizeof(gcAudctusAdvDataDisc),
                        (uint8_t *) gcAudctusAdvDataDisc, sizeof(gcAudctusAdvDataDisc));

        /* Set extended advertising and scan response data for connectable mode */
        AppExtAdvSetData(audctusAdvHandle, APP_ADV_DATA_CONNECTABLE, 0, NULL, 0);
#if (AUDCTUS_PTS_TESTER_ENABLED != 1)
        /* Start extended advertising; automatically set connectable/discoverable mode and bondable mode */
        AppExtAdvStart(1, &audctusAdvHandle, APP_MODE_AUTO_INIT);
#endif
    }

}

/**
 *  @brief Start Advertising.
 *  @param mode  Discoverable/connectable Advertising mode.
 *  @return None.
 */
void BLE_AudctusStartAdv(uint8_t mode)
{
    if (gAudctusCb.useLegacy)
    {
        AppAdvStart(APP_MODE_AUTO_INIT);
    }
    else
    {
        uint8_t audctusAdvHandle = gAudctusCb.audctusAdvHandle;
        AppExtAdvStart(1, &audctusAdvHandle, mode);
    }
}

/**
 *  @brief Set advertising type to Extended or Legacy.
 *  @param isLegacy   mode. true - Legacy, false - Extended
 *  @return None.
 *  @note DmDevReset() needs to be called after this function executes to reset the stack before
 *        starting the advertising again in the new mode.
 */
void BLE_AudctusSetAdvExtType(bool isLegacy)
{
    gAudctusCb.useLegacy = isLegacy;

    DmRegister(BLE_DmHandler);
    DmConnRegister(DM_CLIENT_ID_APP, BLE_DmHandler);
    AttRegister(BLE_AttHandler);
    AttConnRegister(AppServerConnCback);
}

/**
 *  @brief Stop Advertising.
 *  @return None.
 */
void BLE_AudctusStopAdv()
{
    if (gAudctusCb.useLegacy)
    {
        AppAdvStop();
    }
    else
    {
        uint8_t audctusAdvHandle = gAudctusCb.audctusAdvHandle;
        AppExtAdvStop(1, &audctusAdvHandle);
    }
}

/**
 *  @brief BAP Server ASE update callback to TMAS.
 *  @param connId       DM connection identifier.
 *  @param pAcb         ASE control block.
 *  @param aseOldState  ASE old state
 *  @return None.
 */
static void BLE_AudctusMediaEventHandler(dmConnId_t connId, tmapAseCb_t* pAcb, uint8_t aseOldState)
{
    switch (pAcb->aseState)
    {
    case TMAP_IDLE:
        /* if ASE Released (no caching) */
        if (aseOldState == TMAP_RELEASING)
        {
            //Handle ASE Released
        }
        break;

    case TMAP_ENABLING:
    case TMAP_STREAMING:
        if (aseOldState == pAcb->aseState)
        {
            //Handle ASE Metadata Update
        }
        break;

    case TMAP_CODEC_CFG:
    case TMAP_QOS_CFG:
    case TMAP_DISABLING:
    case TMAP_RELEASING:
        break;

    default:
        break;
    }
}

/**
 *  @brief Application  ATT authorized callback for access attriute.
 *  @param permit    The permit of the attribute
 *  @param handle    The handle of the attribute
 *  @return the value to accept or reject authorization.
 */
static uint8_t BLE_AudctusAuthorHandler(dmConnId_t connId, uint8_t permit, uint16_t handle)
{
    return ATT_SUCCESS;
}

/**
 *  @brief Start the application.
 *  @return None
 */
void BLE_AudctusStart(void)
{
    /* Initialize control block */
    gAudctusCb.useLegacy = AUDCTUS_USE_LEGACY;

    DmExtScanInit();
    DmExtAdvInit();
    DmScanModeExt();
    DmAdvModeExt();

    AttsAuthorRegister(BLE_AudctusAuthorHandler);

    TmapCtUnicastRegisterServRecord();
    TmapCtuMediaEventRegister(BLE_AudctusMediaEventHandler);

    /* Microphone (Source Audio Locations: Left, Right) */
    TmapCtUMediaSetAudioLocation(BAP_AUDIO_DIR_SOURCE, GA_AUDIO_LOC_FRT_LEFT_BIT | GA_AUDIO_LOC_FRT_RIGHT_BIT);

    /* Media (Sink Audio Locations: Left, Right) */
    TmapCtUMediaSetAudioLocation(BAP_AUDIO_DIR_SINK, GA_AUDIO_LOC_FRT_LEFT_BIT | GA_AUDIO_LOC_FRT_RIGHT_BIT);

    /* Set Available Audio Contexts */
    TmapCtUMediaSetAvailAudioCtx(BAP_AUDIO_DIR_SINK, AUDCTUS_AV_SNK_AUD_CTX);
    TmapCtUMediaSetAvailAudioCtx(BAP_AUDIO_DIR_SOURCE, AUDCTUS_AV_SRC_AUD_CTX);

    /* Set Supported Audio Contexts */
    TmapCtUMediaSetSupAudioCtx(BAP_AUDIO_DIR_SINK, AUDCTUS_SNK_SUP_AUD_CTX);
    TmapCtUMediaSetSupAudioCtx(BAP_AUDIO_DIR_SOURCE, AUDCTUS_SRC_SUP_AUD_CTX);

    /* Register for app framework discovery callbacks */
    AppDiscRegister(BLE_AudctusDiscCback);

    /* Set Extended Advertising */
    BLE_AudctusSetAdvExtType(false);

    /* Reset the device */
    DmDevReset();
}

/**
 * @brief Process messages from profiles, subsystems and other tasks.
 * @param pMsg Pointer to message.
 * @return None
 */
static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg)
{
    dmConnId_t connId = (dmConnId_t)pMsg->param;

    switch (pMsg->event)
    {
        // External signals.
        case BLE_INIT_SIG:
            BLE_AudctusStart();
            break;

#if (AUDCTUS_PTS_TESTER_ENABLED == 1)

        case DM_SCAN_REPORT_IND:
            BLE_AudctusScanReport((dmEvt_t*)pMsg);
        break;
        case DM_EXT_SCAN_REPORT_IND:
            BLE_AudctusExtScanReport((dmEvt_t*)pMsg);
        break;
        case DM_SCAN_START_IND:
        break;
        case DM_EXT_SCAN_START_IND:
        break;

        case DM_EXT_SCAN_STOP_IND:
            BLE_AudctusScanStop((dmEvt_t*)pMsg);
        break;
        case DM_SCAN_STOP_IND:
            BLE_AudctusExtScanStop((dmEvt_t*)pMsg);
        break;

#endif // AUDCTUS_PTS_TESTER_ENABLED
        // Device manager events.
        case DM_RESET_CMPL_IND:

            AttsCalculateDbHash();
            DmSecGenerateEccKeyReq();
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
#ifdef DBG_PRINT
            printf("Tester enabled with connection to BDADDR of PTS Dongle\r\n");
#endif
            AppDbNvmReadAll();
            BLE_AudctusTestConnect((uint8_t*)gPTSDongleAddr);
#endif  //AUDCTUS_PTS_TESTER_ENABLED
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
            gAudctusCb.connId = connId;
            break;

        case DM_CONN_CLOSE_IND:
            break;

        case DM_SEC_PAIR_IND:
            break;

        case DM_SEC_PAIR_CMPL_IND:
            DmSecGenerateEccKeyReq();
#ifdef DBG_PRINT
            BLE_PrintLTK((dmConnId_t)pMsg->param);
#endif // DBG_PRINT
            break;

        case DM_SEC_PAIR_FAIL_IND:
            DmSecGenerateEccKeyReq();
            break;

        case DM_SEC_ENCRYPT_IND:
            break;

        case DM_SEC_ENCRYPT_FAIL_IND:
            break;

        case DM_SEC_AUTH_REQ_IND:
            //NOTE: No IO to display or enter passkey. Should be handled by the user application and hardware.
            break;

        case DM_SEC_ECC_KEY_IND:
            DmSecSetEccKey(&((dmEvt_t *)pMsg)->eccMsg.data.key);
            BLE_AudctusSetup((dmEvt_t *) pMsg);
            break;

        case DM_SEC_COMPARE_IND:
            break;

        case DM_PRIV_CLEAR_RES_LIST_IND:
            break;

        case DM_HW_ERROR_IND:
            break;

        // ATT server events.
        case ATTS_HANDLE_VALUE_CNF:
            break;

        case ATTS_CCC_STATE_IND:
        case ATTC_WRITE_RSP:
        case ATTC_READ_RSP:
        case ATTC_READ_BY_TYPE_RSP:
            break;
        default:
            break;
    }
}

/**
 * @brief Buffer initialization.
 * @details gcBuffPoolDesc variable can be modified to suit the user's needs.
 * @return None
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
#endif
}

/**
 * @brief Get the current Baseband runtime configuration.
 * @param[out] bbRtCfg returned Baseband runtime configuration.
 * @return None
 */
static inline void BLE_GetCurrentBbRtConfig(BbRtCfg_t *bbRtCfg)
{
    PalBbLoadCfg((PalBbCfg_t*)bbRtCfg);
}

/**
 *  @brief Initialize the stack.
 *  @return memUsed Memory used by stack initialization
 */
static uint32_t BLE_StackInit(LlRtCfg_t *llRtCfg, BbRtCfg_t *bbRtCfg)
{
    bdAddr_t bdAddr;
    wsfHandlerId_t handlerId;

    BLE_GetCurrentBbRtConfig(bbRtCfg);

    BLE_GetCurrentLlRtConfig(llRtCfg);

    // Custom LL configuration.
    llRtCfg->maxConn       = BLE_CONN_MAX;
    llRtCfg->maxAclLen     = ATT_DESIRED_MTU + L2C_HDR_LEN;
    llRtCfg->numTxBufs     = LL_RT_CFG_NUM_TX_BUFS;
    llRtCfg->numRxBufs     = LL_RT_CFG_NUM_RX_BUFS;
    llRtCfg->numIsoTxBuf   = LL_RT_CFG_NUM_ISO_TX_BUFS;
    llRtCfg->numIsoRxBuf   = LL_RT_CFG_NUM_ISO_RX_BUFS;
    llRtCfg->maxAdvReports = LL_RT_CFG_MAX_ADV_REPORTS;

    LlInitRtCfg_t llCfg =
    {
        .pBbRtCfg     = bbRtCfg,
        .wlSizeCfg    = 4, // Whitelist size.
        .rlSizeCfg    = 4, // Resolvable list size.
        .plSizeCfg    = 4, // Periodic list size.
        .pLlRtCfg     = llRtCfg,
        .pFreeMem     = WsfHeapGetFreeStartAddress(),
        .freeMemAvail = WsfHeapCountAvailable()
    };

    // Load the Bluetooth address and use it as the seed for the PRAND.
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));

    uint32_t memUsed = LlInit(&llCfg);

    // Load the Bluetooth address and use it as the seed for the PRAND.
    LlSetBdAddr((uint8_t*)bdAddr);

#ifdef DBG_PRINT
    // Print EM9305 and BT Address
    printf("\r\nEMB. BT ADDR %02X:%02X:%02X:%02X:%02X:%02X \r\n", bdAddr[0],bdAddr[1],bdAddr[2],bdAddr[3],bdAddr[4],bdAddr[5]);
#endif

    // Security Module.
    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    uint16_t aclLength = ATT_DESIRED_MTU + L2C_HDR_LEN;

    // Host Controller Interface.
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);
    HciSetMaxRxAclLen(aclLength);
    HciCoreIsoInit();

    // Device Manager.
    handlerId = WsfOsSetNextHandler(DmHandler);
 #if (AUDCTUS_PTS_TESTER_ENABLED == 1)
    DmDevVsInit(0);
    DmDevPrivInit();
    DmConnInit();
    DmExtScanInit();
    DmExtConnMasterInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmCisInit();
    DmCisMasterInit();
    DmCisSlaveInit();
    DmBisMasterInit();
    DmBisSlaveInit();
    DmIsoInit();
#else
    DmDevVsInit(0);
    DmConnInit();
    DmExtAdvInit();
    DmExtConnSlaveInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmCisInit();
    DmCisSlaveInit();
    DmBisSlaveInit();
    DmIsoInit();
#endif //AUDCTUS_PTS_TESTER_ENABLED
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
    AttcInit();
    // Security Manager.
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
    SmpiInit();
    SmpiScInit();
#endif
    SmprInit();
    SmprScInit();

    // Application framework (AF).
    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
    AppMasterInit();
    AppDiscInit();
#else
    AppSlaveInit();
    AppServerInit();
#endif // AUDCTUS_PTS_TESTER_ENABLED

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gBleHandlerId = handlerId;

    // Set configuration pointers.
    pAppAdvCfg = (appAdvCfg_t*)&gcAudctusAdvCfg;
    pAppSlaveCfg = (appSlaveCfg_t*)&gcAudctusSlaveCfg;
    pAppSecCfg = (appSecCfg_t*)&gcAudctusSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t*)&gcAudctusUpdateCfg;
    pAppDiscCfg = (appDiscCfg_t*)&gcAuductusDiscCfg;

    // Set extended configuration pointers
    pAppExtAdvCfg = (appExtAdvCfg_t *) &gcAudctusExtAdvCfg;
    pAppExtMasterCfg = (appExtMasterCfg_t*)&gcAudctusExtMasterCfg;

    pSmpCfg = (smpCfg_t*)&gcAudctusSmpCfg;
    pAttCfg = (attCfg_t *)&gcAudctusAttCfg;
    pEattCfg = (eattCfg_t *) &gcAudctusEattCfg;

    // Initialize EATT subsystem
    EattInit(EATT_ROLE_ACCEPTOR);
    EattsInit();
    TmapCtUnicastInit(handlerId);

    gAudctusCb.hdlListLen = APP_DB_HDL_LIST_LEN;
    return memUsed;
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/
/**
 *  @brief Initialize the BLE.
 *  @return None
 */
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

    // Initialize attribute server database.
    SvcDisAddGroup();
}

/**
 *  @brief Discovery callback.
 *  @param connId    Connection identifier.
 *  @param status    Service or configuration status.
 *  @return None
 */
static void BLE_AudctusDiscCback(dmConnId_t connId, uint8_t status)
{
  switch (status)
  {
  case APP_DISC_INIT:
    /* set handle list when initialization requested */
    AppDiscSetHdlList(connId, gAudctusCb.hdlListLen, gAudctusCb.hdlList);
    break;

  case APP_DISC_READ_DATABASE_HASH:
    /* Read peer's database hash */
    AppDiscReadDatabaseHash(connId);
    break;

  case APP_DISC_SEC_REQUIRED:
    /* initiate security */
    AppMasterSecurityReq(connId);
    break;

  case APP_DISC_START:
    /* initialize discovery state */
    gAudctusCb.discState = AUDCTUS_DISC_GATT_SVC;
    /* store possible change in cache by hash */
    AppDbNvmStoreCacheByHash(AppDbGetHdl(connId));

#ifdef DBG_PRINT
    printf("gpAudctusGattHdlList %04x\r\n", gpAudctusGattHdlList);
#endif
    /* discover GATT service */
    GattDiscover(connId, gpAudctusGattHdlList);
    break;

  case APP_DISC_FAILED:
    /* Else falls through. */
  case APP_DISC_CMPL:
    /* next discovery state */
    gAudctusCb.discState++;
    if (gAudctusCb.discState == AUDCTUS_DISC_DIS_SVC)
    {
      /* discover device information service */
      DisDiscover(connId, gpAudctusDisHdlList);
    }
    else if (gAudctusCb.discState == AUDCTUS_DISC_MCP_SVC)
    {
      /* discover MCP services */
      TmapCtuMcpDiscover(connId);
    }
    else if (gAudctusCb.discState == AUDCTUS_DISC_CCP_SVC)
    {
      /* discover CCP services */
      if (TmapCtuCcpDiscover(connId) == true)
      {
        /* discovery complete */
        AppDiscComplete(connId, APP_DISC_CMPL);

        /* store cached handle list in NVM */
        AppDbNvmStoreHdlList(AppDbGetHdl(connId));

        /* start configuration: configure DIS service */
        gAudctusCb.cfgState = AUDCTUS_CFG_DIS_SVC;
        AppDiscConfigure(connId, APP_DISC_CFG_START, AUDCTUS_CFG_DIS_LIST_LEN,
                        (attcDiscCfg_t*)gcAudctusCfgDisList,
                        DIS_HDL_LIST_LEN, gpAudctusDisHdlList);
      }
      else
      {
        /* Set state back */
        gAudctusCb.discState--;
      }
    }
    break;

  case APP_DISC_CFG_START:
    /* start configuration: configure DIS service */
    gAudctusCb.cfgState = AUDCTUS_CFG_DIS_SVC;
    AppDiscConfigure(connId, APP_DISC_CFG_START, AUDCTUS_CFG_DIS_LIST_LEN,
                    (attcDiscCfg_t*)gcAudctusCfgDisList,
                    DIS_HDL_LIST_LEN, gpAudctusDisHdlList);
    break;

  case APP_DISC_CFG_CMPL:
    /* next configuration state */
    gAudctusCb.cfgState++;
    if (gAudctusCb.cfgState == AUDCTUS_CFG_GATT_SVC)
    {
      /* configure GATT */
      AppDiscConfigure(connId, APP_DISC_CFG_START, AUDCTUS_CFG_GATT_LIST_LEN,
                        (attcDiscCfg_t*)gcAudctusCfgGattList,
                        GATT_HDL_LIST_LEN, gpAudctusGattHdlList);
    }
    else if (gAudctusCb.cfgState == AUDCTUS_CFG_MCP_SVC)
    {
      /* configure MCS */
      TmapCtuMcpConfigure(connId, APP_DISC_CFG_START);
    }
    else if (gAudctusCb.cfgState == AUDCTUS_CFG_CCP_SVC)
    {
#ifdef DBG_PRINT
        printf("APP_DISC_CFG  CCP SVC\r\n");
#endif
      /* configure TBS GTBS */
      TmapCtuCcpConfigure(connId, APP_DISC_CFG_START);
    }
    else
    {
      AppDiscComplete(connId, status);
      gAudctusCb.cfgState++;
    }
    break;

  case APP_DISC_CFG_CONN_START:
    /* configure audio profile service */
    TmapCtuMcpConfigure(connId, APP_DISC_CFG_START);
    break;

  default:
    break;
  }
}

/* The following codes are for CCP and MCP client testing, in normally case, it won't exists */
#if (AUDCTUS_PTS_TESTER_ENABLED == 1)
/**
 *  @brief The API is for testing to test MCP and CCP
 *  @param bd_addr   Bluetooth Address for connection.
 *  @return None
 */
void BLE_AudctusTestConnect(uint8_t* bd_addr)
{
    memcpy(&gAudctusCb.addr, bd_addr, sizeof(gAudctusCb.addr));
    gAudctusCb.addrType = DM_ADDR_PUBLIC;
    gAudctusCb.autoConnect = true;
    gAudctusCb.doConnect = false;
#ifdef DBG_PRINT
    printf("Connect BDAddr: ");
    // Print EM9305 and BT Address
    printf("%02x %02x %02x %02x %02x %02x \r\n", gAudctusCb.addr[0],gAudctusCb.addr[1],gAudctusCb.addr[2],gAudctusCb.addr[3],gAudctusCb.addr[4],gAudctusCb.addr[5]);
#endif
    AppExtScanStart(HCI_SCAN_PHY_LE_1M_BIT, gcAudctusMasterCfg.discMode,
                    &gcAudctusMasterCfg.scanType, gcAudctusMasterCfg.scanDuration, 0);
}

/**
 *  @brief  Handle a scan report.
 *  @param pMsg    Pointer to DM callback event message.
 *  @return None
 */
static void BLE_AudctusScanReport(dmEvt_t* pMsg)
{
    appDbHdl_t dbHdl = 0;

    /* disregard if not autoconnecting */
    if (!gAudctusCb.autoConnect)
    {
        return;
    }
    if ((gAudctusCb.addrType == pMsg->scanReport.addrType) &&
        (strncmp((char*)gAudctusCb.addr, (char*)&pMsg->scanReport.addr[0], 6) == 0))
    {
        /* stop scanning and connect */
        gAudctusCb.autoConnect = false;
        AppScanStop();

        /* Store peer information for connect on scan stop */
        gAudctusCb.addrType = DmHostAddrType(pMsg->scanReport.addrType);
        memcpy(gAudctusCb.addr, pMsg->scanReport.addr, sizeof(bdAddr_t));
        gAudctusCb.dbHdl = dbHdl;
        gAudctusCb.doConnect = true;
    }
}


/**
 *  @brief  Handle a scan report.
 *  @param pMsg    Pointer to DM callback event message.
 *  @return None
 */
static void BLE_AudctusExtScanReport(dmEvt_t* pMsg)
{
    appDbHdl_t dbHdl;
    /* disregard if not autoconnecting */
    if (!gAudctusCb.autoConnect)
    {
        return;
    }

    dbHdl = AppDbFindByAddr(pMsg->extScanReport.addrType, pMsg->extScanReport.addr);
    if (dbHdl != APP_DB_HDL_NONE)
    {
        gAudctusCb.dbHdl = dbHdl;
    }
    else
    {
        gAudctusCb.dbHdl = 0;
    }

    /* if we already have a bond with this device then connect to it */
    if ((gAudctusCb.addrType == pMsg->extScanReport.addrType) &&
        (strncmp((char*)gAudctusCb.addr, (char*)&pMsg->extScanReport.addr[0], 6) == 0))
    {
        /* stop scanning and connect */
        gAudctusCb.autoConnect = false;
        gAudctusCb.doConnect = true;

        AppExtScanStop();
    }
}

/**
 *  @brief Perform actions on scan stop.
 *  @param pMsg    Pointer to DM callback event message.
 *  @return None
 */
static void BLE_AudctusScanStop(dmEvt_t* pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    gAudctusCb.autoConnect = false;

    /* Open connection */
    if (gAudctusCb.doConnect)
    {
        AppExtConnOpen(HCI_INIT_PHY_LE_1M_BIT, gAudctusCb.addrType, gAudctusCb.addr, gAudctusCb.dbHdl);
        gAudctusCb.doConnect = false;
    }
  }
}

 /**
 *  @brief Perform actions on scan stop.
 *  @param pMsg    Pointer to DM callback event message.
 *  @return None
 */
static void BLE_AudctusExtScanStop(dmEvt_t* pMsg)
{
    if (pMsg->hdr.status == HCI_SUCCESS)
    {
        gAudctusCb.autoConnect = false;

        /* Open connection */
        if (gAudctusCb.doConnect)
        {
            AppExtConnOpen(HCI_INIT_PHY_LE_1M_BIT, gAudctusCb.addrType, gAudctusCb.addr, gAudctusCb.dbHdl);
            gAudctusCb.doConnect = false;
        }
    }
}

#endif // AUDCTUS_PTS_TESTER_ENABLED
