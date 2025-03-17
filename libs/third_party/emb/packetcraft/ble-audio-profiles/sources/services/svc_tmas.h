/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Telephony and Media Audio service (TMAS) implementations.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_TMAS_H
#define SVC_TMAS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Handle Ranges
**************************************************************************************************/
/** \name TMAS Service Handles
 *
 */
/**@{*/
#define TMAS_START_HDL                0x0950                /*!< TMAS start handle */
#define TMAS_END_HDL                  (TMAS_MAX_HDL - 1)  /*!< TMAS end handle */
/**@}*/
#define TMAS_START_HDL                0x0950                /*!< TMAS start handle */

/*! Characteristic read permissions */
#ifndef TMAS_SEC_PERMIT_READ
#define TMAS_SEC_PERMIT_READ SVC_SEC_PERMIT_READ
#endif

#define TMAS_ROLE_CHAR_SIZE         (2)

#define TMAS_ROLE_BIT_CG            (0)
#define TMAS_ROLE_BIT_CT            (1)
#define TMAS_ROLE_BIT_UMS           (2)
#define TMAS_ROLE_BIT_UMR           (3)
#define TMAS_ROLE_BIT_BMS           (4)
#define TMAS_ROLE_BIT_BMR           (5)


/**************************************************************************************************
  Handles
**************************************************************************************************/

/** \name TMAS Service Handles
 *
 */
/**@{*/
/*! \brief TMAS service handle */
enum
{
  TMAS_SVC_HDL = TMAS_START_HDL,    /*!< \brief ASE service declaration */
  TMAS_ROLE_CH_HDL,                 /*!< \brief Sink ASE #1 characteristic */
  TMAS_ROLE_VAL_HDL,                /*!< \brief Sink ASE #1 value */
  TMAS_MAX_HDL                      /*!< \brief ASES maximum handle */
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
void SvcTmasAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTmasRemoveGroup(void);

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
void SvcTmasCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* TMAS_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* #define SVC_TMAS_H */
