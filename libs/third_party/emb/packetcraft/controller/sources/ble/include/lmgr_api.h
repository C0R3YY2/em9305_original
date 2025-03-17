/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager common interface file.
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

#ifndef LMGR_API_H
#define LMGR_API_H

#include "ll_api.h"
#include "ll_defs.h"
#include "hci_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master scan modes. */
enum
{
  LMGR_SCAN_MODE_DISCOVER,              /*!< Scan enabled state. */
  LMGR_SCAN_MODE_INITIATE               /*!< Initiate enabled state. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising broadcast user data. */
typedef struct
{
  uint8_t           buf[LL_ADVBU_MAX_LEN];  /*!< ADVB-U buffer. */
  uint8_t           len;                    /*!< ADVB-U buffer length. */
  bool              modified;               /*!< Content modified by host. */
} lmgrAdvbUser_t;

/*! \brief      Link layer manager persistent control block (persists with resets). */
typedef struct
{
  llEvtCback_t      evtCback;           /*!< Event callback. */
  llAclCback_t      sendCompCback;      /*!< ACL data send complete callback. */
  llAclCback_t      recvPendCback;      /*!< ACL data receive pending callback. */
  wsfHandlerId_t    handlerId;          /*!< System event handler ID. */
  uint16_t          connCtxSize;        /*!< Size of the connection context. */
  uint16_t          advSetCtxSize;      /*!< Size of the advertising set context. */
  uint16_t          extScanCtxSize;     /*!< Size of the extended scanner context. */
  uint16_t          extInitCtxSize;     /*!< Size of the extended initiator context. */
  uint16_t          perScanCtxSize;     /*!< Size of the periodic scanning context. */
  uint16_t          cisCtxSize;         /*!< Size of the CIS context. */
  uint16_t          cigCtxSize;         /*!< Size of the CIG context. */
  uint16_t          bisCtxSize;         /*!< Size of the BIS context. */
  uint16_t          bigCtxSize;         /*!< Size of the BIG context. */
  uint16_t          csCtxSize;          /*!< Size of the CS context. */
  uint64_t          featuresDefault;    /*!< Default supported features. */

  llIsoCback_t      sendIsoCompCback;   /*!< ISO data send complete callback. */
  llIsoCback_t      recvIsoPendCback;   /*!< ISO data receive pending callback. */

  /* Device parameters */
  uint64_t          bdAddr;             /*!< Public device address. */
  uint64_t          supStates;          /*!< Supported states. */
  uint64_t          validHostSupFeat;   /*!< Valid host controlled features bits. */
} lmgrPersistCtrlBlk_t;

/*! \brief      Link layer manager control block (cleared with resets). */
typedef struct
{
  /* Status */
  uint8_t           numConnEnabled;     /*!< Number of active connection. */
  bool              advEnabled;         /*!< Legacy advertising is enabled. */
  uint8_t           numExtAdvEnabled;   /*!< Number of extended advertising is enabled. */
  uint8_t           numScanEnabled;     /*!< Number of scan is enabled. */
  uint8_t           numInitEnabled;     /*!< Number of initiate is enabled. */
  uint8_t           scanMode;           /*!< Discover or initiating scan mode. */
  uint8_t           numWlFilterEnabled; /*!< Number of enabled whitelist filters. */
  uint8_t           numPlFilterEnabled; /*!< Number of enabled periodic filters. */
  bool              testEnabled;        /*!< Test is enabled. */
  uint8_t           resetDelayRefCnt;   /*!< Reset delay reference counter. */
  uint8_t           extAdvEnaDelayCnt;  /*!< Extended advertising enable event delay reference count. */
  uint8_t           advSetEnaStatus;    /*!< Last advertising set enable error status. */
  uint8_t           numExtScanPhys;     /*!< Number of extended scanning PHYs enabled. */
  uint8_t           extScanEnaDelayCnt; /*!< Extended scanning enable event delay reference count. */
  uint8_t           extScanEnaStatus;   /*!< Last extended scanning enable error status. */

  /* Device parameters */
  bool              bdAddrRndModAdv;    /*!< Modified random address (advertising). */
  bool              bdAddrRndModScan;   /*!< Modified random address (scan). */
  bool              bdAddrRndValid;     /*!< Random device address valid. */
  uint64_t          opModeFlags;        /*!< Operational mode flags. */
  uint64_t          bdAddrRnd;          /*!< Random device address. */
  uint64_t          features;           /*!< Used local supported features. */

  /* Operational parameters. */
  int8_t            advTxPwr;           /*!< Advertising Tx power. */
  bool              addrResEna;         /*!< Address resolution enabled. */
  bool              useLegacyCmds;      /*!< Use only legacy advertising, scan or initiate commands. */
  bool              useExtCmds;         /*!< Use only extended advertising, scan or initiate commands. */
  uint64_t          chanClass;          /*!< Channel class. */

  /* Sleep clock accuracy override value. */
  int8_t            scaMod;             /*!< SCA override value. */

  /* Power Class 1. */
  int8_t            powerThreshold[LL_MAX_PHYS];    /*!< Power threshold for each PHY. */
  uint8_t           localMinUsedChan[LL_MAX_PHYS];  /*!< Local minimum number of used channels for each PHY. */

  /* Isochronous channels. */
  bool              sendIsoCmplEvt;                  /*!< ISO event completion notification generation enable. */

#if (LL_ENABLE_RSPOBS)
  /* Response Observation. */
  bool              rspObsEnable;       /*!< Response Observation. */
#endif
} lmgrCtrlBlk_t;

/*! \brief      Channel parameters. */
typedef struct
{
  /* Channel parameters */
  uint64_t      chanMask;           /*!< Channel mask. */
  uint8_t       chanRemapTbl[LL_CHAN_DATA_MAX_IDX + 1]; /*!< Channel remapping table. */
  uint8_t       numUsedChan;        /*!< Number of used channels. */

  uint8_t       usedChSel;          /*!< Used channel selection. */
  uint16_t      chIdentifier;       /*!< Channel identifier. */

  /* Subevent parameters */
  uint16_t      prnLast;            /*!< Last used permutation. */
  uint8_t       subEvtIdx;          /*!< Subevent index. */
} lmgrChanParam_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrPersistCtrlBlk_t lmgrPersistCb;
extern lmgrCtrlBlk_t lmgrCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Reset */
void LmgrSetDefaults(void);
void LmgrIncResetRefCount(void);
void LmgrDecResetRefCount(void);

/* Whitelist */
void LmgrIncWhitelistRefCount(void);
void LmgrDecWhitelistRefCount(void);

/* Periodic List */
void LmgrIncPeriodiclistRefCount(void);
void LmgrDecPeriodiclistRefCount(void);

/* Status */
bool LmgrIsAddressTypeAvailable(uint8_t ownAddrType);
bool LmgrIsLegacyCommandAllowed(void);
bool LmgrIsExtCommandAllowed(void);

/* Utility */
void LmgrBuildRemapTable(lmgrChanParam_t *pChanParam);
uint8_t LmgrSelectNextChannel(lmgrChanParam_t *pChanParam, uint16_t eventCounter, uint16_t numSkip, bool calSubEvt);
uint8_t LmgrSelectNextSubEvtChannel(lmgrChanParam_t *pChanParam);

/* Event Messages */
void LmgrSendAdvEnableCnf(uint8_t status);
void LmgrSendScanEnableCnf(uint8_t status);
void LmgrSendAdvSetTermInd(uint8_t handle, uint8_t status, uint16_t connHandle, uint8_t numEvents);
bool LmgrSendEvent(LlEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief      Get operational mode flag.
 *
 *  \param      flag  Flag to check.
 *
 *  \return     true if flag is set.
 *
 *  Get mode flag governing LL operations.
 */
/*************************************************************************************************/
static inline bool lmgrGetOpFlag(uint64_t flag)
{
  return (lmgrCb.opModeFlags & flag) ? true : false;
}

/*! \} */    /* LL_LMGR_API */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_H */
