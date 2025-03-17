/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Volume Control Profile client.
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

#ifndef VCPC_API_H
#define VCPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup VOLUME_CONTROL_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of characteristics to be discovered - MICS */
/* TODO: A device can have multiple secondary AIC services.
 *       Add or remove AICS service indicies as needed. */
enum
{
  VCPC_VCS_STATE_HDL_INDX,            /*!< Volume control state. */
  VCPC_VCS_STATE_CCC_INDX,            /*!< Volume control state CCC descriptor. */
  VCPC_VCS_CP_HDL_INDX,               /*!< Volume control control point. */
  VCPC_VCS_FLAGS_HDL_INDX,            /*!< Volume control flags. */
  VCPC_VCS_FLAGS_CCC_INDX,            /*!< Volume control flags CCC descriptor. */
  VCPC_VCS_AICS_0_INDX,               /*!< Volume control 1st include AICS. */
  VCPC_VCS_AICS_1_INDX,               /*!< Volume control 2nd include AICS. */
  VCPC_VCS_VOCS_0_INDX,               /*!< Volume control 1st include VOCS. */
  VCPC_VCS_VOCS_1_INDX,               /*!< Volume control 2st include VOCS. */
  VCPC_VCS_HDL_LIST_LEN,              /*!< Handle list length. */
};

/**************************************************************************************************
  Callback Function Declarations
**************************************************************************************************/

/* \brief   Callback to application for notification of change counter synchronization.
 *
 *          The VCPC attempts to synchronize its change counter with the server's change counter.
 *          It is possible to lose sync with the change counter.  If the server responds to a
 *          write request with VCS_ERR_INVALID_CHANGE_CTR, the vcpcCcSyncCback_t callback
 *          is called with the sync parameter set to false.
 *
 *          After losing sync of the change counter, the VCPC will attempt to read the change
 *          counter on the server. On receipt of the new change counter, the vcpcCcSyncCback_t
 *          callback is called with the sync parameter set to true.
 *
 *  \param  sync      Chgange counter sync state.
 *
 *  \return None.
 */
typedef void (*vcpcCcSyncCback_t)(bool sync);

/* \brief   Callback to application for notification of control point write response.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Status of the control point write response.
 *
 *  \return None.
 */
typedef void (*vcpcNtfCback_t)(dmConnId_t connId, uint8_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Requests the volume setting, mute, and change counter from the server.
 *
 *  \param  connId        Connection identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcReadVolumeState(dmConnId_t connId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Requests the volume setting, mute, and change counter from the server.
 *
 *  \param  connId        Connection identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcReadVolumeFlags(dmConnId_t connId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Write to the volume control point - Set Absolute Volume.
 *
 *  \param  connId        Connection identifier.
 *  \param  priority      EATT priority.
 *  \param  volume        Volume setting.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcSetAbsoluteVolume(dmConnId_t connId, uint8_t priority, uint8_t volume);

/*************************************************************************************************/
/*!
 *  \brief  Write to the volume control point - Change Relative volume.
 *
 *  \param  connId        Connection identifier.
 *  \param  priority      EATT priority.
 *  \param  up            true to increase relative volume, false to decrease.
 *  \param  mute          true to unmute, else false.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcSetRelativeVolume(dmConnId_t connId, uint8_t priority, bool up, bool unmute);

/*************************************************************************************************/
/*!
 *  \brief  Write to the volume control point - Set Mute/Unmute.
 *
 *  \param  connId        Connection identifier.
 *  \param  priority      EATT priority.
 *  \param  mute          true to mute, false to unmute.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcSetMute(dmConnId_t connId, uint8_t priority, bool mute);

/*************************************************************************************************/
/*!
 *  \brief  Get the cached mute value.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return Mute value.
 */
/*************************************************************************************************/
bool VcpcGetMute(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Get the cached volume value.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return Volume value.
 */
/*************************************************************************************************/
uint8_t VcpcGetVolume(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Volume Control service.
 *          Parameter pHdlList must point to an array of length \ref VCPC_VCS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcVcsDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can set restored handles after restablishing a link with a bonded device.
 */
/*************************************************************************************************/
void VcpcVcsSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the VCPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref VCPC_VCS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t VcpcVcsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive change counter synchronized callbacks.
 *
 *          The VCPC attempts to synchronize its change counter with the server's change counter.
 *          It is possible to lose sync with the change counter.  If the server responds to a
 *          write request with VCS_ERR_INVALID_CHANGE_CTR, the vcpcCcSyncCback_t callback
 *          is called with the sync parameter set to false.
 *
 *          After losing sync of the change counter, the VCPC will attempt to read the change
 *          counter on the server. On receipt of the new change counter, the vcpcCcSyncCback_t
 *          callback is called with the sync parameter set to true.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcRegisterCcSyncCback(vcpcCcSyncCback_t pCback);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive control point notify callbacks.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcRegisterNtfCback(vcpcNtfCback_t pCback);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive control point notify callbacks.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VcpcRegisterNtfCback(vcpcNtfCback_t pCback);

/*! \} */    /* VOLUME_CONTROL_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* VCPC_API_H */
