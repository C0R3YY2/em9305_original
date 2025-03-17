/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connected isochronous stream interface file.
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

#ifndef LCTR_INT_CIS_H
#define LCTR_INT_CIS_H

#include "lmgr_api_iso.h"
#include "lctr_int.h"
#include "lctr_int_iso.h"
#include "lctr_int_conn.h"
#include "lctr_api_cis.h"
#include "lctr_pdu_conn.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "lctr_pdu_iso.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum number of burst number. */
#define LCTR_MAX_BN                 0x0F

/*! \brief      Get reservation manager handle for CIG from the context pointer. */
#define LCTR_GET_CIG_RM_HANDLE(pCigCtx)   (LL_MAX_CONN + LL_MAX_ADV_SETS + (pCigCtx - &pLctrCigTbl[0]))

/*! \brief      Get topology manager handle for CIG from the context pointer. */
#define LCTR_GET_CIG_TM_HANDLE(pCigCtx)   (LL_MAX_CONN + LL_MAX_PER_SCAN + (pCigCtx - &pLctrCigTbl[0]))

/*! \brief      Get CIG index. */
#define LCTR_GET_CIG_IDX(pCigCtx)         (uint8_t)(pCigCtx - &pLctrCigTbl[0])

/*! \brief      Resolve CIG context pointer from the reservation manager handle. */
#define LCTR_GET_CIG_RM_CTX(rmHandle)     &(pLctrCigTbl[rmHandle - (LL_MAX_CONN + LL_MAX_ADV_SETS)])

/*! \brief      Resolve CIG context pointer from the topology manager handle. */
#define LCTR_GET_CIG_TM_CTX(rmHandle)     &(pLctrCigTbl[rmHandle - (LL_MAX_CONN + LL_MAX_PER_SCAN)])

#define LCTR_GET_CIS_OFFSET_FROM_CIG(pCisCtx)    (pCisCtx->cigSyncDelayUsec - pCisCtx->cisSyncDelayUsec)

/*! \brief      ISO Data PDU start offset in a buffer. */
#define LCTR_CIS_DATA_PDU_START_OFFSET      4

/*! \brief      Data PDU start offset in a buffer. */
#define LCTR_CIS_DATA_TX_PDU_START_OFFSET   0

/*! \brief      Minimum instant for CIS establishment. */
#define LCTR_CIS_MIN_INSTANT                6  /* Note: value increased to work around harmony bug LL/CIS/CEN/BV-41-C */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      CIS main states. */
enum
{
  LCTR_CIS_STATE_IDLE,              /*!< CIS idle state. */
  LCTR_CIS_STATE_EST,               /*!< CIS established state. */
  LCTR_CIS_STATE_SHUTDOWN,          /*!< CIS terminating state. */
  LCTR_CIS_STATE_TOTAL              /*!< Total number of CIS slave states. */
};

/*! \brief      CIS termination states. */
enum
{
  LCTR_CIS_TERM_STATE_IDLE,         /*!< Idle state. */
  LCTR_CIS_TERM_STATE_TERMINATING,  /*!< Wait for LL_CIS_RSP state. */
  LCTR_CIS_TERM_STATE_TOTAL         /*!< Total CIS states. */
};

/*! \brief      CIS termination procedure events. */
enum
{
  LCTR_CIS_TERM_EVENT_HOST_DISC,                    /*!< Received host disconnect CIS. */
  LCTR_CIS_TERM_EVENT_PEER_DISC,                    /*!< Received peer disconnect CIS. */
  LCTR_CIS_TERM_EVENT_INT_START_DISC,               /*!< Start pending host disconnect CIS procedure. */
  LCTR_CIS_TERM_EVENT_INT_START_PEER_DISC,          /*!< Start pending peer disconnect CIS procedure. */
  LCTR_CIS_TERM_EVENT_CIS_TERM,                     /*!< Received CIS terminated event. */
  LCTR_CIS_TERM_EVENT_TOTAL,                        /*!< Total CIS events. */
  LCTR_CIS_TERM_EVENT_INVALID = 0xFF                /*!< Invalid event. */
};

/*! \brief      CIS PDU type. */
enum
{
  LCTR_CIS_PDU_DEFAULT,             /*!< Default PDU. */
  LCTR_CIS_PDU_NULL,                /*!< NULL PDU. */
  LCTR_CIS_PDU_EMPTY,               /*!< Empty PDU. */
  LCTR_CIS_PDU_NON_EMPTY            /*!< Non-empty PDU. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Parameters related to Tx/Rx flush timeout. */
typedef struct
{
  bool      pduAcked;           /*!< true if current PDU is acked, for Tx only. */
  bool      pduRcved;           /*!< true if current PDU is received, for Rx only. */
  uint8_t   bn;                 /*!< Burst number for the flush timeout. */
  uint8_t   pduCounter;         /*!< Current PDU counter in term of BN. */
  uint8_t   subEvtCounter;      /*!< Current subevent counter before flush timeout. */
  uint8_t   intervalTotal;      /*!< Total number interval before flush timeout. */
  uint8_t   intervalCounter;    /*!< Current interval counter. */
  uint32_t  sduRef;             /*!< SDU ref of current FT parameters. */

  uint8_t   lastSubEvtFt[LCTR_MAX_BN];    /*!< Last subevent the PDU could be transmitted before flush timeout for each PDU(in the last interval). */
  bool      isPduDone[LCTR_MAX_BN];       /*!< true if the PDU is either acked or flushed. */
} lctrFtParam_t;

/*! \brief      Flush timeout parameter node. */
typedef struct ftNode
{
  lctrFtParam_t   ftParam;      /*!< Flush time parameter. */
  struct ftNode   *pNext;       /*!< Pointer to the next node. */
} lctrFtParamNode_t;

/*! \brief      Flush timeout parameter list. */
typedef struct ftList
{
  lctrFtParamNode_t *pHead;     /*!< Pointer to the head of the CIS linked list. */
  lctrFtParamNode_t *pTail;     /*!< Pointer to the tail of the CIS linked list. */
  uint8_t           numNodes;   /*!< Number of nodes in the CIS linked list. */
} lctrFtParamList_t;

/*! \brief      Connected isochronous stream context. */
typedef struct
{
  bool        enabled;          /*!< Enable flag. */
  bool        active;           /*!< Active state (Will be processed by CIG if active). */
  uint8_t     state;            /*!< main state. */
  uint8_t     estState;         /*!< Establishment procedure state. */
  uint8_t     termState;        /*!< Termination procedure state. */
  uint16_t    aclHandle;        /*!< ACL handle. */
  uint16_t    cisHandle;        /*!< CIS handle. */
  uint8_t     role;             /*!< Role. */
  uint8_t     cigId;            /*!< Used to identify the connected isochronous group. */
  uint8_t     cisId;            /*!< Used to identify a connected isochronous stream. */
  uint16_t    cisEvtCounter;    /*!< Event counter. */
  uint32_t    cisSyncDelayUsec; /*!< CIS synchronous delay in microsecond. */
  uint32_t    cigSyncDelayUsec; /*!< CIG synchronous delay in microsecond. */
  bool        cisDone;          /*!< CIS transfer is done, no more subevent for the CIS. Used for interleaved CIS only. */
  bool        isClosing;        /*!< true if the context is closing. */
  bool        termPend;         /*!< The CIS has pended a termination. */
  bool        hostInitTerm;     /*!< The host has initiated the termination. */

  uint8_t     subEvtCounter;    /*!< Sub event counter. */
  bool        isTxDone;         /*!< true if all the Tx are done, start sending NULL packet. */
  bool        pduFlushed;       /*!< true if the PDU is flushed, for Tx only. */

  uint8_t     reason;           /*!< Disconnect reason. */
  lctrCisTermInd_t  cisTerm;    /*!< Peer CIS Disconnect reason. */

  uint64_t    txPktCounter;     /*!< Transmit packet counter. */
  uint64_t    rxPktCounter;     /*!< Receive packet counter. */

  /* Buffers */
  uint8_t     dataHdrBuf[LL_DATA_HDR_LEN];  /*!< Data header buffer. */
  uint8_t     dataBuf[10];                  /*!< Data header buffer. */
  uint16_t    dataCounter;      /*!< Data counter. */
  uint8_t     dataSn;           /*!< Data sequence number. */

  /* LLCP */
  bool        isCisReqPend;     /*!< true if CIS_REQ is sent and response is not received yet. */
  uint16_t    ceRef;            /*!< ACL connection event where the offset referenced. */
  uint16_t    cisCeRef;         /*!< Number of CIS event before CIS is started. */
  uint32_t    offsetUsec;       /*!< Time in microsecond between the start of the referenced CE to the start of first CIS event. */
  wsfTimer_t  tmrProcRsp;       /*!< Procedure response timer. */

  union
  {
    struct
    {
      BbBleSlvCisEvent_t bleSlvData;    /*!< BLE BB operation data. Must be 1st member in the structure. */
      uint32_t      anchorOffsetUsec;   /*!< Offset to the stream anchor point. */
      lctrCisReq_t  cisReq;             /*!< CIS request parameters. */
      uint32_t      offsetUsec;         /*!< Offset to the next Rx. */
      uint32_t      firstRxStartTsUsec; /*!< Timestamp of the first received frame regardless of CRC error in microseconds. */
      bool          syncWithMaster;     /*!< Flag indicating synchronize packet received from master. */
      bool          rxFromMaster;       /*!< At least one successful packet received from master. */
      uint8_t       consCrcFailed;      /*!< Number of consecutive CRC failures. */
      uint8_t       rxStatus;           /*!< Rx status. */
    } slv;                              /*!< Slave connection specific data. */

    struct
    {
      BbBleMstCisEvent_t bleMstData;    /*!< BLE BB operation data. Must be 1st member in the structure. */
      bool          txPduIsAcked;       /*!< true if the PDU is acked. */
      bool          rxFromSlave;        /*!< At least one packet received from slave. */
    } mst;                              /*!< CIS master specific data. */
  } data;                               /*!< role-specific data. */

  /* Channel parameters */
  lmgrChanParam_t   chanParam;          /*!< Channel parameter. */
  uint8_t           chIdx;              /*!< LL channel index. */
  uint8_t           nextSubEvtChanIdx;  /*!< Next subevent channel index. */
  uint32_t          accessAddr;         /*!< Access address. */
  uint32_t          crcInit;            /*!< CRC initialization value. */
  int8_t            lastRssi;           /*!< Last RSSI Value. */

  /* Flow control */
  lctrCisDataPduHdr_t   txHdr;          /*!< Transmit data PDU header. */
  lctrCisDataPduHdr_t   rxHdr;          /*!< Receive data PDU header. */
  wsfQueue_t        txArqQ;             /*!< Transmit ARQ queue. */
  uint8_t           numTxComp;          /*!< Number of completed Tx buffers. */
  uint32_t          delayUsec;          /*!< Time between the start of subevent to the start of next subevent in microsecond.
                                             Same as subEvtInter for sequential scheme, different for interleaved scheme. */
  bool              validRx;            /*!< true if the RX buffer is valid and shall be processed. */
  bool              txPduIsAcked;       /*!< true if the TX PDU is acked. */
  bool              txBufPendAck;       /*!< A transmit buffer is pending acknowledgement. */
  bool              lastTxNull;         /*!< Last packet sent was a null. */
  bool              rxErrorPkt;         /*!< Received an error packet. pending LOST packet. */

  /* Flush timeout Tx/Rx list */
  lctrFtParamList_t txFtParamList;      /*!< Tx flush timeout parameters list. */
  lctrFtParamList_t rxFtParamList;      /*!< Rx flush timeout parameters list. */

  /* Data length */
  lctrDataLen_t     localDataPdu;       /*!< Local Data PDU parameters. */

  /* Supervision */
  wsfTimer_t        tmrSupTimeout;      /*!< Supervision timer. */
  uint16_t          supTimeoutMs;       /*!< Supervision timeout in milliseconds. */
  bool              connEst;            /*!< Connection established. */
  bool              powerIndReq;        /*!< Power control indication required when established conn. */

  /* Encryption */
  uint8_t           iv[LL_IV_LEN];      /*!< Initialization vector. */

  /* PHY */
  uint8_t           phyMToS;            /*!< Master to slave PHY. */
  uint8_t           phySToM;            /*!< Slave to master PHY. */

  /* Data */
  uint8_t           sca;                /*!< Sleep clock accuracy. */
  uint8_t           packing;            /*!< Packing scheme. */
  LlFraming_t       framing;            /*!< Indicates the format of CIS Data PDUs.  */
  uint16_t          sduSizeMToS;        /*!< Maximum SDU size from the master Host. */
  uint16_t          sduSizeSToM;        /*!< Maximum SDU size from the slave Host. */
  uint32_t          sduIntervalMToS;    /*!< Time interval between the start of consecutive SDUs from the master Host in microseconds  */
  uint32_t          sduIntervalSToM;    /*!< Time interval between the start of consecutive SDUs from the master Host in microseconds  */
  uint16_t          isoInterval;        /*!< Isochronous PDU interval in 1.25ms unit. */
  uint8_t           ftMToS;             /*!< Master to slave flush time. */
  uint8_t           ftSToM;             /*!< Slave to master flush time. */
  uint8_t           nse;                /*!< Maximum number of subevent in each interval on CIS. */
  uint32_t          subIntervUsec;      /*!< Subevent duration in microsecond. */
  uint8_t           bnMToS;             /*!< Master to slave burst number. */
  uint8_t           bnSToM;             /*!< Slave to master burst number. */
  uint32_t          nextCisOffsetUsec;  /*!< For slave sequential packing only, from the start of this CIS to the start of the next CIS. */

  /* BB/ISR context */
  bool              firstFromPeer;      /*!< true if received once from peer, used for fast supervision timeout. */
  uint8_t           txDataCounter;      /*!< Tx data counter in each ISO interval, used to determine whether continue operation for the BOD or not. */
  uint8_t           rxDataCounter;      /*!< Rx data counter in each ISO interval, used to determine whether continue operation for the BOD or not. */
  uint32_t          rxSduRefTs;         /*!< Timestamp of last Rx sdu. */

  /* Datapath configuration */
  lctrInDataPathCtx_t   dataPathInCtx;  /*!< Input data path config. */
  lctrOutDataPathCtx_t  dataPathOutCtx; /*!< Output data path context. */

  /* ISO test */
  bool              txTestEnabled;      /*!< true ISO test enabled, false otherwise. */
  bool              rxTestEnabled;      /*!< true if ISO RX test enabled, false otherwise. */
  uint32_t          testSduTs;          /*!< Timestamp of last tx sdu. */
  LlIsoPldType_t    testPldType:8;      /*!< Test payload type. */

  uint32_t          expectedPkt;        /*!< Next expected packet for Rx test. */
  uint32_t          testTxPktCtr;       /*!< Packet counter for TX test. */
  uint32_t          numRxSuccess;       /*!< ISO Rx received payload counter. */
  uint32_t          numRxMissed;        /*!< ISO Rx missed payload counter. */
  uint32_t          numRxFailed;        /*!< ISO Rx failed payload counter. */
  uint8_t           isoRxPldType;       /*!< ISO RX payload length type. */
  bool              rxPendInit;         /*!< ISO test pending initialization flag. */

  LlIsoLinkQual_t   isoLinkQualStats;   /*!< ISO Link quality statistics. */

  lctrIsoalTxCtx_t  isoalTxCtx;         /*!< ISOAL transmit context. */
  lctrIsoalRxCtx_t  isoalRxCtx;         /*!< Partial receive context. */
} lctrCisCtx_t;

/*! \brief      Connected isochronous group context. */
typedef struct
{
  bool              enabled;            /*!< Context enabled. */
  uint8_t           state;              /*!< CIG State. */
  LlPacking_t       packing;            /*!< Packing scheme. */
  bool              isBodBuilt;         /*!< true if BOD is built. */
  bool              isBodStarted;       /*!< true if BOD is started. */
  bool              isRmAdded;          /*!< true if reservation is added. */
  uint8_t           cigId;              /*!< Used to identify the connected isochronous group. */
  uint16_t          cigHandle;          /*!< CIG handle. */
  uint32_t          cigSyncDelayUsec;   /*!< CIG synchronous delay in microsecond. */
  uint32_t          adjOffsetUsec;      /*!< Offset from adjacent BOD in microseconds. */
  uint16_t          isoInterval;        /*!< Isochronous PDU interval in 1.25ms unit. */
  bool              isValid;            /*!< true if CIS parameters are valid and is able to be scheduled. */
  uint8_t           numCisEsted;        /*!< Number of CISs that are established. */
  bool              isLoopBack;         /*!< true if all the CIS streams have been traversed once. */
  bool              headCisRmved;       /*!< true if the head CIS in the CIG is removed. */
  uint32_t          offsetUsec;         /*!< Only valid when headCisRmved is true, duration between the old anchor point and new anchor point. */
  uint32_t          firstRxStartTsUsec; /*!< Only valid when headCisRmved is true, timestamp of the first received frame regardless of CRC error. */
  uint32_t          cigRefPoint;        /*!< CIG reference point of scheduled operation. */

  /* BB data */
  BbOpDesc_t        cigBod;             /*!< CIG BOD. */

  /* Linked list of CIS context. */
  lctrCisCtx_t      *pCisCtxList[LL_MAX_CIS];
                                        /*!< List of CIS contexts associated with this CIG. */
  lctrCisCtx_t      *pCisCtx;           /*!< Pointer to the current CIS context. */

  /* BOD data. */
  struct
  {
    struct
    {
      uint16_t      cigEvtCounter;      /*!< Event counter. */
      uint32_t      anchorPointUsec;    /*!< Anchor point in microseconds. */
      uint16_t      lastActiveEvent;    /*!< Last active event counter. */
      uint16_t      totalAcc;           /*!< Combined sleep clock inaccuracy. */
    } slv;                              /*!< Slave BOD data. */

    struct
    {
      uint8_t       numCis;             /*!< Number of CIS. */
    } mst;                              /*!< Master BOD data. */
  } roleData;                           /*!< Role-specific BOD Data. */

  PalBbBleTxBufDesc_t     dataPdu[3];   /*!< Data PDU descriptor. */
} lctrCigCtx_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrCisCtx_t *pLctrCisTbl;
extern lctrCigCtx_t *pLctrCigTbl;
extern lctrCisMsg_t *pLctrCisMsg;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/* State machine */
void lctrCisExecuteSm(lctrCisCtx_t *pCisCtx, uint8_t event);
bool lctrLlcpExecuteCisTermSm(lctrConnCtx_t *pCtx, uint8_t event);
void lctrCisDisp(lctrCisMsg_t *pMsg);

/* Action routines for main state machine */
void lctrCisActCisEst(lctrCisCtx_t *pCtx);
void lctrCisActCisEstFail(lctrCisCtx_t *pCtx);
void lctrCisActDisc(lctrCisCtx_t *pCtx);
void lctrCisActClosed(lctrCisCtx_t *pCisCtx);
void lctrCisActFail(lctrCisCtx_t *pCisCtx);

/* Action routines for LLCP termination state machine */
void lctrCisLlcpActHostDisc(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrCisLlcpActPeerDisc(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrCisLlcpActCisTerm(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrCisLlcpActIntHostDisc(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrCisLlcpActIntPeerDisc(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);

/* CIS Context */
lctrCisCtx_t *lctrAllocCisCtx(lctrCigCtx_t *pCigCtx);
void lctrCleanupCtx(lctrCisCtx_t *pCisCtx);
void lctrFreeCisCtx(lctrCisCtx_t *pCisCtx);
lctrCisCtx_t *lctrFindCisByHandle(uint16_t cisHandle);
lctrCisCtx_t *lctrFindCisById(uint8_t cigId, uint8_t cisId);
bool lctrIsCisConnectedOnAcl(uint16_t aclHandle);
bool lctrIsCisConnectedOnAclExcludeSelf(uint16_t aclHandle, uint16_t cisHandle);
uint16_t lctrGetAclHandleFromCis(uint16_t cisHandle);
bool lctrCisPendingConn(uint16_t cisHandle);
uint8_t lctrGetNumAvailCisCtx(void);
uint8_t lctrGetNumEnabledCisCtx(LlCisCigParams_t *pSetCigParam);
uint8_t lctrGetNumEnabledCisCtxTest(LlCisCigParamsTest_t *pSetCigParam);
uint8_t lctrGetNumEstCisCtx(LlCisCigParams_t *pSetCigParam);
uint8_t lctrGetNumEstCisCtxTest(LlCisCigParamsTest_t *pSetCigParam);
uint8_t lctrGetNumEstCisCtxByCigCtx(lctrCigCtx_t *pCigCtx);

/* CIS list utility functions */
bool lctrCisActivate(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
bool lctrCisDeactivate(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
bool lctrCisInsert(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
bool lctrCisInsertFirst(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
bool lctrCisRemoveFromList(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
bool lctrCisRemoveOrDeactivate(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
bool lctrCisNoActiveCis(lctrCigCtx_t *pCigCtx);
uint8_t lctrCisGetListCount(lctrCigCtx_t *pCigCtx);
lctrCisCtx_t * lctrCisGetFirstActiveCis(lctrCigCtx_t *pCigCtx);
lctrCisCtx_t * lctrCisGetFirstConnectedCis(lctrCigCtx_t *pCigCtx);
bool lctrCisIsFirstActiveCis(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
lctrCisCtx_t * lctrCisGetNextCis(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx);
lctrCisCtx_t * lctrCisGetPreCis(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCurCis);
bool lctrCisAreCisCtxDone(lctrCigCtx_t *pCigCtx);
void lctrCisClearCisDone(lctrCigCtx_t *pCigCtx);
void lctrCisSetCisDone(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCurCisCtx);

/* CIS flush timeout list utility functions */
lctrFtParamNode_t * lctrCisFtCreateFtParamNode(lctrFtParam_t *pFtParam);
bool lctrCisFtInsertHead(lctrFtParamList_t *pList, lctrFtParam_t *pFtParam);
bool lctrCisFtInsertTail(lctrFtParamList_t *pList, lctrFtParam_t *pFtParam);
bool lctrCisFtRemoveHead(lctrFtParamList_t *pList);
bool lctrCisFtRemoveTail(lctrFtParamList_t *pList);
bool lctrCisFtIsListEmpty(lctrFtParamList_t *pList);
void lctrCisFtListClear(lctrFtParamList_t *pList);

/* CIG Context */
lctrCigCtx_t *lctrAllocCigCtx(uint8_t cigId);
void lctrFreeCigCtx(lctrCigCtx_t *pCigCtx);
lctrCigCtx_t *lctrFindCigById(uint8_t cigId);
uint8_t lctrGetNumAvailCigCtx(void);

/* Helper */
void lctrCisDefaults(void);
bool lctrIsCisEst(lctrCisCtx_t *pCtx);
void lctrCisSetupChanParam(lctrCisCtx_t *pCisCtx, uint64_t chanMask);
void lctrCisSetupEncrypt(lctrCisCtx_t *pCisCtx);
void lctrCisStoreTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreDisconnectReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreConnFailEstablishTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreLlcpTimeoutTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreLocalLowResourceTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreMicFailedTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreConnTimeoutTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStoreLlcpPeerRejTerminateReason(lctrCisCtx_t *pCisCtx);
void lctrCisStartLlcpTimer(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrCisStopLlcpTimer(lctrConnCtx_t *pCtx, lctrCisCtx_t *pCisCtx);
void lctrCisInitFtParam(lctrFtParam_t *pFtParam, uint8_t bn, uint8_t ft, uint8_t nse, uint32_t sduRef);
uint32_t lctrCisCalcSubEvtDurationUsecSeq(PalBbPhy_t phyMToS, PalBbPhy_t phySToM, PalBbPhy_t plMToS, PalBbPhy_t plSToM);
uint32_t lctrCisCalcSubEvtDurationUsecInter(LlCisCigParams_t *pSetCigParam);
void LctrCisUpdateChanMap(uint16_t  aclHandle);
void lctrCleanupCigCtx(void);

/* Function used by connection context */
bool lctrCheckForCisLinkTerm(uint16_t aclHandle);
bool lctrCheckIsCisEstAcl(uint16_t aclHandle);
bool lctrCheckIsCisEstCis(uint16_t cisHandle);

/* CIS Tx data path */
uint32_t lctrIsoTxInitMem(uint8_t *pFreeMem, uint32_t freeMemSize);
void lctrCisTxDataPduQueue(lctrCisCtx_t *pCtx, lctrIsoHdr_t *pIsoHdr, uint8_t *pIsoBuf);
uint8_t lctrCisTxQueuePeek(lctrCisCtx_t *pCisCtx, PalBbBleTxBufDesc_t *bbDescs);
bool lctrCisPduTxPending(lctrCisCtx_t *pCisCtx);
bool lctrCisTxQueuePop(lctrCisCtx_t *pCisCtx);
void lctrCisTxQueuePopCleanup(lctrCisCtx_t *pCisCtx);
void lctrCisTxQueueClear(lctrCisCtx_t *pCisCtx);

/* CIS Rx data path */
uint8_t *lctrCisRxPduAlloc(void);
void lctrCisRxPduFree(uint8_t *pBuf);
void lctrCisRxEnq(uint8_t *pBuf, uint32_t sduRef, uint16_t cisHandle);
uint8_t *lctrCisRxDeq(uint16_t *pCisHandle, uint32_t *pSduRef);

/* Message */
void lctrSendCisMsg(lctrCisCtx_t *pCisCtx, uint8_t event);
void lctrSendCisLlcpMsg(lctrCisCtx_t *pCisCtx, uint8_t event);

/* Notification */
void lctrNotifyHostCisEst(lctrCisCtx_t *pCisCtx, uint8_t status, uint32_t cigSyncDelayUsec);
void lctrNotifyHostCisTerm(lctrCisCtx_t *pCisCtx, uint8_t status);

/* ISR */
uint16_t lctrCisSetupForTx(lctrCigCtx_t *pCigCtx, uint8_t rxStatus, bool reqTx);
bool lctrCisProcessRxAck(lctrCisCtx_t *pCisCtx);
bool lctrCisProcessTxAck(lctrCisCtx_t *pCisCtx);
void lctrCisTxPduAck(lctrCisCtx_t *pCisCtx);
void lctrCisProcessTxAckCleanup(lctrCisCtx_t *pCisCtx);
void lctrCisRxPostProcessing(lctrCisCtx_t *pCisCtx, uint8_t *pRxBuf);
void lctrCisTxTestPayloadHandler(lctrCisCtx_t * pCisCtx);
void lctrCisCheckUnframedFlush(lctrCisCtx_t *pCisCtx);
void lctrCisLostPacket(lctrCisCtx_t *pCisCtx);
void lctrCisServicePowerMonitor(lctrConnCtx_t *pConnCtx);

/* Scheduler */
BbOpDesc_t *lctrCisResolveConflict(BbOpDesc_t *pNewOp, BbOpDesc_t *pExistOp);

/*************************************************************************************************/
/*!
 *  \brief  Increment the Tx/encrypt packet counter.
 *
 *  \param  pCisCtx    Connection context.
 */
/*************************************************************************************************/
static inline void lctrCisIncPacketCounterTx(lctrCisCtx_t *pCisCtx)
{
  pCisCtx->txPktCounter++;
}

/*************************************************************************************************/
/*!
 *  \brief  Increment the Rx/decrypt packet counter.
 *
 *  \param  pCisCtx    Connection context.
 */
/*************************************************************************************************/
static inline void lctrCisIncPacketCounterRx(lctrCisCtx_t *pCisCtx)
{
  pCisCtx->rxPktCounter++;
}

/*************************************************************************************************/
/*!
 *  \brief  Set the CIS Tx packet counter value in the BB.
 *
 *  \param  pCisCtx    Connection context.
 */
/*************************************************************************************************/
static inline void lctrSetBbCisPacketCounterTx(lctrCisCtx_t *pCisCtx)
{
  if (lctrSetEncryptPktCountHdlr != NULL)
  {
    PalCryptoEnc_t * const pEnc = &pCisCtx->data.mst.bleMstData.cmn.chan.enc;

    if (!pEnc->enaEncrypt)
    {
      return;
    }
    lctrSetEncryptPktCountHdlr(pCisCtx->txPktCounter);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set the CIS Rx packet counter value in the BB.
 *
 *  \param  pCisCtx    Connection context.
 */
/*************************************************************************************************/
static inline void lctrSetBbCisPacketCounterRx(lctrCisCtx_t *pCisCtx)
{
  if (lctrSetDecryptPktCountHdlr != NULL)
  {
    PalCryptoEnc_t * const pEnc = &pCisCtx->data.mst.bleMstData.cmn.chan.enc;

    if (!pEnc->enaDecrypt)
    {
      return;
    }
    lctrSetDecryptPktCountHdlr(pCisCtx->rxPktCounter);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Return first (earliest) CIS of the CIG CIS list, regardless of active state.
 *
 *  \param      pCigCtx   CIG context.
 *
 *  \return     CIS context of earliest CIS.
 */
/*************************************************************************************************/
static inline lctrCisCtx_t * lctrCisGetFirst(lctrCigCtx_t *pCigCtx)
{
  return pCigCtx->pCisCtxList[0];
}

/*************************************************************************************************/
/*!
 *  \brief      Return next CIS in time, regardless of active state.
 *
 *  \param      pCigCtx   CIG context.
 *  \param      pCisCtx   Connection context.
 *
 *  \return     CIS context of next CIS.
 */
/*************************************************************************************************/
static inline lctrCisCtx_t * lctrCisGetNext(lctrCigCtx_t *pCigCtx, lctrCisCtx_t *pCisCtx)
{
  lctrCisCtx_t * pNxtCisCtx = NULL;
  for (int i = 0; i < LL_MAX_CIS - 1; i++)
  {
    if (pCigCtx->pCisCtxList[i] == pCisCtx)
    {
      pNxtCisCtx = pCigCtx->pCisCtxList[i + 1];
      break;
    }
  }
  return pNxtCisCtx;
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CIS_H */
