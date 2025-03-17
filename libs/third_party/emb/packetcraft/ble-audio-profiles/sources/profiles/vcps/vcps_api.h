/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Volume Control Profile server.
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
#ifndef VCPS_API_H
#define VCPS_API_H

#include "att_api.h"
#include "aicps_api.h"
#include "vocps_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup VOLUME_CONTROL_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* VCPS volume change callback */
typedef void (*vcpsCback_t)(bool mute, uint8_t volume);

/*! VCPS configuration */
typedef struct
{
  uint8_t cccIdxOffset;       /* Starting index of CCC descriptors used by the VCPS */
  vcpsCback_t vcsCallback;    /* Callback for VCPS volume change event */
  uint8_t numAics;            /* Number of include audio input ctrl svcs (AICS) used by the VCPS */
  uint8_t aicsIdOffset;       /* Starting id of include AICS used by the VCPS */
  uint8_t aicsCccIdxOffset;   /* Starting index of CCC descriptors used by include AICS */
  aicpsCback_t aicpsCallback; /* Callback for AICPS gain change event */
  uint8_t numVocs;            /* Number of include volume offset ctrl svcs (VOCS) used by the VCPS */
  uint8_t vocsIdOffset;       /* Starting id of include VOCS used by the VCPS */
  uint8_t vocsCccIdxOffset;   /* Starting index of CCC descriptors used by include VOCS */
  vocpsCback_t vocpsCallback; /* Callback for VOCPS volume offset change event */
} vcpsCfg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief  Set the volume attribute.
 *
 *  \param volume    Volume setting.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpsSetVolume(uint8_t volume);

/*************************************************************************************************/
/*!
 *  \brief Set the relative volume.
 *
 *  \param volume    Volume setting.
 *  \param mute      Mute setting.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void VcpsSetRelativeVolume(uint8_t volume, bool mute);

/*************************************************************************************************/
/*!
 *  \brief Set the relative volume.
 *
 *  \param volume    Volume setting.
 *  \param mute      Mute setting.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void VcpsSetRelativeVolume(uint8_t volume, bool mute);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Volume Control Profile server of DM Events.
 *
 *  \param mute      true to mute, false to unmute.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpsSetMute(bool mute);

/*************************************************************************************************/
/*!
 *  \brief  Volume Control Profile server initialization.
 *
 *  \param  pCfg   Pointer to application configuration - memory must persist.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpsInit(const vcpsCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Volume Control Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpsProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Set the flags attribute.
 *
 *  \param flags    Volume setting.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void VcpsSetFlags(uint8_t flags);

/*! \} */    /* VOLUME_CONTROL_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* VCPS_API_H */
