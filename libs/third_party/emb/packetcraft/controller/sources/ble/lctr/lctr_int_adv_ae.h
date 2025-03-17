
/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Extended advertising common header file
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_INT_ADV_AE_H
#define LCTR_INT_ADV_AE_H

#include "ll_defs.h"
#include "lctr_api.h"
#include "lctr_api_adv_acad.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief      ACAD parameter indices */
enum
{
  LCTR_ACAD_ID_CHAN_MAP_UPDATE,
  LCTR_ACAD_ID_BIG_INFO,
  LCTR_ACAD_ID_PER_ADV_RSP_TIMING_INFO,

  /* Total number of ACAD IDs. */
  LCTR_ACAD_NUM_ID,

  /* Invalid ID type. */
  LCTR_ACAD_INVALID_ID = 0xFF
};

/*! \brief      ACAD states */
enum
{
  LCTR_ACAD_STATE_DISABLED = 0,
  LCTR_ACAD_STATE_ENABLED,
  LCTR_ACAD_STATE_TOTAL
};

/*! \brief      ACAD Common events */
enum
{
  LCTR_ACAD_COMM_MSG_START,
  LCTR_ACAD_COMM_MSG_FINISH,
  LCTR_ACAD_COMM_MSG_TOTAL,
  LCTR_ACAD_COMM_MSG_INVALID = 0xFF
};

/*! \brief      Generic ACAD packet. */
typedef union
{
  LctrAcadHdr_t        hdr;                          /*!< ACAD header. */
  LctrAcadChanMapUpd_t chanMapUpdate;                /*!< Channel map update. */
  LctrAcadBigInfo_t    bigInfo;                      /*!< BIG info. */
  LctrAcadPerAdvRspTimingInfo_t perAdvRspTimingInfo; /*!< Periodic advertising response timing info. */
} lctrAcadParam_t;

/*! \brief      Extended advertising report assembly state. */
enum
{
  LCTR_RPT_STATE_IDLE,                  /*!< No report assembly in progress. */
  LCTR_RPT_STATE_IN_PROGRESS,           /*!< Report assembly in progress. */
  LCTR_RPT_STATE_COMP,                  /*!< Report assembly completed. */
};
typedef uint8_t lctrRptState_t;

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_ADV_AE_H */
