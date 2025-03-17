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
#include "pal_frc.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Timers ***/

/* Timer assignments. Reserve use of "0" for RTOS. */
#define PAL_RTC_FRC_LF_ID       2   /*!< Low frequency (always running) FRC timer ID. */
#define PAL_TMR_FRC_HF_ID       4   /*!< High frequency FRC timer ID. */
#define PAL_TMR_RADIO_ID        3   /*!< High precision radio timer ID (use TIMER3 to satisfy CC requirements). */

/* PAL_TMR_FRC_HF_ID timer capture compare assignments. */
#define PAL_FRC_HF_AUDIO_ID     0   /*!< Audio expiry compare channel. */
#define PAL_FRC_HF_READ_ID      1   /*!< Read current time compare channel. */
#define PAL_FRC_HF_SCH_ID       2   /*!< Scheduler expiry compare channel. */
#define PAL_FRC_HF_BB_ID        3   /*!< BB timer start compare channel. */
#define PAL_FRC_CC_I2S_ID       4   /*!< I2S TXPTRUPD event capture channel. */

/* PAL_TMR_RADIO_ID timer capture compare assignments. */
#define PAL_RADIO_TXEN_ID       0   /*!< TXEN task compare channel. */
#define PAL_RADIO_RXEN_ID       1   /*!< RXEN task compare channel. */
#define PAL_RADIO_TO_ID         2   /*!< Rx timeout offset compare channel. */
#define PAL_RADIO_TS_END_ID     0   /*!< END event offset capture channel. */
#define PAL_RADIO_TS_PHYEND_ID  1   /*!< PHYEND event offset capture channel. */
#define PAL_RADIO_TS_ADDR_ID    2   /*!< ADDRESS event offset capture channel. */

#define PAL_FRC_TMR_REG         NRFX_CONCAT_2(NRF_TIMER, PAL_TMR_FRC_HF_ID)
#define PAL_FRC_HF_BB_EVENT     NRFX_CONCAT_2(NRF_TIMER_EVENT_COMPARE, PAL_FRC_HF_BB_ID)

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

uint32_t palFrcGetI2sEventTime(void);
void palFrcSubscribeI2sEvent(uint8_t dppiChan);
bool palFrcSetBbTimerStartTrigger(uint32_t startTime);
void PalFrcHFTimerStop(void);
void PalFrcHFTimerRestart(void);
void PalFrcHFTimerAudioSet(uint32_t expTimeUsec, PalFrcCompCback_t expCback);

#endif /* PAL_INT_H */
