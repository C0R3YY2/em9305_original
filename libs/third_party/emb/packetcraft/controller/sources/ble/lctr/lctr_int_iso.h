/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connection interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_ISO_H
#define LCTR_INT_ISO_H

#include "lctr_api_iso.h"
#include "lctr_int.h"
#include "lctr_int_conn.h"
#include "lctr_int_codec.h"
#include "lctr_pdu_iso.h"
#include "lmgr_api_iso.h"
#include "ll_defs.h"
#include "wsf_cs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define LCTR_ISO_DATA_PDU_FC_OFFSET     0       /*!< Flow control fields data PDU offset. */
#define LCTR_ISO_DATA_PDU_LEN_OFFSET    1       /*!< Length field CIS or BIS Data PDU offset. */

/*! \brief      First CIS handle (follows ACL handles). */
#define LCTR_FIRST_CIS_HANDLE           (pLctrRtCfg->maxConn)

/*! \brief      First BIS handle (follows CIS handles). */
#define LCTR_FIRST_BIS_HANDLE           (pLctrRtCfg->maxConn + pLctrRtCfg->maxCis)

/*! \brief      Fragment header maximum length. */
#define LCTR_ISO_FRAG_HDR_MAX_LEN       (LL_ISO_DATA_HDR_LEN + LL_ISO_SEG_HDR_LEN + LL_ISO_SEG_TO_LEN)

/*! \brief      Fragment trailer maximum length. */
#define LCTR_ISO_FRAG_TRL_MAX_LEN       LL_DATA_MIC_LEN

/*! \brief      SDU offset to start of the SDU data buffer. */
#define LCTR_ISO_SDU_DATA_START_OFFSET  (HCI_ISO_HDR_LEN + HCI_ISO_DL_MAX_LEN)

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief      Transmit buffer descriptor. */
typedef struct
{
  uint8_t *pIsoSdu;                     /*!< ISO buffer. */
  uint8_t *pPduBuf;                     /*!< PDU data buffer. */
  uint16_t isoLen;                      /*!< ISO SDU length. */
  uint8_t  fragLen;                     /*!< Fragmentation length. */
  uint8_t  fragCnt;                     /*!< Current fragmentation Tx count. */
  int8_t   ptoCnt;                      /*!< PTO offset count. */
  bool     ptoCntFlag;                  /*!< true if the descriptor's counter has already been decremented. */
  bool     tsFlag;                      /*!< Timestamp flag. */
  uint32_t ts;                          /*!< Timestamp. */
  struct
  {
    uint8_t hdrLen;                     /*!< Data PDU header length. */
    uint8_t trlLen;                     /*!< Data PDU trailer length. */
    uint8_t hdr[LCTR_ISO_FRAG_HDR_MAX_LEN];   /*!< Data PDU header. */
    uint8_t trl[LCTR_ISO_FRAG_TRL_MAX_LEN];   /*!< Data PDU trailer (i.e., MIC). */
  } frag[];                             /*!< Fragmented Data PDU packet data. */
} lctrIsoTxBufDesc_t;

/*! \brief      Codec event handlers. */
extern lctrCodecHandlers_t lctrCodecHdlr;

/*! \brief      Transmit ISOAL Context. */
typedef struct
{
  wsfQueue_t        pendingSduQ;        /*!< ISO pending SDU queue. */
  uint8_t           pendQueueSize;      /*!< Pending queue size. */
  uint16_t          sduOffset;          /*!< Offset of incomplete SDU. */
  bool              compSdu;            /*!< Completed SDU sent. */
} lctrIsoalTxCtx_t;

/*! \brief      Receive ISOAL Context. */
typedef struct
{
  uint8_t           *pPendSduBuf;       /*!< Pointer to incomplete SDU buffer. */
  uint16_t          rxSduOffset;        /*!< Offset of SDU currently being assembled. */
  bool              pduFlushed;         /*!< Most recent PDU was flushed due to failed RXs. */
  uint8_t           rxState;            /*!< RX state. */
  uint16_t          packetSequence;     /*!< Packet sequence number. */
  uint32_t          timeStamp;          /*!< Timestamp of packet. */

  union
  {
    struct
    {
      uint32_t      timeOffset;         /*!< Time offset of an SDU. */
    } framed;                           /*!< Framed specific data. */

    struct
    {
      wsfQueue_t    pendSduQ;           /*!< Pending PDU fragments. */
      uint16_t      curLen;             /*!< Current length of SDU being received. */
      lctrPktStatus_t ps;               /*!< Packet status. */
    } unframed;                         /*!< Unframed specific data. */
  } data;                               /*!< Framing-specific data. */
} lctrIsoalRxCtx_t;

/*! \brief      Input datapath context. */
typedef struct
{
  LlIsoDataPath_t   id:8;               /*!< Input data path ID. */

  union
  {
    struct
    {
      uint16_t         streamId;        /*!< Stream ID. */
    } codec;                            /*!< Codec-specific configuration. */
  } cfg;                                /*!< Data path specific configuration. */
} lctrInDataPathCtx_t;                  /*!< Input data path configuration. */

/*! \brief      Output datapath context. */
typedef struct
{
  LlIsoDataPath_t   id:8;               /*!< Output data path ID. */

  union
  {
    struct
    {
      wsfQueue_t    rxDataQ;            /*!< Receive data pending queue. */
      uint8_t       numRxPend;          /*!< Number of messages pending in the Rx queue. */
    } hci;                              /*!< HCI data path configuration. */

    struct
    {
      wsfQueue_t    rxDataQ;            /*!< Receive data pending queue. */
      uint8_t       numRxPend;          /*!< Number of messages pending in the Rx queue. */
      uint16_t      streamId;           /*!< Stream ID. */
    } codec;                            /*!< Codec-specific configuration. */
  } cfg;                                /*!< Data path specific configuration parameters. */
} lctrOutDataPathCtx_t;                 /*!< Output data path configuration. */

/*! \brief      Data path context. */
typedef union
{
  lctrInDataPathCtx_t  in;       /*!< Input context. */
  lctrOutDataPathCtx_t out;      /*!< Output context. */
} lctrDataPathCtx_t;             /*!< Data path context collection. */

/*! \brief      Data path setup parameters. */
typedef struct
{
  uint16_t handle;                      /*!< ISO Handle. */
  uint8_t dpDir;                        /*!< Data path direction. */
  uint32_t isoInt;                      /*!< ISO interval. */
  uint32_t pktCtr;                      /*!< Current packet counter. */
  uint32_t syncDelayUsec;               /*!< Synchronization delay in microseconds. */
  lctrDataPathCtx_t *pDataPathCtx;      /*!< Data path context. */
  uint64_t srcAddr;                     /*!< Source device address. */
  uint64_t dstAddr;                     /*!< Destination device address. */
} lctrDpParams_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void lctrIsoDataInit(void);

/* Task event handler */
void lctrIsoTxCompletedHandler(void);
void lctrCisRxPendingHandler(void);
void lctrIsoTxEventHandler(void);
void lctrNotifyHostIsoEventComplete(uint8_t handle, uint32_t evtCtr);

/* ISO data path */
lctrIsoTxBufDesc_t *lctrAllocIsoTxBufDesc(void);
void lctrFreeIsoTxBufDesc(lctrIsoTxBufDesc_t *pDesc);
uint8_t lctrIsoSetupDataPath(lctrDpParams_t *pDpParam, LlIsoSetupDataPath_t *pSetupDataPath, uint32_t sduRef);
void lctrLoadCodecBuffer(uint16_t handle, uint32_t lastSduRef);
void lctrIsoSendCodecSdu(uint16_t id, uint32_t pktCtr, uint32_t ts, uint8_t *pData, uint16_t actLen);
bool lctrIsoRxConnEnq(lctrOutDataPathCtx_t *pOutDataPathCtx, uint16_t handle, uint32_t pktCtr, uint8_t *pBuf);
void lctrIsoInDataPathClear(lctrDpParams_t *pDpParam);
void lctrIsoOutDataPathClear(lctrDpParams_t *pDpParam);
void lctrIsoalRxDataPathClear(lctrIsoalRxCtx_t *pRxCtx, uint8_t framing);
uint8_t lctrIsoInDataPathSetup(lctrDpParams_t *pDpParam, LlIsoSetupDataPath_t *pSetupDataPath, uint32_t sduRef);
uint8_t lctrIsoOutDataPathSetup(lctrDpParams_t *pDpParam, LlIsoSetupDataPath_t *pSetupDataPath);
uint8_t *lctrIsoRxConnDeq(lctrOutDataPathCtx_t *pOutCtx);
uint8_t *lctrRxSduAlloc(void);
bool lctrRecombineRxUnframedSdu(lctrIsoalRxCtx_t *pRxCtx, uint8_t *pSduFrag);

/* ISO Test mode. */
uint8_t *lctrGenerateIsoTestData(uint16_t handle, LlIsoPldType_t pldType, uint16_t maxSdu, uint32_t pktCtr);
void lctrValidateIsoTestData(uint8_t *pPld, uint8_t actLen, LlIsoTestCtrs_t *pRxStats, uint8_t pldType, uint16_t expMaxSdu, uint32_t expPldCtr);
/* ISOAL Utility. */
uint8_t lctrAssembleTxFramedPdu(lctrIsoalTxCtx_t *pIsoalTxCtx, uint8_t *pPduBuf, uint16_t maxPduLen);
uint8_t lctrAssembleRxFramedSdu(lctrIsoalRxCtx_t *pIsoalRxCtx, wsfQueue_t *pRxQueue,
                                uint16_t handle, uint8_t *pIsoBuf, uint8_t len, uint32_t cigRef, uint16_t maxSduLen);
uint8_t *lctrTxIsoDataPduAlloc(void);

/*************************************************************************************************/
/*!
 *  \brief  Increment available Tx data buffers.
 */
/*************************************************************************************************/
static inline void lctrIsoSduTxIncAvailBuf(void)
{
  WSF_CS_INIT(cs);

  WSF_CS_ENTER(cs);
  lmgrIsoCb.availTxBuf++;
  WSF_CS_EXIT(cs);
}

/*************************************************************************************************/
/*!
 *  \brief  Decrement available Tx data buffers.
 */
/*************************************************************************************************/
static inline void lctrIsoSduTxDecAvailBuf(void)
{
  WSF_CS_INIT(cs);

  WSF_CS_ENTER(cs);
  lmgrIsoCb.availTxBuf--;
  WSF_CS_EXIT(cs);
}

/*************************************************************************************************/
/*!
 *  \brief  Increment available Rx data buffers.
 *
 *  \param  numBufs     Number of additional buffers available.
 */
/*************************************************************************************************/
static inline void lctrIsoDataRxIncAvailBuf(uint8_t numBufs)
{
  WSF_CS_INIT(cs);

  WSF_CS_ENTER(cs);
  lmgrIsoCb.availRxBuf += numBufs;
  WSF_CS_EXIT(cs);
}

/*************************************************************************************************/
/*!
 *  \brief  Decrement available Rx data buffers.
 */
/*************************************************************************************************/
static inline void lctrIsoDataRxDecAvailBuf(void)
{
  WSF_CS_INIT(cs);

  WSF_CS_ENTER(cs);
  lmgrIsoCb.availRxBuf--;
  WSF_CS_EXIT(cs);
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ISO_H */
