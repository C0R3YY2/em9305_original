/*************************************************************************************************/
/*!
 *  \file ots_api.h
 *
 *  \brief  Object Transfer Service application interface.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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
#ifndef OTS_API_H
#define OTS_API_H

#include "dm_api.h"
#include "wsf_types.h"
#include "wsf_os.h"

#ifdef __cplusplus
extern "C" {
#endif
/*! \addtogroup OBJECT_TRANSFER_PROFILE
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief  Set the features attribute.
 *
 *  \param  features   Features byte array.
 */
/*************************************************************************************************/
uint8_t OtpsSetFeatures(uint64_t features);

/*************************************************************************************************/
/*!
 *  \brief  Initialization for OTPS task.
 */
/*************************************************************************************************/
void OtpsHandlerInit(wsfHandlerId_t otpsHandlerId);

/*! \} */    /* OBJECT_TRANSFER_PROFILE */

#ifdef __cplusplus
}
#endif

#endif /* OTPS_API_H */
