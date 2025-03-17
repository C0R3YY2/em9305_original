/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising slave interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
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

#ifndef LMGR_API_ADV_SLAVE_H
#define LMGR_API_ADV_SLAVE_H

#include "lmgr_api.h"
#include <emb_ll_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_ADV_SLV
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Advertising parameters. */
typedef struct
{
  uint32_t          advInterMinUsec;    /*!< Minimum advertising interval in microseconds. */
  uint32_t          advInterMaxUsec;    /*!< Maximum advertising interval in microseconds. */
  uint8_t           advType;            /*!< Advertising type. */
  uint8_t           ownAddrType;        /*!< Address type used by this device. */
  uint8_t           peerAddrType;       /*!< Address type of peer device. Only used for directed advertising. */
  uint64_t          peerAddr;           /*!< Address of peer device. Only used for directed advertising. */
  uint8_t           advChanMap;         /*!< Advertising channel map. */
  uint8_t           advFiltPolicy;      /*!< Advertising filter policy. */
} lmgrAdvParam_t;

/*! \brief      Slave role device parameter definition. */
typedef struct
{
  uint32_t          advTermCntDownUsec; /*!< Advertising termination count down in microseconds. */
  lmgrAdvParam_t    advParam;           /*!< Advertising parameters. */
  lmgrAdvbUser_t    advData;            /*!< Advertising host data buffer. */
  lmgrAdvbUser_t    scanRspData;        /*!< Scan response host data buffer. */
  uint8_t           ownAddrType;        /*!< Actual address type used by this device. */
  uint64_t          localRpa;           /*!< Local RPA used by this device. */
  bool              scanReportEna;      /*!< Scan report events enabled. */
  uint8_t           defTxPhyOpts;       /*!< Default Tx PHY options */
  bool              perAdvEventNotify;  /*!< Periodic Advertising event notify flag. */

#if(USE_EMB_EXTENSIONS_LL)
  bool              advIndEnabled;      /*!< Indications on advertisement are enabled. */
#endif //USE_EMB_EXTENSIONS_LL
  /* RF compliance */
  int8_t            maxPriAdvTxPwr;     /*!< Maximum primary advertising transmit power. */
} lmgrSlvAdvCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrSlvAdvCtrlBlk_t lmgrSlvAdvCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrSlvInit(void);

/*! \} */    /* LL_LMGR_API_ADV_SLV */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_SLAVE_H */
