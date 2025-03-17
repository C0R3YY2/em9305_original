/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Microphone Control Profile client.
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

#ifndef MICPC_API_H
#define MICPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MICROPHONE_CONTROL_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of characteristics to be discovered - MICS */
/* TODO: A device can have multiple AIC and CSIS services.
 *       Add or remove AICS and/or CSIS service indicies as needed. */
enum
{
  MICPC_MICS_MUTE_HDL_INDX,            /*!< Microphone control mute. */
  MICPC_MICS_MUTE_CCC_INDX,            /*!< Microphone control mute CCC descriptor. */
  MICPC_MICS_AICS_0_INDX,              /*!< Microphone control 1st include AICS. */
  MICPC_MICS_AICS_1_INDX,              /*!< Microphone control 2nd include AICS. */
  MICPC_MICS_HDL_LIST_LEN,             /*!< Handle list length. */
};

/**************************************************************************************************
  Callback Function Declarations
**************************************************************************************************/

/* \brief   Callback to application for notification of control point write response.
*
*  \param  connId    Connection identifier.
*  \param  status    Status of the control point write response.
*
*  \return None.
*/
typedef void (*micpcNtfCback_t)(dmConnId_t connId, uint8_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/*************************************************************************************************/
/*!
 *  \brief  Write to the mute attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  priority      EATT priority.
 *  \param  mute          Mute value (MICS_NOT_MUTED, MICS_MUTE, or MICS_DISABLED).
 *
 *  \return None.
 */
/*************************************************************************************************/
void MicpcWriteMute(dmConnId_t connId, uint8_t priority, uint8_t mute);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Microphone Control service.
 *          Parameter pHdlList must point to an array of length \ref MICPC_MICS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MicpcMicsDiscover(dmConnId_t connId, uint16_t *pHdlList);

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
void MicpcMicsSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the MICPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MicpcProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function to receive control point notify callbacks.
 *
 *  \param  pCback      Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MicpcRegisterNtfCback(micpcNtfCback_t pCback);

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
uint8_t MicpcMicsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*! \} */    /* MICROPHONE_CONTROL_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* MICPC_API_H */
