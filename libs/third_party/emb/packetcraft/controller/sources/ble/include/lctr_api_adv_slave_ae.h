/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller extended advertising slave interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_ADV_SLAVE_AE_H
#define LCTR_API_ADV_SLAVE_AE_H

#include "lctr_api.h"
#include "lmgr_api_adv_slave_ae.h"
#include "cfg_mac_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave extended advertising task messages for \a LCTR_DISP_EXT_ADV dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_EXT_ADV_MSG_RESET                = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Advertising events */
  LCTR_EXT_ADV_MSG_START,               /*!< Extended advertising start API event. */
  LCTR_EXT_ADV_MSG_STOP,                /*!< Extended advertising stop API event. */
  LCTR_EXT_ADV_MSG_INT_START,           /*!< Extended advertising start internal event. */
  LCTR_EXT_ADV_MSG_TERMINATE,           /*!< Extended advertising BOD terminated event. */
  LCTR_EXT_ADV_MSG_TMR_DUR_EXP,         /*!< Extended advertising duration timer expired event. */
  LCTR_EXT_ADV_MSG_TOTAL                /*!< Total number of extended advertising events. */
};

/*! \brief      Slave periodic advertising task messages for \a LCTR_DISP_PER_ADV dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_PER_ADV_MSG_RESET                = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Advertising events */
  LCTR_PER_ADV_MSG_START,               /*!< Periodic advertising start API event. */
  LCTR_PER_ADV_MSG_STOP,                /*!< Periodic advertising stop API event. */
  LCTR_PER_ADV_MSG_TERMINATE,           /*!< Periodic advertising BOD terminated event. */
  LCTR_PER_ADV_MSG_TOTAL                /*!< Total number of periodic advertising events. */
};

/*! \brief      ACAD dispatcher messages */
enum
{
  LCTR_ACAD_MSG_CHAN_UPDATE,            /* Start a channel map update */
  LCTR_ACAD_MSG_CHAN_UPDATE_FINISH,     /* Finish a channel map update */
  LCTR_ACAD_MSG_BIG_CREATED,            /* BIG is created*/
  LCTR_ACAD_MSG_BIG_TERMINATED,         /* BIG is terminated */
  LCTR_ACAD_MSG_PARTI_STARTED,          /* Start PAwR */
  LCTR_ACAD_MSG_PARTI_STOPPED,          /* Stop PAwR */
  LCTR_ACAD_MSG_TOTAL
};

/*! \brief      Link layer controller message data. */
typedef union
{
  lctrMsgHdr_t            hdr;              /*!< Message header. */
  lctrChanMapUpdate_t     chanMapUpd;       /*!< ACAD channel map update. */
  lctrBigCreated_t        bigCreated;       /*!< ACAD BIG created. */
} lctrAcadSlvMsg_t;

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended Advertising enable message. */
typedef struct
{
  lctrMsgHdr_t              hdr;        /*!< Message header. */
  uint32_t                  durMs;      /*!< Duration in milliseconds. */
  uint8_t                   maxEvents;  /*!< Maximum number of extended advertising events. */
} LctrExtAdvEnableMsg_t;

/*! \brief      Periodic Advertising enable message. */
typedef struct
{
  lctrMsgHdr_t              hdr;        /*!< Message header. */
} LctrPerAdvEnableMsg_t;

/*! \brief      Extended Advertising message data. */
typedef union
{
  lctrMsgHdr_t              hdr;        /*!< Message header. */
  LctrExtAdvEnableMsg_t     enable;     /*!< Enable message data. */
} LctrExtAdvMsg_t;

/*! \brief      Periodic Advertising message data. */
typedef union
{
  lctrMsgHdr_t              hdr;        /*!< Message header. */
  LctrPerAdvEnableMsg_t     perEnable;  /*!< Periodic enable message data. */
  lctrChanMapUpdate_t       chanUpdate; /*!< Channel map update data. */
} LctrPerAdvMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrSlvExtAdvInit(void);
void LctrSlvExtAdvDefaults(void);
void LctrSlvPeriodicAdvInit(void);

/* Status */
uint8_t LctrIsExtAdvEnableParamValid(uint8_t enable, LlExtAdvEnableParam_t *pEnaParam);
uint8_t LctrIsExtAdvEnableReady(uint8_t handle);
bool LctrIsAdvHandleValid(uint8_t handle);
uint8_t LctrGetExtAdvTxPowerLevel(uint16_t handle, int8_t *pLevel);
bool LctrIsPerAdvEnabled(uint8_t handle);

/* Control */
uint8_t LctrGetAdvHandles(uint8_t pHandles[LL_MAX_ADV_SETS]);
uint8_t LctrSetExtAdvSetRandAddr(uint8_t handle, const uint8_t *pAddr);
uint8_t LctrGetExtAdvSetRandAddr(uint8_t handle, uint8_t *pAddr);
uint8_t LctrSetExtAdvParamV2(uint8_t handle, LlExtAdvParam_t *pExtAdvParam);
uint8_t LctrSetExtAdvData(uint8_t handle, uint8_t op, uint8_t fragPref, uint8_t len, const uint8_t *pData);
uint64_t LctrGetPerAdvChanMap(uint8_t handle);
uint8_t LctrSetExtScanRespData(uint8_t handle, uint8_t op, uint8_t fragPref, uint8_t len, const uint8_t *pData);
uint8_t LctrRemoveAdvSet(uint8_t handle);
uint8_t LctrClearAdvSets(void);
uint8_t LctrSetAuxOffsetDelay(uint8_t handle, uint32_t delayUsec);
uint8_t LctrSetExtAdvDataFragLen(uint8_t handle, uint8_t fragLen);
uint8_t LctrSetExtAdvTxPhyOptions(uint8_t handle, uint8_t priPhyOpts, uint8_t secPhyOpts);
uint8_t LctrSetPeriodicAdvParam(uint8_t handle, LlPerAdvParam_t *pPerAdvParam);
uint8_t LctrPeriodicAdvSetInfoTransfer(uint16_t connHandle, uint16_t serviceData, uint8_t advHandle);
uint8_t LctrSetPeriodicAdvSyncTransParams(uint16_t connHandle, uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType);
void LctrSetPeriodicAdvEnable(uint8_t handle, uint8_t enable);
uint8_t LctrSetPeriodicAdvData(uint8_t handle, uint8_t op, uint8_t len, const uint8_t *pData);
uint8_t LctrExtAdvSetDataRelatedAddrChanges(uint8_t extAdvHandle, uint8_t changeReasons);
void LctrExtAdvCheckDataRelatedAddrChange(uint8_t advHandle, uint8_t reason);
uint8_t LctrSetPeriodicAdvParamV2(uint8_t handle, LlPerAdvParam_t *pPerAdvParam);
uint8_t LctrSetPeriodicAdvSubeventData(uint8_t advHandle, uint8_t numSubevents, LlSubeventParam_t subeventParam[]);
uint8_t LctrSetPeriodicSyncSubevent(uint16_t syncHandle, uint16_t perAdvProp, uint8_t numSubevents, uint8_t *pSubevent);
#if (LL_ENABLE_RSPOBS)
uint8_t LctrSetPeriodicAdvResponseData(uint16_t syncHandle, uint8_t role, uint16_t reqEvent, uint8_t reqSubevent, uint8_t subevent, uint8_t responseSlot, uint8_t responseDataLen, uint8_t *pResponseData);
#else
uint8_t LctrSetPeriodicAdvResponseData(uint16_t syncHandle, uint16_t reqEvent, uint8_t reqSubevent, uint8_t subevent, uint8_t responseSlot, uint8_t responseDataLen, uint8_t *pResponseData);
#endif
uint8_t LctrExtCreateConnV2(const LlExtInitParam_t *pInitParam, const LlConnSpec_t *pConnSpec);
uint8_t LctrSetRspObserverSlots(uint8_t handle, uint8_t subevent, uint8_t count, uint8_t *slotList);
uint8_t LctrSetRspObsRspData(uint8_t handle, uint8_t role, uint16_t reqEvent, uint8_t reqSubevent, uint8_t rspSubevent, uint8_t count, uint8_t *pRspList);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_SLAVE_AE_H */
