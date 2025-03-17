/*************************************************************************************************/
/*!
 *  \file     ccpc_api.h
 *
 *  \brief  Call Control Profile Client API.
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

#ifndef CCPC_API_H
#define CCPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup CALL_CONTROL_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/
/*! \brief Call Control Service Handles */
#define CCPC_DISC_HANDLES(_a, _b)                                                                               \
enum                                                                                                            \
{                                                                                                               \
  CCPC_TBS_NAME_HDL_IDX_##_a = _b,                  /*!< \brief Bearer Provider Name */                         \
  CCPC_TBS_NAME_CCC_HDL_IDX_##_a,                   /*!< \brief Bearer Provider Name CCC */                     \
  CCPC_TBS_UCI_HDL_IDX_##_a,                        /*!< \brief Bearer UCI */                                   \
  CCPC_TBS_TECH_HDL_IDX_##_a,                       /*!< \brief Bearer Technology */                            \
  CCPC_TBS_TECH_CCC_HDL_IDX_##_a,                   /*!< \brief Bearer Technology CCC */                        \
  CCPC_TBS_URI_SCHEMES_HDL_IDX_##_a,                /*!< \brief Bearer URI Schemes List */                      \
  CCPC_TBS_URI_SCHEMES_CCC_HDL_IDX_##_a,            /*!< \brief Bearer URI Schemes List CCC */                  \
  CCPC_TBS_SIGNAL_STRENGTH_HDL_IDX_##_a,            /*!< \brief Bearer Signal Strength */                       \
  CCPC_TBS_SIGNAL_STRENGTH_CCC_HDL_IDX_##_a,        /*!< \brief Bearer Signal Strength CCC */                   \
  CCPC_TBS_SIGNAL_STRENGTH_INT_HDL_IDX_##_a,        /*!< \brief Bearer Signal Strength Reporting Interval*/     \
  CCPC_TBS_SIGNAL_STRENGTH_INT_CCC_HDL_IDX_##_a,    /*!< \brief Bearer Signal Strength Reporting Interval CCC*/ \
  CCPC_TBS_LIST_CURR_CALLS_HDL_IDX_##_a,            /*!< \brief Bearer List Current Calls */                    \
  CCPC_TBS_LIST_CURR_CALLS_CCC_HDL_IDX_##_a,        /*!< \brief Bearer List Current Calls CCC */                \
  CCPC_TBS_CCID_HDL_IDX_##_a,                       /*!< \brief Call Control CCID */                            \
  CCPC_TBS_STATUS_FLAGS_HDL_IDX_##_a,               /*!< \brief Status Flags */                                 \
  CCPC_TBS_STATUS_FLAGS_CCC_HDL_IDX_##_a,           /*!< \brief Status Flags CCC */                             \
  CCPC_TBS_INCOMING_URI_HDL_IDX_##_a,               /*!< \brief Incoming Call Bearer URI */                     \
  CCPC_TBS_INCOMING_URI_CCC_HDL_IDX_##_a,           /*!< \brief Incoming Call Bearer URI CCC */                 \
  CCPC_TBS_STATE_HDL_IDX_##_a,                      /*!< \brief Call Control State */                           \
  CCPC_TBS_STATE_CCC_HDL_IDX_##_a,                  /*!< \brief Call Control State CCC */                       \
  CCPC_TBS_CTRL_PT_HDL_IDX_##_a,                    /*!< \brief Call Control Point */                           \
  CCPC_TBS_CTRL_PT_CCC_HDL_IDX_##_a,                /*!< \brief Call Control Point CCC */                       \
  CCPC_TBS_OPCODE_SUP_HDL_IDX_##_a,                 /*!< \brief Call Control Point Op Code Supported */         \
  CCPC_TBS_TERMINATION_REASON_HDL_IDX_##_a,         /*!< \brief Termination Reason */                           \
  CCPC_TBS_TERMINATION_REASON_CCC_HDL_IDX_##_a,     /*!< \brief Termination Reason CCC */                       \
  CCPC_TBS_INCOMING_CALL_URI_HDL_IDX_##_a,          /*!< \brief Incoming Call */                                \
  CCPC_TBS_INCOMING_CALL_URI_CCC_HDL_IDX_##_a,      /*!< \brief Incoming Call CCC */                            \
  CCPC_TBS_CALL_FRIENDLY_NAME_HDL_IDX_##_a,         /*!< \brief Call Friendly Name */                           \
  CCPC_TBS_CALL_FRIENDLY_NAME_CCC_HDL_IDX_##_a,     /*!< \brief Call Friendly Name CCC */                       \
  CCPC_TBS_HDL_LIST_LEN_##_a,                                                                                   \
};

/*! \brief TBS handle sub-indexes */
#define CCPC_TBS_NAME_HDL_IDX                       CCPC_TBS_NAME_HDL_IDX_0
#define CCPC_TBS_NAME_CCC_HDL_IDX                   CCPC_TBS_NAME_CCC_HDL_IDX_0
#define CCPC_TBS_UCI_HDL_IDX                        CCPC_TBS_UCI_HDL_IDX_0
#define CCPC_TBS_TECH_HDL_IDX                       CCPC_TBS_TECH_HDL_IDX_0
#define CCPC_TBS_TECH_CCC_HDL_IDX                   CCPC_TBS_TECH_CCC_HDL_IDX_0
#define CCPC_TBS_URI_SCHEMES_HDL_IDX                CCPC_TBS_URI_SCHEMES_HDL_IDX_0
#define CCPC_TBS_URI_SCHEMES_CCC_HDL_IDX            CCPC_TBS_URI_SCHEMES_CCC_HDL_IDX_0
#define CCPC_TBS_SIGNAL_STRENGTH_HDL_IDX            CCPC_TBS_SIGNAL_STRENGTH_HDL_IDX_0
#define CCPC_TBS_SIGNAL_STRENGTH_CCC_HDL_IDX        CCPC_TBS_SIGNAL_STRENGTH_CCC_HDL_IDX_0
#define CCPC_TBS_SIGNAL_STRENGTH_INT_HDL_IDX        CCPC_TBS_SIGNAL_STRENGTH_INT_HDL_IDX_0
#define CCPC_TBS_SIGNAL_STRENGTH_INT_CCC_HDL_IDX    CCPC_TBS_SIGNAL_STRENGTH_INT_CCC_HDL_IDX_0
#define CCPC_TBS_LIST_CURR_CALLS_HDL_IDX            CCPC_TBS_LIST_CURR_CALLS_HDL_IDX_0
#define CCPC_TBS_LIST_CURR_CALLS_CCC_HDL_IDX        CCPC_TBS_LIST_CURR_CALLS_CCC_HDL_IDX_0
#define CCPC_TBS_CCID_HDL_IDX                       CCPC_TBS_CCID_HDL_IDX_0
#define CCPC_TBS_STATUS_FLAGS_HDL_IDX               CCPC_TBS_STATUS_FLAGS_HDL_IDX_0
#define CCPC_TBS_STATUS_FLAGS_CCC_HDL_IDX           CCPC_TBS_STATUS_FLAGS_CCC_HDL_IDX_0
#define CCPC_TBS_INCOMING_URI_HDL_IDX               CCPC_TBS_INCOMING_URI_HDL_IDX_0
#define CCPC_TBS_INCOMING_URI_CCC_HDL_IDX           CCPC_TBS_INCOMING_URI_CCC_HDL_IDX_0
#define CCPC_TBS_STATE_HDL_IDX                      CCPC_TBS_STATE_HDL_IDX_0
#define CCPC_TBS_STATE_CCC_HDL_IDX                  CCPC_TBS_STATE_CCC_HDL_IDX_0
#define CCPC_TBS_CTRL_PT_HDL_IDX                    CCPC_TBS_CTRL_PT_HDL_IDX_0
#define CCPC_TBS_CTRL_PT_CCC_HDL_IDX                CCPC_TBS_CTRL_PT_CCC_HDL_IDX_0
#define CCPC_TBS_OPCODE_SUP_HDL_IDX                 CCPC_TBS_OPCODE_SUP_HDL_IDX_0
#define CCPC_TBS_TERMINATION_REASON_HDL_IDX         CCPC_TBS_TERMINATION_REASON_HDL_IDX_0
#define CCPC_TBS_TERMINATION_REASON_CCC_HDL_IDX     CCPC_TBS_TERMINATION_REASON_CCC_HDL_IDX_0
#define CCPC_TBS_INCOMING_CALL_URI_HDL_IDX          CCPC_TBS_INCOMING_CALL_URI_HDL_IDX_0
#define CCPC_TBS_INCOMING_CALL_URI_CCC_HDL_IDX      CCPC_TBS_INCOMING_CALL_URI_CCC_HDL_IDX_0
#define CCPC_TBS_CALL_FRIENDLY_NAME_HDL_IDX         CCPC_TBS_CALL_FRIENDLY_NAME_HDL_IDX_0
#define CCPC_TBS_CALL_FRIENDLY_NAME_CCC_HDL_IDX     CCPC_TBS_CALL_FRIENDLY_NAME_CCC_HDL_IDX_0
#define CCPC_TBS_HDL_LIST_LEN                       CCPC_TBS_HDL_LIST_LEN_0

/* Number of audio input control service handles */
#define CCPC_TBS_HDL_LIST_LEN                       CCPC_TBS_HDL_LIST_LEN_0

/*! \brief Enumeration of handle indexes of characteristics to be discovered - GTBS/TBS */
/* NOTE: A device can have multiple TBS instances.
 *       Add service handle indexes as needed. */

CCPC_DISC_HANDLES(0, 0);
CCPC_DISC_HANDLES(1, CCPC_TBS_HDL_LIST_LEN_0);
// CCPC_DISC_HANDLES(2, CPC_TBS_HDL_LIST_LEN_1);

#define CCPC_TBS_INSTANCES 2

#define CCPC_NAME_LEN                 ATT_DEFAULT_PAYLOAD_LEN * 3
#define CCPC_UCI_LEN                  8
#define CCPC_URI_LEN                  ATT_DEFAULT_PAYLOAD_LEN * 2
#define CCPC_URI_SCHEMES_LEN          ATT_DEFAULT_PAYLOAD_LEN * 3
#define CCPC_CURR_CALLS_LIST_LEN      8
#define CCPC_CALL_STATE_LIST_LEN      8
#define CCPC_STATUS_FLAGS_LEN         2
#define CCPC_READ_LONG_BUF_LEN        sizeof(ccpcCurrCallsList_t) * CCPC_CURR_CALLS_LIST_LEN

/**************************************************************************************************
* Data Types
* *************************************************************************************************/

/*! \brief Call Control Point Notification type. */
typedef struct
{
  uint8_t  requestedOpcode;                /*!< \brief Requested Opcode */
  uint8_t  callIndex;                      /*!< \brief Call Index */
  uint8_t  resultCode;                     /*!< \brief Result Code */
} ccpcCtrlPtNtf_t;

/*! \brief Application callback type. */
typedef void(*ccpcCback_t)(dmConnId_t connId, ccpcCtrlPtNtf_t* notification);

/*! \brief Bearer List Current Calls structure */
typedef struct
{
  uint8_t  listItemLength;              /*!< \brief Length of the list item */
  uint8_t  callIndex;                   /*!< \brief Call Index */
  uint8_t  callState;                   /*!< \brief Call State */
  uint8_t  callFlags;                   /*!< \brief Call Flags */
  uint8_t  callUri[CCPC_URI_LEN];       /*!< \brief Call URI */
} ccpcCurrCallsList_t;

/*! \brief Incoming Call URI structure */
typedef struct
{
  uint8_t  callIndex;                   /*!< \brief Call Index */
  uint8_t  callUri[CCPC_URI_LEN];       /*!< \brief Call URI */
} ccpcCallUri_t;

/*! \brief Incoming Call URI structure */
typedef struct
{
  uint8_t  callIndex;                   /*!< \brief Call Index */
  uint8_t  callFName[CCPC_NAME_LEN];    /*!< \brief Call URI */
} ccpcCallFriendlyName_t;

/*! \brief Call State structure */
typedef struct
{
  uint8_t  callIndex;                   /*!< \brief Call Index */
  uint8_t  state;                       /*!< \brief Call State */
  uint8_t  flags;                       /*!< \brief Call Flags */
} ccpcCallState_t;

/*! \brief Termination Reason structure */
typedef struct
{
  uint8_t  callIndex;                   /*!< \brief Call Index */
  uint8_t  reasonCode;                  /*!< \brief Reason Code */
} ccpcTermintation_t;

/*! \brief Structure for storing the characteristics read by CCP */
typedef struct
{
uint8_t bearerProviderName[CCPC_NAME_LEN];                              /*!< \brief Bearer Provider Name */
  uint8_t bearerProviderNameLen;                                        /*!< \brief Bearer Provider Name Length */
  uint8_t bearerUci[CCPC_UCI_LEN];                                      /*!< \brief Bearer UCI */
  uint8_t bearerTech;                                                   /*!< \brief Bearer Technology */
  uint8_t bearerUriSchemesSup[CCPC_URI_SCHEMES_LEN];                    /*!< \brief Bearer URI Schemes Supported */
  uint8_t bearerUriSchemesSupLen;                                       /*!< \brief Bearer URI Schemes Supported Length */
  uint8_t bearerSignalStr;                                              /*!< \brief Bearer Signal Strength */
  uint8_t bearerSignalStrRepInt;                                        /*!< \brief Bearer Signal Strength Reporting Interval */
  ccpcCurrCallsList_t bearerCurrentCallsList[CCPC_CURR_CALLS_LIST_LEN]; /*!< \brief Bearer List Current Calls */
  uint8_t bearerCurrentCallsLen;                                        /*!< \brief Bearer List Current Calls Length */
  uint8_t ccid;                                                         /*!< \brief Call Control ID */
  ccpcCallUri_t incCallBearerUri;                                       /*!< \brief Incoming Call Bearer URI */
  uint8_t incCallBearerUriLen;                                          /*!< \brief Incoming Call Bearer URI Length */
  uint16_t statusFlags;                                                 /*!< \brief Status Flags */
  ccpcCallState_t callState[CCPC_CALL_STATE_LIST_LEN];                  /*!< \brief Call State */
  uint8_t callStateLen;                                                 /*!< \brief Call State Length */
  uint16_t optionalOpCodes;                                             /*!< \brief Optional OpCodes */
  ccpcTermintation_t terminationReason;                                 /*!< \brief Termination Reason */
  ccpcCallUri_t incCallUri;                                             /*!< \brief Incoming Call URI */
  uint8_t incCallUriLen;                                                /*!< \brief Incoming Call URI Length */
  ccpcCallFriendlyName_t callFriendlyName;                              /*!< \brief Call Friendly Name */
  uint8_t callFriendlyNameLen;                                          /*!< \brief Call Friendly Name Length */
  ccpcCtrlPtNtf_t ctrlPtNtf;                                            /*!< \brief Last Received Call Control Point Notification */
} ccpcTbsDataCb_t;


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a Call Control service.
 *          Parameter pHdlList must point to an array of length \ref CCPC_TBS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     TBS service identifier.
 *  \param  pHdlList  Pointer to array to hold discovered handles.
 *  \param  cont      true if searching for more services, false if searching for the first time.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcTbsDiscover(dmConnId_t connId, uint8_t svcId, uint16_t *pHdlList, bool cont);

/*************************************************************************************************/
/*!
 *  \brief  Set service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     TBS service identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can set restored handles after restablishing a link with a bonded device.
 */
/*************************************************************************************************/
void CcpcTbsSetHandles(dmConnId_t connId, uint8_t svcId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for a Generic Media Control service.
 *          Parameter pHdlList must point to an array of length \ref CCPC_TBS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     TBS service identifier.
 *  \param  pHdlList  Pointer to array to hold discovered handles.
 *  \param  cont      true if searching for more services, false if searching for the first time.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcGtbsDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Read the bearer provider name attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadProviderName(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Read the UCI attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadUci(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the Bearer Technology attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadBearerTechnology(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the URI Schemes List attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadUriSchemesList(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Read the signal strength attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadSignalStrength(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the signal strength reporting interval attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadSignalStrengthReportingInterval(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the track position attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadListCurrentCalls(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Read the Content Control ID (CCID) attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadCcid(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the incoming call bearer URI attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadIncomingCallBearerUri(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Read the status flag attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadStatusFlags(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the state attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadState(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Read the control point opcodes supported attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadCtrlPtOpcodesSupported(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Read the incoming call attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */

/*************************************************************************************************/
void CcpcReadIncomingCall(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Read the call friendly name attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcReadCallFriendlyName(dmConnId_t connId, uint8_t svcId, bool isLong);

/*************************************************************************************************/
/*!
 *  \brief  Write to the media control point attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  opcode        Opcode.
 *  \param  parameter     Opcode specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcWriteControlPt(dmConnId_t connId, uint8_t svcId, uint8_t opcode, const uint8_t *pParam,
  uint8_t paramLen);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  opcode        Opcode.
 *  \param  parameter     Opcode specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcAcceptCall(dmConnId_t connId, uint8_t svcId, uint8_t callIdx);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  opcode        Opcode.
 *  \param  parameter     Opcode specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcTerminateCall(dmConnId_t connId, uint8_t svcId, uint8_t callIdx);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute. This function is used to hold a call.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  callIdx       Call index.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcHoldCall(dmConnId_t connId, uint8_t svcId, uint8_t callIdx);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute. This function is used to retrieve a call.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  callIdx       Call index.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcRetrieveCall(dmConnId_t connId, uint8_t svcId, uint8_t callIdx);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  opcode        Opcode.
 *  \param  parameter     Opcode specific parameter.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcOriginateCall(dmConnId_t connId, uint8_t svcId, uint8_t *pUri, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute. This function is used to join a call.
 *
 * \param  connId        Connection identifier.
 * \param  svcId         TBS service identifier.
 * \param  pCallIdx      Call index list.
 * \param  len           Length of the call index.
 *
 * \return None.
 */
 /*************************************************************************************************/
void CcpcJoinCall(dmConnId_t connId, uint8_t svcId, const uint8_t* pCallIdx, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Write to the call control point attribute. This function is used to set the signal
 *  strength reporting interval.
 *
 *  \param  connId        Connection identifier.
 *  \param  svcId         TBS service identifier.
 *  \param  interval      Reporting interval.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcSetSignalStrengthReportingInterval(dmConnId_t connId, uint8_t svcId, uint8_t* interval);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the CCPC of System Events.
 *
 *  \param  pEvt   Pointer to the Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CcpcProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Get the data from a specified TBS instance.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     TBS service identifier.
 *
 *  \return Pointer to the TBS data structure.
 */
 /*************************************************************************************************/
ccpcTbsDataCb_t* CcpcGetTbsData(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in a read response, read long response, notification,
 *          or indication message.
 *
 *  \param  pMsg      Pointer to the ATT message.
 *
 *  \return \ref ATT_SUCCESS if handle is found and length is correct, \ref ATT_ERR_NOT_FOUND
 *          if the handle is not found, \ref ATT_ERR_LENGTH if the length is incorrect.
 */
 /*************************************************************************************************/
uint8_t CcpcValueUpdate(attEvt_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Perform a read long request on the given handle for a TBS/GTBS characteristic.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     TBS service identifier.
 *
 *  \param  handle    Attribute handle index.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcReadLongReq(dmConnId_t connId, uint8_t svcId, uint16_t handleIndex);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message. Parameter pHdlList must point to an array of length \ref CCPC_TBS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  connId    Connection identifier.
 *  \param  handle    Attribute Handle
 *  \param  pBuf      Pointer to the buffer containing the raw received value.
 *  \param  len       Length of the received value.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
 /*************************************************************************************************/
uint8_t CcpcTbsProccessValue(dmConnId_t connId, uint16_t handle, uint8_t* pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Reset the saved tbsDataCb structure for the given service.
 *
 *  \param  connId    Connection identifier.
 *  \param  svcId     TBS service identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcResetTbsData(dmConnId_t connId, uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Process Call Control Point Indications or Notifications.
 *
 *  \param  connId    Connection identifier.
 *  \param  pValue    Pointer to the buffer containing the raw received value.
 *  \param  len       Length of the received value.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcCtrlPtIndNtfHandler(dmConnId_t connId, uint8_t* pValue, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback function for CCP indications or notifications.
 *
 *  \param  handlerId    Handler ID.
 *  \param  handler      Callback function.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcCbackRegister(dmConnId_t connId, ccpcCback_t handler);

/*************************************************************************************************/
/*!
 *  \brief  Delete registered callback function for the specified connection ID.
 *
 *  \param  handlerId    Handler ID.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CcpcCbackReset(dmConnId_t connId);

/*! \} */    /* CALL_CONTROL_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* CCPC_API_H */
