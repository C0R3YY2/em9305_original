/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      FRC interface file.
 *
 *  Copyright (c) 2019-2022 Packetcraft, Inc.  All rights reserved.
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

#ifndef PAL_FRC_H
#define PAL_FRC_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_FRC
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Completion callback. */
typedef void (*PalFrcCompCback_t)(void);

/*! \brief      Timer states. */
enum
{
  PAL_TIMER_STATE_UNINIT = 0,
  PAL_TIMER_STATE_ERROR = 0,
  PAL_TIMER_STATE_READY,
  PAL_TIMER_STATE_BUSY
};
typedef uint8_t PalTimerState_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */

/*************************************************************************************************/
/*!
 *  \brief      FRC initialization.
 */
/*************************************************************************************************/
void PalFrcInit(void);

/*************************************************************************************************/
/*!
 *  \brief      FRC de-initialization.
 */
/*************************************************************************************************/
void PalFrcDeInit(void);

/* Control and Status */

/*************************************************************************************************/
/*!
 *  \brief      Convert time in milliseconds to FRC ticks.
 *
 *  \param      timeMs     Time in milliseconds.
 *
 *  \return     Conversion from \a deltaMs to FRC ticks.
 */
/*************************************************************************************************/
uint32_t PalFrcMsToTicks(uint32_t timeMs);

/*************************************************************************************************/
/*!
 *  \brief      Compute delta time in milliseconds.
 *
 *  \param      endTime     FRC end time.
 *  \param      startTime   FRC start time.
 *
 *  \return     Delta time in milliseconds.
 */
/*************************************************************************************************/
uint32_t PalFrcDeltaMs(uint32_t endTime, uint32_t startTime);

/*************************************************************************************************/
/*!
 *  \brief      Compute delta time in microseconds.
 *
 *  \param      endTime     FRC end time.
 *  \param      startTime   FRC start time.
 *
 *  \return     Delta time in microseconds.
 */
/*************************************************************************************************/
uint32_t PalFrcDeltaUs(uint32_t endTime, uint32_t startTime);

/*************************************************************************************************/
/*! \brief      Set wakeup timer.
 *
 *  \param      expTimeUsec     Absolute BB expire time in microseconds.
 *  \param      expCback        TBD
 */
/*************************************************************************************************/
void PalFrcWakeupSet(uint32_t expTimeUsec, PalFrcCompCback_t expCback);

/*************************************************************************************************/
/*!
 *  \brief      Start the scheduler timer.
 *
 *  \param      expTimeUsec      Absolute BB expire time in microseconds.
 *  \param      expCback         TBD
 */
/*************************************************************************************************/
void PalFrcHFTimerSet(uint32_t expTimeUsec, PalFrcCompCback_t expCback);

/*************************************************************************************************/
/*!
 *  \brief      Stop the scheduler timer.
 */
/*************************************************************************************************/
void PalFrcHFTimerClear(void);

/*************************************************************************************************/
/*!
 *  \brief      Get time to the next timer expiration.
 *
 *  \return     Time to next expiration in microseconds; 0 if no timer is set..
 */
/*************************************************************************************************/
uint32_t PalFrcHFTimerNextExpiration(void);

/*************************************************************************************************/
/*!
 *  \brief      Get current time.
 *
 *  \return     FRC time.
 *
 *  \note       Caution must be taken when performing calculations with FRC time. FRC value bit width
 *              and resolution may vary.
 */
/*************************************************************************************************/
uint32_t PalFrcHFTimerGetCurrentTime(void);
/*! \} */    /* PAL_FRC */

#ifdef __cplusplus
};
#endif

#endif
