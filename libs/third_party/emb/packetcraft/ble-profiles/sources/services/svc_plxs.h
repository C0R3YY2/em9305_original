/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Pulse Oximiter Service Server implementation.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_PLXS_H
#define SVC_PLXS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PULSE_OXIMITER_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Pulse Oximiter Service Handles
 *
 */
/**@{*/
#define PLXS_START_HDL               0x04A0             /*!< Start handle. */
#define PLXS_END_HDL                 (PLXS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Pulse Oximiter Service Handles */
enum
{
  PLXS_SVC_HDL = PLXS_START_HDL,       /*!< Pulse Oximiter Server Service declaration */
  PLXS_FEATURES_CH_HDL,                /*!< Pulse Oximiter Features characteristic */
  PLXS_FEATURES_HDL,                   /*!< Pulse Oximiter Features */
  PLXS_SPOT_CHECK_CH_HDL,              /*!< Pulse Oximiter Spot Check Measurement characteristic */
  PLXS_SPOT_CHECK_HDL,                 /*!< Pulse Oximiter Spot Check Measurement */
  PLXS_SPOT_CHECK_CH_CCC_HDL,          /*!< Pulse Oximiter Spot Check Measurement Client Characteristic Configuration Descriptor */
  PLXS_CONTINUOUS_CH_HDL,              /*!< Pulse Oximiter Continuous Measurement characteristic */
  PLXS_CONTINUOUS_HDL,                 /*!< Pulse Oximiter Continuous Measurement */
  PLXS_CONTINUOUS_CH_CCC_HDL,          /*!< Pulse Oximiter Continuous Measurement Client Characteristic Configuration Descriptor */
  PLXS_RECORD_ACCESS_CH_HDL,           /*!< Pulse Oximiter Record Access Control Point characteristic */
  PLXS_RECORD_ACCESS_HDL,              /*!< Pulse Oximiter Record Access Control Point  */
  PLXS_RECORD_ACCESS_CH_CCC_HDL,       /*!< Pulse Oximiter Record Access Control Point Client Characteristic Configuration Descriptor */
  PLXS_MAX_HDL                         /*!< Maximum handle. */
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
void SvcPlxsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcPlxsRemoveGroup(void);

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
void SvcPlxsCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* PULSE_OXIMITER_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_PLXS_H */
