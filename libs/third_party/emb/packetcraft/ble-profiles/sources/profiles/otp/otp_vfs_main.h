/*************************************************************************************************/
/*!
 *  \file otp_vfs_main.h
 *
 *  \brief  Handlers for OTS Requests.
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
#ifndef OTP_VFS_MAIN
#define OTP_VFS_MAIN

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "att_defs.h"
#include "wsf_types.h"

/**************************************************************************************************
 Macros
**************************************************************************************************/
#define OTP_VFS_MAX_FILE_SIZE         (4096)
#define OTP_VFS_MAX_NO                (2)
#define OTP_VFS_OBJECT_ID_LEN         (6)
#define OTP_VFS_TIMESTAMP_SIZE        (7)
#define OTP_VFS_MAX_SUPPORTED_FILE_NO (32)
#define OTP_VFS_MAX_FILE_NAME         (64)
#define OTP_VFS_NO_SUPPORTED_FILTERS  (3)
#define OTP_VFS_MAX_BLACKLIST_SIZE    (4)

#define VFS_MIN_VALID_ID      0x000000000100ULL
#define VFS_MAX_VALID_ID      0xFFFFFFFFFFFFULL
#define VFS_RESERVED_START_ID 0x000000000000ULL
#define VFS_RESERVED_END_ID   0x0000000000FFULL

#define OTP_OBJECT_RECORD_LENGTH_SIZE           (0x02)
#define OTP_OBJECT_RECORD_OBJ_ID_SIZE           (0x06)
#define OTP_OBJECT_RECORD_OBJ_NAME_LEN_SIZE     (0x01)
#define OTP_OBJECT_RECORD_FLAG_SIZE             (0x01)
#define OTP_OBJECT_RECORD_OBJ_TYPE_SIZE         (0x02)
#define OTP_OBJECT_RECORD_OBJ_CURRENT_SIZE_SIZE (0x04)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Object Attribute Types. */
enum OtpVfsAttributeType
{
  OTP_VFS_ATTRIBUTE_ALLOCATE_SIZE = 0x01,
  OTP_VFS_ATTRIBUTE_CURRENT_SIZE  = 0x02,
  OTP_VFS_ATTRIBUTE_TYPE          = 0x03,
  OTP_VFS_ATTRIBUTE_PROPERTY      = 0x04,
  OTP_VFS_ATTRIBUTE_CREATE_TIME   = 0x05,
  OTP_VFS_ATTRIBUTE_MODIFY_TIME   = 0x06,
  OTP_VFS_ATTRIBUTE_NAME          = 0x07,
  OTP_VFS_ATTRIBUTE_ID            = 0X08,
  OTP_VFS_SERVER_FEATURES         = 0X09,
  OTP_VFS_ATTRIBUTE_FULL_SIZE     = 0x0A,
};

/*! \brief Structure to store blacklisted UUIDs. */
typedef struct
{
  bool     is128bitUUID;
  uint8_t  length;
  uint8_t* uuid;
} BlacklistUUID;

/*! \brief Structure to store filter conditions. */
typedef struct _OtpsFilter
{
  uint8_t  opCode[OTP_VFS_NO_SUPPORTED_FILTERS];
  uint8_t  pCond[OTP_VFS_NO_SUPPORTED_FILTERS][OTP_VFS_MAX_FILE_NAME];
  uint16_t pCondLen[OTP_VFS_NO_SUPPORTED_FILTERS];
} OtpsFilter;

/*! \brief Structure to manage the Object linked list. */
typedef struct _OtpVFSList
{
  struct _OtpVFSList* pPrev;
  struct _OtpVFSList* pNext;
} OtpVFSList;

/*! \brief Structure containing object metadata. */
typedef struct _OtpVFSMetaData
{
  bool     isCurrObjValid;
  bool     isTypeLong;
  uint8_t  type[ATT_128_UUID_LEN];
  uint8_t  createTime[OTP_VFS_TIMESTAMP_SIZE];
  uint8_t  modifyTime[OTP_VFS_TIMESTAMP_SIZE];
  uint8_t  name[OTP_VFS_MAX_FILE_NAME];
  uint8_t name_len;
  uint32_t allocSize;
  uint32_t currSize;
  uint64_t sizeAttr;
  uint32_t property;
  uint32_t checksum;
} OtpVFSMetaData;

/*! \brief Structure containing object information. */
typedef struct _OtpVFSId
{
  OtpVFSList     list;
  uint64_t       id;
  uint16_t       handler;
  OtpVFSMetaData metaData;
} OtpVFSId;

/*! \brief Structure containing data used by OACP operations. */
typedef struct _OtpOACPOperation
{
  uint8_t  op;
  uint8_t  mode;
  uint32_t attOffset;
  uint32_t attLength;
  uint32_t currOpSize;
  uint16_t mtuLen;
} OtpOACPOperation;

/*! \brief Structure containing data used by OLCP operations. */
typedef struct _OtpOLCPOperation
{
  uint8_t     op;
  uint16_t    filteredNoOfObj;
  uint16_t    totalNoOfObj;
  bool        isCurrObjLocked;
  OtpVFSList* currObj;
} OtpOLCPOperation;

/*! \brief Structure containing data used the Object Server. */
typedef struct _OtpVFSConnection
{
  bool             isChannelAvailable;
  OtpVFSList       usedIdHead;
  OtpVFSList       filteredIdHead;
  OtpOACPOperation oacp;
  OtpOLCPOperation olcp;
  uint8_t          oacpCccIdx;  // Object List Action Point characteristic ccId for indication
  uint8_t          olcpCccIdx;  // Object List Control Point characteristic ccId for indication
  uint8_t          objChangedCccIdx;  // Object Changed Characteristic ccId for indication
  uint64_t         otsFeatures;       // Supported OTS features
  OtpsFilter       listFilters;       // Current filter settings
  uint16_t         numBlacklistedUUIDs;
  BlacklistUUID    blacklist[OTP_VFS_MAX_BLACKLIST_SIZE];
} OtpVFSConnection;

/*! \brief Structure containing all the data needed for the Object Server operation. */
typedef struct _OtpVFSContext
{
  OtpVFSId         idTable[OTP_VFS_MAX_SUPPORTED_FILE_NO];
  OtpVFSList       freeIdHead;
  OtpVFSConnection conn[OTP_VFS_MAX_NO];
} OtpVFSContext;

/*************************************************************************************************/
/*!
 *  \brief  Initialize the Object Transfer Protocol Service (OTPS).
 */
/*************************************************************************************************/
void OtpVFSInitialize(void);

/*************************************************************************************************/
/*!
 *  \brief  Retrieve the current ID for the OTPS service associated with a given channel.
 *
 *  \param  [in] cid  Channel ID for the current connection.
 *  \param  [out]     The channel ID if valid, or 0 if not valid.
 */
/*************************************************************************************************/
uint64_t OtpGetCurrId(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Set the CCCD indices used by the application for OTPS service characteristics.
 *
 *  \param  [in] oacpCccIdx         Object Action Control Point CCCD index.
 *  \param  [in] olcpCccIdx         Object List Control Point CCCD index.
 *  \param  [in] objChangedCccIdx   Object Changed CCCD index.
 */
/*************************************************************************************************/
void OtpSetCccIdx(uint8_t oacpCccIdx, uint8_t olcpCccIdx, uint8_t objChangedCccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Get the CCCD index used by the application for OTPS service characteristics.
 *
 *  \param  [in]  cid                Current cid.
 *  \param  [in]  type               Type of index requested.
 *  \param  [out] idx                Output index.
 *
 *  \return 0 if successful, 1 if there is an error.
 */
/*************************************************************************************************/
uint8_t OtpGetCccIdx(uint8_t cid, uint8_t type, uint8_t* idx);

/*************************************************************************************************/
/*!
 * \brief Get a pointer to a specific attribute of the current object in the Object List
 *
 * \param[in]  cid       Connection ID for the current connection.
 * \param[in]  attrType  Type of attribute to retrieve (e.g., size, type, property, etc.).
 * \param[out] pAttribute Pointer to store the attribute data if found.
 *
 * \return 0 if the current object is valid and the attribute exists, or an error code if the
 * current object is invalid or the attribute does not exist.
 */
/*************************************************************************************************/
uint8_t OtpVFSSetAttribute(uint8_t cid, uint8_t attrType, uint8_t* attr);

/*************************************************************************************************/
/*!
 * \brief Get a pointer to a specific attribute of the current object in the Object List
 *
 * \param[in]  cid       Connection ID for the current connection.
 * \param[in]  attrType  Type of attribute to retrieve (e.g., size, type, property, etc.).
 * \param[out] pAttribute Pointer to store the attribute data if found.
 *
 * \return 0 if the current object is valid and the attribute exists, or an error code if the
 * current object is invalid or the attribute does not exist.
 */
/*************************************************************************************************/
uint8_t OtpVFSGetAttribute(uint8_t cid, uint8_t attrType, uint8_t* pAttribute, uint16_t* pLen);

/*************************************************************************************************/
/*!
 *  \brief  Sets the name attribute for the currently selected object.
 *
 *  \param[in]  cid     Connection ID for the current connection.
 *  \param[in]  pValue  Pointer to the name data.
 *  \param[in]  len     Length of the name data.
 *
 *  \return  The result of the name setting operation. Possible return values include:
 *           - ATT_SUCCESS if the name is successfully set.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 *           - ATT_OTP_ERR_OBJ_NOT_SELECTED if the object is not selected.
 *           - ATT_OTP_ERR_NAME_ALREADY_EXISTS if the name already exists.
 */
/*************************************************************************************************/
uint8_t OtpVFSSetName(uint8_t cid, uint8_t* pValue, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Retrieves the name attribute for the currently selected object.
 *
 *  \param[in]   cid      Connection ID for the current connection.
 *  \param[out]  pValue   Pointer to the buffer to store the name data.
 *  \param[out]  pLen     Pointer to the variable to store the length of the name data.
 *
 *  \return  The result of the name retrieval operation. Possible return values include:
 *           - ATT_SUCCESS if the name is successfully retrieved.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 *           - ATT_OTP_ERR_OBJ_NOT_SELECTED if the object is not selected.
 */
/*************************************************************************************************/
uint8_t OtpVFSGetName(uint8_t cid, uint8_t* pValue, uint16_t* pLen);

/*************************************************************************************************/
/*!
 *  \brief  Sets a filter condition for the specified filter ID in the Object Transfer Service.
 *
 *  This function sets a filter condition for the specified filter ID in the Object Transfer
 * Service. The filter condition is used to filter objects during an object listing operation.
 *
 *  \param[in]   cid       Connection ID for the current connection.
 *  \param[in]   filterId  Filter ID to set the condition for (1 to 3).
 *  \param[in]   pValue    Pointer to the buffer containing the filter condition data.
 *  \param[in]   len       Length of the filter condition data.
 *
 *  \return  The result of the filter condition setting operation. Possible return values include:
 *           - ATT_SUCCESS if the filter condition is successfully set.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 */
/*************************************************************************************************/
uint8_t OtpVFSSetFilter(uint8_t cid, uint8_t filterId, uint8_t* pValue, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Retrieves the filter condition for the specified filter ID in the Object Transfer
 * Service.
 *
 *  This function retrieves the filter condition for the specified filter ID in Object Transfer
 * Service. The filter condition is used to filter objects during an object listing operation.
 *
 *  \param[in]    cid       Connection ID for the current connection.
 *  \param[in]    filterId  Filter ID to retrieve the condition for (1 to 3).
 *  \param[out]   pValue    Pointer to the buffer to store the retrieved filter condition data.
 *  \param[out]   pLen      Pointer to the variable to store the length of the retrieved filter
 * condition data.
 *
 *  \return  The result of the filter condition retrieval operation. Possible return values include:
 *           - ATT_SUCCESS if the filter condition is successfully retrieved.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 */
/*************************************************************************************************/
uint8_t OtpVFSGetFilter(uint8_t cid, uint8_t filterId, uint8_t* pValue, uint16_t* pLen);

/*************************************************************************************************/
/*!
 *  \brief  Create a request to create an object in the Object Transfer Protocol Service (OTPS).
 *
 *  \param[in]   cid      Connection ID.
 *  \param[in]   size     Pointer to the desired object size.
 *  \param[in]   type     Pointer to the object type UUID.
 *  \param[in]   isTypeLong  true if the type is 128 bits long, false if 16 bits long.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the request is successful,
 *           or an error code if the operation failed.
 */
/*************************************************************************************************/
uint8_t OtpVFSCreateRequest(uint8_t cid, uint32_t* size, uint8_t* type, bool isTypeLong);

/*************************************************************************************************/
/*!
 *  \brief  Create a request to delete an object in the Object Transfer Protocol Service (OTPS).
 *
 *  \param[in]   cid  Connection ID.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the request is successful,
 *           or an error code if the operation failed.
 */
/*************************************************************************************************/
uint8_t OtpVFSDeleteRequest(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Deletes an object in the Object Server.
 *
 *  \param[in]  cid      Connection ID for the current connection.
 *  \param[in]  objId    Object ID of the selected object.
 *
 *  \return  The result of the attribute setting operation. Possible return values include:
 *           - ATT_SUCCESS if the attribute is successfully set.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 *           - ATT_OTP_ERR_OBJ_NOT_SELECTED if the object is not selected.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ for other invalid write requests.
 */
/*************************************************************************************************/
uint8_t OtpVFSDeleteObjectUi(uint8_t cid, uint8_t objId[OTP_VFS_OBJECT_ID_LEN]);

/*************************************************************************************************/
/*!
 *  \brief  Write data to the selected object in the OTP VFS.
 *
 *  \param  cid     Connection ID for the current connection.
 *  \param  objId   Object Id
 *  \param  length  Length of the data to be written.
 *  \param  offset  Offset at which to start writing the data.
 *  \param  pBuff   Pointer to the data to be written.
 *
 *  \return 0 on success, or an error code on failure.
 */
/*************************************************************************************************/
uint8_t OtpVFSWriteDataUi(
    uint8_t cid, uint8_t objId[6], uint32_t length, uint32_t offset, uint8_t* pBuff);

/*************************************************************************************************/
/*!
 *  \brief  Write data to the selected object in the OTP VFS.
 *
 *  \param[in]   cid     Connection ID for the current connection.
 *  \param[in]   objId   Object Id
 *  \param[in]   length  Length of the data to be read.
 *  \param[in]   offset  Offset at which to start reading the data.
 *  \param[out]  pBuff   Pointer to the read data.
 *
 *  \return 0 on success, or an error code on failure.
 */
/*************************************************************************************************/
uint8_t OtpVFSReadDataUi(
    uint8_t cid, uint8_t objId[6], uint32_t length, uint32_t offset, uint8_t* pBuff);

/*************************************************************************************************/
/*!
 *  \brief  Create a request to calculate the CRC checksum for a specified range of data within an
 * object.
 *
 *  \param[in]   cid      Connection ID.
 *  \param[in]   objId   Object Id
 *  \param[in]   offset   Offset within the object's data where CRC calculation should start.
 *  \param[in]   length   Length of the data range for CRC calculation.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the request is successful,
 *           or an error code if the operation failed.
 */
/*************************************************************************************************/
uint8_t OtpVFSCalcChecksumRequestUi(
    uint8_t cid, uint8_t objId[6], uint32_t* length, uint32_t* offset);
/*************************************************************************************************/
/*!
 *  \brief  Performs an Object List Control Point (OLCP) operation.
 *
 *  This function performs an OLCP operation on objects based on the provided operation code.
 *
 *  \param[in]    cid                 Connection ID for the current connection.
 *  \param[in]    pValue              Pointer to the buffer containing the OLCP operation data.
 *  \param[out]   operationResponse   Pointer to the variable to store the OLCP operation response.
 *
 *  \return  The result of the OLCP operation. Possible return values include:
 *           - ATT_SUCCESS if the OLCP operation is successfully executed.
 *           - ATT_OTP_ERR_WRITE_REQ_REJ if the write request is rejected.
 */
/*************************************************************************************************/
uint8_t OtpVFSOlcpOpObject(uint8_t cid, uint8_t* pValue, uint8_t* operationResponse);

/*************************************************************************************************/
/*!
 *  \brief  Read data from the object based on the read request attributes.
 *
 *  \param[in]   cid    Connection ID.
 *  \param[out]  pBuff  Pointer to the buffer to store the read data.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the read operation is successful,
 *           or an error code if the operation failed.
 */
/*************************************************************************************************/
uint8_t OtpVFSReadData(uint8_t cid, uint8_t* pBuff);

/*************************************************************************************************/
/*!
 *  \brief  Initiates a read operation request from the current object in the Object Access Control
 *          Point (OACP) over L2CAP.
 *
 *  \param[in]  cid       Connection ID for the current connection.
 *  \param[in]  l2cCid    L2CAP Connection ID.
 *  \param[out]  isOperationFinished  Pointer to a boolean indicating whether the read operation is
 * finished.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the read operation request is successfully initiated,
 *           or an error code if the operation cannot be initiated. Possible error codes include:
 *           - OTP_VFS_OACP_ERR_OPERATION_FAILED when the operation fails.
 *           - OTP_VFS_OACP_ERR_SUCCESS when the operation is successful.
 */
/*************************************************************************************************/
uint8_t OtpVFSL2CRead(uint8_t cid, uint16_t l2cCid,  bool* isOperationFinished);

/*************************************************************************************************/
/*!
 *  \brief  Handle the start of the OACP read operation.
 *
 *  \param[in]  connId   Connection identifier.
 *
 *  \return  0 on successful operation.
 */
/*************************************************************************************************/
uint8_t OtpsL2CReadOpHandler(uint8_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Handle the start of the OACP write operation.
 *
 *  \param[in]  connId   Connection identifier.
 *
 *  \return  0 on successful operation.
 */
/*************************************************************************************************/
uint8_t OtpsL2CWriteOpHandler(uint8_t connId);

/*************************************************************************************************/
/*!
 * \brief Get the checksum of the currently selected object in the OTP VFS list.
 *
 * \param[in]   objId   Object Id
 * \param[in]   cid  Connection ID.
 * \return Checksum of the currently selected object.
 */
/*************************************************************************************************/
uint32_t OtpVFSReadCheckSumUi(uint8_t objId[6], uint8_t cid);

/*************************************************************************************************/
/*!
 * \brief Cleanup the OTP VFS list by removing objects with a name length of 0.
 *
 * This function removes objects with a name length of 0 from the filteredIdHead list
 * and moves them to the freeIdHead list. It also updates the total number of objects.
 *
 * \param[in] cid  Connection ID.
 */
/*************************************************************************************************/
void OtpVFSCleanup(uint8_t cid);

/*************************************************************************************************/
/*!
 * \brief Sort the objects in the OTP VFS list based on the specified sort type.
 *
 * This function sorts the objects in the filteredIdHead list of the given connection
 * based on the specified sort type.
 *
 * \param[in] cid       Connection ID.
 * \param[in] sortType  Sort type.
 */
/*************************************************************************************************/
void OtpVFSSortObject(uint8_t cid, uint8_t sortType);

/*************************************************************************************************/
/*!
 * \brief Clear the "mark" attribute for all objects in both filtered and used lists.
 *
 * \param[in] cid  Connection ID.
 */
/*************************************************************************************************/
void OtpVFSClearAllMarkObject(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Retrieves the total number of filtered objects for the current connection in the Object
 * Transfer Protocol (OTP) Service.
 *
 *  This function retrieves the total number of filtered objects for the current connection in the
 * OTP Service.
 *
 *  \param[in]   cid  Connection ID for the current connection.
 *
 *  \return  The total number of filtered objects for the current connection.
 */
/*************************************************************************************************/
uint16_t OtpVFSGetTotalObjectNumber(uint8_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Set the features field for the OTPS service for a given connection.
 *
 *  \param[in]  cid            Connection ID for the current connection.
 *  \param[in]  featuresField  Features field value to be set.
 */
/*************************************************************************************************/
void OtpSetFeatures(uint8_t cid, uint64_t featuresField);

/*************************************************************************************************/
/*!
 *  \brief  Set the MTU length for the L2Cap Communication for a given connection.
 *
 *  \param[in]  cid            Connection ID for the current connection.
 *  \param[in]  mtuLen         MTU Length for the current connection.
 */
/*************************************************************************************************/
void OtpSetMtu(uint8_t cid, uint16_t mtuLen);

/*************************************************************************************************/
/*!
 *  \brief  Initialize a linked list node.
 *
 *  \param[out]  node  Pointer to the node to be initialized.
 */
/*************************************************************************************************/
void OtpVFSUtilListInit(OtpVFSList* node);

/*************************************************************************************************/
/*!
 *  \brief  Insert a node into the linked list after the head.
 *
 *  \param[in]   head  Pointer to the head of the linked list.
 *  \param[out]  node  Pointer to the node to be inserted.
 */
/*************************************************************************************************/
void OtpVFSUtilListInsert(OtpVFSList* head, OtpVFSList* node);

/*************************************************************************************************/
/*!
 *  \brief  Remove a node from the linked list.
 *
 *  \param[in]   node  Pointer to the node to be removed.
 */
/*************************************************************************************************/
void OtpVFSUtilListRemove(OtpVFSList* node);

/*************************************************************************************************/
/*!
 *  \brief  Initiates a write operation request for the current object in the Object Access Control
 *          Point (OACP) over L2CAP.
 *
 *  \param[in]   cid                  Connection ID for the current connection.
 *  \param[in]   length               Length of the data to write.
 *  \param[in]   pBuff                Pointer to the data buffer to be written.
 *  \param[out]  isOperationFinished  Pointer to a boolean indicating whether the write operation is
 * finished.
 *
 *  \return  OTP_VFS_OACP_ERR_SUCCESS if the write operation request is successfully initiated.
 *           OTP_VFS_OACP_ERR_OPERATION_FAILED when an operation is already in progress or if there
 * is an invalid handle.
 */
/*************************************************************************************************/
uint8_t OtpVFSL2CWrite(uint8_t cid, uint32_t length, uint8_t* pBuff, bool* isOperationFinished);

/*************************************************************************************************/
/*!
 *  \brief  Write data to the OTP Filesystem.
 *
 *  \param  hdl     Handle for the entry.
 *  \param  offset  Offset within the entry to start writing.
 *  \param  buff    Pointer to the data to be written.
 *  \param  len     Length of the data to be written.
 *
 *  \return The number of bytes successfully written.
 */
/*************************************************************************************************/
uint32_t OtpVFSCoreWrite(uint16_t hdl, uint32_t offset, uint8_t* buff, uint32_t len);

#endif
