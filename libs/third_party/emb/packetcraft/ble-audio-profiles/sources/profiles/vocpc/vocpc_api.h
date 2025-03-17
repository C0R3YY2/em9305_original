/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Volume Offset Control profile client.
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

#ifndef VOCPC_API_H
#define VOCPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup VOLUME_OFFSET_CONTROL_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Volume Offset Control Service Handles macro */
#define VOCPC_VOCS_DISC_HANDLES(_a, _b)                                                            \
enum                                                                                               \
{                                                                                                  \
  VOCPC_VOCS_STATE_HDL_IDX_##_a=_b,   /*!< Volume Offset Control state */                   \
  VOCPC_VOCS_STATE_CCC_HDL_IDX_##_a,  /*!< Volume Offset Control state CCC */               \
  VOCPC_VOCS_LOC_HDL_IDX_##_a,        /*!< Volume Offset Control location */                \
  VOCPC_VOCS_LOC_CCC_HDL_IDX_##_a,    /*!< Volume Offset Control location CCC */            \
  VOCPC_VOCS_CP_HDL_IDX_##_a,         /*!< Volume Offset Control control point */           \
  VOCPC_VOCS_DESC_HDL_IDX_##_a,       /*!< Volume Offset Control description */             \
  VOCPC_VOCS_DESC_CCC_HDL_IDX_##_a,   /*!< Volume Offset Control description state CCC */   \
  VOCPC_VOCS_HDL_LIST_LEN_##_a,       /*!< Handle list length */                            \
};

/* VOCS service handle sub-index */
#define VOCPC_VOCS_STATE_HDL_IDX          VOCPC_VOCS_STATE_HDL_IDX_0
#define VOCPC_VOCS_STATE_CCC_HDL_IDX      VOCPC_VOCS_STATE_CCC_HDL_IDX_0
#define VOCPC_VOCS_LOC_HDL_IDX            VOCPC_VOCS_LOC_HDL_IDX_0
#define VOCPC_VOCS_LOC_CCC_HDL_IDX        VOCPC_VOCS_LOC_CCC_HDL_IDX_0
#define VOCPC_VOCS_CP_HDL_IDX             VOCPC_VOCS_CP_HDL_IDX_0
#define VOCPC_VOCS_DESC_HDL_IDX           VOCPC_VOCS_DESC_HDL_IDX_0
#define VOCPC_VOCS_DESC_CCC_HDL_IDX       VOCPC_VOCS_DESC_CCC_HDL_IDX_0

/* Number of volume offset control service handles */
#define VOCPC_VOCS_HDL_LIST_LEN           VOCPC_VOCS_HDL_LIST_LEN_0

/*! \brief Enumeration of handle indexes of characteristics to be discovered - VOCS */
/* TODO: A device can have multiple secondary VOCS services.
 *       Add service handle indicies as needed. */
VOCPC_VOCS_DISC_HANDLES(0, 0);
VOCPC_VOCS_DISC_HANDLES(1, VOCPC_VOCS_HDL_LIST_LEN_0);

/**************************************************************************************************
  Callback Function Declarations
**************************************************************************************************/

/* \brief   Callback to application for notification of change counter synchronization.
 *
 *          The VOCPC attempts to synchronize its change counter with the server's change counter.
 *          It is possible to lose sync with the change counter.  If the server responds to a
 *          write request with VOCS_ERR_INVALID_CHANGE_CTR, the vocpcCcSyncCback_t callback
 *          is called with the sync parameter set to false.
 *
 *          After losing sync of the change counter, the VOCPC will attempt to read the change
 *          counter on the server. On receipt of the new change counter, the vocpcCcSyncCback_t
 *          callback is called with the sync parameter set to true.
 *
 *  \param  sync      Chgange counter sync state.
 *
 *  \return None.
 */
typedef void (*vocpcCcSyncCback_t)(bool sync);

/* \brief   Callback to application for notification of control point write response.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Status of the control point write response.
 *
 *  \return None.
 */
typedef void (*vocpcNtfCback_t)(dmConnId_t connId, uint8_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Volume Offset Control service.
 *          Parameter pHdlList must point to an array of length \ref VOCPC_VOCS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  vocsIndx  VOCS secondary service index.
 *  \param  startHdl  First characteristic handle.
 *  \param  endHdl    Last characteristic handle.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcVocsDiscover(dmConnId_t connId, uint8_t vocsIndx, uint16_t startHdl, uint16_t endHdl,
                       uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Volume Offset Control Service Control Point - Volume offset.
 *
 *  \param  connId        Connection identifier.
 *  \param  vocsId        VOCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  volumeOffset  Volume offset.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcSetVolumeOffset(dmConnId_t connId, uint8_t vocsId, uint8_t priority, int16_t volumeOffset);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Volume Offset Control Service Audio Location.
 *
 *  \param  connId        Connection identifier.
 *  \param  vocsId        VOCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  location      Location bitfield.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcSetAudioLocation(dmConnId_t connId, uint8_t vocsId, uint8_t priority, uint8_t location);

/*************************************************************************************************/
/*!
 *  \brief  Write to the Volume Offset Control Service Input Description.
 *
 *  \param  connId        Connection identifier.
 *  \param  vocsId        VOCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  pDescription  Description string.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcSetAudioOutputDescription(dmConnId_t connId, uint8_t vocsId, uint8_t priority, char *description);

/*************************************************************************************************/
/*!
 *  \brief  Read the control state attribute.  This will update the change counter.
 *
 *  \param  connId      Connection identifier.
 *  \param  vocsId      VoCS service identifier.
 *  \param  priority    EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcVocsReadControlState(dmConnId_t connId, uint8_t vocsId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref DIS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t VocpcVocsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive change counter synchronized callbacks.
 *
 *          The VOCPC attempts to synchronize its change counter with the server's change counter.
 *          It is possible to lose sync with the change counter.  If the server responds to a
 *          write request with VOCS_ERR_INVALID_CHANGE_CTR, the vocpcCcSyncCback_t callback
 *          is called with the sync parameter set to false.
 *
 *          After losing sync of the change counter, the VOCPC will attempt to read the change
 *          counter on the server. On receipt of the new change counter, the vocpcCcSyncCback_t
 *          callback is called with the sync parameter set to true.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcRegisterCcSyncCback(vocpcCcSyncCback_t pCback);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive control point notify callbacks.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcRegisterNtfCback(vocpcNtfCback_t pCback);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the VOCPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void VocpcProcMsg(wsfMsgHdr_t *pEvt);

/*! \} */    /* VOLUME_OFFSET_CONTROL_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* VOCPC_API_H */
