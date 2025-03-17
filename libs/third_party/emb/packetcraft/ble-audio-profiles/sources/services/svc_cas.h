/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Common Audio Service implementation.
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


#ifndef SVC_CAS_H
#define SVC_CAS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup COMMON_AUDIO_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*!< \brief Start handle. */
#define CAS_START_HDL                 0x08E0

/*!< \brief Number of AICS handles. */
#define CAS_NUM_HDL                   (CAS_END_HDL - CAS_START_HDL)

/*! \brief Enumeration of handle indexes of characteristics to be discovered */
enum
{
  CAS_SVC_HDL = CAS_START_HDL,        /*!< \brief Common Audio Service declaration */
  CAS_INC_HDL,                        /*!< \brief Included Service declaration */
  CAS_END_HDL                         /*!< \brief Handle list length */
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
void SvcCasAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCasRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Add an included service to the CAS service.
 *
 *  \param  startHdl  Start handle of the included service.
 *  \param  endHdl    End handle of the included service.
 *  \param  svcUuid   UUID of the included service.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCasAddIncSvc(uint16_t startHdl, uint16_t endHdl, uint16_t svcUuid);

/*! \} */    /* COMMON_AUDIO_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_CAS_H */
