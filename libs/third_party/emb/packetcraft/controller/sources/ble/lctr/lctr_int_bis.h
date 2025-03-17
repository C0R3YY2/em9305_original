/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connected isochronous interface file.
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

#ifndef LCTR_INT_BIS_H
#define LCTR_INT_BIS_H

#include "lctr_int_adv_slave_ae.h"
#include "lctr_int_adv_master_ae.h"
#include "lctr_int.h"
#include "lctr_int_iso.h"
#include "lctr_pdu_iso.h"
#include "bb_ble_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Get encryption ID from BIG context. */
#define LCTR_BIG_CTRL_ENC_ID(pBigCtx)   (LL_MAX_CONN + LL_MAX_CIG + LL_MAX_BIG + (pBigCtx - &pLctrBigTbl[0]))

/*! \brief  ISO Data PDU start offset in a buffer. */
#define LCTR_ISO_SDU_START_OFFSET       (HCI_ISO_HDR_LEN + HCI_ISO_DL_MAX_LEN - LL_ISO_DATA_HDR_LEN)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

struct lctrBigCtx_tag;

/*! \brief      BIS receiver diversity context. */
typedef struct
{
  uint8_t rxdBisState;          /*!< Receiver diversity loop state. */
  uint8_t threshold;            /*!< Receiver diversity threshold. */
  uint8_t priAntID;             /*!< Primary BIS antenna. */
  bool   statusFlag;            /*!< true if RX on primary antenna was successful. */
  int8_t rssi[BB_MAX_ANT];      /*!< RSSI values. */
  int8_t rssiSample[BB_MAX_ANT][LCTR_RXD_MAX_SAMPLES];
                                /*!< RSSI sample values. */
  uint16_t rxdIntervMs;         /*!< Receiver diversity update interval. */
  uint16_t rssiCnt[BB_MAX_ANT]; /*!< Number of RSSI samples recorded. */
  wsfTimer_t tmrUpd;            /*!< Receiver diversity timer. */
} lctrBisRxdCtx_t;

/*! \brief      Broadcast Isochronous Stream (BIS) context. */
typedef struct
{
  bool          enabled;        /*!< Enable flag. */
  uint8_t       bisNum;         /*!< BIS positional sequence number. */
  uint16_t      handle;         /*!< BIS handle. */
  struct lctrBigCtx_tag *pBigCtx;   /*!< BIG context. */

  union
  {
    struct
    {
      /* Data */
      wsfQueue_t           txDataQ;       /*!< Transmit ISO queue. */
      uint8_t              numTxSduComp;  /*!< Number of Tx completed SDUs. */
      lctrIsoalTxCtx_t     isoalTxCtx;    /*!< ISOAL transmit context. */
      lctrInDataPathCtx_t  dataPathInCtx; /*!< Datapath input context. */
    } slv;                                /*!< BIS slave specific data. */

    struct
    {
      /* Data */
      wsfQueue_t           rxDataQ;       /*!< Receive ISO Data PDU pending queue. */
      lctrIsoalRxCtx_t     isoalRxCtx;    /*!< ISOAL receive context. */
      lctrOutDataPathCtx_t dataPathOutCtx;/*!< Output data path context. */

      /* Receive status */
      LlIsoLinkQual_t      linkStats;     /*!< ISO link quality statistics. */

      /* ISO test */
      LlIsoTestCtrs_t      testStats;     /*!< Rx test statistics. */

      /* Reception status. */
      int8_t    lastRssi;                 /*!< Last RSSI. */

      /* Receiver diversity */
      bool   rxdEnabled;                  /*!< true if receiver diversity enabled. */
      lctrBisRxdCtx_t *pBisRxdCtx;        /*!< Receiver diversity context. */
    } mst;                                /*!< BIS master specific data. */
  } roleData;                             /*!< Role specific data. */

  /* ISO test */
  /*! \brief    TBD */
  struct
  {
    bool        enabled;        /*!< true ISO test enabled, false otherwise. */
    bool        term;           /*!< Flag for ISO test termination. */
    bool        pendInit;       /*!< Receive pending init. */
    union
    {
      struct
      {
        uint32_t    payloadCtr; /*!< Payload counter for framed transmissions. */
      } framed;                 /*!< TBD */
      struct
      {
        uint8_t     burstIdx;   /*!< BN Index for unframed transmissions. */
      } unframed;               /*!< TBD */
    } util;                     /*!< TBD */
    LlIsoPldType_t pldType:8;   /*!< Test payload type. */
  } test;

  /* BB */
  lmgrChanParam_t chSelInfo;    /*!< Channel selection state. */
  PalBbBleChan_t chan;          /*!< Channelization parameters. */

  /* Data */
  LlIsoLlid_t   lastLlid;       /*!< Last LLID. */
} lctrBisCtx_t;

/*! \brief      Broadcast Isochronous Group (BIG) context. */
typedef struct lctrBigCtx_tag
{
  bool          enabled;        /*!< Context enabled. */
  uint8_t       state;          /*!< Current state. */
  uint8_t       handle;         /*!< BIG handle. */
  LlRole_t      role:8;         /*!< Role. */

  /* Data */
  uint16_t      maxPdu;         /*!< Maximum size of BIS Data PDU. */
  uint16_t      maxSdu;         /*!< Maximum size of ISO SDU. */

  /* Host */
  uint32_t      sduInterUsec;   /*!< SDU interval in microseconds. */
  uint32_t      bisSpaceUsec;   /*!< BIS space in microseconds. */
  uint32_t      isoInterUsec;   /*!< Isochronous PDU interval in microseconds. */
  uint32_t      adjOffsetUsec;  /*!< Offset from adjacent BOD in microseconds. */
  uint8_t       bn;             /*!< Burst number. */
  uint8_t       nse;            /*!< Maximum number of subevent in each interval on BIS. */
  uint8_t       pto;            /*!< Pre-transmission offset. */
  uint8_t       irc;            /*!< Immediate repetition count. */
  int8_t        ptoOffset;      /*!< PTO offset. */
  bool          allowAdjust;    /*!< Allow BIG configuration adjustments. */
  LlFraming_t   framing;        /*!< BIS Data PDU format. */
  LlPacking_t   packing:8;      /*!< Packing sequence scheme. */

  /* ISO Event */
  uint64_t      eventCounter;   /*!< Event counter. */
  uint32_t      syncDelayUsec;  /*!< Synchronization delay in microseconds. */
  uint32_t      transLatUsec;   /*!< The maximum transmission latency, in microseconds. */
  uint32_t      subInterUsec;   /*!< Subevent interval in microseconds. */

  /* BIS */
  uint8_t       numBis;               /*!< Number of BISs. */
  lctrBisCtx_t  *pBisCtx[LL_MAX_BIS]; /*!< BIS contexts. */

  union
  {
    struct
    {
      BbBleSlvBisEvent_t   bleSlvData;        /*!< BLE BB operation data. Must be 1st member in the structure. */
      lctrAdvSet_t *pAdvSet;    /*!< Advertising Set parent. */
      wsfQueue_t txCtrlQ;       /*!< Transmit BIG control queue. */
      bool      notifyHostEst;  /*!< Notify host event sent flag. */

      /* PTO */
      uint8_t   ptoSduCount;    /*!< PTO SDU count. */
      uint8_t   ptoSduDepthMax; /*!< PTO SDU maximum depth. */
    } slv;                      /*!< BIG slave specific data. */

    struct
    {
      BbBleMstBisEvent_t   bleMstData;        /*!< BLE BB operation data. Must be 1st member in the structure. */
      /* BIG Create Sync */
      lctrPerScanCtx_t *pPerScanCtx;  /*!< Periodic Scan parent. */
      uint8_t   syncLostReason;       /*!< BIG synchronization lost. */
      uint8_t   mse;                  /*!< Maximum number of subevents. */
      uint8_t   numBisIdx;            /*!< Total number of BISes in the BIG. */
      uint8_t   bisIdx[LL_MAX_BIS];   /*!< List of indices of BISes. */

      /* Sync timeout */
      wsfTimer_t bigSyncTmr;          /*!< Synchronization timeout timer. */
      uint32_t   bigSyncTimeoutMs;    /*!< Synchronization timeout in microseconds. */

      /* Encryption */
      uint8_t   bcstCode[LL_BC_LEN];  /*!< Broadcast Code. Array address should be word-aligned. */

      /* Event state */
      uint16_t  totalAcc;             /*!< Total clock accuracy. */
      uint16_t  initWwUsec;           /*!< Initial synchronization window widening time in microseconds. */
      uint32_t  rxSyncTime;           /*!< Last received BIG anchor point. */
      uint32_t  anchorPoint;          /*!< BIG anchor point. */
      lctrBisCtx_t *pFirstBisCtx;     /*!< First BIS context (ISR optimization to reduce lookups). */
      lctrBisCtx_t *pSecondBisCtx;    /*!< Second BIS context (ISR optimization to reduce lookups). */
      uint32_t  firstBisOffsUsec;     /*!< First BIS offset from the anchor point in microseconds. */
      uint8_t   firstBisEvtIdx;       /*!< First BIS index. */
      uint8_t   firstBisIdxSpacing;   /*!< Difference between the second BIS index and first BIS index */
    } mst;                            /*!< BIG master specific data. */
  } roleData;                         /*!< Role-specific data. */

  /* Control */
  struct
  {
    uint8_t     actMsk;         /*!< Active control procedure bitmask. */
    uint8_t     pendMsk;        /*!< Pending control procedure bitmask. */
    uint8_t     cssn;           /*!< Control Subevent Sequence Number */
    struct
    {
      uint16_t  inst;           /*!< Instant. */
      uint64_t  chanMap;        /*!< Channel map. */
    } chanMapUpd;               /*!< Channel Map Update data. */
    struct
    {
      uint16_t  inst;           /*!< Instant. */
      uint8_t   reason;         /*!< Termination reason. */
    } term;                     /*!< Terminate data. */
  } bcp;                        /*!< BIG Control Procedure data. */

  /* BB */
  PalBbPhy_t    phy;            /*!< PHY used for the BIG. */
  BbOpDesc_t    bod;            /*!< BIG BOD. */
  uint32_t      seedAccAddr;    /*!< Seed access address. */
  uint16_t      baseCrcInit;    /*!< Base CRC Init. */
  lmgrChanParam_t ctrChSelInfo; /*!< Control channel selection state. */
  PalBbBleChan_t ctrChan;       /*!< BIG Control channelization parameters. */

  /* Encryption */
  /* Note: located at end of structure for non-encryption optimization */
  bool          encrypt;                /*!< Encryption enable for BIS. */
  uint8_t       giv[LL_GIV_LEN];        /*!< GIV. */
  uint8_t       gskd[LL_GSKD_LEN];      /*!< GSKD. */
} lctrBigCtx_t;

/*! \brief  ISR subevent context. */
typedef struct
{
  uint8_t bisEvtIdx;            /*!< BIS event index within an event. */
  uint8_t burstIdx;             /*!< Burst index within a subevent. */
  uint8_t repIdx;               /*!< Repeat index within a subevent. */
  uint8_t ptIdx;                /*!< Pre-transmission index within a subevent. */
  uint8_t numSeComp;            /*!< Total number of completed subevents. */
} lctrSeCtx_t;

/*! \brief      BIS master ISR control block. */
typedef struct
{
  lctrSeCtx_t se;           /*!< Subevent context. */
  uint8_t numRcvdPkt;       /*!< Number of successfully received packets. */
  bool    firstRxComp;      /*!< First Rx in ISO Event completion flag. */
  bool    cstf;             /*!< BIG Event's valid Control Subevent Transmission Flag. */
  uint8_t cssn;             /*!< BIG Event's valid Control Subevent Sequence Number. */
  uint32_t nextSeOffs;      /*!< Next subevent time. */
  PalBbBleChan_t *pNextChan;/*!< Next subevent channel. */
  uint8_t *pCtrlBuf;        /*!< Received BIS Control PDU buffer. */
  uint8_t *pDataBuf[LL_MAX_BIS][LL_MAX_BN];
                            /*!< Received data buffers. */
  uint8_t numRxPdus[LL_MAX_BIS];
                            /*!< Number of successfully received packets for each BIS. */
  uint8_t  curChanIdx;      /*!< Current channel index. */
  uint64_t curPktCtr;       /*!< Current payload counter. */
} lctrMstBisIsr_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrBisCtx_t *pLctrBisTbl;
extern lctrBigCtx_t *pLctrBigTbl;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* BIG Context */
lctrBigCtx_t *lctrAllocBigCtx(uint8_t bigHandle);
void lctrFreeBigCtx(lctrBigCtx_t *pBigCtx);
uint8_t lctrBigIsPerAdvUsed(uint8_t handle);
lctrBigCtx_t *lctrFindBigByHandle(uint8_t bigHandle);
lctrBigCtx_t *lctrFindBigBySyncHandle(uint16_t syncHandle);
bool lctrIsBigSynchronizing(void);

/* BIS Context */
lctrBisCtx_t *lctrAllocBisCtx(lctrBigCtx_t *pBigCtx);
void lctrFreeBisCtx(lctrBisCtx_t *pBisCtx);
lctrBisCtx_t *lctrFindBisByHandle(uint16_t bisHandle);
uint8_t lctrGetNumAvailBisCtx(void);
uint32_t lctrComputeBisAccessAddr(uint32_t seedAccAddr, uint8_t bisNum);
void lctrSetupBisContext(lctrBisCtx_t *pBisCtx, uint32_t seedAccAddr, uint16_t baseCrcInit, uint64_t chMap, LlPhy_t phy);
void lctrSelectBigChannels(lctrBigCtx_t *pBigCtx);
void lctrRemapBigChannels(lctrBigCtx_t *pBigCtx, uint64_t chanMap);

/* BIS Tx Data Path */
void lctrBisTxIsoPduQueue(lctrBisCtx_t *pBisCtx, lctrIsoHdr_t *pIsoHdr, uint8_t *pIsoSdu);
void lctrBisFlushStaleSdus(lctrBisCtx_t *pBisCtx);
uint8_t lctrBisTxQueuePeek(lctrBisCtx_t *pBisCtx, uint8_t burstIdx, PalBbBleTxBufDesc_t *descs);
void lctrBisTxQueuePopCleanup(lctrBisCtx_t *pBisCtx, uint8_t numFrag);
void lctrBisTxPtoQueuePopCleanup(lctrBisCtx_t *pBisCtx, uint8_t numFrag, uint8_t numDequeue);
uint8_t *lctrBigTxCtrlAlloc(uint8_t pduLen);
void lctrBigTxCtrlQueue(lctrBigCtx_t *pBigCtx, uint8_t *pBuf, uint8_t numReTx);
uint8_t *lctrBigTxCtrlQueuePeek(lctrBigCtx_t *pBigCtx, bool *isFirst);
void lctrBigTxCtrlQueuePop(lctrBigCtx_t *pBigCtx);
void lctrBigTxCtrlQueuePopCleanup(lctrBigCtx_t *pBigCtx);

/* BIS Rx Data Path */
uint8_t *lctrBisRxIsoSduDeq(lctrBisCtx_t *pBisCtx);
uint8_t *lctrBisRxIsoDataPduAlloc(void);
void lctrBisRxIsoDataPduFree(uint8_t *pPdu);
void lctrBisEnqueueRxDataPdu(lctrBisCtx_t *pBisCtx, uint8_t *pRxBuf, uint8_t burstIdx);
uint8_t *lctrBisDequeueRxDataPdu(lctrBisCtx_t *pBisCtx, uint8_t burstIdx);
uint8_t *lctrBisDequeueRxDataPduTop(lctrBisCtx_t *pBisCtx);

/* ISO Test mode */
uint8_t lctrBisTxTest(lctrBisCtx_t *pBisCtx, uint8_t pldType);
uint8_t lctrBisRxTest(lctrBisCtx_t *pBisCtx, uint8_t pldType);
uint8_t LctrBisReadTestCounters(lctrBisCtx_t *pBisCtx, LlIsoTestCtrs_t *pStats);

/* BIS helper functions */
void lctrBisDefaults(void);
void lctrNotifyIsoTxComplete(lctrBigCtx_t *pBigCtx);
void lctrBisCalcGroupSessionKey(const uint8_t *pGSKD, const uint8_t *pBC, uint8_t *pGSK);
bool lctrBisCalcNextIdxSequential(lctrBigCtx_t *pBigCtx, lctrSeCtx_t *pSeCtx);
bool lctrBisCalcNextIdxInterleaved(lctrBigCtx_t *pBigCtx, lctrSeCtx_t *pSeCtx);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_BIS_H */
