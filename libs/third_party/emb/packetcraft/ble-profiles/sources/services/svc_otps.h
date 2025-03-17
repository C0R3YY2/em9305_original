/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Object Transfer Service implementation.
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

#ifndef SVC_OTP_H
#define SVC_OTP_H

#include "att_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup OBJECT_TRANSFER_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/*! Max transfer data length. */
#define OTPS_OTS_FEATURE_ATT_PAYLOAD_LEN          8
#define OTPS_OBJECT_NAME_ATT_PAYLOAD_LEN          64 + 1
#define OTPS_OBJECT_TYPE_ATT_PAYLOAD_LEN          16
#define OTPS_OBJECT_SIZE_ATT_PAYLOAD_LEN          8
#define OTPS_OBJECT_FIRST_CREATED_ATT_PAYLOAD_LEN 7
#define OTPS_OBJECT_LAST_MODIFIED_ATT_PAYLOAD_LEN 7
#define OTPS_OBJECT_ID_ATT_PAYLOAD_LEN            6
#define OTPS_OBJECT_PROPERTIES_ATT_PAYLOAD_LEN    4
#define OTPS_OACP_ATT_PAYLOAD_LEN                 21
#define OTPS_OLCP_ATT_PAYLOAD_LEN                 7
#define OTPS_IND_PAYLOAD_LEN                      3
#define OTPS_OBJECT_LIST_FILTER_ATT_PAYLOAD_LEN   (OTPS_OBJECT_NAME_ATT_PAYLOAD_LEN + 1)
#define OTPS_OBJECT_CHANGED_ATT_PAYLOAD_LEN       7

#define OTP_OBJ_CHANGED_SERVER   (0 << 0) /*!< Object Changed Source Sever Mask Bit. */
#define OTP_OBJ_CHANGED_CLIENT   (1 << 0) /*!< Object Changed Source Client Mask Bit. */
#define OTP_OBJ_CHANGED_CONTENTS (1 << 1) /*!< Object Changed Contents Mask Bit. */
#define OTP_OBJ_CHANGED_METADATA (1 << 2) /*!< Object Changed Metadata Mask Bit. */
#define OTP_OBJ_CHANGED_CREATION (1 << 3) /*!< Object Changed Creation Mask Bit. */
#define OTP_OBJ_CHANGED_DELETION (1 << 4) /*!< Object Changed Deletion Mask Bit. */

// Object Property Bits
#define OTP_VFS_PROPERTY_DELETE_BIT   (1)
#define OTP_VFS_PROPERTY_EXECUTE_BIT  (1 << 1)
#define OTP_VFS_PROPERTY_READ_BIT     (1 << 2)
#define OTP_VFS_PROPERTY_WRITE_BIT    (1 << 3)
#define OTP_VFS_PROPERTY_APPEND_BIT   (1 << 4)
#define OTP_VFS_PROPERTY_TRUNCATE_BIT (1 << 5)
#define OTP_VFS_PROPERTY_PATCH_BIT    (1 << 6)
#define OTP_VFS_PROPERTY_MARK_BIT     (1 << 7)

#define OTS_CHANGED_IND_SOURCE_SERVER_BIT (0)
#define OTS_CHANGED_IND_SOURCE_CLIENT_BIT (1)
#define OTS_CHANGED_IND_OBJ_CONTENTS_BIT  (1 << 1)
#define OTS_CHANGED_IND_OBJ_METADATA_BIT  (1 << 2)
#define OTS_CHANGED_IND_OBJ_CREATION_BIT  (1 << 3)
#define OTS_CHANGED_IND_OBJ_DELETION_BIT  (1 << 4)

  // OACP Result Codes
  enum OtsOacpResultCode
  {
    OTP_VFS_OACP_ERR_SUCCESS                = 0x01,
    OTP_VFS_OACP_ERR_NOT_SUPPORTED          = 0x02,
    OTP_VFS_OACP_ERR_INVALID_PARAMETER      = 0x03,
    OTP_VFS_OACP_ERR_INSUFFICIENT_RESOURCES = 0x04,
    OTP_VFS_OACP_ERR_INVALID_OBJECT         = 0x05,
    OTP_VFS_OACP_ERR_CHANNEL_NOT_AVAILABLE  = 0x06,
    OTP_VFS_OACP_ERR_UNSUPPORTED_TYPE       = 0x07,
    OTP_VFS_OACP_ERR_PROC_NOT_PERMITTED     = 0x08,
    OTP_VFS_OACP_ERR_OBJECT_LOCKED          = 0x09,
    OTP_VFS_OACP_ERR_OPERATION_FAILED       = 0x0A,
  };

  // OLCP Result Codes
  enum OtsOlcpResultCode
  {
    OTP_VFS_OLCP_ERR_SUCCESS             = 0x01,
    OTP_VFS_OLCP_ERR_NOT_SUPPORTED       = 0x02,
    OTP_VFS_OLCP_ERR_INVALID_PARAMETER   = 0x03,
    OTP_VFS_OLCP_ERR_OPERATION_FAILED    = 0x04,
    OTP_VFS_OLCP_ERR_OUT_OF_BOUNDS       = 0x05,
    OTP_VFS_OLCP_ERR_TOO_MANY_OBJECTS    = 0x06,
    OTP_VFS_OLCP_ERR_NO_OBJECT           = 0x07,
    OTP_VFS_OLCP_ERR_OBJECT_ID_NOT_FOUND = 0x08,
  };

  // Object List Control Point Operations
  enum OtsOlcpOpCode
  {
    OTP_VFS_OLCP_OPERATION_FIRST      = 0x01,
    OTP_VFS_OLCP_OPERATION_LAST       = 0x02,
    OTP_VFS_OLCP_OPERATION_PREV       = 0x03,
    OTP_VFS_OLCP_OPERATION_NEXT       = 0x04,
    OTP_VFS_OLCP_OPERATION_GOTO       = 0x05,
    OTP_VFS_OLCP_OPERATION_ORDER      = 0x06,
    OTP_VFS_OLCP_OPERATION_REQ_NUM    = 0x07,
    OTP_VFS_OLCP_OPERATION_CLEAR_MARK = 0x08
  };

  // Object Access Control Point Operations
  enum OtsOacpOpCode
  {
    OTP_VFS_OACP_OPERATION_NONE         = 0x00,
    OTP_VFS_OACP_OPERATION_CREATE       = 0x01,
    OTP_VFS_OACP_OPERATION_DELETE       = 0x02,
    OTP_VFS_OACP_OPERATION_CAL_CHECKSUM = 0x03,
    OTP_VFS_OACP_OPERATION_EXECUTE      = 0x04,
    OTP_VFS_OACP_OPERATION_READ         = 0x05,
    OTP_VFS_OACP_OPERATION_WRITE        = 0x06,
    OTP_VFS_OACP_OPERATION_ABORT        = 0x07
  };

  // Object List Control Point Filter Conditions
  enum OtsFilterCondition
  {
    OTPS_FILTER_TYPE_NO_FILTER            = 0x00,
    OTPS_FILTER_TYPE_NAME_START_WITH      = 0x01,
    OTPS_FILTER_TYPE_NAME_END_WITH        = 0x02,
    OTPS_FILTER_TYPE_NAME_CONTAINS        = 0x03,
    OTPS_FILTER_TYPE_NAME_IS_EXACTLY      = 0x04,
    OTPS_FILTER_TYPE_OBJECT_TYPE          = 0x05,
    OTPS_FILTER_TYPE_CREATED_BETWEEN      = 0x06,
    OTPS_FILTER_TYPE_MODIFIED_BETWEEN     = 0x07,
    OTPS_FILTER_TYPE_CURRENT_SIZE_BETWEEN = 0x08,
    OTPS_FILTER_TYPE_ALLOC_SIZE_BETWEEN   = 0x09,
    OTPS_FILTER_TYPE_MARKED_OBJ           = 0x0A,
  };

  // List Sort Order Types for OLCP Order Operation
  enum OtsListSortOrder
  {
    OTS_ORDER_BY_NAME_ASCENDING          = 0x01,
    OTS_ORDER_BY_TYPE_ASCENDING          = 0x02,
    OTS_ORDER_BY_CURRENT_SIZE_ASCENDING  = 0x03,
    OTS_ORDER_BY_CREATE_TIME_ASCENDING   = 0x04,
    OTS_ORDER_BY_MODIFY_TIME_ASCENDING   = 0x05,
    OTS_ORDER_BY_NAME_DESCENDING         = 0x11,
    OTS_ORDER_BY_TYPE_DESCENDING         = 0x12,
    OTS_ORDER_BY_CURRENT_SIZE_DESCENDING = 0x13,
    OTS_ORDER_BY_CREATE_TIME_DESCENDING  = 0x14,
    OTS_ORDER_BY_MODIFY_TIME_DESCENDING  = 0x15,
  };

/** \name OLCP Response Value Bytes
 * Format of the OLCP response
 */
/**@{*/
#define OTP_RESPONSE_OPERATION_BYTE 0
#define OTP_RESPONSE_OPCODE_BYTE    1
#define OTP_RESPONSE_VALUE_BYTE     2
#define OTP_RESPONSE_PARAM_BYTE     3

/** \name OACP/OLCP Request Value Bits
 * Format of the OACP/OLCP request
 */
/**@{*/
#define OTP_REQUEST_OPCODE_BYTE 0
#define OTP_REQUEST_PARAM_BYTE  1

/** \name OLCP Command Operation Codes
 *
 */
/**@{*/
#define OTP_OLCP_OPCODE_RESPONSE 0X70

#define OTP_OACP_OPCODE_RESPONSE 0X60

/*!< \brief  ATT Application error codes */
#define ATT_OTP_ERR_WRITE_REQ_REJ    0x80 /*!< Value is invalid or not supported. */
#define ATT_OTP_ERR_OBJ_NOT_SELECTED 0x81 /*!< Current Object is an invalid object. */
#define ATT_OTP_ERR_CONCURRENT_LIMITE_EXCEED \
  0x82                                       /*!< Exceeded the concurrency limit of the service. */
#define ATT_OTP_ERR_NAME_ALREADY_EXISTS 0x83 /*!< Name is already in use. */

/** \name Object Transfer Server Feature Bits
 * Object Transfer Feature Bits of the Server Features Characteristic.
 */
/**@{*/
#define OTP_VFS_OACP_CREATE_FEATURE_BIT            (        1)
#define OTP_VFS_OACP_DELETE_FEATURE_BIT            (1ULL << 1)
#define OTP_VFS_OTP_OACP_CALC_CHECKSUM_FEATURE_BIT (1ULL << 2)
#define OTP_VFS_OACP_EXECUTE_FEATURE_BIT           (1ULL << 3)
#define OTP_VFS_OACP_READ_FEATURE_BIT              (1ULL << 4)
#define OTP_VFS_OACP_WRITE_FEATURE_BIT             (1ULL << 5)
#define ATT_OTP_ADD_DATA_TO_OBJ_FEATURE_BIT        (1ULL << 6)
#define OTP_VFS_TRUNCATION_OF_OBJ_FEATURE_BIT      (1ULL << 7)
#define OTP_VFS_PATCHING_OF_OBJ_FEATURE_BIT        (1ULL << 8)
#define OTP_VFS_OACP_ABORT_FEATURE_BIT             (1ULL << 9)
#define OTP_VFS_OLCP_GO_TO_FEATURE_BIT             (1ULL << 32)
#define OTP_VFS_OLCP_ORDER_FEATURE_BIT             (1ULL << 33)
#define OTP_VFS_OLCP_REQ_NUM_OF_OBJ_FEATURE_BIT    (1ULL << 34)
#define OTP_VFS_OLCP_CLEAR_MAKING_FEATURE_BIT      (1ULL << 35)

/*! \brief Set to all supported features */
#define OTP_ALL_FEATURES (0xF000003FFULL) /*!< All Supported Feature Mask */
/**@}*/

/** \name Object Transfer Properties Bits
 * Object Transfer Properties Bits of the Object Properties Characteristic.
 */
/**@{*/
#define OTP_OBJ_DELETE_PROPERTY   (1 << 0)
#define OTP_OBJ_EXECUTE_PROPERTY  (1 << 1)
#define OTP_OBJ_READ_PROPERTY     (1 << 2)
#define OTP_OBJ_WRITE_PROPERTY    (1 << 3)
#define OTP_OBJ_APPEND_PROPERTY   (1 << 4)
#define OTP_OBJ_TRUNCATE_PROPERTY (1 << 5)
#define OTP_OBJ_PATCH_PROPERTY    (1 << 6)
#define OTP_OBJ_MARK_PROPERTY     (1 << 7)

/** \name  Mode Parameter for OACP Write Op Code
 * Mode Parameter Bits for OACP Write Op Code
 */
/**@{*/
#define OACP_WRITE_MODE_TRUNCATE_BIT (1 << 1)

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Object Transfer Service Handles
 *
 */
/**@{*/
#define OTPS_START_HDL 0x500              /*!< Start handle. */
#define OTPS_END_HDL   (OTPS_MAX_HDL - 1) /*!< End handle. */

  /**************************************************************************************************
   Handles
  **************************************************************************************************/

  /*! \brief Object transfer service handles. */
  enum
  {
    OTPS_SVC_HDL = OTPS_START_HDL, /*!< Object Transfer Service Declaration. */

    OTPS_OTS_FEATURE_CH_HDL, /*!< OTPS OTS Feature Characteristic Declaration. */
    OTPS_OTS_FEATURE_HDL,    /*!< OTPS OTS Feature Characteristic Value. */

    OTPS_OBJECT_NAME_CH_HDL, /*!< OTPS Object Name Characteristic Declaration. */
    OTPS_OBJECT_NAME_HDL,    /*!< OTPS Object Name Characteristic Value. */

    OTPS_OBJECT_TYPE_CH_HDL, /*!< OTPS Object Type Characteristic Declaration. */
    OTPS_OBJECT_TYPE_HDL,    /*!< OTPS Object Type Characteristic Value. */

    OTPS_OBJECT_SIZE_CH_HDL, /*!< OTPS Object Size Characteristic Declaration. */
    OTPS_OBJECT_SIZE_HDL,    /*!< OTPS Object Size Characteristic Value. */

    OTPS_OBJECT_FIRST_CREATED_CH_HDL, /*!< OTPS Object First-Created Characteristic Declaration. */
    OTPS_OBJECT_FIRST_CREATED_HDL,    /*!< OTPS Object First-Created Characteristic Value. */

    OTPS_OBJECT_LAST_MODIFIED_CH_HDL, /*!< OTPS Object Last-Modified Characteristic Declaration. */
    OTPS_OBJECT_LAST_MODIFIED_HDL,    /*!< OTPS Object Last-Modified Characteristic Value. */

    OTPS_OBJECT_ID_CH_HDL, /*!< OTPS Object ID Characteristic Declaration. */
    OTPS_OBJECT_ID_HDL,    /*!< OTPS Object ID Characteristic Value. */

    OTPS_OBJECT_PROPERTIES_CH_HDL, /*!< OTPS Object Properties Characteristic Declaration. */
    OTPS_OBJECT_PROPERTIES_HDL,    /*!< OTPS Object Properties Characteristic Value. */

    OTPS_OACP_CH_HDL,     /*!< OTPS Object Action Control Point Characteristic Declaration. */
    OTPS_OACP_HDL,        /*!< OTPS Object Action Control Point Characteristic Value. */
    OTPS_OACP_CH_CCC_HDL, /*!< OTPS Object Action Control Point CCCD.*/

    OTPS_OLCP_CH_HDL,     /*!< OTPS Object List Control Point Characteristic Declaration. */
    OTPS_OLCP_HDL,        /*!< OTPS Object List Control Point Characteristic Value. */
    OTPS_OLCP_CH_CCC_HDL, /*!< OTPS Object List Control Point CCCD.*/

    OTPS_OBJECT_LIST_FILTER_1_CH_HDL, /*!< OTPS Object List Filter Characteristic Declaration. */
    OTPS_OBJECT_LIST_FILTER_1_HDL,    /*!< OTPS Object List Filter Characteristic Value. */

    OTPS_OBJECT_LIST_FILTER_2_CH_HDL, /*!< OTPS Object List Filter Characteristic Declaration. */
    OTPS_OBJECT_LIST_FILTER_2_HDL,    /*!< OTPS Object List Filter Characteristic Value. */

    OTPS_OBJECT_LIST_FILTER_3_CH_HDL, /*!< OTPS Object List Filter Characteristic Declaration. */
    OTPS_OBJECT_LIST_FILTER_3_HDL,    /*!< OTPS Object List Filter Characteristic Value. */

    OTPS_OBJECT_CHANGED_CH_HDL,     /*!< OTPS Object Changed Characteristic Declaration. */
    OTPS_OBJECT_CHANGED_HDL,        /*!< OTPS Object Changed Characteristic Value. */
    OTPS_OBJECT_CHANGED_CH_CCC_HDL, /*!< OTPS Object Changed CCCD.*/

    OTPS_MAX_HDL
  };
  /**@}*/

  /**************************************************************************************************
    Function Declarations
  **************************************************************************************************/

  /*************************************************************************************************/
  /*!
   *  \brief  Add the service to the attribute server.
   *
   *  \return None.
   */
  /*************************************************************************************************/
  void SvcOtpsAddGroup(void);

  /*************************************************************************************************/
  /*!
   *  \brief  Remove the service from the attribute server.
   *
   *  \return None.
   */
  /*************************************************************************************************/
  void SvcOtpsRemoveGroup(void);

  /*************************************************************************************************/
  /*!
   *  \brief  Register a write callback functions for the ATT Group.
   *
   *  \param  writeCback   Write callback function.
   */
  /*************************************************************************************************/
  void SvcOtpsRegister(attsWriteCback_t writeCback, attsReadCback_t readCback);

  /*! \} */ /* OBJECT_TRANSFER_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_OTP_H */
