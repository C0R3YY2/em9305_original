/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller advertising slave interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_ADV_SLAVE_H
#define LCTR_INT_ADV_SLAVE_H

#include "lctr_int.h"
#include "lctr_api_adv_slave.h"
#include "lctr_pdu_adv.h"
#include "bb_ble_api.h"
#include "bb_ble_api_op.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave advertising states. */
enum
{
  LCTR_ADV_STATE_DISABLED,              /*!< Advertising disabled state. */
  LCTR_ADV_STATE_ENABLED,               /*!< Advertising enabled state. */
  LCTR_ADV_STATE_SHUTDOWN,              /*!< Advertising shutdown in progress. */
  LCTR_ADV_STATE_RESET,                 /*!< Advertising reset in progress. */
  LCTR_ADV_STATE_TOTAL                  /*!< Total number of advertising states. */
};

/*! \brief      Common extended advertising PDU types. */
enum
{
  LCTR_PDU_ADV_EXT_IND,
  LCTR_PDU_ADV_DECISION_IND,
  LCTR_PDU_AUX_ADV_IND,
  LCTR_PDU_AUX_SCAN_RSP,
  LCTR_PDU_AUX_SYNC_IND,
  LCTR_PDU_AUX_CHAIN_IND,
  LCTR_PDU_AUX_CONNECT_RSP,
  LCTR_PDU_AUX_SYNC_SUBEVENT_IND,
  LCTR_PDU_AUX_SYNC_SUBEVENT_RSP
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Slave advertising state context. */
typedef struct
{
  /* Adv buffer (placed here to 32-bit align) */
  uint8_t           advBuf[LL_ADVB_MAX_LEN];
                                        /*!< Advertising host data buffer. */

  uint8_t           state;              /*!< Advertising state. */

  /* BB/ISR context */
  bool              connIndRcvd;        /*!< Connection indication received flag. */
  bool              shutdown;           /*!< Client initiated shutdown flag. */
  uint8_t           usedChSel;          /*!< Used channel selection. */
  BbOpDesc_t        advBod;             /*!< Advertising BOD. */
  BbBleSlvAdvEvent_t bleData;           /*!< BLE BB operation data. */
  uint32_t          reqEndTsUsec;       /*!< Last received request end of packet timestamp in microseconds. */

  /* Scan buffer (placed here to 32-bit align) */
  uint8_t           scanRspBuf[LL_ADVB_MAX_LEN];
                                        /*!< Advertising host data buffer. */

  wsfQueue_t        rxScanReqQ;         /*!< Received SCAN_REQ queue. */

  /* Privacy: Address changing triggers */
  uint8_t           addrChangeTrig;     /*!< Address related change triggers. */
} lctrSlvAdvCtx_t;

/*! \brief      Scan request. */
typedef struct
{
  uint8_t          scanAddrType;        /*!< Scanner address type. */
  uint64_t         scanAddr;            /*!< Scanner address. */
  uint8_t          scanIdAddrType;      /*!< Scanner ID address type. */
  uint64_t         scanIdAddr;          /*!< Scanner ID address. */
} lctrSlvScanReport_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrSlvAdvCtx_t lctrSlvAdv;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Builder */
void lctrSlvAdvBuildOp(void);
void lctrSlvAdvCleanupOp(void);

/* Event handlers */
void lctrSlvRxScanReq(void);

/* Address selection */
void lctrChooseAdvA(BbBleData_t * const pBle, lctrAdvbPduHdr_t *pPduHdr,
                    uint8_t ownAddrType, uint8_t peerAddrType, uint64_t peerAddr,
                    uint64_t *pAdvA);
void lctrChoosePeerAddr(BbBleData_t * const pBle, uint8_t ownAddrType,
                        uint8_t peerAddrType, uint64_t peerAddr, uint64_t *pPeerRpa);

/* ISR */
bool lctrSlvAdvHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
bool lctrScanReqHandler(BbOpDesc_t *pOp, uint8_t reqLen);
void lctrConnIndHandler(BbOpDesc_t *pOp, uint8_t reqLen, const uint8_t *pReqBuf);
void lctrSlvAdvPostProcessHandler(BbOpDesc_t *pOp, const uint8_t *pReqBuf);
void lctrSlvAdvEndOp(BbOpDesc_t *pOp);

/* Channel */
uint32_t lctrCalcAdvDelay(void);

/* Action routines */
void lctrAdvActStart(void);
void lctrAdvActSelfStart(void);
void lctrAdvActShutdown(void);
void lctrAdvActAdvCnf(void);
void lctrAdvActDisallowAdvCnf(void);
void lctrAdvActSelfTerm(void);
void lctrAdvActAdvTerm(void);
void lctrAdvActResetTerm(void);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_SLAVE_H */
