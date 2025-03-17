/*************************************************************************************************/
/*!
 *  \file   wsf_qpc.h
 *
 *  \brief  Software foundation OS API.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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
#ifndef WSF_QPC_H
#define WSF_QPC_H

#include "wsf_types.h"

#include "bsp.h"
#include "qf.h"
#include "qf_pkg.h"
#include "qassert.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_QPC
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Common type for all tasks. */
typedef struct
{
    QActive super;
    QTimeEvt timeEvt; /* private time event generator */
} QL_Task_t;

/**************************************************************************************************
  External Variables
**************************************************************************************************/

/* WSF Dispatcher QPC task handle */
extern QL_Task_t WsfTask;

/*! \} */    /* WSF_QPC */

#ifdef __cplusplus
};
#endif

#endif /* WSF_QPC_H */
