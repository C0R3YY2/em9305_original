/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Asset Tracking profile server.
 *
 *  Copyright (c) 2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.  All rights reserved.
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
#ifndef ATPS_API_H
#define ATPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup ASSET_TRACKING_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Asset Tracking Profile server initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AtpsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the antenna identifiers for a connection ID.
 *
 *  \param  connId        Connection identifier.
 *  \param  numAntenna    Number of antenna and len of pAntennaIds in bytes.
 *  \param  pAntennaIds   Array containing identifiers of antenna for this connection.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AtpsSetAntennaIds(dmConnId_t connId, uint8_t numAntenna, uint8_t *pAntennaIds);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Asset Tracking Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AtpsProcDmMsg(dmEvt_t *pEvt);

/*! \} */    /* ASSET_TRACKING_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* ATPS_API_H */
