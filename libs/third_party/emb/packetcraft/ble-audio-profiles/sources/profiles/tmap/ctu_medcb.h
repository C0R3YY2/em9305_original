/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  TMAP Unicast Sink Interface.
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

#ifndef CTU_MEDCB_H
#define CTU_MEDCB_H

#include "att_api.h"
#include "bap_defs.h"
#include "baps/baps_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define tmapAseCb_t bapsAseCb_t
#define TmapAseCback_t BapsAseCback_t

enum
{
  TMAP_IDLE,                           /*!< Idle state */
  TMAP_CODEC_CFG,                      /*!< Codec Configured state */
  TMAP_QOS_CFG,                        /*!< QoS Configured state */
  TMAP_ENABLING,                       /*!< Enabling state */
  TMAP_STREAMING,                      /*!< Streaming state */
  TMAP_DISABLING,                      /*!< Disabling state */
  TMAP_RELEASING,                      /*!< Releasing state */

  TMAP_NUM_STATES
};

/*************************************************************************************************/
/*!
 *  \brief  Register Media callback function
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMediaEventRegister(TmapAseCback_t cb);

#ifdef __cplusplus
};
#endif

#endif /* TMAPCTUS_API_H */
