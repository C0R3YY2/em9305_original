/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Application framework main module.
 *
 *  Copyright (c) 2011-2019 Arm Ltd. All Rights Reserved.
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
#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "wsf_os.h"
#include "wsf_timer.h"
#include "dm_api.h"
#include "att_api.h"
#include "app_api.h"
#include "app_db.h"
#include "app_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief No security record handle  */
#define APP_DB_HDL_NONE             NULL

/*! \brief Scanning mode types */
#define APP_SCAN_MODE_LEG           0     /*!< Legacy scanning mode */
#define APP_SCAN_MODE_EXT           1     /*!< Extended scanning mode */
#define APP_SCAN_MODE_NONE          255   /*!< Unknown scanning mode */

/*! \brief App WSF handler event bitmasks */
#define APP_BTN_DOWN_EVT            0x10  /*!< Button pressed down event */

/*! \brief App WSF message event starting values */
#define APP_MSG_START               0x00
#define APP_SLAVE_MSG_START         0x10
#define APP_MASTER_MSG_START        0x20

/*! \brief App WSF message event enumeration */
enum
{
  APP_BTN_POLL_IND = APP_MSG_START,       /*! Button poll timer expired */
  APP_UI_TIMER_IND                        /*! UI timer expired */
};

/*! App slave WSF message event enumeration */
enum
{
  APP_CONN_UPDATE_TIMEOUT_IND = APP_SLAVE_MSG_START   /*! Connection parameter update timer expired */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Message handling function type */
typedef void (*appMsgHandler_t)(wsfMsgHdr_t *pMsg);

/*! \brief Advertising callback function type */
typedef void(*appAdvCback_t)(dmEvt_t *pMsg);

/*! \brief Master control block */
typedef struct
{
  appDevInfo_t      scanResults[APP_SCAN_RESULT_MAX];                       /*! Scan result storage */
  uint8_t           numScanResults;                                         /*! Number of scan results */
  uint8_t           idx;                                                    /*! Index of address being resolved in scan result list */
  appDbHdl_t        dbHdl;                                                  /*! Database record handle for address being resolved */
  bool              inProgress;                                             /*! true if address resolution is in progress */
  uint8_t           scanMode;                                               /*! Scan and connect mode in use */
} appMasterCb_t;

/*! Slave control block */
typedef struct
{
  uint8_t           *pAdvData[DM_NUM_ADV_SETS][APP_NUM_DATA_LOCATIONS];     /*! Advertising data pointers */
  uint16_t          advDataLen[DM_NUM_ADV_SETS][APP_NUM_DATA_LOCATIONS];    /*! Advertising data lengths */
  uint16_t          advDataBufLen[DM_NUM_ADV_SETS][APP_NUM_DATA_LOCATIONS]; /*! Length of advertising data buffer maintained by Application */
  uint16_t          advDataOffset[DM_NUM_ADV_SETS][APP_NUM_DATA_LOCATIONS]; /*! Advertising data offsets */
  uint16_t          maxAdvDataLen[DM_NUM_ADV_SETS];                         /*! Maximum advertising data length supported by Controller */
  bool              bondable;                                               /*! true if in bondable mode */
  bool              advDataSynced[DM_NUM_ADV_SETS];                         /*! true if advertising/scan data is synced */
  uint8_t           advState[DM_NUM_ADV_SETS];                              /*! Advertising state */
  uint8_t           advType[DM_NUM_ADV_SETS];                               /*! Advertising type */
  bool              advTypeChanged[DM_NUM_ADV_SETS];                        /*! true if advertising type is changed */
  uint8_t           discMode;                                               /*! Discoverable/connectable mode */
  bdAddr_t          peerAddr[DM_NUM_ADV_SETS];                              /*! Peer address */
  uint8_t           peerAddrType[DM_NUM_ADV_SETS];                          /*! Peer address type */
  bool              findLtk;                                                /*! true if LTK request received while resolving address */
  appDbHdl_t        dbHdl;                                                  /*! Database record handle for address being resolved */
  bool              inProgress;                                             /*! true if address resolution is in progress */
  bool              advDirected;                                            /*! true if legacy directed advertising is in progress */
  appAdvCback_t     advStopCback;                                           /*! Advertising stopped callback */
  appAdvCback_t     advRestartCback;                                        /*! Advertising restart callback */
} appSlaveCb_t;

/*! Connection control block */
typedef struct
{
  appDbHdl_t        dbHdl;                                                  /*! Device database handle */
  dmConnId_t        connId;                                                 /*! Connection ID */
  uint16_t          handle;                                                 /*! Connection handle */
  bool              bonded;                                                 /*! true if bonded with peer device */
  bool              bondByLtk;                                              /*! true if bonded state being determined by LTK */
  bool              bondByPairing;                                          /*! true if bonded state being determined by pairing */
  bool              initiatingSec;                                          /*! true if initiating security */
  bool              setConnectable;                                         /*! true if switching to connectable mode */
  bool              connWasIdle;                                            /*! true if connection was idle at last check */
  uint8_t           rcvdKeys;                                               /*! Bitmask of keys received during pairing */
  uint8_t           attempts;                                               /*! Connection parameter update attempts */
  uint8_t           updateState;                                            /*! Connection Update State */
  wsfTimer_t        updateTimer;                                            /*! Connection parameter update timer */
} appConnCb_t;

/*! Main control block */
typedef struct
{
  appMsgHandler_t   slaveCback;                                             /*! Slave message handler callback */
  appMsgHandler_t   masterCback;                                            /*! Slave message handler callback */
} appCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief Connection control block array */
extern appConnCb_t appConnCb[DM_CONN_MAX];

/*! \brief WSF handler ID */
extern wsfHandlerId_t appHandlerId;

/*! \brief Main control block */
extern appCb_t appCb;

/*! \brief Master control block */
extern appMasterCb_t appMasterCb;

/*! \brief Slave control block */
extern appSlaveCb_t appSlaveCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

bool AppCheckBonded(dmConnId_t connId);
bool appCheckBondByLtk(dmConnId_t connId);
uint8_t appNumConns(uint8_t role);

void appUiBtnPoll(void);
void appUiTimerExpired(wsfMsgHdr_t *pMsg);

/* slave utility functions */
extern void appSlaveResetAdvDataOffset(uint8_t advHandle);
extern void appAdvStart(uint8_t numSets, uint8_t *pAdvHandles, uint16_t *pInterval, uint16_t *pDuration,
                        uint8_t *pMaxEaEvents, bool cfgAdvParam);
extern void appAdvSetData(uint8_t advHandle, uint8_t location, uint16_t len, uint8_t *pData, uint16_t bufLen,
                          uint16_t maxLen);
extern void appSlaveAdvStart(uint8_t numSets, uint8_t *pAdvHandles, uint16_t *pInterval, uint16_t *pDuration,
                             uint8_t *pMaxEaEvents, bool cfgAdvParam, uint8_t mode);
extern void appAdvStop(uint8_t numSets, uint8_t *pAdvHandles);
extern bool appAdvSetAdValue(uint8_t advHandle, uint8_t location, uint8_t adType, uint8_t len,
                             uint8_t *pValue, bool append);
extern void appSetAdvType(uint8_t advHandle, uint8_t advType, uint16_t interval, uint16_t duration,
                          uint8_t maxEaEvents, bool cfgAdvParam);
extern dmConnId_t appConnAccept(uint8_t advHandle, uint8_t advType, uint16_t interval, uint16_t duration,
                                uint8_t maxEaEvents, uint8_t addrType, uint8_t *pAddr, appDbHdl_t dbHdl,
                                bool cfgAdvData);

/* master utility functions */
extern dmConnId_t appConnOpen(uint8_t advHandle, uint8_t subevent, uint8_t initPhys,
                              uint8_t addrType, uint8_t *pAddr, appDbHdl_t dbHdl);

#ifdef __cplusplus
};
#endif

#endif /* APP_MAIN_H */
