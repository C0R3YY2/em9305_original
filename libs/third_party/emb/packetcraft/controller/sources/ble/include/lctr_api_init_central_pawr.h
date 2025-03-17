/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller PAwR extended initiating central interface file.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef LCTR_API_INIT_CENTRAL_PAWR_H
#define LCTR_API_INIT_CENTRAL_PAWR_H

#include "lctr_api.h"
#include "wsf_assert.h"
#include "lctr_api_init_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_INIT_CEN_PAWR
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master extended initiate task messages for \a LCTR_DISP_EXT_INIT_PAWR dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_EXT_INIT_MSG_RESET_PAWR                   = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Initiate events */
  LCTR_EXT_INIT_MSG_INITIATE_PAWR,               /*!< Initiate API event. */
  LCTR_EXT_INIT_MSG_INITIATE_CANCEL_PAWR,        /*!< Initiate cancel API event. */
  LCTR_EXT_INIT_MSG_TERMINATE_PAWR,              /*!< Initiate scan BOD terminated event. */
  LCTR_EXT_INIT_MSG_PAWR_TOTAL                   /*!< Total number of initiate events. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */

/* Control */

/* Utility */

/*! \} */    /* LL_LCTR_API_INIT_CEN_PAWR */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_INIT_CENTRAL_PAWR_H */
