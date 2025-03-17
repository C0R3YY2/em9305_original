/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Hearing Access Profile Server (HAPS).
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

#ifndef HAPS_MAIN_H
#define HAPS_MAIN_H

#include "haps_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief HAP Server connection control block */
typedef struct
{
  uint16_t          handle;               /*!< Connection handle */
} hapsCcb_t;

/*! \brief HAP Server main control block */
typedef struct
{
  hapsCcb_t         connCb[DM_CONN_MAX];  /*!< Connection control block */
  hapsCback_t       cback;                /*!< Server application's callback */
} hapsCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern hapsCb_t hapsCb;

/**************************************************************************************************
  Globals
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

extern void hapsHaInit(void);
extern void hapsHaProcMsg(wsfMsgHdr_t *pEvt);

#ifdef __cplusplus
};
#endif

#endif /* HAPS_MAIN_H */
