/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller slave extended advertising interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
 *  Packetcraft, Inc. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef LCTR_INT_ADV_SLAVE_AE_H
#define LCTR_INT_ADV_SLAVE_AE_H

#include "lctr_int_adv_ae.h"
#include "lctr_int.h"
#include "lctr_api_adv_slave_ae.h"
#include "lmgr_api_adv_slave_ae.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "lctr_pdu_adv.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum value of the AuxPtr offset field. */
#define LCTR_AUX_PTR_MAX_OFFSET         0x1FFF

/*! \brief      Number of shifted bytes for Used PHY field from the AUX Offset. */
#define LCTR_AUX_OFFS_USED_PHY_SHIFT    13

/*! \brief      Extended Advertising packet length. */
#define LCTR_EXT_ADVB_LEN(ehLen, dLen)  (LL_ADV_HDR_LEN + ehLen + dLen)

/*! \brief      Extended header common field length. */
#define LCTR_EXT_HDR_CMN_LEN            1

/*! \brief      Extended header flags field length. */
#define LCTR_EXT_HDR_FLAG_LEN           1

/*! \brief      Resolve the extended advertising index from the context pointer. */
#define LCTR_GET_EXT_ADV_INDEX(pAdvSet)   (pAdvSet - &pLctrAdvSetTbl[0])

/*! \brief      Get reservation manager handle for periodic ADV from the context pointer. */
#define LCTR_GET_PER_RM_HANDLE(pAdvSet)   (LL_MAX_CONN + LCTR_GET_EXT_ADV_INDEX(pAdvSet))

/*! \brief       Call signature for slave ACAD BIG related function. */
typedef void (*lctrAcadBigHdlr_t)(uint8_t advHandle);

/*! \brief       Call signature for AdvSet Remove event handler. */
typedef void (*lctrRemoveHdlr_t)(uint8_t advHandle);

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave extended advertising states. */
enum
{
  LCTR_EXT_ADV_STATE_DISABLED,          /*!< Extended advertising disabled state. */
  LCTR_EXT_ADV_STATE_ENABLED,           /*!< Extended advertising enabled state. */
  LCTR_EXT_ADV_STATE_SHUTDOWN,          /*!< Extended advertising shutdown in progress. */
  LCTR_EXT_ADV_STATE_RESET,             /*!< Extended advertising reset in progress. */
  LCTR_EXT_ADV_STATE_TOTAL              /*!< Total number of extended advertising states. */
};

/*! \brief      Slave periodic advertising states. */
enum
{
  LCTR_PER_ADV_STATE_DISABLED,          /*!< Periodic advertising disabled state. */
  LCTR_PER_ADV_STATE_ENABLED,           /*!< Periodic advertising enabled state. */
  LCTR_PER_ADV_STATE_SHUTDOWN,          /*!< Periodic advertising shutdown in progress. */
  LCTR_PER_ADV_STATE_RESET,             /*!< Periodic advertising reset in progress. */
  LCTR_PER_ADV_STATE_TOTAL              /*!< Total number of Periodic advertising states. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended advertising parameters. */
typedef struct
{
  uint16_t      advEventProp;           /*!< Advertising Event Properties. */
  uint32_t      priAdvInterMinUsec;     /*!< Primary Advertising Interval Minimum in microseconds. */
  uint32_t      priAdvInterMaxUsec;     /*!< Primary Advertising Interval Maximum in microseconds. */
  uint32_t      priAdvTermCntDownUsec;  /*!< Primary Advertising termination count down in microseconds. */
  uint64_t      peerAddr;               /*!< Peer Address. */
  uint8_t       priAdvChanMap;          /*!< Primary Advertising Channel Map. */
  uint8_t       ownAddrType;            /*!< Own Address Type. */
  uint8_t       peerAddrType;           /*!< Peer Address Type. */
  uint8_t       advFiltPolicy;          /*!< Advertising Filter Policy. */
  int8_t        advTxPwr;               /*!< Advertising Tx Power. */
  uint8_t       priAdvPhy;              /*!< Primary Advertising PHY. */
  uint8_t       secAdvMaxSkip;          /*!< Secondary Advertising Maximum Skip. */
  uint8_t       secAdvPhy;              /*!< Secondary Advertising PHY. */
  uint16_t      advDID;                 /*!< Advertising Data ID. */
  uint8_t       advSID;                 /*!< Advertising SID. */
  uint8_t       scanReqNotifEna;        /*!< Scan Request Notification Enable. */
  uint8_t       priPhyOpts;             /*!< Primary Advertising PHY Options */
  uint8_t       secPhyOpts;             /*!< Secondary Advertising PHY Options */
} lctrExtAdvParam_t;

/*! \brief      Periodic advertising parameters. */
typedef struct
{
  uint8_t       perState;           /*!< Current periodic adv state. */
  bool          perAdvEnabled;      /*!< Periodic advertising enabled or not. */
  uint32_t      perAccessAddr;      /*!< Periodic advertising access address. */
  uint16_t      perEventCounter;    /*!< Periodic event counter. */
  bool          perAuxStart;        /*!< true if need to start aux BOD due to periodic adv. */

  BbOpDesc_t    perAdvBod;          /*!< Periodic advertising BOD. */
  BbBleSlvAuxAdvEvent_t perBleData; /*!< Periodic advertising BLE data. */
  uint32_t      perOffsUsec;        /*!< Offset in microseconds to the next periodic PDU. */
  uint8_t       perChHopInc;        /*!< Periodic channel hop increment value. */
  uint8_t       perChIdx;           /*!< Periodic LL Channel. */
  bool          shutdown;           /*!< Client initiated shutdown flag. */
  uint32_t      perAdvInterUsec;    /*!< Periodic advertising interval in microseconds. */

  bool          enableAdi;          /*!< Enable ADI field in Periodic Advertising. */
  uint16_t      advDID;             /*!< Advertising Data ID. */

  uint32_t      advInterMinUsec;    /*!< Periodic Advertising Interval Minimum in microseconds. */
  uint32_t      advInterMaxUsec;    /*!< Periodic Advertising Interval Maximum in BB ticks. */
  uint16_t      advEventProp;       /*!< Periodic Advertising Event Properties. */
  bool          advParamReady;      /*!< Periodic Advertising Parameter is ready or not. */

  /* Channel parameters. */
  lmgrChanParam_t   perChanParam;   /*!< Periodic Advertising Channel parameter. */
  uint64_t          updChanMask;    /*!< Last updated channel mask */

  /* Version 2. */
  uint32_t      rspAA;              /*!< Responder's access address. */
  uint8_t       numSubevents;       /*!< Number of subevents. */
  uint8_t       subeventInt;        /*!< Interval between subevents in units of 1.25 ms. */
  uint32_t      subeventInterUs;    /*!< Interval between subevents in microseconds. */
  uint8_t       respSlotDelay;      /*!< Time between adv packet in a subevent and 1st resp slot .*/
  uint8_t       respSlotSpacing;    /*!< Time between response slots. */
  uint8_t       numRespSlots;       /*!< Number of subevent response slots. */
  uint8_t       paSubeventCounter;  /*!< PAwR subevent counter. */
  uint32_t      lastPerAdvInterTime;/*!< Time of the start of the last periodic advertising interval. */
} lctrPerAdvParam_t;

/*! \brief      Constant tone extension parameters. */
typedef struct
{
  bool          enable;             /*!< Enable status of CTE. */
  uint8_t       cteCount;           /*!< Number of constant tone extensions to transmit in each periodic advertising interval. */
  uint8_t       curRemCte;          /*!< Remaining CTE count. */
  bool          paramReady;         /*!< CTE Parameters have been set. */
} lctrConnlessCteTxParam_t;

/*! \brief      Periodic Advertising with Response buffer description. */
typedef struct
{
  struct
  {
    bool      modified;     /*!< Content modified by host. */
    uint8_t   len;          /*!< Advertising data buffer length. */
    uint16_t  did;          /*!< Advertising Data ID. */
    uint8_t   buf[LL_COMP_EXT_ADV_DATA_MAX_LEN];
                            /*!< Advertising data buffer. */
    uint8_t   fragPref;     /*!< Host fragmentation preference. */
#if (LL_ENABLE_RSPOBS)
    uint8_t   rspObsLen;    /*!< Response observation buffer length array. */
#endif
  } data[LL_MAX_SUBEVENT];
  uint8_t   rspSlotStart;   /*!< The first response slot to be used in this subevent. */
  uint8_t   rspSlotCnt;     /*!< The number of response slots to be used. */
  uint64_t  memBmp;         /*!< Bitmap of available data descriptors. */
  uint8_t   subEvtBufTbl[LL_NUM_PAWR_SUBEVT];
                            /*!< Lookup table mapping 128 subevents to LL_MAX_SUBEVENT buffers. */
} lctrPerAdvWithRspDataBuf_t;

/*! \brief      Advertising data buffer descriptor. */
typedef struct
{
  uint16_t    len;              /*!< Advertising data length. */
  uint8_t     *pBuf;            /*!< Advertising data buffer. */
  bool        ready;            /*!< Advertising data buffer complete. */
  uint16_t    txOffs;           /*!< Tx buffer offset. */
  uint8_t     fragPref;         /*!< Host fragmentation preference. */

  struct
  {
    lctrPerAdvWithRspDataBuf_t *pRspData;
                                /*!< Periodic Advertising with Response buffer descriptor. */
#if (LL_ENABLE_RSPOBS)
    uint8_t                    *pRspObsBuf;
                                /*!< Response observation data buffer. */
#endif
  } pawr;

  union
  {
    struct
    {
      bool      modified;       /*!< Content modified by host. */
      uint8_t   len;            /*!< Advertising data buffer length. */
      uint16_t  did;            /*!< Advertising Data ID. */
      uint8_t   buf[LL_COMP_EXT_ADV_DATA_MAX_LEN];
                                /*!< Advertising data buffer. */
      uint8_t   fragPref;       /*!< Host fragmentation preference. */
    } ext;                      /*!< Extended advertising data buffer cache (temporary storage during active advertising). */

    struct
    {
      bool      modified;       /*!< Content modified by host. */
      uint8_t   len;            /*!< Advertising data buffer length. */
      uint8_t   buf[LL_ADVBU_MAX_LEN];
                                /*!< Advertising data buffer. */
    } legacy;                   /*!< Legacy advertising data buffer. */

  } alt;                        /*!< Alternate data storage. */
} lctrAdvDataBuf_t;

/*! \brief      Extended advertising channel PDU header. */
typedef struct
{
  uint8_t       extHdrLen;      /*!< Extended header length. */
  uint8_t       advMode;        /*!< Advertising mode. */
  uint8_t       extHdrFlags;    /*!< Extended header flags. */
  uint64_t      advAddr;        /*!< Advertiser's address. */
  uint64_t      tgtAddr;        /*!< Target address. */
  uint8_t       cteTime;        /*!< CTE Time. */
  uint8_t       cteType;        /*!< CTE Type. */
  uint16_t      did;            /*!< Advertising data ID in ADI. */
  uint8_t       sid;            /*!< Advertising set ID in ADI. */
  int8_t        txPwr;          /*!< Tx Power. */
  uint8_t       acadLen;        /*!< ACAD length. */
  const uint8_t *pAcad;         /*!< Buffer pointer to ACAD. */
  const uint8_t *pAuxPtr;       /*!< Buffer pointer to AuxPtr field. */
  const uint8_t *pSyncInfo;     /*!< Buffer pointer to SyncInfo field. */
} lctrExtAdvHdr_t;

/*! \brief      Periodic advertising with response set. */
typedef struct
{
  uint8_t                 *pPawrRspData;                           /*!< PAwR received response data. */
  lctrExtAdvHdr_t         extAdvHdr;                               /*!< Ext adv header of AUX_SYNC_SUBEVENT_RSP PDU. */
  LlPerAdvRspReportInd_t  pawrRpt;                                 /*!< PAwR report. */
  lctrRptState_t          pawrRptState:8;                          /*!< Periodic advertising with response report state. */
  uint8_t                 ownAddrType;                             /*!< Type of address being used in the connection request packets. */
  uint8_t                 peerAddrType;                            /*!< Type of address used in the connectable advertisement sent by the peer. */
  uint64_t                peerAddr;                                /*!< Peer's Public Device Address, Random (static) Device Address, Non-Resolvable
                                                                        Private Address, or Resolvable Private Address depending on the Peer_Address_Type parameter. */
  uint8_t                 connReqSubevent;                         /*!< The subevent where a connection request shall be initiated from a periodic advertising train. */
  uint8_t                 connReqFlag;                             /*!< true if current subevent equals connReqSubevent. */
  uint8_t                 subeventStart;                           /*!< The first subevent that data is requested for. */
  uint8_t                 subeventDataCount;                       /*!< The number of subevents that data is requested for. */
  uint8_t                 pawrConnState;                           /*!< PAwR init connection state. */
  LlConnSpec_t            connSpec;                                /*!< PAwR connection specification. */
  uint16_t                connHandle;                              /*!< Connection handle. */
  uint16_t                connInterval;                            /*!< Connection interval. */
  lctrConnInd_t           connInd;                                 /*!< Connection indication. */
#if (LL_ENABLE_RSPOBS)
  uint8_t                 perAdvRspObsHdrBuf[LCTR_EXT_ADVB_LEN(LL_EXT_ADV_HDR_MAX_LEN, 0)];
                                                                   /*!< Periodic advertising response header buffer. */
#endif
} lctrPawrData_t;

/*! \brief      Periodic advertising with response set. */
typedef struct
{
  lctrPerAdvWithRspDataBuf_t  *pLctrPawrRspDataBufTbl;  /*!< Periodic Advertising with Response buffer description. */
  lctrPawrData_t              *pLctrPawrDataBufTbl;     /*!< Periodic Advertising with Response set data. */
  bool                        isUsed;                   /*!< true if PAwR Advertising Set in use, false otherwise. */
  uint8_t                     handle;                   /*!< Advertising handle. */
  uint8_t                     number;                   /*!< Number of PAwR Advertising Set. */
} lctrPawrAdvSet_t;

/*! \brief      Advertising set. */
typedef struct lctrAdvSet_t
{
  bool        enabled;          /*!< Enable flag. */
  uint8_t     handle;           /*!< Advertising handle. */

  uint8_t     state;            /*!< Current state. */

  /* Host parameters */
  bool        bdAddrRndMod;     /*!< Modified random address. */
  bool        bdAddrRndValid;   /*!< Random device address valid. */
  uint64_t    bdAddrRnd;        /*!< Random device address. */
  uint64_t    advA;             /*!< Advertiser address. */
  uint64_t    tgtA;             /*!< Target address. */
  lctrExtAdvParam_t param;      /*!< Extended advertising parameters. */
  uint32_t    auxDelayUsec;     /*!< Auxiliary advertising event delay. */
  uint8_t     advDataFragLen;   /*!< Advertising data fragmentation length. */

  /* ACAD control block */
  lctrAcadParam_t acadParams[LCTR_ACAD_NUM_ID];  /*!< ACAD parameters. */

  /* Periodic advertising parameters */
  lctrPerAdvParam_t perParam;   /*!< Periodic advertising parameters. */

  /* Constant tone extension parameters */
  lctrConnlessCteTxParam_t advCteParam;
                                /*!< Connectionless CTE parameters. */

  /* Enable parameters */
  wsfTimer_t  tmrAdvDur;        /*!< Advertising duration timer. */
  uint8_t     maxEvents;        /*!< Maximum number of AE. */

  /* Advertising task context */
  uint8_t     numEvents;        /*!< Number of completed AE. */
  uint8_t     termReason;       /*!< Termination reason. */
  uint8_t     extHdrFlags;      /*!< Extended header flags. */
  uint8_t     *pExtAdvAuxPtr;   /*!< Extended advertising PDU buffer location of AuxPtr field. */
  uint8_t     auxChHopInc;      /*!< Auxiliary channel hop increment value. */
  uint32_t    auxSkipInterUsec; /*!< Total skip time in microseconds. */

  /* Buffers */
  uint8_t     advHdrBuf[LCTR_EXT_ADVB_LEN(LL_EXT_ADV_HDR_MAX_LEN, 0)];
                                /*!< Primary channel legacy advertising and extended advertising header buffer. */
  uint8_t     scanRspHdrBuf[LL_ADVB_MAX_LEN];
                                /*!< Primary channel legacy scan response buffer. */
  uint8_t     auxAdvHdrBuf[LCTR_EXT_ADVB_LEN(LL_EXT_ADV_HDR_MAX_LEN, 0)];
                                /*!< Auxiliary extended advertising header buffer. */
  uint8_t     auxRspHdrBuf[LCTR_EXT_ADVB_LEN(LL_EXT_ADV_HDR_MAX_LEN, 0)];
                                /*!< Auxiliary scan or connect response header buffer. */
  uint8_t     perAdvHdrBuf[LCTR_EXT_ADVB_LEN(LL_EXT_ADV_HDR_MAX_LEN, 0)];
                                /*!< Periodic advertising header buffer. */
  lctrAdvDataBuf_t perAdvData;  /*!< Periodic advertising data buffer. */

#if (LL_SHARE_ADV_SCAN_DATA)
  /* Reduce capabilities to mutual exclusive AdvData and ScanRspData to save memory. */
  union
#else
  struct
#endif
  {
    lctrAdvDataBuf_t advData;      /*!< Advertising data buffer. */
    lctrAdvDataBuf_t scanRspData;  /*!< Scan response data buffer. */
    lctrAdvDataBuf_t decisionData; /*!< Decision data buffer. */
  } data;

  /* Data lengths need to be separated data values. */
  uint16_t    scanRspDataLen;     /*!< Scan response data length. */
  uint16_t    advDataLen;         /*!< Advertising data length. */

  /* Connection context. */
  bool        isAuxConnReq;     /*!< true if AUX_CONN_REQ is received, false if CONN_IND is received. */
  uint32_t    connIndEndTsUsec; /*!< Connection indication end timestamp in microseconds. */
  bool        connIndRcvd;      /*!< Connection request received flag. */
  uint8_t     usedChSel;        /*!< Used channel selection. */

  /* BB/ISR context */
  bool        shutdown;         /*!< Client initiated shutdown flag. */
  BbOpDesc_t  advBod;           /*!< Advertising BOD. */
  BbBleSlvAdvEvent_t bleData;   /*!< BLE data. */
  BbOpDesc_t  auxAdvBod;        /*!< Auxiliary advertising BOD. */
  BbBleSlvAuxAdvEvent_t auxBleData; /*!< Auxiliary BLE data. */
  uint32_t    auxOffsUsec;      /*!< Offset in microseconds to the next auxiliary PDU. */
  uint8_t     auxChIdx;         /*!< AUX LL Channel. */
  bool        auxBodUsed;       /*!< Auxiliary BOD in use flag. */
  bool        didPerUpdate;     /*!< Data ID update due to periodic enable or disable. */
  bool        advBodAbort;      /*!< true if extended advertising BOD is aborted. */
  lctrAdvbPduHdr_t  rspPduHdr;  /*!< Response PDU header. */

  /* BIG */
  lctrAcadBigHdlr_t  bigCreated;    /*!< Function pointer to the BIG created action function. */
  lctrAcadBigHdlr_t  bigTerminated; /*!< Function pointer to the BIG terminated action function. */
  lctrRemoveHdlr_t   removeCback;   /*!< Function pointer to the AdvSet remove action function. */

  /* Data related address changing. */
  uint8_t    addrChangeTrig;    /*!< Triggers for data-related address changing. */

  /* PAwR */
  bool               pawrUsed;  /*!< true if PAwR is used. */
  lctrPawrData_t     *pPawr;    /*!< Periodic advertising with response set. */
  void               (*txSyncIndConnReq)(struct lctrAdvSet_t*);
                                /*!< Pointer to function that sends CONN_REQ instead of SYNC_SUBEVENT_IND. */

  /* DBAF */
  uint8_t decisionTypeFlags;    /*!< Specifies decision data to be used with advertising set. */
} lctrAdvSet_t;

/*! \brief      Slave extended advertising state context. */
typedef struct
{
  wsfQueue_t  rxScanReqQ;        /*!< Received SCAN_REQ queue. */
} lctrSlvExtAdvCtx_t;

/*! \brief      Call signature for lctrCalcMaxConnlessCteUsecFn function. */
typedef uint32_t (*lctrCalcMaxConnlessCteUsec_t)(void);

/*! \brief      Call signature for lctrPackAdvDecisionIndPdu_t function. */
typedef uint8_t (*lctrPackAdvDecisionIndPdu_t)(lctrAdvSet_t *pAdvSet, uint8_t *pPduBuf, lctrAdvDataBuf_t *pAdvData, bool isPeriodic);

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrAdvSet_t *pLctrAdvSetTbl;
extern LctrExtAdvMsg_t *pLctrSlvExtAdvMsg;
extern lctrSlvExtAdvCtx_t lctrSlvExtAdv;
extern LctrPerAdvMsg_t *pLctrSlvPerAdvMsg;

/*! \brief      Slave ACAD message. */
extern lctrAcadSlvMsg_t *pLctrAcadSlvMsg;

/*! \brief      Pointer to lctrCalcMaxConnectionlessCteUsec function. */
extern lctrCalcMaxConnlessCteUsec_t lctrCalcMaxConnlessCteUsecFn;

/*! \brief      Pointer to lctrPackDecisionIndHeader function. */
extern lctrPackAdvDecisionIndPdu_t lctrPackAdvDecisionIndPduFn;

extern uint8_t *(*pLctrPawrPerRxDequeue)(void);
extern void (*pLctrPawrPerRxEnqueue)(uint8_t **);

extern lctrPawrAdvSet_t lctrPawrAdvSetTbl[LL_MAX_PAWR_ADV_SETS];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Context */
void lctrFreeAdvSet(lctrAdvSet_t *pAdvSet);
lctrAdvSet_t *lctrFindAdvSet(uint8_t handle);
lctrPawrAdvSet_t *lctrFindPawrAdvSet(uint8_t handle);
void lctrFreePawrAdvSet(lctrAdvSet_t *pAdvSet);

/* Address selection */
void lctrChooseSetAdvA(lctrAdvbPduHdr_t *pPduHdr, BbBleData_t * const pBle, lctrAdvSet_t *pAdvSet);
void lctrChooseSetPeerA(lctrAdvbPduHdr_t *pPduHdr, BbBleData_t * const pBle, lctrAdvSet_t *pAdvSet);

/* Builder */
uint8_t lctrSlvExtAdvBuildOp(lctrAdvSet_t *pAdvSet, uint32_t maxStartMs);
void lctrSlvAuxNonConnNonScanBuildOp(lctrAdvSet_t *pAdvSet);
void lctrSlvAuxScanBuildOp(lctrAdvSet_t *pAdvSet);
void lctrSlvAuxConnBuildOp(lctrAdvSet_t *pAdvSet);
void lctrSlvAuxRescheduleOp(lctrAdvSet_t *pAdvSet, BbOpDesc_t * const pOp);
uint8_t lctrSlvPeriodicAdvBuildOp(lctrAdvSet_t *pAdvSet);
uint8_t lctrSlvPeriodicAdvWithRspBuildOp(lctrAdvSet_t *pAdvSet);
void lctrSlvPeriodicAdvWithRspCommitOp(lctrAdvSet_t *pAdvSet, BbOpDesc_t * const pOp);

/* Channel */
void lctrSelectNextAuxChannel(lctrAdvSet_t *pAdvSet);
void lctrSelectNextPerChannel(lctrAdvSet_t *pAdvSet);

/* ISR: Packet handlers */
void lctrSlvTxSetupExtAdvHandler(BbOpDesc_t *pOp, uint32_t txTime);
uint32_t lctrSlvTxSetupAuxAdvDataHandler(BbOpDesc_t *pOp, bool isChainInd);
uint32_t lctrSlvTxSetupAuxScanRspDataHandler(BbOpDesc_t *pOp, bool isChainInd);
bool lctrSlvRxAuxScanReqHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
void lctrSlvRxAuxScanReqPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
void lctrSlvRxLegacyScanReqPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
bool lctrSlvRxAuxConnReqHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
bool lctrSlvRxLegacyReqHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
void lctrSlvRxLegacyReqPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
uint32_t lctrSlvTxSetupPeriodicAdvDataHandler(BbOpDesc_t *pOp, bool isChainInd);
void lctrSlvAcadHandler(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadDisable(lctrAcadParam_t *pAcadParam);
bool lctrSlvRxAuxSyncSubeventRspHandler(BbOpDesc_t *pOp, const uint8_t *pAdvBuf);
void lctrSlvRxAuxSyncSubeventRspPostHandler(BbOpDesc_t *pOp, uint8_t);

/* ISR: BOD handlers */
void lctrSlvExtAdvEndOp(BbOpDesc_t *pOp);
void lctrSlvExtAdvAbortOp(BbOpDesc_t *pOp);
void lctrSlvAuxAdvEndOp(BbOpDesc_t *pOp);
void lctrSlvPeriodicAdvEndOp(BbOpDesc_t *pOp);
void lctrSlvPeriodicAdvAbortOp(BbOpDesc_t *pOp);

/* Action routines */
void lctrExtAdvActStart(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActSelfStart(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActRestart(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActShutdown(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActResetShutdown(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActAdvCnf(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActDisallowAdvCnf(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActSelfTerm(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActAdvTerm(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActReset(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActResetTerm(lctrAdvSet_t *pAdvSet);
void lctrExtAdvActDurationExpired(lctrAdvSet_t *pAdvSet);
void lctrPeriodicAdvActStart(lctrAdvSet_t *pAdvSet);
void lctrPeriodicAdvActUpdate(lctrAdvSet_t *pAdvSet);
void lctrPeriodicAdvActDisallowAdvCnf(lctrAdvSet_t *pAdvSet);
void lctrPeriodicAdvActShutdown(lctrAdvSet_t *pAdvSet);
void lctrPeriodicAdvActResetTerm(lctrAdvSet_t *pAdvSet);
void lctrPeriodicAdvActAdvTerm(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadActChanMapUpdateStart(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadActChanMapUpdateFinish(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadActBigCreated(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadActBigTerminated(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadActPartiStart(lctrAdvSet_t *pAdvSet);
void lctrSlvAcadActPartiStop(lctrAdvSet_t *pAdvSet);

/* Reservation */
uint32_t lctrGetPerRefTime(uint8_t perHandle, uint32_t *pDurUsec);

/* State machine */
void lctrSlvExtAdvExecuteSm(lctrAdvSet_t *pAdvSet, uint8_t event);
void lctrSlvPeriodicAdvExecuteSm(lctrAdvSet_t *pAdvSet, uint8_t event);
void lctrSlvAcadExecuteSm(lctrAdvSet_t *pAdvSet, uint8_t event);

/* Messaging */
void lctrSendAdvSetMsg(lctrAdvSet_t *pAdvSet, uint8_t event);
void lctrSendPeriodicAdvSetMsg(lctrAdvSet_t *pAdvSet, uint8_t event);

/* Queue */
uint8_t *lctrPawrPerRxDequeue(void);
void lctrPawrPerRxEnqueue(uint8_t **pBuf);
void lctrPawrPerRxResetQueue(BbBleMstPerScanEvent_t *pPerScan);

/*************************************************************************************************/
/*!
 *  \brief      Convert LL Coded PHY option to BB Coded PHY option
 *
 *  \param      phyOpts       LL Coded PHY Option
 *
 *  \return     BB Coded PHY option
 */
/*************************************************************************************************/
static inline uint8_t lctrConvertLlPhyOptsToBbPhyOpts(uint8_t phyOpts)
{
  uint8_t bbCodedPhy;
  switch (phyOpts)
  {
    case LL_PHY_OPTIONS_NONE:
    default:
      bbCodedPhy = BB_PHY_OPTIONS_DEFAULT;
      break;
    case LL_PHY_OPTIONS_S8_PREFERRED:
    case LL_PHY_OPTIONS_S8_REQUIRED:
      bbCodedPhy = BB_PHY_OPTIONS_BLE_S8;
      break;
    case LL_PHY_OPTIONS_S2_PREFERRED:
    case LL_PHY_OPTIONS_S2_REQUIRED:
      bbCodedPhy = BB_PHY_OPTIONS_BLE_S2;
      break;
  }
  return bbCodedPhy;
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_SLAVE_AE_H */
