/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      RTC timer interface file.
 *
 *  Copyright (c) 2018 Arm Ltd. All Rights Reserved.
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

#ifndef PAL_RTC_H
#define PAL_RTC_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_RTC
 *  \{ */

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief      Operational states. */
enum
{
  PAL_RTC_STATE_UNINIT = 0,     /*!< Uninitialized state. */
  PAL_RTC_STATE_ERROR  = 0,     /*!< Error state. */
  PAL_RTC_STATE_READY  = 1      /*!< Ready state. */
};
typedef uint8_t PalRtcState_t;

/*! \brief  Platform RTC callback. */
typedef void (*palRtcIrqCback_t)(void);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalRtcInit(void);   /*!< TBD */

/* Control and Status */

/*************************************************************************************************/
/*!
 *  \brief      TBD
 *
 *  \param      channelId     TBD
 */
/*************************************************************************************************/
void PalRtcEnableCompareIrq(uint8_t channelId);

/*************************************************************************************************/
/*!
 *  \brief      TBD
 *
 *  \param      channelId     TBD
 */
/*************************************************************************************************/
void PalRtcDisableCompareIrq(uint8_t channelId);

/*************************************************************************************************/
/*!
 *  \brief      TBD
 *
 *  \return     TBD
 */
/*************************************************************************************************/
uint32_t PalRtcCounterGet(void);

/*************************************************************************************************/
/*!
 *  \brief      TBD
 *
 *  \param      channelId     TBD
 *  \param      value         TBD
 */
/*************************************************************************************************/
void PalRtcCompareSet(uint8_t channelId, uint32_t value);

/*! \} */    /* PAL_RTC */

#ifdef __cplusplus
};
#endif

#endif
