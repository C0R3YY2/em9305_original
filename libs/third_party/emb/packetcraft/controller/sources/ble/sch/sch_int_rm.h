/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Internal scheduler resource manager interface file.
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

#ifndef SCH_INT_RM_H
#define SCH_INT_RM_H

#include "sch_api.h"
#include "sch_api_ble.h"
#include "ll_math.h"
#include "cfg_mac_ble.h"
#include "pal_bb.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Divide by minimum periodicity. */
#define SCH_RM_DIV_SLOT_SIZE(n)     (n / SCH_RM_MIN_SLOT_SIZE)

/*! \brief      Total number of slots. */
#define SCH_RM_SLOTS                32

/*! \brief      Divide by minimum periodicity. */
#define SCH_RM_ECOSYSTEM_INTERVAL_TO_US(n)  (n * 1250)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Reservation descriptor. */
typedef struct
{
  uint8_t handle;           /*!< Reservation handle. */
  bool cmdInterUsed;      /*!< Reservation is controlled by common interval. */
  uint32_t interUsec;       /*!< Interval in microseconds. */
  uint32_t durUsec;         /*!< Duration in microseconds. */
} schRmRsvn_t;

/*! \brief      Scheduler resource manager control block. */
typedef struct
{
  uint8_t numRsvn;                              /*!< Number of reservations. */
  uint32_t slotSize;                            /*!< Size of reservation slots. */
  uint32_t cmnInter;                            /*!< Common interval. */
  bool setEcosystem;                          /*!< Base interval has been set by host. */
  uint32_t rsvnInterUsec[SCH_RM_MAX_RSVN];      /*!< Reserved intervals indexed by handle. */
  schRmRsvn_t rsvn[SCH_RM_MAX_RSVN];            /*!< Reservation info for each handle. */
} SchRmCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern SchRmCb_t schRmCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void schRmSortListDescending(uint32_t item[], uint8_t numItems);

#ifdef __cplusplus
};
#endif

#endif /* SCH_INT_RM_H */
