/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  DM privacy module.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
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
#ifndef DM_PRIV_H
#define DM_PRIV_H

#include "util/bda.h"
#include "wsf_os.h"
#include "wsf_timer.h"
#include "sec_api.h"
#include "dm_main.h"
#include "smp_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* DM privacy event handler messages */
enum
{
  DM_PRIV_MSG_API_RESOLVE_ADDR = DM_MSG_START(DM_ID_PRIV),
  DM_PRIV_MSG_API_ADD_DEV_TO_RES_LIST,
  DM_PRIV_MSG_API_REM_DEV_FROM_RES_LIST,
  DM_PRIV_MSG_API_CLEAR_RES_LIST,
  DM_PRIV_MSG_API_SET_ADDR_RES_ENABLE,
  DM_PRIV_MSG_API_SET_PRIVACY_MODE,
  DM_PRIV_MSG_API_GEN_ADDR,
  DM_PRIV_MSG_API_SET_DATA_REL_ADDR_CHANGES
};

/* DM privacy AES event handler messages */
enum
{
  DM_PRIV_MSG_RESOLVE_AES_CMPL = DM_MSG_START(DM_ID_PRIV_AES),
  DM_PRIV_MSG_GEN_ADDR_AES_CMPL
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data structure for DM_PRIV_MSG_API_RESOLVE_ADDR */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 irk[SMP_KEY_LEN];
  bdAddr_t                addr;
} dmPrivApiResolveAddr_t;

/* Data structure for DM_PRIV_MSG_API_GEN_ADDR */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 irk[SMP_KEY_LEN];
} dmPrivApiGenAddr_t;

/* Data structure for DM_PRIV_MSG_API_ADD_DEV_TO_RES_LIST */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 addrType;
  bdAddr_t                peerAddr;
  uint8_t                 peerIrk[SMP_KEY_LEN];
  uint8_t                 localIrk[SMP_KEY_LEN];
  bool                    enableLlPriv;
} dmPrivApiAddDevToResList_t;

/* Data structure for DM_PRIV_MSG_API_REM_DEV_FROM_RES_LIST */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 addrType;
  bdAddr_t                peerAddr;
} dmPrivApiRemDevFromResList_t;

/* Data structure for DM_PRIV_MSG_API_SET_ADDR_RES_ENABLE */
typedef struct
{
  wsfMsgHdr_t             hdr;
  bool                    enable;
} dmPrivApiSetAddrResEnable_t;

/* Data structure for DM_PRIV_MSG_API_SET_PRIVACY_MODE */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 addrType;
  bdAddr_t                peerAddr;
  uint8_t                 mode;
} dmPrivApiSetPrivacyMode_t;

/* Data structure for DM_PRIV_MSG_API_SET_DATA_REL_ADDR_CHANGES */
typedef struct
{
  wsfMsgHdr_t             hdr;
  uint8_t                 changeReasons;
} dmPrivApiSetDataRelAddrChg_t;

/* Union of all privacy messages */
typedef union
{
  wsfMsgHdr_t                  hdr;
  dmPrivApiResolveAddr_t       apiResolveAddr;
  dmPrivApiAddDevToResList_t   apiAddDevToResList;
  dmPrivApiRemDevFromResList_t apiRemDevFromResList;
  dmPrivApiSetAddrResEnable_t  apiSetAddrResEnable;
  dmPrivApiSetPrivacyMode_t    apiSetPrivacyMode;
  dmPrivApiGenAddr_t           apiGenerateAddr;
  dmPrivGenAddrIndEvt_t        genAddrInd;
  dmPrivApiSetDataRelAddrChg_t apiSetDataRelAddrChg;
  secAes_t                     aes;
} dmPrivMsg_t;

/* Action function */
typedef void (*dmPrivAct_t)(dmPrivMsg_t *pMsg);

/* Control block for privacy module */
typedef struct
{
  uint8_t     hash[DM_PRIV_HASH_LEN];            /* Hash part of resolvable address */
  uint8_t     inProgress;                        /* Address resolution in progress */
  uint16_t    addDevToResListParam;              /* 'Add device to resolving list' callback param */
  uint16_t    remDevFromResListParam;            /* 'Remove device from resolving list' callback param */
  bool        enableLlPriv;                      /* 'Add device to resolving list' input param */
  bool        addrResEnable;                     /* 'Set address resolution enable' input param */
  uint8_t     genAddrBuf[HCI_ENCRYPT_DATA_LEN];  /* Random value buffer for generating an RPA */
} dmPrivCb_t;

/**************************************************************************************************
  Function declarations
**************************************************************************************************/

/* Privacy component interface */
void dmPrivMsgHandler(wsfMsgHdr_t *pMsg);
void dmPrivHciHandler(hciEvt_t *pEvent);
void dmPrivReset(void);

/* Privacy action functions */
void dmPrivActResolveAddr(dmPrivMsg_t *pMsg);
void dmPrivActAddDevToResList(dmPrivMsg_t *pMsg);
void dmPrivActRemDevFromResList(dmPrivMsg_t *pMsg);
void dmPrivActSetAddrResEnable(dmPrivMsg_t *pMsg);
void dmPrivActClearResList(dmPrivMsg_t *pMsg);
void dmPrivActSetPrivacyMode(dmPrivMsg_t *pMsg);
void dmPrivActGenAddr(dmPrivMsg_t *pMsg);
void dmPrivActSetDataRelAddrChanges(dmPrivMsg_t *pMsg);

/* Privacy ASE component interface */
void dmPrivAesMsgHandler(wsfMsgHdr_t *pMsg);

/* Privacy ASE action functions */
void dmPrivAesActResAddrAesCmpl(dmPrivMsg_t *pMsg);
void dmPrivAesActGenAddrAesCmpl(dmPrivMsg_t *pMsg);

#ifdef __cplusplus
};
#endif

#endif /* DM_PRIV_H */
