/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Enhanced attribute protocol client and server API.
 *
 *  Copyright (c) 2019-2020 Arm Ltd. All Rights Reserved.
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
#ifndef EATT_API_H
#define EATT_API_H

#include "wsf_timer.h"
#include "att_defs.h"
#include "att_uuid.h"
#include "dm_api.h"
#include "l2c_api.h"
#include "cfg_stack.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \addtogroup STACK_ATT_API
 *  \{ */
/** \name EATT Role
 * EATT role can be initiator or acceptor.
 */
/**@{*/
#define EATT_ROLE_INITIATOR       L2C_COC_ROLE_INITIATOR
#define EATT_ROLE_ACCEPTOR        L2C_COC_ROLE_ACCEPTOR
/**@}*/
/*! \} */    /* STACK_ATT_API */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \addtogroup STACK_ATT_API
 *  \{ */
/**
 * \name EATT Data Types
 *
 */
/**@{*/
/*! \brief EATT multiple notify tuple structure */
typedef struct
{
  uint16_t handle;                /*!< Attribute handle */
  uint16_t len;                   /*!< Length of pValue in bytes. */
  uint8_t *pValue;                /*!< Attribute value */
} eattTuple_t;
/**@}*/
/*! \} */    /* STACK_ATT_API */

/*! \addtogroup STACK_ATTS_API
 *  \{ */
/** \name EATT Server Functions
 *
 */
/**@{*/
/*************************************************************************************************/
/*!
 *  \brief  Send multiple attribute protocol Handle Value Notification.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  numTuples   The number of tuples.
 *  \param  pTupleList  Pointer to a list of tuples.
 */
/*************************************************************************************************/
void EattsMultiValueNtf(dmConnId_t connId, uint8_t priority, uint16_t numTuples, eattTuple_t *pTupleList);

/*************************************************************************************************/
/*!
 *  \brief  Send an attribute protocol Handle Value Indication.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 */
/*************************************************************************************************/
void EattsHandleValueInd(dmConnId_t connId, uint8_t priority, uint16_t handle, uint16_t valueLen,
                         uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Send an attribute protocol Handle Value Notification.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 */
/*************************************************************************************************/
void EattsHandleValueNtf(dmConnId_t connId, uint8_t priority, uint16_t handle, uint16_t valueLen,
                         uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Send an attribute protocol Handle Value Indication without copying the attribute
 *          value data.
 *
 *          Note: attribute value buffer 'pValue' must be allocated with AttMsgAlloc().
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 */
/*************************************************************************************************/
void EattsHandleValueIndZeroCpy(dmConnId_t connId, uint8_t priority, uint16_t handle,
                                uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Send an attribute protocol Handle Value Notification without copying the attribute
 *          value data.
 *
 *          Note: attribute value buffer 'pValue' must be allocated with AttMsgAlloc().
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 */
/*************************************************************************************************/
void EattsHandleValueNtfZeroCpy(dmConnId_t connId, uint8_t priority, uint16_t handle,
                                uint16_t valueLen, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Send a response to a pending write request.  For use with ATT_RSP_PENDING.
 *
 *  \param  connId      Connection ID.
 *  \param  slot        EATT channel slot ID.
 *  \param  handle      Attribute handle.
 *  \param  status      Status of the write request.
 *
 *  \note   When a higher layer returns ATT_RSP_PENDING to an ATT write callback indicating the
 *          response status is pending, the higher layer must subsequently call this function
 *          with the status of the write request.
 */
/*************************************************************************************************/
void EattsContinueWriteReq(dmConnId_t connId, uint8_t slot, uint16_t handle, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Send a response to a pending read request.  For use with ATT_RSP_PENDING.
 *
 *  \param  connId      Connection ID.
 *  \param  slot        EATT channel slot ID.
 *  \param  handle      Attribute handle.
 *  \param  status      Status of the read request.
 *  \param  pData       Attribute data.
 *  \param  len         Length of pData in bytes.
 *
 *  \note   When a higher layer returns ATT_RSP_PENDING to an ATT read callback indicating the
 *          response status is pending, the higher layer must subsequently call this function
 *          with the status of the read request.
 */
/*************************************************************************************************/
void EattsContinueReadReq(dmConnId_t connId, uint8_t slot, uint16_t handle, uint8_t status,
                          uint8_t *pData, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the Enhanced ATT Server.
 *
 *  \note   EattInit must be called after the ATT task is created and before calling EattsInit
 *          and/or EattcInit.
 */
 /*************************************************************************************************/
void EattsInit();
/**@}*/
/*! \} */    /* STACK_ATTS_API */

/*! \addtogroup STACK_ATTC_API
 *  \{ */
/** \name EATT Client Functions
 *
 */
/**@{*/
/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Find Information Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  continuing  true if ATTC continues sending requests until complete.
 */
/*************************************************************************************************/
void EattcFindInfoReq(dmConnId_t connId, uint8_t priority, uint16_t startHandle, uint16_t endHandle,
                      bool continuing);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Find By Type Value Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuid16      16-bit UUID to find.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *  \param  continuing  true if ATTC continues sending requests until complete.
 */
/*************************************************************************************************/
void EattcFindByTypeValueReq(dmConnId_t connId, uint8_t priority, uint16_t startHandle, uint16_t endHandle,
                             uint16_t uuid16, uint16_t valueLen, uint8_t *pValue, bool continuing);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read By Type Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *  \param  continuing  true if ATTC continues sending requests until complete.
 */
/*************************************************************************************************/
void EattcReadByTypeReq(dmConnId_t connId, uint8_t priority, uint16_t startHandle, uint16_t endHandle,
                        uint8_t uuidLen, uint8_t *pUuid, bool continuing);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read Request.
 *
 *  \param  connId    DM connection ID.
 *  \param  priority  Operation priority.
 *  \param  handle    Attribute handle.
 */
/*************************************************************************************************/
void EattcReadReq(dmConnId_t connId, uint8_t priority, uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  offset      Read attribute data starting at this offset.
 *  \param  continuing  true if ATTC continues sending requests until complete.
 */
/*************************************************************************************************/
void EattcReadLongReq(dmConnId_t connId, uint8_t priority, uint16_t handle, uint16_t offset, bool continuing);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read Multiple Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  numHandles  Number of handles in attribute handle list.
 *  \param  pHandles    List of attribute handles.
 */
/*************************************************************************************************/
void EattcReadMultipleReq(dmConnId_t connId, uint8_t priority, uint8_t numHandles, uint16_t *pHandles);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read By Group Type Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  startHandle Attribute start handle.
 *  \param  endHandle   Attribute end handle.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *  \param  continuing  true if ATTC continues sending requests until complete.
 */
/*************************************************************************************************/
void EattcReadByGroupTypeReq(dmConnId_t connId, uint8_t priority, uint16_t startHandle, uint16_t endHandle,
                            uint8_t uuidLen, uint8_t *pUuid, bool continuing);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Write Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 */
/*************************************************************************************************/
void EattcWriteReq(dmConnId_t connId, uint8_t priority, uint16_t handle, uint16_t valueLen,
                   uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Cancel an attribute protocol request in progress.
 *
 *  \param  connId    DM connection ID.
 *  \param  priority  Operation priority.
 */
/*************************************************************************************************/
void EattcCancelReq(dmConnId_t connId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Send an attribute protocol indication confirmation.
 *
 *  \param  connId    DM connection ID.
 *  \param  cid       L2Cap channel ID.
 */
/*************************************************************************************************/
void EattcIndConfirm(dmConnId_t connId, uint16_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Write Command.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 */
/*************************************************************************************************/
void EattcWriteCmd(dmConnId_t connId, uint8_t priority, uint16_t handle, uint16_t valueLen,
                   uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Prepare Write Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  handle      Attribute handle.
 *  \param  offset      Write attribute data starting at this offset.
 *  \param  valueLen    Length of value data.
 *  \param  pValue      Pointer to value data.
 *  \param  valueByRef  true if pValue data is accessed by reference rather than copied.
 *  \param  continuing  true if ATTC continues sending requests until complete.
 */
/*************************************************************************************************/
void EattcPrepareWriteReq(dmConnId_t connId, uint8_t priority, uint16_t handle, uint16_t offset,
                          uint16_t valueLen, uint8_t *pValue, bool valueByRef, bool continuing);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Execute Write Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  writeAll    true to write all queued writes, false to cancel all queued writes.
 */
/*************************************************************************************************/
void EattcExecuteWriteReq(dmConnId_t connId, uint8_t priority, bool writeAll);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an attribute protocol Read Multiple Variable Length Request.
 *
 *  \param  connId      DM connection ID.
 *  \param  priority    Operation priority.
 *  \param  numHandles  The number of handles in pHandles.
 *  \param  pHandles    List of attribute handles to read.
 */
/*************************************************************************************************/
void EattcReadMultVarLenReq(dmConnId_t connId, uint8_t priority, uint8_t numHandles, uint16_t *pHandles);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the Enhanced ATT Client.
 *
 *  \note   EattInit must be called after the ATT task is created and before calling EattsInit
 *          and/or EattcInit.
 */
 /*************************************************************************************************/
void EattcInit();
/**@}*/
/*! \} */    /* STACK_ATTC_API */

/*! \addtogroup STACK_ATT_API
 *  \{ */
/** \name EATT Functions
 *
 */
/**@{*/
/*************************************************************************************************/
/*!
 *  \brief  Begin requesting EATT L2CAP coc channels.
 *
 *  \note   When pEattCfg->initiateEatt is true, EattEstablishChannels is called automatically
 *          on DM_CONN_OPEN_IND. If pEattCfg->initiateEatt is false, EattEstablishChannels can be
 *          called by the application after DM_CONN_OPEN_IND to begin creating EATT channels.
 *
 *  \param  connId  DM connection identifier.
 */
/*************************************************************************************************/
void EattEstablishChannels(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Returns the number of open EATT channels on a given connection.
 *
 *  \param  connId  DM connection identifier.
 *
 *  \return Number of open EATT channels.
 */
/*************************************************************************************************/
uint8_t EattGetNumChannelsInUse(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the Enhanced ATT subsystem.
 *
 *  \param  roleBits    TBD
 *
 *  \note   EattInit must be called after the ATT task is created and before calling EattsInit
 *          and/or EattcInit.
 */
 /*************************************************************************************************/
void EattInit(uint8_t roleBits);
/**@}*/
/*! \} */    /* STACK_ATT_API */

#ifdef __cplusplus
};
#endif

#endif /* EATT_API_H */
