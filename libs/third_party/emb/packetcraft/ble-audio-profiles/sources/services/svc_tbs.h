/*************************************************************************************************/
/*!
 *  \file   svc_tbs.h
 *
 *  \brief  Example Telephone Bearer Service implementation.
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

#ifndef SVC_TBS_H
#define SVC_TBS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup TELEPHONE_BEARER_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Characteristic value lengths */
#define TBS_MAX_STRING_LEN               64
#define TBS_PROVIDER_NAME_LEN            TBS_MAX_STRING_LEN
#define TBS_UCI_LEN                      5
#define TBS_URI_SCHEME_LIST_LEN          TBS_MAX_STRING_LEN
#define TBS_STATUS_FLAG_LEN              2

#ifndef TBS_CALL_INDEX_MAX
#define TBS_CALL_INDEX_MAX               3
#endif

#ifndef TBS_URI_LEN_MAX
#define TBS_URI_LEN_MAX                  20
#endif

#define TBS_CALL_LEN                     (4 + TBS_URI_LEN_MAX)
#define TBS_CURR_CALL_LEN                ((TBS_CALL_INDEX_MAX) * (TBS_CALL_LEN))

#define TBS_LIST_CURR_CALLS_LEN          (4 + TBS_MAX_STRING_LEN)

#define TBS_CALL_INDEX_LEN               1
#define TBS_CCID_LEN                     1
#define TBS_CALL_STATE_LEN               3
#define TBS_CTRL_PT_OPCODE_LEN           1
#define TBS_CONTROL_PT_OC_SUP_LEN        1

#if (TBS_URI_LEN_MAX <  TBS_CALL_INDEX_MAX)
#define TBS_CONTROL_POINT_LEN            (1 + TBS_CTRL_PT_OPCODE_LEN + TBS_CALL_INDEX_MAX)
#else
#define TBS_CONTROL_POINT_LEN            (1 + TBS_CTRL_PT_OPCODE_LEN + TBS_URI_LEN_MAX)
#endif

#define TBS_INCOMING_CALL_LEN            (1 + TBS_MAX_STRING_LEN)

/* Notification value lengths */
#define TBS_CONTROL_POINT_NTF_LEN        2

/* Call control states */
#define TBS_STATE_INCOMING               0x00
#define TBS_STATE_DIALING                0x01
#define TBS_STATE_ALERTING               0x02
#define TBS_STATE_ACTIVE                 0x03
#define TBS_STATE_LOCAL_HELD             0x04
#define TBS_STATE_REMOTE_HELD            0x05
#define TBS_STATE_LOCAL_REMOTE_HELD      0x06
#define TBS_STATE_IDLE                   0xFF

/* Opcode set mask */
#define TBS_CTRL_PT_OP_SET_MASK          0x70

/* Call control point opcode supported bits */
#define TBS_CO_SUPPORT_ALL               0x001FFFFF

/* Call Control Point Opcodes */
#define TBS_CCP_OPC_ACCEPT               0x00
#define TBS_CCP_OPC_TERMINATE            0x01
#define TBS_CCP_OPC_LOCAL_HOLD           0x02
#define TBS_CCP_OPC_LOCAL_RETRIEVE       0x03
#define TBS_CCP_OPC_ORIGINATE            0x04
#define TBS_CCP_OPC_JOIN                 0x05

/* Call Control Point Result Code */
#define TBS_CCP_RES_SUCCESS              0x00
#define TBS_CCP_RES_UNSUPPORTED_OPC      0x01
#define TBS_CCP_RES_NOT_POSSIBLE         0x02
#define TBS_CCP_RES_INVALID_CALL_NDX     0x03
#define TBS_CCP_RES_STATE_MISMATCH       0x04
#define TBS_CCP_RES_LACK_RESOURCE        0x05
#define TBS_CCP_RES_INVALID_URI_TX       0x06

/* Call Control Point Optional Opcode Behavior bits */
#define TBS_CCP_OPT_LOCAL_HOLD           0x00
#define TBS_CCP_OPT_JOIN                 0x01

/* Termination Reason Codes */
#define TBS_TERMINATION_OPC_BAD_URI      0x00 /*!< \brief The URI value used to originate a call was formed improperly. */
#define TBS_TERMINATION_OPC_CALL_FAILED  0x01 /*!< \brief The call failed. */
#define TBS_TERMINATION_OPC_REMOTE_ENDED 0x02 /*!< \brief The remote party ended the call. */
#define TBS_TERMINATION_OPC_SERVER_ENDED 0x03 /*!< \brief The call ended from the server. */
#define TBS_TERMINATION_OPC_LINE_BUSY    0x04 /*!< \brief The line was busy. */
#define TBS_TERMINATION_OPC_NET_CNGST    0x05 /*!< \brief Network congestion. */
#define TBS_TERMINATION_OPC_CLIENT_ENDED 0x06 /*!< \brief The client terminated the call. */
#define TBS_TERMINATION_OPC_NO_SERVICE   0x07 /*!< \brief No service. */
#define TBS_TERMINATION_OPC_NO_ANSWER    0x08 /*!< \brief No answer. */

/* Bearer Technology */
#define TBS_BEARER_TECHNOLOGY_RESERVED   0x00 /*!< \brief Reserved. */
#define TBS_BEARER_TECHNOLOGY_3G         0x01 /*!< \brief 3G. */
#define TBS_BEARER_TECHNOLOGY_4G         0x02 /*!< \brief 4G. */
#define TBS_BEARER_TECHNOLOGY_LTE        0x03 /*!< \brief LTE. */
#define TBS_BEARER_TECHNOLOGY_WIFI       0x04 /*!< \brief Wi-Fi. */
#define TBS_BEARER_TECHNOLOGY_5G         0x05 /*!< \brief 5G. */
#define TBS_BEARER_TECHNOLOGY_GSM        0x06 /*!< \brief GSM. */
#define TBS_BEARER_TECHNOLOGY_CDMA       0x07 /*!< \brief CDMA. */
#define TBS_BEARER_TECHNOLOGY_2G         0x08 /*!< \brief 2G. */
#define TBS_BEARER_TECHNOLOGY_WCDMA      0x09 /*!< \brief WCDMA. */

/* CCCD Index Offsets */
typedef enum
{
  TBS_NTF_IDX_NAME,
  TBS_NTF_IDX_TECH,
  TBS_NTF_IDX_URI_LIST,
  TBS_NTF_IDX_CURR_CALLS,
  TBS_NTF_IDX_STATUS_FLAG,
  TBS_NTF_IDX_STATE,
  TBS_NTF_IDX_CTRL_PT,
  TBS_NTF_IDX_TERM_RSN,
  TBS_NTF_IDX_INC_CALL,
  TBS_NTF_IDX_MAX
} TbsNtfIdx_t;

/* CCCD Index Offsets */
#define TBS_CCCD_IDX_NAME               TBS_NTF_IDX_NAME
#define TBS_CCCD_IDX_TECH               TBS_NTF_IDX_TECH
#define TBS_CCCD_IDX_URI_LIST           TBS_NTF_IDX_URI_LIST
#define TBS_CCCD_IDX_CURR_CALLS         TBS_NTF_IDX_CURR_CALLS
#define TBS_CCCD_IDX_STATUS_FLAG        TBS_NTF_IDX_STATUS_FLAG
#define TBS_CCCD_IDX_STATE              TBS_NTF_IDX_STATE
#define TBS_CCCD_IDX_CTRL_PT            TBS_NTF_IDX_CTRL_PT
#define TBS_CCCD_IDX_TERM_RSN           TBS_NTF_IDX_TERM_RSN
#define TBS_CCCD_IDX_INC_CALL           TBS_NTF_IDX_INC_CALL

/* Number of CCCD attributes per service */
#define TBS_NUM_CCCD                    TBS_NTF_IDX_MAX

/* Max supported call control services for the device. */
#define TBS_SVC_MAX                     2

 /* GTBS Reserved Service ID */
#define GTBS_SVC_ID                     0

/* Unknown TBS service ID */
#define TBS_SVC_UNKNOWN                 0xFF

/*!< \brief  ATT Application error codes */
#define TBS_ERR_VALUE_CHANGED           0x80  /*!< Value Changed During Read Long */

/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Telephone Bearer Service Handles
 *
 */
/**@{*/
/*!< \brief Start handle. */
#define TBS_START_HDL                0x0900              /*!< TBS start handle */
#define TBS_END_HDL                  (TBS_MAX_HDL - 1)   /*!< TBS end handle */
/**@}*/

/*!< \brief Number of TBS handles. */
#define TBS_NUM_HDL                     (TBS_MAX_HDL - TBS_START_HDL)

/* Calculate an value handle from a base value handle and a service id. */
#define TBS_HANDLE_ID(base, index)      (base + TBS_END_HDL * index)

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Telephone Bearer Service Handles macro */
enum
{
  TBS_SVC_HDL = TBS_START_HDL, /*!< \brief Telephone Bearer Service declaration */
  TBS_NAME_CH_HDL,             /*!< \brief Telephone Bearer Provider name characteristic */
  TBS_NAME_HDL,                /*!< \brief Telephone Bearer Provider name */
  TBS_NAME_CH_CCC_HDL,         /*!< \brief Telephone Bearer Provider name CCCD */
  TBS_UCI_CH_HDL,              /*!< \brief Bearer UCI characteristic */
  TBS_UCI_HDL,                 /*!< \brief Bearer UCI */
  TBS_TECH_CH_HDL,             /*!< \brief Bearer Technology characteristic */
  TBS_TECH_HDL,                /*!< \brief Bearer Technology */
  TBS_TECH_CH_CCC_HDL,         /*!< \brief Bearer Technology CCCD */
  TBS_URI_LIST_CH_HDL,         /*!< \brief Bearer URI Schemes Supported List characteristic */
  TBS_URI_LIST_HDL,            /*!< \brief Bearer URI Schemes Supported List */
  TBS_URI_LIST_CH_CCC_HDL,     /*!< \brief Bearer URI Schemes Supported List CCCD */
  TBS_CURR_CALLS_CH_HDL,       /*!< \brief Bearer List Current Calls characteristic */
  TBS_CURR_CALLS_HDL,          /*!< \brief Bearer List Current Calls */
  TBS_CURR_CALLS_CH_CCC_HDL,   /*!< \brief Bearer List Current Calls CCCD */
  TBS_CCID_CH_HDL,             /*!< \brief Content Control ID (CCID) characteristic */
  TBS_CCID_HDL,                /*!< \brief Content Control ID (CCID) */
  TBS_STATUS_FLAG_CH_HDL,      /*!< \brief Status Flag characteristic */
  TBS_STATUS_FLAG_HDL,         /*!< \brief Status Flag */
  TBS_STATUS_FLAG_CH_CCC_HDL,  /*!< \brief Status Flag CCCD */
  TBS_STATE_CH_HDL,            /*!< \brief Call state characteristic */
  TBS_STATE_HDL,               /*!< \brief Call state */
  TBS_STATE_CH_CCC_HDL,        /*!< \brief Call state CCCD */
  TBS_CTRL_PT_CH_HDL,          /*!< \brief Call control point characteristic */
  TBS_CTRL_PT_HDL,             /*!< \brief Call control point */
  TBS_CTRL_PT_CH_CCC_HDL,      /*!< \brief Call control point CCCD */
  TBS_CTRL_PT_OC_SUP_CH_HDL,   /*!< \brief Call control point opcodes supported characteristic */
  TBS_CTRL_PT_OC_SUP_HDL,      /*!< \brief Call control point opcodes supported */
  TBS_TERM_RSN_CH_HDL,         /*!< \brief Termination Reason characteristic */
  TBS_TERM_RSN_HDL,            /*!< \brief Termination Reason */
  TBS_TERM_RSN_CH_CCC_HDL,     /*!< \brief Termination Reason CCCD */
  TBS_INC_CALL_CH_HDL,         /*!< \brief Incoming Call characteristic */
  TBS_INC_CALL_HDL,            /*!< \brief Incoming Call */
  TBS_INC_CALL_CH_CCC_HDL,     /*!< \brief Incoming Call CCCD */
  TBS_MAX_HDL                  /*!< \brief Maximum handle. */
};
/**@}*/

/*! \brief Telephone Bearer Service Handles */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTbsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTbsRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *  \param  readCback   Read callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTbsCbackRegister(attsWriteCback_t writeCback, attsReadCback_t readCback);

/*! \} */    /* TELEPHONE_BEARER_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_TBS_H */
