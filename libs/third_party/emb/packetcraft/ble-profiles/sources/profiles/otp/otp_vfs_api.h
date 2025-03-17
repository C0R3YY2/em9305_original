/*************************************************************************************************/
/*!
 *  \file otp_vfs_api.h
 *
 *  \brief  Virtual filesystem implementation for OTP, OTS. Includes functions to handle objects
 *          stored in memory
 *
 *  Copyright (c) 2012-2019 Arm Ltd. All Rights Reserved.
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

#ifndef OTP_VFS_API
#define OTP_VFS_API

#include "otp_vfs_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

// Object List Control Point Invalid Object States
#define OTP_VFS_OLCP_INVALID_OBJ 0
#define OTP_VFS_OLCP_VALID_OBJ   1

// CCCD Index Definitions
#define OTP_VFS_CCCD_IDX_OACP   1
#define OTP_VFS_CCCD_IDX_OLCP   2
#define OTP_VFS_CCCD_IDX_OBJ_CH 3

#define OTP_VFS_CORE_INVALID_HANDLE 0xFFFFFFFF

#define OTP_VFS_OLCP_OPERATION_OPCODE_SIZE (0x01)
#define OTP_VFS_OLCP_OPERATION_GOTO_SIZE   (0x06)
#define OTP_VFS_OLCP_OPERATION_ORDER_SIZE  (0x01)
#define OTP_VFS_OLCP_OPERATION_MAX_SIZE    (OTP_VFS_OLCP_OPERATION_GOTO_SIZE + 1)

#define OTP_VFS_OACP_OPERATION_MAX_SIZE       (0x0A)
#define OTP_VFS_OACP_OPERATION_OPCODE_SIZE    (0x01)
#define OTP_VFS_OACP_OPERATION_CREATE_SIZE    (0x06)
#define OTP_VFS_OACP_OPERATION_CHECKSUM_SIZE  (0x08)
#define OTP_VFS_OACP_OPERATION_READ_SIZE      (0x08)
#define OTP_VFS_OACP_OPERATION_WRITE_SIZE     (0x09)
#define OTP_VFS_OLCP_OPERATION_EXECUTION_SIZE (0x30)

#define OACP_CREATE_LEN_UUID16  (sizeof(uint8_t) + sizeof(uint32_t) + ATT_16_UUID_LEN)
#define OACP_CREATE_LEN_UUID128 (sizeof(uint8_t) + sizeof(uint32_t) + ATT_128_UUID_LEN)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the OTP VFS core.
 */
/*************************************************************************************************/
void OtpVFSCoreInitialize(void);

/*!
 *  \brief  Sets the specified attribute for the selected object in the Object Server.
 *
 *  This function sets the specified attribute for the selected object in the Object Server based on
 * the attribute type and the provided data. It performs validation checks and updates the object's
 * metadata accordingly.
 *
 *  \param[in]  cid      Connection ID for the current connection.
 *  \param[in]  objId    Object ID of the selected object.
 *  \param[in]  attType  Attribute type to be set.
 *  \param[in]  att      Pointer to the attribute data.
 *  \param[in]  attLen   Length of the attribute data.
 *
 *  \return  The result of the attribute setting operation. Possible return values include:
 *           - ATT_SUCCESS if the attribute is successfully set.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 *           - ATT_OTP_ERR_OBJ_NOT_SELECTED if the object is not selected.
 *           - ATT_OTP_ERR_NAME_ALREADY_EXISTS if the name already exists.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ for other invalid write requests.
 */
uint8_t OtpVFSSetAttributeUi(uint8_t cid, uint8_t objId[OTP_VFS_OBJECT_ID_LEN], uint8_t attType,
    uint8_t* att, uint8_t attLen);

/*************************************************************************************************/
/*!
 *  \brief  Create an entry in the OTP VFS core.
 *
 *  \param  size  Size of the entry to be created.
 *
 *  \return Handle for the created entry, or OTP_VFS_CORE_INVALID_HANDLE if creation fails.
 */
/*************************************************************************************************/
uint32_t OtpVFSCoreCreate(uint32_t size);

/*************************************************************************************************/
/*!
 *  \brief  Create a read request to retrieve data from a specified range within an object.
 *
 *  \param[in]   cid      Connection ID.
 *  \param[in]   offset   Offset within the object's data where the read operation should start.
 *  \param[in]   length   Length of the data range to be read.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the request is successful,
 *           or an error code if the operation failed.
 */
/*************************************************************************************************/
uint8_t OtpVFSReadRequest(uint8_t cid, uint32_t* offset, uint32_t* length);

/*************************************************************************************************/
/*!
 *  \brief  Read data from the OTP VFS core.
 *
 *  \param  hdl     Handle for the entry.
 *  \param  offset  Offset within the entry to start reading.
 *  \param  buff    Pointer to the buffer to store the read data.
 *  \param  len     Length of the data to read.
 *
 *  \return The number of bytes successfully read.
 */
/*************************************************************************************************/
uint32_t OtpVFSCoreRead(uint16_t hdl, uint32_t offset, uint8_t* buff, uint32_t len);

/*************************************************************************************************/
/*!
 *  \brief  Delete an entry from the OTP VFS core.
 *
 *  \param  hdl  Handle for the entry to be deleted.
 *
 *  \return Handle of the deleted entry or OTP_VFS_CORE_INVALID_HANDLE if unsuccessful.
 */
/*************************************************************************************************/
uint32_t OtpVFSCoreDelete(uint16_t hdl);

/*************************************************************************************************/
/*!
 *  \brief  Read data from the OTP VFS core and calculate CRC32.
 *
 *  \param  hdl     Handle for the entry.
 *  \param  offset  Offset within the entry to start reading.
 *  \param  len     Length of the data to read and calculate CRC32.
 *
 *  \return The CRC32 value calculated from the read data.
 */
/*************************************************************************************************/
uint32_t OtpVFSCoreCRCRead(uint16_t hdl, uint32_t offset, uint32_t len);

/*************************************************************************************************/
/*!
 *  \brief  Write data to the current object in the OTP VFS.
 *
 *  \param  cid     Connection ID for the current connection.
 *  \param  length  Length of the data to be written.
 *  \param  offset  Offset at which to start writing the data.
 *  \param  pBuff   Pointer to the data to be written.
 *
 *  \return 0 on success, or an error code on failure.
 */
/*************************************************************************************************/
uint8_t OtpVFSWriteData(uint8_t cid, uint32_t length, uint32_t offset, uint8_t* pBuff);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an abort operation request for the current OACP Read operation in progress
 *
 *
 *  \param[in]   cid  Connection ID for the current connection.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the abort operation request is successfully initiated,
 *           or an error code if the operation cannot be initiated. Possible error codes include:
 *           - OTP_VFS_OACP_ERR_NOT_SUPPORTED when the abort feature is not supported.
 *           - OTP_VFS_OACP_ERR_OPERATION_FAILED when an operation is not in progress or cannot be
 * aborted.
 */
/*************************************************************************************************/
uint8_t OtpVFSAbortRequest(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Handle the abort of the current read operation.
 *
 *  \param[in]  cid   Channel identifier.
 *
 *  \return  None.
 */
/*************************************************************************************************/
void OtpsL2CAbortOpHandler(uint8_t cid);

/*!
 *  \brief  Performs an Object Access Control Point (OACP) operation on objects in the Object
 * Transfer Protocol (OTP) Service.
 *
 *  This function performs an OACP operation on objects in the OTP Service based on the provided
 * operation code.
 *
 *  \param[in]    cid                 Connection ID for the current connection.
 *  \param[in]    len                 Length of the OACP operation data.
 *  \param[in]    pValue              Pointer to the buffer containing the OACP operation data.
 *  \param[out]   operationResponse   Pointer to the variable to store the OACP operation response.
 *
 *  \return  The result of the OACP operation. Possible return values include:
 *           - ATT_SUCCESS if the OACP operation is successfully executed.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 */
uint8_t OtpVFSOacpOpObject(uint8_t cid, uint16_t len, uint8_t* pValue, uint8_t* operationResponse);

/*************************************************************************************************/
/*!
 *  \brief  Create a request to calculate the CRC checksum for a specified range of data within an
 * object.
 *
 *  \param[in]   cid      Connection ID.
 *  \param[in]   offset   Offset within the object's data where CRC calculation should start.
 *  \param[in]   length   Length of the data range for CRC calculation.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the request is successful,
 *           or an error code if the operation failed.
 */
/*************************************************************************************************/
uint8_t OtpVFSCalcChecksumRequest(uint8_t cid, uint32_t* offset, uint32_t* length);

/**
 * \brief Get the checksum of the currently selected object in the OTP VFS list.
 *
 * \param[in] cid  Connection ID.
 * \return Checksum of the currently selected object.
 */
uint32_t OtpVFSReadCheckSum(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Add a new UUID to the blacklist for a specific connection.
 *
 *  \param[in]  cid           Object Transfer Channel ID.
 *  \param[in]  uuid          Pointer to the UUID to add to the blacklist.
 *  \param[in]  is128bitUUID  Flag indicating if the UUID is 128 bits long.
 *
 *  \return true if the UUID is successfully added to the blacklist, false otherwise.
 */
/*************************************************************************************************/
bool addToBlacklist(uint8_t cid, uint8_t* uuid, bool is128bitUUID);

/*************************************************************************************************/
/*!
 *  \brief  Print information about objects in the OTP VFS for the specified connection.
 *
 *  \param  cid  Connection ID for the current connection.
 */
/*************************************************************************************************/
void printObjList(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Set the availability status of a specified channel.
 *
 *  \param[in]  cid                 Channel ID.
 *  \param[in]  channelAvailability true to set the channel as available, false to set as
 * unavailable.
 *
 *  \return
 *    - 0 if the operation is successful.
 *    - 1 if the parameters are invalid.
 */
/*************************************************************************************************/
uint8_t setChannelStatus(uint8_t cid, bool channelAvailability);

/*************************************************************************************************/
/*!
 *  \brief  Get the availability status of a specified channel.
 *
 *  \param[in]   cid                  Channel ID.
 *  \param[out]  channelAvailability true if the channel is available, false if it is unavailable.
 *
 *  \return
 *    - 0 if the operation is successful.
 *    - 1 if the parameter is invalid.
 */
/*************************************************************************************************/
uint8_t getChannelStatus(uint8_t cid, bool* channelAvailability);

/*************************************************************************************************/
/*!
 *  \brief  Update the object list based on filters for a specific connection ID.
 *
 *  This function moves the old filtered objects to the usedId list and filters them again
 *  based on the specified conditions.
 *
 *  \param[in] cid  Connection ID.
 */
/*************************************************************************************************/
void OTPVFSUpdateObjectList(uint8_t cid);
#endif  // OTP_VFS_API
