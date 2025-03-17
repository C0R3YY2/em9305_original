/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Health Thermometer service implementation.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_HTS_H
#define SVC_HTS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup HEALTH_THERMOMETER_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Health Thermometer Service Handles
 *
 */
/**@{*/
#define HTS_START_HDL               0x0120            /*!< Start handle. */
#define HTS_END_HDL                 (HTS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Health Thermometer Service Handles */
enum
{
  HTS_SVC_HDL = HTS_START_HDL,      /*!< Health thermometer service declaration */
  HTS_TM_CH_HDL,                    /*!< Temperature measurement characteristic */
  HTS_TM_HDL,                       /*!< Temperature measurement */
  HTS_TM_CH_CCC_HDL,                /*!< Temperature measurement client characteristic configuration */
  HTS_IT_CH_HDL,                    /*!< Intermediate temperature characteristic */
  HTS_IT_HDL,                       /*!< Intermediate temperature */
  HTS_IT_CH_CCC_HDL,                /*!< Intermediate temperature client characteristic configuration */
  HTS_TT_CH_HDL,                    /*!< Temperature type characteristic */
  HTS_TT_HDL,                       /*!< Temperature type */
  HTS_MAX_HDL                       /*!< Maximum handle. */
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
void SvcHtsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcHtsRemoveGroup(void);

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
void SvcHtsCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* HEALTH_THERMOMETER_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_HTS_H */
