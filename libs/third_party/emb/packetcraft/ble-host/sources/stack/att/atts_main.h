/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  ATT server main module.
 *
 *  Copyright (c) 2009-2019 Arm Ltd. All Rights Reserved.
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
#ifndef ATTS_MAIN_H
#define ATTS_MAIN_H

#include "wsf_queue.h"
#include "wsf_timer.h"
#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* ATTS event handler messages */
enum
{
  ATTS_MSG_IDLE_TIMEOUT = ATTS_MSG_START,
  ATTS_MSG_API_VALUE_IND_NTF,
  ATTS_MSG_IND_TIMEOUT,
  ATTS_MSG_SIGN_CMAC_CMPL,
  ATTS_MSG_DBH_CMAC_CMPL
};

/*!
 * Data buffer format for API request messages:
 *
 * | attsPktParam_t | ATT request data |
 * | bytes 0 to 7   | bytes 8 -        |
 */

/* API parameters */
typedef struct
{
  uint16_t          len;
  uint16_t          handle;
} attsPktParam_t;

/* verify attsPktParam_t will work in data buffer format described above */
WSF_CT_ASSERT(sizeof(attsPktParam_t) <= L2C_PAYLOAD_START);

/* API message structure */
typedef struct
{
  wsfMsgHdr_t       hdr;
  attsPktParam_t    *pPkt;
  uint8_t           slot;
} attsApiMsg_t;

/* ATTS connection control block */
typedef struct
{
  wsfTimer_t        outIndTimer;       /* Outstanding indication timer */
  attCcb_t          *pMainCcb;         /* Pointer to ATT main CCB */
  wsfTimer_t        idleTimer;         /* service discovery idle timer */
  dmConnId_t        connId;            /* DM connection ID */
  uint8_t           slot;              /* ATT/EATT slot ID */
  uint16_t          outIndHandle;      /* Waiting for confirm from peer for this indication handle */
  uint16_t          pendIndHandle;     /* Callback to application pending for this indication handle */
  uint16_t          pendNtfHandle[ATT_NUM_SIMUL_NTF]; /* Callback to application pending for this notification handle */
} attsCcb_t;

/* Client characteristic configuration descriptor callback type
 *
 *  \param  connId      DM connection ID.
 *  \param  method      Read or write.
 *  \param  handle      Attribute handle of the descriptor.
 *  \param  pValue      Pointer to the attribute value of the descriptor.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 */
typedef uint8_t (*attsCccFcn_t)(dmConnId_t connId, uint8_t method, uint16_t handle, uint8_t *pValue);

/* Main control block of the ATTS subsystem */
typedef struct
{
  attsCcb_t         ccb[DM_CONN_MAX][ATT_BEARER_MAX];         /* Connection slot control block */
  bool              ltkAvail[DM_CONN_MAX];                    /* Whether LTK is available for connection */
  wsfQueue_t        prepWriteQueue[DM_CONN_MAX];              /* Connection prepare write queue */
  wsfQueue_t        groupQueue;                               /* Queue of attribute groups */
  attFcnIf_t const  *pInd;                                    /* Indication callback interface */
  attMsgHandler_t   signMsgCback;                             /* Signed data callback interface */
  attsAuthorCback_t authorCback;                              /* Authorization callback */
  attsCccFcn_t      cccCback;                                 /* CCC callback */
  uint8_t           writeCbackSlot;                           /* ATT bearer slot used in current att write callback */
  uint8_t           readCbackSlot;                            /* ATT bearer slot used in current att read callback */
} attsCb_t;

/* PDU processing function type */
typedef void (*attsProcFcn_t)(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);

/* CSF Control block */
typedef struct
{
  attsCsfRec_t        attsCsfTable[DM_CONN_MAX];              /* connected clients' supported features record table. */
  attsCsfWriteCback_t writeCback;                             /* Write callback. */
  bool                isHashUpdating;                         /* Database hash update status. */
} attsCsfCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* PDU processing function lookup table, indexed by method */
extern attsProcFcn_t attsProcFcnTbl[ATT_METHOD_SIGNED_WRITE_CMD+1];

/* Control block */
extern attsCb_t attsCb;

/* Minimum PDU lengths, indexed by method */
extern const uint8_t attsMinPduLen[];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

attsCcb_t *attsCcbByConnId(dmConnId_t connId, uint8_t slot);
attsCcb_t *attsCcbByHandle(uint16_t handle, uint8_t slot);

void attsClearPrepWrites(attsCcb_t *pCcb);
bool attsUuidCmp(attsAttr_t *pAttr, uint8_t uuidLen, uint8_t *pUuid);
bool attsUuid16Cmp(uint8_t *pUuid16, uint8_t uuidLen, uint8_t *pUuid);
attsAttr_t *attsFindByHandle(uint16_t handle, attsGroup_t **pAttrGroup);
uint16_t attsFindInRange(uint16_t startHandle, uint16_t endHandle, attsAttr_t **pAttr);
uint16_t attsFindUuidInRange(uint16_t startHandle, uint16_t endHandle, uint8_t uuidLen,
                             uint8_t *pUuid, attsAttr_t **pAttr, attsGroup_t **pAttrGroup);
uint8_t attsPermissions(dmConnId_t connId, uint8_t permit, uint16_t handle, uint8_t permissions);
void attsDiscBusy(attsCcb_t *pCcb);
void attsCheckPendDbHashReadRsp(void);
void attsProcessDatabaseHashUpdate(secCmacMsg_t *pMsg);
uint16_t attsIsHashableAttr(attsAttr_t *pAttr);
void attsMsgCback(wsfMsgHdr_t *pMsg);

void attsProcMtuReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcFindInfoReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcFindTypeReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadTypeReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadBlobReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadMultReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadGroupTypeReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcWrite(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcPrepWriteReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcExecWriteReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcValueCnf(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);
void attsProcReadMultiVarReq(attsCcb_t *pCcb, uint16_t len, uint8_t *pPacket);

uint8_t attsCsfActClientState(dmConnId_t connId, uint8_t opcode, uint8_t *pPacket);
bool attsCsfIsClientChangeAware(dmConnId_t connId, uint16_t handle);
void attsCsfSetHashUpdateStatus(bool isUpdating);
bool attsCsfGetHashUpdateStatus(void);

void attsIndNtfCallback(dmConnId_t connId, attsCcb_t *pCcb, uint8_t status);
void attsHandleValueIndNtf(dmConnId_t connId, uint16_t handle, uint8_t slot, uint16_t valueLen,
                           uint8_t *pValue, uint8_t opcode, bool zeroCpy);

#ifdef __cplusplus
};
#endif

#endif /* ATTS_MAIN_H */
