/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller CIS slave interface file.
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

#ifndef LCTR_API_CIS_H
#define LCTR_API_CIS_H

#include "lctr_api.h"
#include "lmgr_api_cis_slave.h"
#include "cfg_mac_ble.h"
#include "lmgr_api_iso.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Maximum value for maximum Data PDU length (spec limit is 251) */
#define LCTR_CIS_MAX_DATA_LEN_MAX       BB_DATA_PLD_MAX_LEN

/*! \brief      Data channel PDU length (header + payload + MIC). */
#define LCTR_CIS_DATA_PDU_LEN(len)      ((len) + LL_DATA_HDR_LEN + LL_DATA_MIC_LEN)

/*! \brief      Maximum data channel PDU length (header + payload + MIC). */
#define LCTR_CIS_DATA_PDU_MAX_LEN       LCTR_CIS_DATA_PDU_LEN(LCTR_CIS_MAX_DATA_LEN_MAX)

/*! \brief      Minimum data channel PDU length (header + payload + MIC). */
#define LCTR_CIS_DATA_PDU_MIN_LEN       LCTR_CIS_DATA_PDU_LEN(0)

/*! \brief      CIS messages. */
enum
{
  /* Broadcast events */
  LCTR_CIS_MSG_RESET               = LCTR_MSG_RESET,    /*!< Reset API message. */
  /* Scan events */
  LCTR_CIS_MSG_CIS_EST,                                 /*!< CIS established event. */
  LCTR_CIS_MSG_CIS_EST_FAIL,                            /*!< CIS establishment failed event. */
  LCTR_CIS_MSG_CIS_DISC,                                /*!< CIS disconnect event. */
  LCTR_CIS_MSG_CIS_CONN_FAIL,                           /*!< CIS connection fail to maintain event. */
  LCTR_CIS_MSG_CIS_CLOSED,                              /*!< CIS closed event. */
  LCTR_CIS_MSG_CIS_TERM_MIC_FAILED,                     /*!< CIS terminated due to MIC failure event. */
  LCTR_CIS_MSG_TOTAL,                                   /*!< Total number of CIS slave events. */
  LCTR_CIS_MSG_INVALID             = 0xFF,              /*!< Invalid CIS message. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Disconnect message. Make sure it has the same structure as lctrDisconnect_t. */
typedef struct
{
  lctrMsgHdr_t          hdr;        /*!< Message header. */
  uint8_t               reason;     /*!< Disconnect reason. */
} lctrCisDisconnect_t;

/*! \brief      Link layer controller message data. */
typedef union
{
  lctrMsgHdr_t          hdr;        /*!< Message header. */
  lctrCisDisconnect_t   disc;       /*!< Disconnect message data. */
} lctrCisMsg_t;

/*! \brief      CIG states. */
enum
{
  LCTR_CIG_STATE_NO_CIG = 0,            /*!< CIG does not exist. */
  LCTR_CIG_STATE_CONFIGUREABLE = LCTR_CIG_STATE_NO_CIG,
                                        /*!< CIG Configureable state -- functionally same state as configurable. */
  LCTR_CIG_STATE_ACTIVE,                /*!< CIG state is active. */
  LCTR_CIG_STATE_INACTIVE,              /*!< CIG state is inactive. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrSlvCisInit(void);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_CIS_H */
