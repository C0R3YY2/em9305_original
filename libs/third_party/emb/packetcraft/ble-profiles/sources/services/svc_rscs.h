/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Running Speed and Cadence Service Server implementation.
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

#ifndef SVC_RSCS_H
#define SVC_RSCS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup RUNNING_SPEED_AND_CADENCE_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name RSC Feature Bits
 * Running Speed and Cadence Feature Bits of the Feature Characteristic
 */
/**@{*/
#define RSCS_ISLMS_FEATURE_BIT           (1<<0)  /*!< Instantaneous Stride Length Measurement Supported Feature Mask Bit */
#define RSCS_TDMS_FEATURE_BIT            (1<<1)  /*!< Total Distance Measurement Supported Feature Mask Bit */
#define RSCS_WRSS_FEATURE_BIT            (1<<2)  /*!< Walking or Running Status Supported Feature Mask Bit */
#define RSCS_CPS_FEATURE_BIT             (1<<3)  /*!< Calibration Procedure Supported Feature Mask Bit */
#define RSCS_MSLS_FEATURE_BIT            (1<<4)  /*!< Multiple Sensor Locations Supporte Feature Mask Bit */

/*! \brief TODO: Set to all supported features */
#define RSCS_ALL_FEATURES                (0x7)   /*!< All Supported Feature Mask */
/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Running Speed and Cadence Service Handles
 *
 */
/**@{*/
#define RSCS_START_HDL               0x04A0             /*!< Start handle. */
#define RSCS_END_HDL                 (RSCS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Running Speed Service Handles */
enum
{
  RSCS_SVC_HDL = RSCS_START_HDL,       /*!< Running Speed Server Service declaration */
  RSCS_RSF_CH_HDL,                     /*!< Running Speed Feature characteristic */
  RSCS_RSF_HDL,                        /*!< Running Speed Feature */
  RSCS_RSM_CH_HDL,                     /*!< Running Speed Measurement characteristic */
  RSCS_RSM_HDL,                        /*!< Running Speed Measurement */
  RSCS_RSM_CH_CCC_HDL,                 /*!< Running Speed Measurement Client Characteristic Configuration Descriptor */
  RSCS_SL_CH_HDL,                      /*!< Running Speed Sensor Location characteristic */
  RSCS_SL_HDL,                         /*!< Running Speed Sensor Location */
  RSCS_MAX_HDL                         /*!< Maximum handle. */
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
void SvcRscsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcRscsRemoveGroup(void);

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
void SvcRscsCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* RUNNING_SPEED_AND_CADENCE_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_RSCS_H */
