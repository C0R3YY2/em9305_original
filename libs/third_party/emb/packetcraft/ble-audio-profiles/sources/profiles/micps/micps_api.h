/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Microphone Control Profile server.
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
#ifndef MICPS_API_H
#define MICPS_API_H

#include "att_api.h"
#include "aicps_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MICROPHONE_CONTROL_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* MICPS mute change callback */
typedef void (*micpsCback_t)(bool mute);

/*! MICPS configuration */
typedef struct
{
  uint8_t cccIdxOffset;       /* Starting index of CCC descriptors used by the MICPS */
  micpsCback_t micsCallback;  /* Callback for MICPS events */
  uint8_t numAics;            /* Number of include audio input ctrl svcs (AICS) used by the MICPS */
  uint8_t aicsIdOffset;       /* Starting id of include AICS used by the MICPS */
  uint8_t aicsCccIdxOffset;   /* Starting index of CCC descriptors used by include AICS */
  aicpsCback_t aicpsCallback; /* Callback for AICPS gain change event */
} micpsCfg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Set the mute state attribute value.
 *
 *  \param mute      Mute state (MICS_MUTE_NOT_MUTED, MICS_MUTE_MUTED, or MICS_MUTE_DISABLED)
 *
 *  \return none.
 */
/*************************************************************************************************/
void MicpsSetMute(uint8_t mute);

/*************************************************************************************************/
/*!
 *  \brief  Get the mute state attribute value.
 *
 *  \return MICS_MUTE_NOT_MUTED, MICS_MUTE_MUTED, or MICS_MUTE_DISABLED.
 */
/*************************************************************************************************/
uint8_t MicpsGetMuteState(void);

/*************************************************************************************************/
/*!
 *  \brief  Microphone Control Profile server initialization.
 *
 *  \param  pCfg   Pointer to application configuration - memory must persist.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MicpsInit(const micpsCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Microphone Control Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MicpsProcDmMsg(dmEvt_t *pEvt);

/*! \} */    /* MICROPHONE_CONTROL_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* MICPS_API_H */
