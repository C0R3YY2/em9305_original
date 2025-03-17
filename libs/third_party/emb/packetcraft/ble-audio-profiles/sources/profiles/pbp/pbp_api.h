/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Public Broadcast Profile Application Interface.
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

#ifndef PBP_API_H
#define PBP_API_H

#include "att_api.h"
#include "cap_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!< \addtogroup PUBLIC_BROADCAST_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define PBP_ANN_FEAT_IDX                  4       /*!< Public broadcast announcement feature index */

/*!< PBP Procedures */
#define PBP_PROC_NONE                     0x00    /*!< CAP procedure none */

/*!< PBP Source Procedures */
#define PBP_PROC_BCAST_AUDIO_START        0x01    /*!< PBP broadcast audio start procedure */
#define PBP_PROC_BCAST_AUDIO_UPDATE       0x02    /*!< PBP broadcast audio update procedure */
#define PBP_PROC_BCAST_AUDIO_STOP_DIS     0x03    /*!< PBP broadcast audio stop (disable) procedure */
#define PBP_PROC_BCAST_AUDIO_STOP_REL     0x04    /*!< PBP broadcast audio stop (release) procedure */

/*!< PBP Broadcast Assist Procedures */
#define PBP_PROC_BCAST_AUDIO_RX_START     0x05    /*!< PBP broadcast audio reception start procedure */
#define PBP_PROC_BCAST_AUDIO_RX_STOP      0x06    /*!< PBP broadcast audio reception stop procedure */
#define PBP_PROC_DISTRIB_BCAST_CODE       0x07    /*!< PBP Distribute Broadcast_Code procedure */

#define PBP_ANN_FEATURE_ENCRYPTION        (1<<0)
#define PBP_ANN_FEATURE_STANDART_QUAL     (1<<1)
#define PBP_ANN_FEATURE_HIGH_QUAL         (1<<2)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the PBP Source profile role.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void PbpScInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the PBP BA profile role.
 *
 *  \param  brsCback  Application BRS callback.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void PbpBaInit(BapbaBrsCback_t brsCback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the PBP Sink profile role.
 *
 *  \param  skCback   Application's BAP Sink BAS callback.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void PbpSkInit(BapskBasCback_t skCback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the PBP Scan Delegator subsystem.
 *
 *  \param  sdCback   Application's BAP Scan Delegator BAS callback.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void PbpSdInit(BapsdBasCback_t sdCback);

/*! \} */    /* PUBLIC_BROADCAST_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* PBP_API_H */
