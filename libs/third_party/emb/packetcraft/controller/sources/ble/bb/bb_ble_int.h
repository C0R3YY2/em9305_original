/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal BLE baseband interface file.
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

#ifndef BB_BLE_INT_H
#define BB_BLE_INT_H

#include "bb_ble_api.h"
#include "bb_ble_api_op.h"
#include "pal_bb_ble.h"
#include "pal_bb.h"
#include "ll_defs.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "cfg_mac_ble.h"
#include "wsf_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum length of an advertising packet. */
#define BB_ADVB_MAX_LEN         WSF_MAX(BB_FIXED_ADVB_PKT_LEN, LL_ADVB_MAX_LEN)

#if (BB_ENABLE_STATS)
/*! \brief      Mark the start of an ISR. */
#define BB_ISR_START()          bbIsrStartTime = PalBbGetCurrentTime()

/*! \brief      Mark the ISR duration, recording the high watermark. */
#define BB_ISR_MARK(x)          (x) = WSF_MAX(x, (uint16_t)(PalBbGetCurrentTime() - bbIsrStartTime))
#else
#define BB_ISR_START()
#define BB_ISR_MARK(x)
#endif  /* BB_ENABLE_STATS */

/*! \brief    Event states for advertising operations. */
enum
{
  BB_EVT_STATE_TX_ADV_IND_OR_CONN_REQ_OR_RSP_OBS,                /*!< Advertising indication or PAwR AUX_CONN_REQ. */
  BB_EVT_STATE_RX_SCAN_OR_CONN_INIT_OR_SYNC_RSP_OR_AUX_CONN_RSP, /*!< Scan or connection init packet or PAwR SYNC_RSP or AUX_CONN_RSP. */
  BB_EVT_STATE_TX_SCAN_OR_CONN_RSP,                              /*!< Scan or connection response. */
  BB_EVT_STATE_TX_CHAIN_IND                                      /*!< Chain indication. */
};

/*! \brief    Event states for scanning operations. */
enum
{
  BB_EVT_STATE_RX_ADV_IND_OR_CONN_REQ_OR_RSP_OBS,                /*!< Advertising indication or PAwR AUX_CONN_REQ. */
  BB_EVT_STATE_TX_SCAN_OR_CONN_INIT_OR_SYNC_RSP_OR_AUX_CONN_RSP, /*!< Scan or connection init packet or PAwR SYNC_RSP or AUX_CONN_RSP. */
  BB_EVT_STATE_RX_SCAN_OR_CONN_RSP,                              /*!< Scan or connection response. */
  BB_EVT_STATE_RX_CHAIN_IND                                      /*!< Chain indication. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Execution operation function. */
typedef void (*bbBleExecOpFn_t)(BbOpDesc_t *pBod, BbBleData_t *pBle);

/*! \brief      BB control block. */
typedef struct
{
  struct
  {
    bbBleExecOpFn_t execOpCback;        /*!< Execute operation handler. */
    bbBleExecOpFn_t cancelOpCback;      /*!< Cancel operation handler. */
  } opCbacks[BB_BLE_OP_NUM];            /*!< Operation handlers. */

  uint8_t evtState;                     /*!< Action state of the currently operating BOD. */
  uint8_t advChIdx;                     /*!< Current advertising channel index. */
  uint8_t numChUsed;                    /*!< Total number of channels visited. */
  uint32_t lastScanStartUsec;           /*!< Last scan start time in microseconds. */

  PalBbBleDataParam_t bbParam;          /*!< Baseband data parameters. */

  uint16_t rxDataLen;                   /*!< Receive data buffer length. */
  uint8_t *pRxDataBuf;                  /*!< Current Rx data buffer. */
} bbBleCtrlBlk_t;


/**************************************************************************************************
  Functions
**************************************************************************************************/
extern void (*pBbMstPawrTxSyncRspCback)(BbOpDesc_t *pBod, uint32_t base, uint32_t offset, PalBbBleTxBufDesc_t *descs, uint8_t numDescs);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* BB BLE Control Block. */
extern bbBleCtrlBlk_t bbBleCb;

/* ISR start time. */
extern uint32_t bbIsrStartTime;

extern BbBleDataPktStats_t bbConnStats;    /*!< Connection packet statistics. */

extern BbBleDataPktStats_t  bbCisStats;    /*!< Connection packet statistics. */

/* Combine BIS statistics; use instantiation in bb_ble_bis_slave.c. */
extern BbBleDataPktStats_t bbBisStats;    /*!< BIS packet statistics. */

extern BbBleCsOpCompCback_t pBbBleCsReflRxCompCback;
extern BbBleCsOpCompCback_t pBbBleCsReflTxCompCback;

extern BbBleCsOpCompCback_t pBbBleCsInitTxCompCback;
extern BbBleCsOpCompCback_t pBbBleCsInitRxCompCback;

extern const BbRtCfg_t *pBbRtCfg;

extern BbBleAuxAdvPktStats_t bbAuxAdvStats; /*!< Auxiliary advertising packet statistics. */
extern void (*pBbSlvPawrTxCompCback)(uint8_t **, uint32_t, uint32_t, uint32_t, BbBleRxQueueGet_t);
extern bool (*pBbSlvPawrRxSubRspCback)(BbOpDesc_t *, BbBleSlvAuxAdvEvent_t *, bool);
extern bool (*pBbSlvPawrRxAuxSubRspCback)(BbOpDesc_t *, BbBleSlvAuxAdvEvent_t *, bool);
extern bool (*pBbSlvPawrTxAuxCback)(BbOpDesc_t *, BbBleSlvAuxAdvEvent_t *);

extern BbBlePerScanPktStats_t bbPerScanStats;
extern void (*pBbMstPerScanTxCompPawrCback)(uint8_t status);
extern bool (*pBbPawrRspObservationCback)(BbOpDesc_t *);
extern bool (*pBbMstPawrRxRspObsAuxCback)(BbOpDesc_t *, BbBleMstPerScanEvent_t *, bool, uint32_t);
extern bool (*pBbMstPawrTxConReqCback)(BbBleMstPerScanEvent_t *, bool);
extern bool (*pBbMstPawrTxSubIndCback)(BbOpDesc_t *, BbBleMstPerScanEvent_t *, uint8_t *, bool, uint32_t);
extern void (*pBbMstPawrRxRspObsCback)(BbOpDesc_t *pBod, uint32_t base, uint32_t offset, uint8_t **pBuf, uint16_t len, uint32_t wwUsec, BbBleRxQueueGet_t pRxQueueGetCb);
#if (LL_ENABLE_RSPOBS)
extern void (*pBbCenPawrTxSyncRspObsCback)(BbOpDesc_t *, uint32_t, uint32_t, PalBbBleTxBufDesc_t *, uint8_t);
#endif

#if (LL_ENABLE_RSPOBS && LL_ENABLE_TESTER)
extern uint8_t *pPawrPeripheralTxBuf[LL_MAX_PAWR_RSP_SLOTS];
#else
extern uint8_t *pPawrPeripheralTxBuf;
#endif

extern uint8_t *pPawrCentralReportBuf;
extern wsfQueue_t lctrPawrCentralRxBufQ;
extern uint8_t *pPawrCentralTxBuf;
extern uint8_t *pPawrPeripheralReportBuf;
/*************************************************************************************************/
/*!
 *  \brief      Register operation handlers.
 *
 *  \param      opType          Operation type.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 */
/*************************************************************************************************/
void bbBleRegisterOp(uint8_t opType, bbBleExecOpFn_t execOpCback, bbBleExecOpFn_t cancelOpCback);

#ifdef __cplusplus
};
#endif

/*************************************************************************************************/
/*!
 *  \brief      Clear IFS in operation.
 *
 *  The BB may choose not to enable IFS after the next Tx or Rx.
 */
/*************************************************************************************************/
static inline void bbBleClrIfs(void)
{
  PalBbBleOpParam_t opParams = { .ifsMode = PAL_BB_IFS_MODE_CLR, .ifsTime = 0, .pIfsChan = NULL };
  PalBbBleSetOpParams(&opParams);
}

/*************************************************************************************************/
/*!
 *  \brief      Set TIFS in operation.
 *
 *  The BB must enable TIFS after the next Tx or Rx.
 */
/*************************************************************************************************/
static inline void bbBleSetTifs(void)
{
  PalBbBleOpParam_t opParams = { .ifsMode = PAL_BB_IFS_MODE_TOGGLE_TIFS, .ifsTime = 0, .pIfsChan = NULL };
  PalBbBleSetOpParams(&opParams);
}

/*************************************************************************************************/
/*!
 *  \brief      Set absolute time IFS in operation.
 *
 *  \param  ifsTime     Due time of the next PDU.
 *  \param  pIfsChan    Channel of the next PDU.
 *
 *  The BB must enable IFS with absolute time after the next Tx or Rx.
 */
/*************************************************************************************************/
static inline void bbBleSetAbsIfs(uint32_t ifsTime, PalBbBleChan_t *pIfsChan)
{
  PalBbBleOpParam_t opParams = { .ifsMode = PAL_BB_IFS_MODE_SAME_ABS, .ifsTime = ifsTime, .pIfsChan = pIfsChan };
  PalBbBleSetOpParams(&opParams);
}

/*************************************************************************************************/
/*!
 *  \brief      Set CS IFS in operation.
 *
 *  \param  csStepIfsUsec     Due time of the next CS PDU.
 *
 *  The BB must enable CS IFS after the next Tx.
 */
/*************************************************************************************************/
static inline void bbBleSetCsIfs(uint32_t csStepIfsUsec, uint32_t rxTimeout)
{
  PalBbBleOpParam_t opParams = { .ifsMode = PAL_BB_IFS_MODE_TOGGLE_CS, .ifsTime = 0, .pIfsChan = NULL, .csStepIfsUsec = csStepIfsUsec, .rxTimeout = rxTimeout };
  PalBbBleSetOpParams(&opParams);
}

/*************************************************************************************************/
/*!
 *  \brief      Compute CS IFS.
 *
 *  \param  pBle       BLE operation data.
 *  \param  isSameStep true if IFS calculation is within same step.
 *
 *  The BB must enable CS IFS after the next Tx.
 */
/*************************************************************************************************/
uint16_t bbCsComputeIfs(BbBleData_t *pBle, bool isSameStep);

#endif /* BB_BLE_INT_H */
