/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Enhanced ATT (EATT) main module.
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
#ifndef EATT_MAIN_H
#define EATT_MAIN_H

#include "wsf_queue.h"
#include "wsf_timer.h"
#include "l2c_api.h"
#include "dm_api.h"
#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* EATT channel control block. */
typedef struct
{
  uint16_t      cid;                          /* L2CAP channel identifier. */
  uint16_t      priority;                     /* Slot priority */
  uint16_t      peerMtu;                      /* Peer MTU of L2CAP channel */
  uint16_t      localMtu;                     /* Local  MTU of L2CAP channel */
  uint16_t      localMps;                     /* Local MPS of L2CAP channel */
  bool          inUse;                        /* L2CAP channel is open */
} eattChanCb_t;

/* EATT connection control block. */
typedef struct
{
  uint8_t       state;                        /* Connection state. */
  uint16_t      pendingMtu;                   /* Pending MTU */
  uint16_t      pendingMps;                   /* Pending MPS */
  uint16_t      connInterval;                 /* The connection interval. */
  wsfTimer_t    backoffTimer;                 /* Backoff timer for L2CAP en conn req collisions. */
  eattChanCb_t  *pChanCb;                     /* Channel control block list. */
} eattConnCb_t;

/* EATT main control block. */
typedef struct
{
  eattConnCb_t  ccb[DM_CONN_MAX];             /* Connection control blocks. */
  l2cCocRegId_t cocRegId;                     /* L2CAP COC registration ID. */
} eattCb_t;

extern eattCb_t eattCb;

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

uint16_t eattGetCid(dmConnId_t connId, uint8_t slot);
uint8_t eattGetSlotId(dmConnId_t connId, uint16_t cid);
void eattReleaseSlot(dmConnId_t connId, uint8_t slot);
eattConnCb_t *eattGetConnCb(dmConnId_t connId);
void eattHandler(wsfMsgHdr_t *pMsg);
void eattExecCallback(dmConnId_t connId, uint8_t event, uint8_t status);
void attEmptyL2cCocDataCback(l2cCocEvt_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* EATT_MAIN_H */
