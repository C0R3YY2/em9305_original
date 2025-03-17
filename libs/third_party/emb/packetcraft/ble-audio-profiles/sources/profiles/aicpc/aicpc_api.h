/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Aidio Input Control profile client.
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

#ifndef AICPC_API_H
#define AICPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup AUDIO_INPUT_CONTROL_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Audio Input Control Service Handles macro */
#define AICPC_AICS_DISC_HANDLES(_a, _b)                                                            \
enum                                                                                               \
{                                                                                                  \
  AICPC_AICS_STATE_HDL_IDX_##_a=_b,   /*!< Audio Input Control input state */               \
  AICPC_AICS_STATE_CCC_HDL_IDX_##_a,  /*!< Audio Input Control input state CCC */           \
  AICPC_AICS_GSA_HDL_IDX_##_a,        /*!< Audio Input Control gain setting */              \
  AICPC_AICS_TYPE_HDL_IDX_##_a,       /*!< Audio Input Control input type */                \
  AICPC_AICS_STATUS_HDL_IDX_##_a,     /*!< Audio Input Control input stauts */              \
  AICPC_AICS_STATUS_CCC_HDL_IDX_##_a, /*!< Audio Input Control input stauts CCC */          \
  AICPC_AICS_AICP_HDL_IDX_##_a,       /*!< Audio Input Control input control point */       \
  AICPC_AICS_AID_HDL_IDX_##_a,        /*!< Audio Input Control input description */         \
  AICPC_AICS_AID_CCC_HDL_IDX_##_a,    /*!< Audio Input Control input description CCC */     \
  AICPC_AICS_HDL_LIST_LEN_##_a,       /*!< Handle list length */                            \
};

/* AICS service handle sub-index */
#define AICPC_AICS_STATE_HDL_IDX          AICPC_AICS_STATE_HDL_IDX_0
#define AICPC_AICS_STATE_CCC_HDL_IDX      AICPC_AICS_STATE_CCC_HDL_IDX_0
#define AICPC_AICS_GSA_HDL_IDX            AICPC_AICS_GSA_HDL_IDX_0
#define AICPC_AICS_TYPE_HDL_IDX           AICPC_AICS_TYPE_HDL_IDX_0
#define AICPC_AICS_STATUS_HDL_IDX         AICPC_AICS_STATUS_HDL_IDX_0
#define AICPC_AICS_STATUS_CCC_HDL_IDX     AICPC_AICS_STATUS_CCC_HDL_IDX_0
#define AICPC_AICS_AICP_HDL_IDX           AICPC_AICS_AICP_HDL_IDX_0
#define AICPC_AICS_AID_HDL_IDX            AICPC_AICS_AID_HDL_IDX_0
#define AICPC_AICS_AID_CCC_HDL_IDX        AICPC_AICS_AID_CCC_HDL_IDX_0

/* Gain Modes */
#define AICPC_GAIN_MODE_MANUAL            AICS_GAIN_MODE_MANUAL
#define AICPC_GAIN_MODE_AUTO              AICS_GAIN_MODE_AUTOMATIC

/* Number of audio input control service handles */
#define AICPC_AICS_HDL_LIST_LEN           AICPC_AICS_HDL_LIST_LEN_0

/*! \brief Enumeration of handle indexes of characteristics to be discovered - AICS */
/* TODO: A device can have multiple secondary AIC services.
 *       Add service handle indicies as needed. */
AICPC_AICS_DISC_HANDLES(0, 0);
AICPC_AICS_DISC_HANDLES(1, AICPC_AICS_HDL_LIST_LEN_0);

/**************************************************************************************************
  Callback Function Declarations
**************************************************************************************************/

/* \brief   Callback to application for notification of change counter synchronization.
 *
 *          The AICPC attempts to synchronize its change counter with the server's change counter.
 *          It is possible to lose sync with the change counter.  If the server responds to a
 *          write request with AICS_ERR_INVALID_CHANGE_CTR, the aicpcCcSyncCback_t callback
 *          is called with the sync parameter set to false.
 *
 *          After losing sync of the change counter, the AICPC will attempt to read the change
 *          counter on the server. On receipt of the new change counter, the aicpcCcSyncCback_t
 *          callback is called with the sync parameter set to true.
 *
 *  \param  sync      Chgange counter sync state.
 *
 *  \return None.
 */
typedef void (*aicpcCcSyncCback_t)(bool sync);

/* \brief   Callback to application for notification of control point write response.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Status of the control point write response.
 *
 *  \return None.
 */
typedef void (*aicpcNtfCback_t)(dmConnId_t connId, uint8_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Audio Input Control service.
 *          Parameter pHdlList must point to an array of length \ref AICPC_AICS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  aicsIndx  AICS secondary service index.
 *  \param  startHdl  First characteristic handle.
 *  \param  endHdl    Last characteristic handle.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcAicsDiscover(dmConnId_t connId, uint8_t aicsIndx, uint16_t startHdl, uint16_t endHdl,
                       uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Audio Input Control Service Input Control Point - Gain setting.
 *
 *  \param  connId      Connection identifier.
 *  \param  aicsId      AICS service identifier.
 *  \param  priority    EATT priority.
 *  \param  gain        Gain setting.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcAicsSetGainSetting(dmConnId_t connId, uint8_t aicsId, uint8_t priority, uint8_t gain);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Audio Input Control Service Input Control Point - Mute.
 *
 *  \param  connId      Connection identifier.
 *  \param  aicsId      AICS service identifier.
 *  \param  priority    EATT priority.
 *  \param  mute        true to mute, false to unmute.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcAicsSetMuteSetting(dmConnId_t connId, uint8_t aicsId, uint8_t priority, bool mute);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Audio Input Control Service Input Control Point - Gain mode.
 *
 *  \param  connId      Connection identifier.
 *  \param  aicsId      AICS service identifier.
 *  \param  priority    EATT priority.
 *  \param  mode        Gain mode (AICPC_GAIN_MODE_AUTO or AICPC_GAIN_MODE_MANUAL).
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcAicsSetGainMode(dmConnId_t connId, uint8_t aicsId, uint8_t priority, uint8_t mode);

/*************************************************************************************************/
/*!
 *  \brief  Read the input state attribute.  This will update the change counter.
 *
 *  \param  connId      Connection identifier.
 *  \param  aicsId      AICS service identifier.
 *  \param  priority    EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcAicsReadInputState(dmConnId_t connId, uint8_t aicsId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Audio Input Control Service Input Description.
 *
 *  \param  connId        Connection identifier.
 *  \param  aicsId        AICS service identifier.
 *  \param  priority      EATT priority.
 *  \param  pDescription  Description string.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcAicsAudioInputDescription(dmConnId_t connId, uint8_t aicsId, uint8_t priority, char *description);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref AICPC_AICS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t AicpcAicsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive change counter synchronized callbacks.
 *
 *          The AICPC attempts to synchronize its change counter with the server's change counter.
 *          It is possible to lose sync with the change counter.  If the server responds to a
 *          write request with AICS_ERR_INVALID_CHANGE_CTR, the aicpcCcSyncCback_t callback
 *          is called with the sync parameter set to false.
 *
 *          After losing sync of the change counter, the AICPC will attempt to read the change
 *          counter on the server. On receipt of the new change counter, the aicpcCcSyncCback_t
 *          callback is called with the sync parameter set to true.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcRegisterCcSyncCback(aicpcCcSyncCback_t pCback);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive control point notify callbacks.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcRegisterNtfCback(aicpcNtfCback_t pCback);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the AICPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AicpcProcMsg(wsfMsgHdr_t *pEvt);

/*! \} */    /* AUDIO_INPUT_CONTROL_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* AICPC_API_H */
