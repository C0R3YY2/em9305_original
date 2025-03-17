////////////////////////////////////////////////////////////////////////////////
///
/// @file       rpa_main.c
///
/// @project    EM9305
///
/// @brief      RPA main host interface file
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

#include "app_api.h"
#include "dm_api.h"
#include "hci_api.h"
#include "ll_api.h"
#include "rpa.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "smp_api.h"
#include "smp_handler.h"
#include "att_api.h"
#include "att_handler.h"
#include "hci_api.h"
#include "hci_handler.h"
#include "emb_database_api.h"

/******************************************************************************\
 *  External Variables
\******************************************************************************/
extern rpacb_t gRpaCb;
extern EMBDB_Record_t gRpaBondRec;
extern EMBDB_StorageBackend_t gRpaStorageBackend;

/******************************************************************************\
 *  Global Constants
\******************************************************************************/

/*! \brief Advertisement data array. */
static uint8_t advData[]  =
{
    /*! flags */
    2,                                      /*! length */
    DM_ADV_TYPE_FLAGS,                      /*! AD type */
    DM_FLAG_LE_GENERAL_DISC |               /*! flags */
    DM_FLAG_LE_BREDR_NOT_SUP,

    /*! device name */
    0x0B,                                      /*! length */
    DM_ADV_TYPE_LOCAL_NAME,                    /*! AD type */
    'E','M', '9', '3', '0', '5', '_', 'R', 'P', 'A'
};

/*! \brief Peripheral connection specification. */
const hciConnSpec_t gcRPAUpdateCfg =
{
    .connIntervalMin = 24,  /*! Minimum connection interval in 1.25ms units */
    .connIntervalMax = 800, /*! Maximum connection interval in 1.25ms units */
    .connLatency     = 5,   /*! Connection latency */
    .minCeLen        = 0,   /*! Minimum CE length */
    .maxCeLen        = 0xffff,   /*! Maximum CE length */
};


/*! \brief Battery measurement configuration. */
static const basCfg_t gcRPABasCfg =
{
    30,                 /*! Battery measurement timer expiration period in seconds */
    1,                  /*! Perform battery measurement after this many timer periods */
    100                 /*! Send battery level notification to peer when below this level */
};

/*! configurable parameters for security */
static const appSecCfg_t gcRPASecCfg =
{
  DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,    /*! Authentication and bonding flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK,      /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK,      /*! Responder key distribution flags */
  false,                                  /*! true if Out-of-band pairing data is present */
  true                                    /*! true to initiate security upon connection */
};

/*! SMP security parameter configuration */
static const smpCfg_t gcRpaSmpCfg =
{
  500,                                    /*! 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  1,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  0,                                      /*! Device authentication requirements */
  64000,                                  /*! Maximum repeated attempts timeout in msec */
  64000,                                  /*! Time msec before attemptExp decreases */
  2                                       /*! Repeated attempts multiplier exponent */
};

/*! ATT configurable parameters (increase MTU) */
static const attCfg_t gcRpaAttCfg =
{
  15,                                     /* ATT server service discovery connection idle timeout in seconds */
  241,                                    /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,                  /* transaction timeout in seconds */
  4                                       /* number of queued prepare writes supported by server */
};


/******************************************************************************\
 *  private functions definitions
\******************************************************************************/

/******************************************************************************/
/*!
 *  \brief  DM Start Advertisement procedure.
 *
 *  \param  None
 *  \return None
 */
/******************************************************************************/
static void rpaDmStartAdv()
{
    uint8_t  advHandle   = DM_ADV_HANDLE_DEFAULT;
    uint8_t  maxEaEvents = 1;
    uint16_t duration    = 0;
    uint16_t advDataLen  = sizeof(advData);


    DmAdvSetData(DM_ADV_HANDLE_DEFAULT, HCI_ADV_DATA_OP_COMP_FRAG, DM_DATA_LOC_ADV,
                     (uint8_t)advDataLen, advData);

    // No peer addr in provisioning mode
    DmAdvSetInterval(DM_ADV_HANDLE_DEFAULT, ADV_INTERVAL_MIN, ADV_INTERVAL_MAX);
    DmAdvConfig(DM_ADV_HANDLE_DEFAULT, DM_ADV_CONN_UNDIRECT, gRpaCb.peerAddrType, gRpaCb.peerAddr);

    DmAdvStart(1, &advHandle, &duration, &maxEaEvents);
}

/******************************************************************************/
/*!
 *  \brief  DM connection open procedure.
 *          Take action based on this connection open.
 *
 *  \param  pDmEvt    Device manager event message.
 *  \return None
 */
/******************************************************************************/
static void rpaDmConnOpen(dmEvt_t *pDmEvt)
{
    // store the connection handle
    gRpaCb.connId = (dmConnId_t)pDmEvt->hdr.param;
    /* check if we should do connection parameter update */
    if ((pDmEvt->connOpen.connInterval < gcRPAUpdateCfg.connIntervalMin) ||
        (pDmEvt->connOpen.connInterval > gcRPAUpdateCfg.connIntervalMax) ||
        (pDmEvt->connOpen.connLatency != gcRPAUpdateCfg.connLatency) ||
        (pDmEvt->connOpen.supTimeout != gcRPAUpdateCfg.supTimeout))
    {
        // If a filter accept listed device is able to send LL_CONN_IND PDU, we
        // accept the parameters from the connection indication, but if they are
        // outside of what we specify in gcRPAUpdateCfg, we'll try to tell the
        // other device to update the connection parameters to our specification.
        // The peer may or may not accept those terms.
        /* This is where a conn update could be scheduled. */
        gRpaCb.doConnUpdate       = true;
        gRpaCb.connUpdateAttempts = 0;
    }

    // Request security to initiate IRK exchange
    if (DmConnSecLevel(gRpaCb.connId) == DM_SEC_LEVEL_NONE)
    {
        DmSecSlaveReq(gRpaCb.connId, gcRPASecCfg.auth);
    }
}

/******************************************************************************/
/*!
 *  \brief  DM Connection Parameter update request process.
 *  Based on conditions defined here, send reponse to accept or reject the update request.
 *
 *  \param  pDmEvt    Device manager event message.
 *  \return None
 */
/******************************************************************************/
static void rpaDmConnParam(dmEvt_t *pDmEvt)
{
    // If remote connection parameter request is going to use more power than
    // we expect (specified in gcRPAUpdateCfg), then reject. This is an example
    // of how it could be done. We leave further power optimization to app developer.
    if ((pDmEvt->remConnParamReq.intervalMin < gcRPAUpdateCfg.connIntervalMin) ||
        (pDmEvt->remConnParamReq.intervalMax > gcRPAUpdateCfg.connIntervalMax) ||
        (pDmEvt->remConnParamReq.latency > gcRPAUpdateCfg.connLatency))
    {
        /* Reject the remote device's request to change connection parameters. */
        DmRemoteConnParamReqNegReply(gRpaCb.connId, HCI_ERR_UNSUP_FEAT);
    }
    /* Looks good we can accept. */
    else
    {
        hciConnSpec_t connSpec;
        connSpec.connIntervalMin = pDmEvt->remConnParamReq.intervalMin;
        connSpec.connIntervalMax = pDmEvt->remConnParamReq.intervalMax;
        connSpec.connLatency     = pDmEvt->remConnParamReq.latency;
        connSpec.supTimeout      = pDmEvt->remConnParamReq.timeout;
        /* Accept the remote device's request to change connection parameters. */
        DmRemoteConnParamReqReply(gRpaCb.connId, &connSpec);
    }
}

/******************************************************************************/
/*!
 *  \brief  DM Pairing procedure
 *
 *  \param  pDmEvt    Device manager event message.
 *  \return None
 */
/******************************************************************************/
static void rpaDmPair(dmEvt_t *pDmEvt)
{
    /* initialize key distribution */
    uint8_t rKeyDist = gcRPASecCfg.rKeyDist;
    uint8_t iKeyDist = gcRPASecCfg.iKeyDist;

    /* initialize stored keys */
    gRpaCb.rcvdKeys = 0;

    /* We must have the IRK for RPA */
    iKeyDist |= DM_KEY_DIST_IRK;

    /* only distribute keys both sides have agreed to */
    rKeyDist &= pDmEvt->pairInd.rKeyDist;
    iKeyDist &= pDmEvt->pairInd.iKeyDist;

    /* accept pairing request */
    DmSecPairRsp(gRpaCb.connId, gcRPASecCfg.oob, gcRPASecCfg.auth, iKeyDist, rKeyDist);
}

/******************************************************************************/
/*!
 *  \brief  Link key exchange procedure
 *
 *  \param  pDmEvt    Device manager event message.
 *  \return None
 */
/******************************************************************************/
static void rpaDmKeyExchange(dmEvt_t *pDmEvt)
{
    /* Store the received key in the nvm database. */
    gRpaCb.rcvdKeys |= pDmEvt->keyInd.type;
    switch (pDmEvt->keyInd.type)
    {
        case DM_KEY_LOCAL_LTK:
            gRpaBondRec.localLtk = pDmEvt->keyInd.keyData.ltk;
            gRpaBondRec.localLtkSecLevel = pDmEvt->keyInd.secLevel;
            gRpaBondRec.localLtkValid = 1;
            break;

        case DM_KEY_PEER_LTK:
            gRpaBondRec.peerLtk = pDmEvt->keyInd.keyData.ltk;
            gRpaBondRec.peerLtkSecLevel = pDmEvt->keyInd.secLevel;
            gRpaBondRec.peerLtkValid = 1;
            break;

        case DM_KEY_IRK:
            gRpaBondRec.peerIrk = pDmEvt->keyInd.keyData.irk;
            BdaCpy(gRpaBondRec.peerIrk.bdAddr, pDmEvt->keyInd.keyData.irk.bdAddr);
            BdaCpy(gRpaBondRec.peerAddress, pDmEvt->keyInd.keyData.irk.bdAddr);
            gRpaBondRec.peerIrkValid = 1;
            break;

        case DM_KEY_CSRK:
            gRpaBondRec.peerCsrk = pDmEvt->keyInd.keyData.csrk;
            gRpaBondRec.peerCsrkSecLevel = pDmEvt->keyInd.secLevel;
            gRpaBondRec.peerCsrkValid = 1;
            break;

        default:
            break;
    }
}

/******************************************************************************/
/*!
 *  \brief  DM Pairing complete procedure
 *
 *  \param  pDmEvt    Device manager event message.
 *  \return None
 */
/******************************************************************************/
static void rpaDmPairingComplete(dmEvt_t *pDmEvt)
{
    (void)pDmEvt; // unused

    /* if LL Privacy is supported and the peer device has distributed its IRK */
    if (HciLlPrivacySupported() && gRpaBondRec.peerIrkValid == 1)
    {
        BdaCpy(gRpaCb.peerAddr, gRpaBondRec.peerIrk.bdAddr);
        gRpaCb.peerAddrType = gRpaBondRec.peerIrk.addrType;

        (void)EMBDB_InvalidateAllRecordsWithPeerAddress(gRpaBondRec.peerIrk.bdAddr);

        if (EMBDB_IsDatabaseFull())
        {
            (void)EMBDB_EraseDatabaseMemory();
        }
        int32_t status = EMBDB_StoreRecord(&gRpaBondRec);
        if (0 == status)
        {
            gRpaCb.bonded       = true;
            gRpaCb.bondable     = false;

            // Add device to the resolvable list
            DmPrivAddDevToResList(gRpaCb.peerAddrType, gRpaCb.peerAddr, gRpaBondRec.peerIrk.key,
                               DmSecGetLocalIrk(), true, pDmEvt->hdr.param);
        }
        else
        {
            // Paring info not stored, notify peer with disconnect pairing not allowed.
            gRpaCb.bonded = false;
            gRpaCb.bondable = true;
            DmConnClose(DM_CLIENT_ID_APP, gRpaCb.connId, HCI_ERR_PAIRING_NOT_ALLOWED);
        }
    }
}

/******************************************************************************/
/*!
 *  \brief  RPA ltk response
 *
 *  \param  pDmEvt    Device manager event message.
 *  \return None
 */
/******************************************************************************/
static void rpaDmRspLtk(dmEvt_t *pDmEvt)
{
    (void)pDmEvt; // unused

    if (gRpaBondRec.localLtkValid == 1)
    {
        /* we found the key */
        DmSecLtkRsp(gRpaCb.connId, true, gRpaBondRec.localLtkSecLevel, gRpaBondRec.localLtk.key);
    }
    else
    {
        /* key not found */
        DmSecLtkRsp(gRpaCb.connId, false, 0, NULL);
    }
}

/******************************************************************************\
 *  Public functions declarations.
\******************************************************************************/

/******************************************************************************/
/*!
 *  \brief  Application device manager message handler.
 *
 *  \param  pDmEvt  Device manager message.
 *  \return event handling state
 */
/******************************************************************************/
bool RPA_DmHandler(dmEvt_t *pDmEvt)
{
    bool handled = true;
    switch (pDmEvt->hdr.event)
    {
        case DM_CONN_CLOSE_IND:
            rpaDmStartAdv();
            break;
        case DM_RESET_CMPL_IND:
            AttsCalculateDbHash();

            // Add bonded device to resolving list
            if(gRpaCb.bonded)
            {
                DmPrivAddDevToResList(gRpaCb.peerAddrType, gRpaCb.peerAddr, gRpaBondRec.peerIrk.key, DmSecGetLocalIrk(), true, pDmEvt->hdr.param);
            }
            // If we are not already bonded, advertise using identity address
            else
            {
                rpaDmStartAdv();
            }

            break;

        case DM_CONN_OPEN_IND:
            rpaDmConnOpen(pDmEvt);
            break;

        case DM_REM_CONN_PARAM_REQ_IND:
            rpaDmConnParam(pDmEvt);
            break;

        case DM_SEC_PAIR_IND:
            rpaDmPair(pDmEvt);
            break;

        case DM_SEC_ECC_KEY_IND:
            DmSecSetEccKey(&pDmEvt->eccMsg.data.key);
            break;

        case DM_SEC_PAIR_FAIL_IND:
            DmSecGenerateEccKeyReq();
            break;

        case DM_SEC_KEY_IND:
            rpaDmKeyExchange(pDmEvt);
            break;

        case DM_SEC_PAIR_CMPL_IND:
            rpaDmPairingComplete(pDmEvt);
            break;

        case DM_SEC_LTK_REQ_IND:
            rpaDmRspLtk(pDmEvt);
            break;

        case DM_PRIV_SET_ADDR_RES_ENABLE_IND:
            SvcCoreGapCentAddrResUpdate(DmLlPrivEnabled());
            break;

        case DM_PRIV_ADD_DEV_TO_RES_LIST_IND:
            /* Adv procedure if bonding info exists is:
             * - init resolving list (DM_PRIV_ADD_DEV_TO_RES_LIST_IND)
             * - enable privacy mode (DM_PRIV_SET_ADDR_RES_ENABLE_IND)
             * - start advertising as soon as the RPA is generated (DM_ADV_NEW_ADDR_IND)
             */

            DmDevPrivStart(RPA_TIMEOUT_SEC);
            break;

        case DM_ADV_NEW_ADDR_IND:
            rpaDmStartAdv();
            break;

        case DM_ADV_START_IND:
        case DM_CONN_DATA_LEN_CHANGE_IND:
        case DM_SEC_ENCRYPT_IND:
            break;

        default:
            handled = false;
            break;
    };

    return handled;
}

/******************************************************************************/
/*!
 *  \brief  Application ATT message handler.
 *
 *  \param  pMsg     WSF message.
 *  \return Event handling state
 */
/******************************************************************************/
bool RPA_AttHandler(wsfMsgHdr_t *pMsg)
{
    bool handled = true;
    switch (pMsg->event)
    {
        /* Notification/Indication subscription. */
        case ATTS_CCC_STATE_IND:
            /* Placeholder for processing ccc state */
            break;

        /* Other ATT events can be handled here. */
        default:
            handled = false;
            break;
    };

    return handled;
}

/******************************************************************************/
/*!
 *  \brief  Application generic message handler.
 *
 *  \param  event     WSF event.
 *
 *  \param  pMsg      Generic WSF message.
 */
/******************************************************************************/
void RPA_Handler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    (void)event; // unused

    if (pMsg == NULL)
    {
        return;
    }
    // Process server-related ATT messages.
    if (pMsg->event >= ATT_CBACK_START && pMsg->event <= ATT_CBACK_END)
    {
        RPA_AttHandler(pMsg);  // handler procedure for ATT messages
    }
    // Process DM messages.
    if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END)
    {
        RPA_DmHandler((dmEvt_t *)pMsg);  // handler procedure for DM messages
    }
}

/******************************************************************************/
/*!
 *  \brief  Initilize RPA message handler.
 *
 *  \param  handlerId     WSF handler enumeration.
 */
/******************************************************************************/
void RPA_HandlerInit(wsfHandlerId_t handlerId)
{
    // Store handler ID.
    gRpaCb.handlerId = handlerId;

    /* Set configuration pointers */
    pSmpCfg = (smpCfg_t *) &gcRpaSmpCfg;
    pAttCfg = (attCfg_t *) &gcRpaAttCfg;

    // Initialize battery service server.
    BasInit(handlerId, (basCfg_t *)&gcRPABasCfg);
}

/******************************************************************************/
/*!
 *  \brief  Callback for handling connection events related to ATT.
 *
 *  \param  pEvt     ATT Client Characteristic Configuration event.
 *  \return None
 */
/******************************************************************************/
void RPA_CccCback(attsCccEvt_t *pEvt)
{
    /* Placeholder for Ccc event handling */
}

/******************************************************************************/
/*!
 *  \brief  Callback for handling connection events related to ATT.
 *
 *  \param  pDmEvt     Device manager event.
 *  \return None
 */
/******************************************************************************/
void RPA_ServerConnCback(dmEvt_t *pDmEvt)
{
    switch (pDmEvt->hdr.event)
    {
        case DM_CONN_CLOSE_IND:
        case DM_CONN_OPEN_IND:
         /* Placeholder for any action to be taken on Connection Open or close */
            break;
        case DM_SEC_ENCRYPT_IND:
        case DM_SEC_PAIR_CMPL_IND:
        /* Placeholder for any action to be taken on pairing complete*/
            break;

        default:
            break;
    };
}

/******************************************************************************/
/*!
 *  \brief  Application device manager callback. Forwards the received message
 *          to the RPA handler (generic application message handler).
 *
 *  \param  pEvt     Device manager message.
 */
/******************************************************************************/
void RPA_DmCback(dmEvt_t *pEvt)
{
    uint16_t len  = DmSizeOfEvt(pEvt);
    dmEvt_t *pMsg = WsfMsgAlloc(len);

    if (pMsg != NULL)
    {
        memcpy(pMsg, pEvt, len);
        WsfMsgSend(gRpaCb.handlerId, pMsg);
    }
}

/******************************************************************************/
/*!
 *  \brief  Application Att event callback. Forwards the received message
 *          to the RPA handler (generic application message handler).
 *
 *  \param  pEvt     Att event.
 */
/******************************************************************************/
void RPA_AttCback(attEvt_t *pEvt)
{
     attEvt_t *pMsg;

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t *) (pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(gRpaCb.handlerId, pMsg);
    }
}
