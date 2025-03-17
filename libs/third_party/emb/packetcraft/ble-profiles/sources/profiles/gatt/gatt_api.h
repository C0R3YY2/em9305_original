/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  GATT profile.
 *
 *  Copyright (c) 2011-2019 Arm Ltd. All Rights Reserved.
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
#ifndef GATT_API_H
#define GATT_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GATT_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of characteristics to be discovered */
enum
{
  GATT_SC_HDL_IDX,          /*!< Service changed */
  GATT_SC_CCC_HDL_IDX,      /*!< Service changed client characteristic configuration descriptor */
  GATT_CSF_HDL_IDX,         /*!< Client Supported Features */
  GATT_HDL_LIST_LEN         /*!< Handle list length */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for GATT service.  Note that pHdlList
 *          must point to an array of handles of length \ref GATT_HDL_LIST_LEN.  If discovery is
 *          successful the handles of discovered characteristics and descriptors will be set
 *          in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GattDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref GATT_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t GattValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for gatt service.
 *
 *  \param  connId     Connection identifier.
 *  \param  handle     ATT handle.
 *  \param  operation  Operation selected.
 *  \param  offset     Offset to begin read from.
 *  \param  pAttr      Attribute to read from.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t GattReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                      uint16_t offset, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for gatt service.
 *
 *  \param  connId     Connection identifier.
 *  \param  handle     ATT handle.
 *  \param  operation  Operation selected.
 *  \param  offset     Offset to begin write.
 *  \param  len        Length of write.
 *  \param  pValue     Pointer to buffer to write.
 *  \param  pAttr      Attribute to write to.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t GattWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                       uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  Set Index of the Service Changed CCCD in the ATT Server.
 *
 *  \param  idx  Index of the Service Changed CCCD in the ATT Server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GattSetSvcChangedIdx(uint8_t idx);

/*************************************************************************************************/
/*!
 *  \brief  Send Service Change Indications to the specified connections if they are configured to
 *          do so.
 *
 *  \param  connId    DM Connection identifier or \ref DM_CONN_ID_NONE to send to all connections.
 *  \param  start     start handle for service changed value.
 *  \param  end       end handle for service changed value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void GattSendServiceChangedInd(dmConnId_t connId, uint16_t start, uint16_t end);

/*! \} */    /* GATT_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* GATT_API_H */
