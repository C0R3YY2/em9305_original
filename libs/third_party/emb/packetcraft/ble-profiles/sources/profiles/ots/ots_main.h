/*************************************************************************************************/
/*!
*  \file ots_main.h
*
*  \brief Implementation of the Object Transfer Service.
*
*  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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
#ifndef OTS_MAIN_H
#define OTS_MAIN_H

#include "att_defs.h"
#include "cfg_stack.h"
#include "dm_api.h"
#include "otp/otp_vfs_api.h"
#include "ots_ui.h"
#include "svc_otps.h"
#include "wsf_os.h"
#include "wsf_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup OBJECT_TRANSFER_PROFILE
*  \{ */

/**************************************************************************************************
Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief OTPS L2Cap Control Block. */

typedef struct
{
  uint16_t   cid;
  wsfTimer_t timer;
} otsL2cap_t;

typedef struct
{
  otsL2cap_t     l2capCcb[DM_CONN_MAX];
  wsfHandlerId_t handlerId;
} otsCb;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
  *  \brief  Handle writing to the Object Name characteristic.
  *
  *  This function handles write operations to the Object Name characteristic.
  * It is called when a client writes to the Object Name attribute.
  *
  *  \param[in]  connId    DM connection identifier.
  *  \param[in]  offset    Write offset.
  *  \param[in]  len       Write length.
  *  \param[in]  pValue    Pointer to the value to write.
  *  \param[out] pAttr     Attribute to write.
  *  \param[in]  operation ATT operation (e.g., write with or without
  * response).
  *
  *  \return ATT status indicating the success or failure of the write
  * operation.
  */
/*************************************************************************************************/
uint8_t OtpsWriteName(dmConnId_t connId, uint16_t offset, uint16_t len, uint8_t *pValue,
    attsAttr_t *pAttr, uint8_t operation);

/*************************************************************************************************/
/*!
  *  \brief  Handle reading from the Object Name characteristic.
  *
  *  This function handles read operations from the Object Name
  * characteristic. It is called when a client reads the Object Name
  * attribute.
  *
  *  \param[in]  connId    DM connection identifier.
  *  \param[in]  offset    Read offset.
  *  \param[in]  len       Read length.
  *  \param[out] pValue    Pointer to store the read value.
  *  \param[in]  pAttr     Attribute to read.
  *  \param[in]  operation ATT operation (e.g., read request or read
  * response).
  *
  *  \return ATT status indicating the success or failure of the read
  * operation.
  */
/*************************************************************************************************/
uint8_t OtpsReadName(dmConnId_t connId, uint16_t offset, attsAttr_t *pAttr, uint8_t operation);

/*************************************************************************************************/
/*!
  *  \brief  Handle setting a filter for the Object Server.
  *
  *  \param[in]  connId    DM connection identifier.
  *  \param[in]  offset    Read offset.
  *  \param[in]  len       Read length.
  *  \param[out] pValue    Pointer to store the read value.
  *  \param[in]  pAttr     Attribute to read.
  *  \param[in]  operation ATT operation (e.g., read request or read
  * response). \param[in]  filterId  Filter Id to be updated, 1, 2 or 3
  *
  *  \return ATT status indicating the success or failure of the read
  * operation.
  */
/*************************************************************************************************/
uint8_t OtpsWriteFilter(dmConnId_t connId, uint16_t offset, uint16_t len, attsAttr_t *pAttr,
    uint8_t *pValue, uint8_t operation, uint8_t filterId);

/*************************************************************************************************/
/*!
  *  \brief  Handle getting a filter from the Object Server.
  *
  *  \param[in]  connId    DM connection identifier.
  *  \param[in]  offset    Read offset.
  *  \param[in]  pAttr     Attribute to read.
  *  \param[in]  operation ATT operation (e.g., read request or read
  * response). \param[in]  filterId  Filter Id to be updated, 1, 2 or 3
  *
  *  \return ATT status indicating the success or failure of the read
  * operation.
  */
/*************************************************************************************************/
uint8_t OtpsReadFilter(
    dmConnId_t connId, uint16_t offset, attsAttr_t *pAttr, uint8_t operation, uint8_t filterId);

/*************************************************************************************************/
/*!
  *  \brief  Handle writing to Object Metadata characteristic.
  *
  *  This function handles write operations to the Object Metadata
  * characteristic. It is called when a client writes to the Object Metadata
  * attribute.
  *
  *  \param[in]  connId    DM connection identifier.
  *  \param[in]  offset    Write offset.
  *  \param[in]  len       Write length.
  *  \param[in]  pValue    Pointer to the value to write.
  *  \param[out] pAttr     Attribute to write.
  *  \param[in]  operation ATT operation (e.g., write with or without
  * response). \param[in]  attrType  Type of attribute within Object Metadata
  * (e.g., allocate size, current size).
  *
  *  \return ATT status indicating the success or failure of the write
  * operation.
  */
/*************************************************************************************************/
uint8_t OtpsWriteMetadata(dmConnId_t connId, uint16_t offset, uint16_t len, uint8_t *pValue,
    attsAttr_t *pAttr, uint8_t operation, uint8_t attrType);

/*************************************************************************************************/
/*!
  *  \brief  Handle reading from the Object Metadata characteristic.
  *
  *  This function handles read operations from the Object Metadata
  * characteristic. It is called when a client reads the Object Metadata
  * attribute.
  *
  *  \param[in]  connId    DM connection identifier.
  *  \param[in]  offset    Read offset.
  *  \param[in]  len       Read length.
  *  \param[out] pValue    Pointer to store the read value.
  *  \param[in]  pAttr     Attribute to read.
  *  \param[in]  operation ATT operation (e.g., read request or read
  * response). \param[in]  attrType  Type of attribute within Object Metadata
  * (e.g., allocate size, current size).
  *
  *  \return ATT status indicating the success or failure of the read
  * operation.
  */
/*************************************************************************************************/
uint8_t OtpsReadMetadata(
    dmConnId_t connId, uint16_t offset, attsAttr_t *pAttr, uint8_t operation, uint8_t attrType);

/*************************************************************************************************/
/*!
  *  \brief  Handler for OLCP operations
  * *
  *  \return ATT status of the current request.
  */
/*************************************************************************************************/
uint8_t OlcpHandler(dmConnId_t connId, uint16_t offset, uint16_t len, attsAttr_t *pAttr,
    uint8_t *pValue, uint8_t operation);

/*************************************************************************************************/
/*!
  *  \brief  Send an indication in response to an OLCP operation.
  *
  *  \param pPreset  Pointer to preset.
  *
  *  \return None.
  */
/*************************************************************************************************/
void OlcpIndication(dmConnId_t connId, uint8_t *pValue, uint8_t *operationResponse);

/*************************************************************************************************/
/*!
  *  \brief  Handler for OACP operations
  * *
  *  \return ATT status of the current request.
  */
/*************************************************************************************************/
uint8_t OacpHandler(dmConnId_t connId, uint16_t offset, uint16_t len, attsAttr_t *pAttr,
    uint8_t *pValue, uint8_t operation, otsCb *pOtpsCb);

/*************************************************************************************************/
/*!
  *  \brief Update the attribute server values to match the current object
  * selected in the VFS.
  *
  *  \param[in]  connId    DM connection identifier.
  */
/*************************************************************************************************/
void OtpsUpdateMetadata(dmConnId_t connId);

/*************************************************************************************************/
/*!
  *  \brief Handler for the abortion of the current read operation.
  *
  *  \param[in]  connId    DM connection identifier.
  */
/*************************************************************************************************/
void OtpsL2CAbortOpHandler(uint8_t cid);

/*************************************************************************************************/
/*!
  *  \brief Update the attribute server values to match the current object
  * selected in the VFS.
  *
  *  \param[in]  connId    DM connection identifier.
  */
/*************************************************************************************************/
void OtpsUpdateMetadata(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Utility to manage ObjID types
 *
 *  \param[in]  input   ID in UINT64 type.
 *  \param[out] output  Pointer to uint48 data.
 *
 *  \return  None
 */
 /*************************************************************************************************/
void convertu64tou48(uint64_t input, uint8_t output[6]);

/*! \} */ /* OBJECT_TRANSFER_PROFILE */

#ifdef __cplusplus
}
#endif

#endif /* OTS_MAIN_H */
