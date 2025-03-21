/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Proximity services implementation.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_PX_H
#define SVC_PX_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PROXIMITY_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Proximity Service Handles
 *
 */
/**@{*/
#define PX_START_HDL                      0x50             /*!< Start handle. */
#define PX_END_HDL                        (PX_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Service Handles */
enum
{
  LLS_SVC_HDL = PX_START_HDL,       /*!< Link loss service declaration */
  LLS_AL_CH_HDL,                    /*!< Alert level characteristic */
  LLS_AL_HDL,                       /*!< Alert level */
  IAS_SVC_HDL,                      /*!< Immediate alert service declaration */
  IAS_AL_CH_HDL,                    /*!< Alert level characteristic */
  IAS_AL_HDL,                       /*!< Alert level */
  TXS_SVC_HDL,                      /*!< TX power service declaration */
  TXS_TX_CH_HDL,                    /*!< TX power level characteristic */
  TXS_TX_HDL,                       /*!< TX power level */
  PX_MAX_HDL                        /*!< Maximum handle. */
};
/**@}*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcPxAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcPxRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcPxCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* PROXIMITY_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_PX_H */
