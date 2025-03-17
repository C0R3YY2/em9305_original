/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising extension interface file.
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

#ifndef LMGR_API_ADV_MASTER_AE_H
#define LMGR_API_ADV_MASTER_AE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Periodic advertising sync established event */
typedef struct
{
  uint8_t       advSID;         /*!< Advertising SID. */
  uint8_t       addrType;       /*!< Address type. */
  bdAddr_t      addr;           /*!< Address. */
  uint8_t       advPhy;         /*!< Advertising PHY. */
  uint16_t      advInterval;    /*!< Advertising interval. */
  uint8_t       advClkAccuracy; /*!< Advertising clock accuracy. */
  /* Version 2 */
  uint8_t       numSubevents;   /*!< Number of subevents. */
  uint8_t       subeventInterv; /*!< PAwR subevent  interval. */
  uint8_t       rspSlotDelay;   /*!< PAwR response slot delay. */
  uint8_t       rspSlotSpacing; /*!< PAwR response slot spacing. */
} lmgrPerAdvSyncEstdInd_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Events */
void LmgrSendExtScanEnableCnf(uint8_t status);
void LmgrSendScanTimeoutInd(void);
void LmgrSendExtAdvRptInd(LlExtAdvReportInd_t *pEvt);
void LmgrSendPerAdvRptInd(LlPerAdvReportInd_t *pEvt);
void LmgrSendSyncEstInd(uint8_t status, uint16_t handle, lmgrPerAdvSyncEstdInd_t *pEvt, bool pawrUsed);
void LmgrSendSyncLostInd(uint16_t handle);
void LmgrSendPerAdvSubeventDataReq(uint8_t handle, uint8_t subeventStart, uint8_t subeventDataCount);

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_MASTER_AE_H */
