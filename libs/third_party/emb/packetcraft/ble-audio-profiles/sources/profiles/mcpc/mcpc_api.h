/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Media Control profile client.
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

#ifndef MCPC_API_H
#define MCPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MEDIA_CONTROL_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Media Control Service Handles macro */
#define MCPC_MCS_DISC_HANDLES(_a, _b)                                                               \
enum                                                                                                \
{                                                                                                   \
  MCPC_MCS_NAME_HDL_IDX_##_a=_b,        /*!< \brief Media Control player name */                    \
  MCPC_MCS_NAME_CCC_HDL_IDX_##_a,       /*!< \brief Media Control player name CCC */                \
  MCPC_MCS_ICON_HDL_IDX_##_a,           /*!< \brief Media Control icon URI */                       \
  MCPC_MCS_TRK_CH_HDL_IDX_##_a,         /*!< \brief Media Control track changed */                  \
  MCPC_MCS_TRK_CH_CCC_HDL_IDX_##_a,     /*!< \brief Media Control track changed CCC */              \
  MCPC_MCS_TRK_TITLE_HDL_IDX_##_a,      /*!< \brief Media Control track title */                    \
  MCPC_MCS_TRK_TITLE_CCC_HDL_IDX_##_a,  /*!< \brief Media Control track title CCC */                \
  MCPC_MCS_TRK_DUR_HDL_IDX_##_a,        /*!< \brief Media Control track duration */                 \
  MCPC_MCS_TRK_DUR_CCC_HDL_IDX_##_a,    /*!< \brief Media Control track duration CCC */             \
  MCPC_MCS_TRK_POS_HDL_IDX_##_a,        /*!< \brief Media Control track position */                 \
  MCPC_MCS_TRK_POS_CCC_HDL_IDX_##_a,    /*!< \brief Media Control track position CCC */             \
  MCPC_MCS_PB_SPEED_HDL_IDX_##_a,       /*!< \brief Media Control playback speed */                 \
  MCPC_MCS_PB_SPEED_CCC_HDL_IDX_##_a,   /*!< \brief Media Control playback speed CCC */             \
  MCPC_MCS_SEEK_SPEED_HDL_IDX_##_a,     /*!< \brief Media Control seek speed */                     \
  MCPC_MCS_SEEK_SPEED_CCC_HDL_IDX_##_a, /*!< \brief Media Control seek speed CCC */                 \
  MCPC_MCS_ORDER_HDL_IDX_##_a,          /*!< \brief Media Control playback order */                 \
  MCPC_MCS_ORDER_CCC_HDL_IDX_##_a,      /*!< \brief Media Control playback order CCC */             \
  MCPC_MCS_ORDER_SUP_HDL_IDX_##_a,      /*!< \brief Media Control playback order supported */       \
  MCPC_MCS_STATE_HDL_IDX_##_a,          /*!< \brief Media Control state */                          \
  MCPC_MCS_STATE_CCC_HDL_IDX_##_a,      /*!< \brief Media Control state CCC */                      \
  MCPC_MCS_CTRL_PT_HDL_IDX_##_a,        /*!< \brief Media Control control point */                  \
  MCPC_MCS_CTRL_PT_CCC_HDL_IDX_##_a,    /*!< \brief Media Control control point CCC */              \
  MCPC_MCS_CP_OC_SUP_HDL_IDX_##_a,      /*!< \brief Media Control control point opcode supported */ \
  MCPC_MCS_CP_OC_SUP_CCC_HDL_IDX_##_a,  /*!< \brief Media Control control point opcode supported CCC */ \
  MCPC_MCS_CCID_HDL_IDX_##_a,           /*!< \brief Media Control content control ID */             \
  MCPC_MCS_HDL_LIST_LEN_##_a,           /*!< \brief Handle list length */                           \
};

/* MCS service handle sub-index */
#define MCPC_MCS_NAME_HDL_IDX             MCPC_MCS_NAME_HDL_IDX_0
#define MCPC_MCS_NAME_CCC_HDL_IDX         MCPC_MCS_NAME_CCC_HDL_IDX_0
#define MCPC_MCS_ICON_HDL_IDX             MCPC_MCS_ICON_HDL_IDX_0
#define MCPC_MCS_TRK_CH_HDL_IDX           MCPC_MCS_TRK_CH_HDL_IDX_0
#define MCPC_MCS_TRK_CH_CCC_HDL_IDX       MCPC_MCS_TRK_CH_CCC_HDL_IDX_0
#define MCPC_MCS_TRK_TITLE_HDL_IDX        MCPC_MCS_TRK_TITLE_HDL_IDX_0
#define MCPC_MCS_TRK_TITLE_CCC_HDL_IDX    MCPC_MCS_TRK_TITLE_CCC_HDL_IDX_0
#define MCPC_MCS_TRK_DUR_HDL_IDX          MCPC_MCS_TRK_DUR_HDL_IDX_0
#define MCPC_MCS_TRK_DUR_CCC_HDL_IDX      MCPC_MCS_TRK_DUR_CCC_HDL_IDX_0
#define MCPC_MCS_TRK_POS_HDL_IDX          MCPC_MCS_TRK_POS_HDL_IDX_0
#define MCPC_MCS_TRK_POS_CCC_HDL_IDX      MCPC_MCS_TRK_POS_CCC_HDL_IDX_0
#define MCPC_MCS_PB_SPEED_HDL_IDX         MCPC_MCS_PB_SPEED_HDL_IDX_0
#define MCPC_MCS_PB_SPEED_CCC_HDL_IDX     MCPC_MCS_PB_SPEED_CCC_HDL_IDX_0
#define MCPC_MCS_SEEK_SPEED_HDL_IDX       MCPC_MCS_SEEK_SPEED_HDL_IDX_0
#define MCPC_MCS_SEEK_SPEED_CCC_HDL_IDX   MCPC_MCS_SEEK_SPEED_CCC_HDL_IDX_0
#define MCPC_MCS_ORDER_HDL_IDX            MCPC_MCS_ORDER_HDL_IDX_0
#define MCPC_MCS_ORDER_CCC_HDL_IDX        MCPC_MCS_ORDER_CCC_HDL_IDX_0
#define MCPC_MCS_ORDER_SUP_HDL_IDX        MCPC_MCS_ORDER_SUP_HDL_IDX_0
#define MCPC_MCS_STATE_HDL_IDX            MCPC_MCS_STATE_HDL_IDX_0
#define MCPC_MCS_STATE_CCC_HDL_IDX        MCPC_MCS_STATE_CCC_HDL_IDX_0
#define MCPC_MCS_CTRL_PT_HDL_IDX          MCPC_MCS_CTRL_PT_HDL_IDX_0
#define MCPC_MCS_CTRL_PT_CCC_HDL_IDX      MCPC_MCS_CTRL_PT_CCC_HDL_IDX_0
#define MCPC_MCS_CP_OC_SUP_HDL_IDX        MCPC_MCS_CP_OC_SUP_HDL_IDX_0
#define MCPC_MCS_CP_OC_SUP_CCC_HDL_IDX    MCPC_MCS_CP_OC_SUP_CCC_HDL_IDX_0
#define MCPC_MCS_CCID_HDL_IDX             MCPC_MCS_CCID_HDL_IDX_0
#define MCPC_MCS_HDL_LIST_LEN             MCPC_MCS_HDL_LIST_LEN_0

/*! \brief Enumeration of handle indexes of characteristics to be discovered - MCS */
/* TODO: A device can have more than one media control services.
 *       Add service handle indicies as needed. */
MCPC_MCS_DISC_HANDLES(0, 0);
MCPC_MCS_DISC_HANDLES(1, MCPC_MCS_HDL_LIST_LEN_0);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a Media Control service.
 *          Parameter pHdlList must point to an array of length \ref MCPC_MCS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     MCS service identifier.
 *  \param  pHdlList  Pointer to array to hold discovered handles.
 *  \param  cont      true if searching for more services, false if searching for the first time.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcMcsDiscover(dmConnId_t connId, uint8_t svcId, uint16_t *pHdlList, bool cont);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a Generic Media Control service.
 *          Parameter pHdlList must point to an array of length \ref MCPC_MCS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     MCS service identifier.
 *  \param  pHdlList  Pointer to array to hold discovered handles.
 *  \param  cont      true if searching for more services, false if searching for the first time.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcGmcsDiscover(dmConnId_t connId, uint8_t svcId, uint16_t *pHdlList, bool cont);

/*************************************************************************************************/
/*!
 *  \brief  Set service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     MCS service identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can set restored handles after restablishing a link with a bonded device.
 */
/*************************************************************************************************/
void McpcMcsSetHandles(dmConnId_t connId, uint8_t svcId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Read the player name attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadPlayerName(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the icon URL attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadIconUrl(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the track title attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadTrackTitle(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the track duration attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadTrackDuration(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the track position attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadTrackPosition(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the playback speed attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadPlaybackSpeed(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the seek speed attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadSeekSpeed(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the play order attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadPlayOrder(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the play order supported attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadPlayOrderSupported(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the state attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadState(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the control point opcodes supported attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadCtrlPtOpcodesSupported(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Read the Content Control ID (CCID) attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcReadCcid(dmConnId_t connId, uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Write to the track position attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  position      Track position.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcWriteTrackPosition(dmConnId_t connId, uint8_t svcId, uint8_t priority, int32_t position);

/*************************************************************************************************/
/*!
 *  \brief  Write to the playback speed attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  speed         Playback speed.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcWritePlaySpeed(dmConnId_t connId, uint8_t svcId, uint8_t priority, int8_t speed);

/*************************************************************************************************/
/*!
 *  \brief  Write to the play order attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  speed         Play order.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcWritePlayOrder(dmConnId_t connId, uint8_t svcId, uint8_t priority, uint8_t order);

/*************************************************************************************************/
/*!
 *  \brief  Write to the media control point attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         MCS service identifier.
 *  \param  priority      EATT priority.
 *  \param  opcode        Opcode.
 *  \param  parameter     Opcode specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcWriteControlPt(dmConnId_t connId, uint8_t svcId, uint8_t priority, uint8_t opcode, int32_t parameter);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref MCPC_MCS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t McpcMcsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the MCPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpcProcMsg(wsfMsgHdr_t *pEvt);

/*! \} */    /* MEDIA_CONTROL_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* MCPC_API_H */
