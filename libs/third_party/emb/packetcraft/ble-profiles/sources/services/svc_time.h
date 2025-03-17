/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Time-related service implementation.
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

#ifndef SVC_TIME_H
#define SVC_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup TIME-RELATED_SERVICE
 *  \{ */


/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Time Service Handles
 *
 */
/**@{*/
#define TIME_START_HDL               0xC0               /*!< Start handle. */
#define TIME_END_HDL                 (TIME_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Time-Related Service Handles */
enum
{
  TIME_CTS_SVC_HDL = TIME_START_HDL,    /*!< Current time service declaration */
  TIME_CTS_CT_CH_HDL,                   /*!< CT time characteristic */
  TIME_CTS_CT_HDL,                      /*!< CT time */
  TIME_CTS_CT_CH_CCC_HDL,               /*!< CT time client characteristic configuration */
  TIME_CTS_LOC_CH_HDL,                  /*!< Local time information characteristic */
  TIME_CTS_LOC_HDL,                     /*!< Local time information */
  TIME_CTS_REF_CH_HDL,                  /*!< Reference time information characteristic */
  TIME_CTS_REF_HDL,                     /*!< Reference time information */

  TIME_DST_SVC_HDL,                     /*!< DST change service declaration */
  TIME_DST_WDST_CH_HDL,                 /*!< Time with DST characteristic */
  TIME_DST_WDST_HDL,                    /*!< Time with DST */

  TIME_RTU_SVC_HDL,                     /*!< Reference time update service declaration */
  TIME_RTU_CP_CH_HDL,                   /*!< Time update control point characteristic */
  TIME_RTU_CP_HDL,                      /*!< Time update control point */
  TIME_RTU_STATE_CH_HDL,                /*!< Time update state characteristic */
  TIME_RTU_STATE_HDL,                   /*!< Time update state */

  TIME_MAX_HDL                          /*!< Maximum handle. */
};
/**@}*/

/*! \brief Indexes of CCC descriptor handle table entries */
enum
{
  TIME_CTS_CT_CH_CCC_IDX /*!< CT time CCCD index. */
};

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
void SvcTimeAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTimeRemoveGroup(void);

/*! \} */    /* TIME-RELATED_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_TIME_H */
