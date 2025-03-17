/*************************************************************************************************/
/*!
 *  \file ots_ui.h
 *
 *  \brief User Interface for the Object Transfer Service
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
#ifndef OTS_UI_H
#define OTS_UI_H

#include "dm_api.h"
#include "svc_otps.h"
#include "wsf_os.h"
#include "wsf_types.h"

#ifdef __cplusplus
extern "C"
{
#endif
/*! \addtogroup OBJECT_TRANSFER_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define OTS_UI_CID 0
#define OTPS_L2CAP_DATA_TIMER_MS 30000
#define OTPS_L2CAP_TIMEOUT_EVT   0xA0

/*************************************************************************************************/
/*!
  *  \brief  Initialize the Test OTPS Server.
  *
  *  \param[in]  otpsHandlerId   ID of the OTPS task handler.
  */
/*************************************************************************************************/
void OtsHandlerInit(wsfHandlerId_t otpsHandlerId);

/*************************************************************************************************/
/*!
  *  \brief  L2CAP Timeout Handler for OACP Operation Timeouts.
  */
/*************************************************************************************************/
void OtsL2cTimeoutHandler();

/*************************************************************************************************/
/*!
  *  \brief Connection reset procedure on the Object Transfer Server
  *
  *  \param[in]  connId    DM connection identifier.
  */
/*************************************************************************************************/
void OtsUiConnectionResetHandler(dmConnId_t connId);

/*************************************************************************************************/
/*!
  *  \brief  Send an indication to the client if the contents or metadata of
  * the current object changed.
  *
  *  \param[in]  connId   Connection identifier.
  *  \param[in]  cid      Channel identifier.
  *  \param[in]  flags    Flags indicating the source and type of changes in
  * the object.
  *  \param[in]  objId    Object identifier for the changed object.
  */
/*************************************************************************************************/
void OtsUiObjChangedSendIndication(dmConnId_t connId, uint8_t cid, uint8_t flags, uint64_t objId);

/*************************************************************************************************/
/*!
  *  \brief  Set the features attribute.
  *
  *  \param[in]  features   Features bitmask to set.
  *
  *  \return  Status of the attribute setting operation.
  *           - ATT_SUCCESS: Operation successful.
  *           - ATT_ERR_NOT_FOUND: Attribute not found.
  *           - ATT_ERR_LENGTH: Invalid write length.
  */
/*************************************************************************************************/
uint8_t OtsUiSetFeatures(uint64_t features);

/*************************************************************************************************/
/*!
  *  \brief  Create an Object in the Object Server and send an object changed
  * indication if a client is connected.
  *
  *  \param[in]  size            Desired Object Size.
  *  \param[in]  type            Desired Object Type UUID, can be either 16 or
  * 128 bits long.
  *  \param[in]  isTypeLong      Set to true if the type is 128
  * bits long.
  *
  *  \return 1 if the request is unsuccessful, 0 if the request is successful.
  */
/*************************************************************************************************/
uint8_t OtsUiCreateObject(uint32_t size, uint8_t *type, bool isTypeLong);

/*************************************************************************************************/
/*!
  *  \brief Delete the selected Object in the Object Server.
  *
  *  \param[in] Object to be deleted
  *  \return 1 if the request is unsuccessful, 0 if the request is successful.
  */
/*************************************************************************************************/
uint8_t OtsUiDeleteObject(uint8_t objId[6]);

/*************************************************************************************************/
/*!
  *  \brief  Calculate the checksum for the currently selected object.
  *
  *  \param[in]  objId    Object to calculate.
  *  \param[in]  offset   Offset of the data within the object.
  *  \param[in]  length   Length of the data for which the checksum is
  * calculated.
  *  \param[out] checksum Calculated checksum
  *  \return 1 if the
  * request is unsuccessful, 0 if the request is successful.
  */
/*************************************************************************************************/
uint8_t OtsUiCalculateChecksum(uint8_t objId[6], uint32_t offset, uint32_t length, uint32_t *checksum);

/*************************************************************************************************/
/*!
 *  \brief  Read an Object from the Object Server and print its contents.
 *
 *  \param[in]   objId   Object to read.
 *  \param[in]   offset  Pointer to the offset of the data within the object.
 *  \param[in]   length  Pointer to the length of the data to be read.
 *  \param[out]  readData Ponter to the data read from the object if succesful, NULL otherwise
 *
 *  \return 1 if the request is unsuccessful, 0 if the request is successful.
 */
/*************************************************************************************************/
uint8_t OtsUiReadObject(uint8_t objId[6], uint32_t *offset, uint32_t *length, uint8_t *readData);

/*************************************************************************************************/
/*!
  *  \brief  Write data to an Object in the Object Server.
  *
  *  \param[in]  objId    Object to read.
  *  \param[in]  length   Length of the data to be written.
  *  \param[in]  offset   Offset of the data within the object.
  *  \param[in]  pBuff    Pointer to the buffer containing the data to be
  * written.
  *
  *  \return 1 if the request is unsuccessful, 0 if the request is successful.
  */
/*************************************************************************************************/
uint8_t OtsUiWriteObjectData(uint8_t objId[6], uint32_t length, uint32_t offset, uint8_t *pBuff);

/*************************************************************************************************/
/*!
  *  \brief  Attempt to abort the data transfer of an OACP read operation in
  * progress.
  */
/*************************************************************************************************/
void OtsUiAbort();

/*************************************************************************************************/
/*!
  *  \brief  Clear one of the filters.
  *
  *  \param[in] filterId    Filter Characteristic to update: 1, 2, or 3.
  *
  *  \return 0 if the operation is successful, 1 if the operation fails.
  */
/*************************************************************************************************/
uint8_t OtsUiFilterNone(uint8_t filterId);

/*************************************************************************************************/
/*!
  *  \brief  Update one of the 3 Object Filter characteristics with an Object
  * name condition.
  *
  *  \param[in] filterId    Filter Characteristic to update: 1, 2, or 3.
  *  \param[in] condition   Object name condition to apply:
  *                         - OTPS_FILTER_TYPE_NAME_START_WITH: Indicates the
  * object name should start with the specified name.
  *                         - OTPS_FILTER_TYPE_NAME_END_WITH: Indicates the
  * object name should end with the specified name.
  *                         - OTPS_FILTER_TYPE_NAME_CONTAINS: Indicates the
  * object name should contain the specified name.
  *                         - OTPS_FILTER_TYPE_NAME_IS_EXACTLY: Indicates the
  * object name should exactly match the specified name.
  *  \param[in] name     Object name to use as a condition in the filter.
  *  \param[in] nameLen  Length
  * of the Object name.
  *
  *  \return 0 if the operation is successful, 1 if the operation fails.
  */
/*************************************************************************************************/
uint8_t OtsUiFilterName(
    uint8_t filterId, enum OtsFilterCondition condition, char *name, uint8_t nameLen);

/*************************************************************************************************/
/*!
  *  \brief  Update one of the 3 Object Filter characteristics with an Object
  * type condition.
  *
  *  \param[in] filterId    Filter Characteristic to update: 1, 2, or 3.
  *  \param[in] type        Object type to use as a condition in the filter.
  *  \param[in] typeLen     Length of the Object type. It can be either
  * ATT_128_UUID_LEN or ATT_16_UUID_LEN.
  *
  *  \return 0 if the operation is successful, 1 if the operation fails.
  */
/*************************************************************************************************/
uint8_t OtsUiFilterType(uint8_t filterId, uint8_t *type, uint8_t typeLen);

/*************************************************************************************************/
/*!
  *  \brief  Update one of the 3 Object Filter characteristics with an Object
  * date condition.
  *
  *  \param[in] filterId    Filter Characteristic to update: 1, 2, or 3.
  *  \param[in] condition   Object date condition to apply:
  *                         - OTPS_FILTER_TYPE_CREATED_BETWEEN: Indicates the
  * object should be created between the specified dates.
  *                         - OTPS_FILTER_TYPE_MODIFIED_BETWEEN: Indicates the
  * object should be modified between the specified dates.
  *  \param[in] date1 7-byte date array representing the start date.
  *  \param[in] date2 7-byte date array representing the end date.
  *
  *  \note                  The date array follows the following format:
  *                         - Indexes 0 and 1 contain a 2-byte year (e.g.,
  * 0x07E7 for 2023).
  *                         - Index 2 contains the month.
  *                         - Index 3 contains the day.
  *                         - Index 4 contains the hour.
  *                         - Index 5 contains the minute.
  *                         - Index 6 contains the second.
  *
  *  \return 0 if the operation is successful, 1 if the operation fails.
  */
/*************************************************************************************************/
uint8_t OtsUiFilterDate(
    uint8_t filterId, enum OtsFilterCondition condition, uint8_t date1[7], uint8_t date2[7]);

/*************************************************************************************************/
/*!
  *  \brief  Update one of the 3 Object Filter characteristics with an Object
  * size condition.
  *
  *  \param[in] filterId    Filter Characteristic to update: 1, 2, or 3.
  *  \param[in] condition   Object size condition to apply:
  *                         - OTPS_FILTER_TYPE_ALLOC_SIZE_BETWEEN: Indicates
  * the object should have allocated size between the specified sizes.
  *                         - OTPS_FILTER_TYPE_CURRENT_SIZE_BETWEEN: Indicates
  * the object should have current size between the specified sizes.
  *  \param[in] size1       First size parameter for the condition.
  *  \param[in] size2       Second size parameter for the condition.
  *
  *  \return 0 if the operation is successful, 1 if the operation fails.
  */
/*************************************************************************************************/
uint8_t OtsUiFilterSize(
    uint8_t filterId, enum OtsFilterCondition condition, uint32_t size1, uint32_t size2);

/*************************************************************************************************/
/*!
  *  \brief  Set one of the possible attributes for a desired object.
  *
  *  \param[in]  objId      Object ID representing the desired object.
  *  \param[in]  attType    Type of attribute to be set.
  *                         - OTP_VFS_ATTRIBUTE_NAME: String of up to 64 bytes.
  *                         - OTP_VFS_ATTRIBUTE_CREATE_TIME: 7-byte long date formatted.
  *                         - OTP_VFS_ATTRIBUTE_MODIFY_TIME: 7-byte long date formatted.
  *                         - OTP_VFS_ATTRIBUTE_PROPERTY: 32-bit unsigned integer.
  *  \param[in]  att        Pointer to the attribute value to be set.
  *  \param[in]  attLen     Length of the attribute value.
  *
  *  \return 1 if the request is unsuccessful, 0 if the request is successful.
  */
/*************************************************************************************************/
uint8_t OtsUiSetObjAttribute(uint8_t objId[6], uint8_t attType, uint8_t *att, uint8_t attLen);

/*************************************************************************************************/
/*!
  *  \brief  Perform the OLCP GoTo Operation as if the client requested it, to select an object
  * from the Object List by its ID.
  *
  * \param[in] ObjId   48-bit Object ID to look for.
  *
  * \return 0 if the operation is successful, 1 if the operation fails.
  *
  * \note   This is a debug function! Be careful when using it.
  *          It WILL cause sync issues with the connected client, as it modifies the currently
  * selected object for the server.
  */
/*************************************************************************************************/
uint8_t OtsUiListGotoDebug(uint8_t ObjId[6]);

/*! \} */ /* OBJECT_TRANSFER_SERVICE */
#ifdef __cplusplus
}
#endif

#endif /* OTS_UI_H */
