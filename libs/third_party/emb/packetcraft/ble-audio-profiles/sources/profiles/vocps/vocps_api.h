/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Volume Offset Control Profile server.
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
#ifndef VOCPS_API_H
#define VOCPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup VOLUME_OFFSET_CONTROL_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Bitfields of changed attributes */
#define VOCPS_CH_STATE_BIT          (1 << 0)
#define VOCPS_CH_LOCATION_BIT       (1 << 1)

#define VOCPS_CH_MAX_BIT            2

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* VOCPS volume offset change callback */
typedef void (*vocpsCback_t)(uint8_t svcId, uint16_t volumeOffset);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief  Send volume offset state notification.
 *
 *  \param connId    DM connection ID.
 *  \param svcId     VOCS service index.
 *  \param priority  EATT priority.
 *
 *  \return none.
 */
/*************************************************************************************************/
void VocpsSendStateNtf(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Send volume offset location notification.
 *
 *  \param connId    DM connection ID.
 *  \param svcId     VOCS service index.
 *  \param priority  EATT priority.
 *
 *  \return none.
 */
/*************************************************************************************************/
void VocpsSendLocationNtf(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Set the volume offset state.
 *
 *  \param svcId     VOCS service index.
 *  \param state     Volume offset
 *
 *  \return none.
 */
/*************************************************************************************************/
void VocpsSetOffsetState(uint8_t svcId, uint16_t state);

/*************************************************************************************************/
/*!
 *  \brief  Set gain setting attribute values.
 *
 *  \param svcId     VOCS service index.
 *  \param mask      Audio location bitmask
 *
 *  \return none.
 */
/*************************************************************************************************/
void VocpsSetAudioLocation(uint8_t svcId, uint8_t mask);

/*************************************************************************************************/
/*!
 *  \brief  Set the audio output description value.
 *
 *  \param svcId     VOCS service index.
 *  \param desc      Output description
 *
 *  \return none.
 */
/*************************************************************************************************/
void VocpsSetDescription(uint8_t svcId, const char *desc);

/*************************************************************************************************/
/*!
 *  \brief  Get the volume offset for the volume offset source
 *
 *  \param svcId     VOCS service index.
 *
 *  \return          Volume offset.
 */
/*************************************************************************************************/
uint16_t VocpsGetVolumeOffset(uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Register CCC indicies and event callback function for VOCS services.
 *
 *  \param svcId     AICS service index.
 *  \param cccStart  First CCC index.
 *  \param cback     Callback for event notification.
 *
 *  \return none.
 */
/*************************************************************************************************/
void VocpsRegister(uint8_t svcId, uint8_t cccStart, vocpsCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Volume Offset Control Profile server initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Volume Offset Control Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpsProcDmMsg(dmEvt_t *pEvt);

/*! \} */    /* VOLUME_OFFSET_CONTROL_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* VOCPS_API_H */
