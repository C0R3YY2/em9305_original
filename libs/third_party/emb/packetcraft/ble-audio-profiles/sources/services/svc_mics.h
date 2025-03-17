/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Microphone Control Service implementation.
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

#ifndef SVC_MICS_H
#define SVC_MICS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MICROPHONE_CONTROL_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Microphone Service attribute value lengths */
#define MICS_MUTE_LEN                1

/* Microphone Service mute values */
#define MICS_MUTE_NOT_MUTED          0
#define MICS_MUTE_MUTED              1
#define MICS_MUTE_DISABLED           2

/* CCCD Index Offsets */
#define MICS_CCCD_IDX_MUTE           0

/* Number of CCCD attributes */
#define MICS_NUM_CCCD                1

/**@}*/

/** \name ATT MICS Application Error Codes
* The ATT Application error codes defined by MIC service.
*/
/**@{*/
#define ATT_ERR_MICS_MUTE_DISABLE   0x80      /*!< Mute disable */
/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Microphone Control Service Handles
 *
 */
/**@{*/
#define MICS_START_HDL                0x0500             /*!< Start handle. */
#define MICS_END_HDL                  (MICS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Microphone Control Service Handles */
enum
{
  MICS_SVC_HDL = MICS_START_HDL,     /*!< Microphone Control Service declaration */
  MICS_MUTE_CH_HDL,                  /*!< Microphone Control mute characteristic */
  MICS_MUTE_HDL,                     /*!< Microphone Control mute value */
  MICS_MUTE_CH_CCC_HDL,              /*!< Microphone Control mute Client Characteristic Configuration Descriptor */
  MICS_AICS_INC_HDL_0,               /*!< Microphone Control AICS include characteristic */
  MICS_AICS_INC_HDL_1,               /*!< Microphone Control AICS include characteristic */
  MICS_MAX_HDL                       /*!< Maximum handle. */
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
void SvcMicsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcMicsRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcMicsCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* MICROPHONE_CONTROL_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_MICS_H */
