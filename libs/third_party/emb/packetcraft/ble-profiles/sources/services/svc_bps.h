/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Blood Pressure service implementation.
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

#ifndef SVC_BPS_H
#define SVC_BPS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BLOOD_PRESSURE_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Blood Pressure Service Handles
 *
 */
/**@{*/
#define BPS_START_HDL               0xE0              /*!< Start handle. */
#define BPS_END_HDL                 (BPS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Blood Pressure Service Handles */
enum
{
  BPS_SVC_HDL = BPS_START_HDL,      /*!< Blood pressure service declaration */
  BPS_BPM_CH_HDL,                   /*!< Blood pressure measurement characteristic */
  BPS_BPM_HDL,                      /*!< Blood pressure measurement */
  BPS_BPM_CH_CCC_HDL,               /*!< Blood pressure measurement client characteristic configuration */
  BPS_ICP_CH_HDL,                   /*!< Intermediate cuff pressure characteristic */
  BPS_ICP_HDL,                      /*!< Intermediate cuff pressure */
  BPS_ICP_CH_CCC_HDL,               /*!< Intermediate cuff pressure client characteristic configuration */
  BPS_BPF_CH_HDL,                   /*!< Blood pressure feature characteristic */
  BPS_BPF_HDL,                      /*!< Blood pressure feature */
  BPS_MAX_HDL                       /*!< Maximum handle. */
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
void SvcBpsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcBpsRemoveGroup(void);

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
void SvcBpsCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*************************************************************************************************/
/*!
 *  \brief  Toggle flag values.
 *
 *  \param  flag  Flag to manipulate.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcBpsToggleFeatureFlags(uint8_t flag);

/*! \} */    /* BLOOD_PRESSURE_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_BPS_H */
