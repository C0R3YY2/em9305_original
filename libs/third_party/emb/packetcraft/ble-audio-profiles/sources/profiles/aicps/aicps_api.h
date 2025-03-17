/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Audio Input Control Profile server.
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
#ifndef AICPS_API_H
#define AICPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup AUDIO_INPUT_CONTROL_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* AICPS gain change callback */
typedef void (*aicpsCback_t)(uint8_t svcId, uint8_t gain, uint8_t mode, uint8_t mute);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Get the mute setting for the audio input source
 *
 *  \param svcId     AICS service index.
 *
 *  \return true if control point is muted, else false.
 */
 /*************************************************************************************************/
bool AicpsGetMute(uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Called to disable mute commands from clients.
 *
 *  \param svcId     AICS service index.
 *  \param disabled  true to disable mute, false to enable mute.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsDisableMute(uint8_t svcId, bool disabled);

/*************************************************************************************************/
/*!
 *  \brief  Send audio input state notification.
 *
 *  \param connId    DM connection ID.
 *  \param svcId     AICS service index.
 *  \param priority  EATT priority.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSendStateNtf(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Send audio input description notification.
 *
 *  \param connId    DM connection ID.
 *  \param svcId     AICS service index.
 *  \param priority  EATT priority.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSendDescriptionNtf(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Set the mute value for the audio input source
 *
 *  \param svcId     AICS service index.
 *  \param mute      Mute value.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSetMuteValue(uint8_t svcId, uint8_t mute);

/*************************************************************************************************/
/*!
 *  \brief  Set the gain mode value for the audio input source
 *
 *  \param svcId     AICS service index.
 *  \param gainMode      Gain mode value.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSetGainModeValue(uint8_t svcId, uint8_t gainMode);

/*************************************************************************************************/
/*!
 *  \brief  Set gain setting attribute values.
 *
 *  \param svcId     AICS service index.
 *  \param stepSize  Gain setting step size (0.1 decibel units).
 *  \param min       Gain setting minumum.
 *  \param max       Gain setting maximum.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSetGainAttributes(uint8_t svcId, uint8_t stepSize, uint8_t min, uint8_t max);

/*************************************************************************************************/
/*!
 *  \brief  Set the input type value.
 *
 *  \param svcId     AICS service index.
 *  \param type      Input type
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSetInputType(uint8_t svcId, uint8_t type);

/*************************************************************************************************/
/*!
 *  \brief  Set the input status value.
 *
 *  \param svcId     AICS service index.
 *  \param status    Input status
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSetInputStatus(uint8_t svcId, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Set the input description value.
 *
 *  \param svcId     AICS service index.
 *  \param pDesc     Input description
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsSetInputDescription(uint8_t svcId, const char *pDesc);

/*************************************************************************************************/
/*!
 *  \brief  Register CCC indicies and an event callback function for AICS services.
 *
 *  \param svcId     AICS service index.
 *  \param cccStart  First CCC index.
 *  \param cback     Callback for event notification.
 *
 *  \return none.
 */
/*************************************************************************************************/
void AicpsRegister(uint8_t svcId, uint8_t cccStart, aicpsCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Audio Input Control Profile server initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Audio Input Control Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpsProcDmMsg(dmEvt_t *pEvt);

/*! \} */    /* AUDIO_INPUT_CONTROL_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* AICPS_API_H */
