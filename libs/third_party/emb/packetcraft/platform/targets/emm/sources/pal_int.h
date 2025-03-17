/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      PAL internal definitions.
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

#ifndef PAL_INT_H
#define PAL_INT_H

#include "pal_types.h"
#include "pal_board.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Timers ***/

/* FRC timer capture compare assignments. */
#define PAL_FRC_CC_SCH_ID       0   /*!< Scheduler expiry compare channel. */
#define PAL_FRC_CC_BB_ID        1   /*!< BB timer start compare channel. */
#define PAL_FRC_CC_WAKE_ID      3   /*!< Wakeup compare channel. */

/* High-precision BB timer capture compare assignments.
 * Only channels 0-3 can be used for HW capture events. */
#define PAL_RADIO_TS_TXEND_ID   0   /*!< END event offset capture channel. */
#define PAL_RADIO_TS_RXEND_ID   1   /*!< PHYEND event offset capture channel. */
#define PAL_RADIO_TS_ADDR_ID    3   /*!< ADDRESS event offset capture channel. */

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

bool palFrcSetBbTimerStartTrigger(uint32_t startTime);

#endif /* PAL_INT_H */
