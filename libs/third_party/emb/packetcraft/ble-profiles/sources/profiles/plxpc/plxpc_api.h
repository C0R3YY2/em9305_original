/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Pulse Oximeter profile collector.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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
#ifndef PLXPC_API_H
#define PLXPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PULSE_OXIMETER_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Pulse Oximeter service enumeration of handle indexes of characteristics to be discovered */
enum
{
  PLXPC_PLXS_PLXSC_HDL_IDX,         /*!< Pulse Oximeter Spot Check measurement */
  PLXPC_PLXS_PLXSC_CCC_HDL_IDX,     /*!< Pulse Oximeter Spot Check measurement CCC descriptor */
  PLXPC_PLXS_PLXC_HDL_IDX,          /*!< Pulse Oximeter Continuous measurement */
  PLXPC_PLXS_PLXC_CCC_HDL_IDX,      /*!< Pulse Oximeter Continuous measurement CCC descriptor */
  PLXPC_PLXS_PLXF_HDL_IDX,          /*!< Pulse Oximeter features */
  PLXPC_PLXS_RACP_HDL_IDX,          /*!< Record access control point */
  PLXPC_PLXS_RACP_CCC_HDL_IDX,      /*!< Record access control point CCC descriptor */
  PLXPC_PLXS_HDL_LIST_LEN           /*!< Handle list length */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Pulse Oximeter service.
 *          Parameter pHdlList must point to an array of length \ref PLXPC_PLXS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpcPlxsDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref PLXPC_PLXS_HDL_LIST_LEN.
 *          If the ATT handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t PlxpcPlxsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Send a command to the pulse oximeter service record access control point.
 *
 *  \param  connId  Connection identifier.
 *  \param  handle  Attribute handle.
 *  \param  opcode  Command opcode.
 *  \param  oper    Command operator or 0 if no operator required.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpcPlxsRacpSend(dmConnId_t connId, uint16_t handle, uint8_t opcode, uint8_t oper);


/*! \} */    /* PULSE_OXIMETER_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* PLXPC_API_H */
