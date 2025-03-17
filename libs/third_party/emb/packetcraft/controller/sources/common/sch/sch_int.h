/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Internal multi-protocol scheduler interface file.
 *
 *  Copyright (c) 2013-2019 ARM Ltd. All Rights Reserved.
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

#ifndef SCH_INT_H
#define SCH_INT_H

#include "sch_api.h"
#include "pal_bb.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Delay in microseconds to start timer. */
#define SCH_TIMER_DELAY_US      25

/*! \brief      Event queue size, must be multiple of ^2. */
#define SCH_EVT_Q_SIZE          4U

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Scheduler states. */
enum
{
  SCH_STATE_IDLE,               /*!< Scheduler idle state. */
  SCH_STATE_LOAD,               /*!< Scheduler load delay state. */
  SCH_STATE_EXECUTE,            /*!< Scheduler execute in progress state. */
  SCH_STATE_UNLOAD,             /*!< Scheduler unload state (BOD completion). */
  SCH_STATE_BACKGROUND          /*!< Scheduler background execute in progress state. */
};
typedef uint8_t schState_t;

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Scheduler control block. */
typedef struct
{
  schState_t state:8;           /*!< Current scheduler state. */
  wsfHandlerId_t handlerId;     /*!< System event handler ID. */

  struct
  {
    size_t prodIdx;             /*!< Producer index, must be atomic size. */
    size_t consIdx;             /*!< Consumer index, must be atomic size. */
    uint8_t events[SCH_EVT_Q_SIZE];
                                /*!< Circular queue of events. */
  } eventQ;                     /*!< Lock-free event queue. */

  BbOpDesc_t *pHead;            /*!< Head element of scheduled list of BOD. */
  BbOpDesc_t *pTail;            /*!< Tail element of scheduled list of BOD. */

  uint16_t schHandlerWatermarkUsec; /*!< Statistics: Handler duration watermark in microseconds. */

  struct
  {
    BbOpDesc_t *pBod;           /*!< Expedited execute BOD element. */
    uint32_t toUsec;            /*!< Expedited BOD start time-out time. */
  } ex;                         /*!< Expedited BOD. */

#if (SCH_BG_BOD_ENABLE)
  struct
  {
    BbOpDesc_t *pBod;           /*!< Perpetual background BOD element. */
    uint32_t endUsec;           /*!< End time of last background BOD instance. */
    uint32_t maxDur;            /*!< Original maximum background BOD duration */
    bool busy;                  /*!< Indicate if background BOD is busy. */
    schState_t listState;       /*!< BOD list state at start of background BOD. */
  } bg;                         /*!< Background BOD. */
#endif
} SchCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchCtrlBlk_t schCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void schSetBodStartEvent(void);
void schSetBodAbortEvent(void);
void schRemoveHead(void);

/*************************************************************************************************/
/*!
 *  \brief      Is BOD due time in the future.
 *
 *  \param      pBod    Target BOD.
 *
 *  \return     true if BOD time is in the future, false otherwise.
 */
/*************************************************************************************************/
static inline bool schDueTimeInFuture(BbOpDesc_t *pBod)
{
  const uint32_t curTime = PalBbGetCurrentTime() + SCH_TIMER_DELAY_US;

  return (BbGetTargetTimeDelta(pBod->dueUsec, curTime) > 0);
}

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_H */
