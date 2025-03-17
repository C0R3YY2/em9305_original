/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Hearing Access Profile Client (HAPC).
 *
 *  Copyright (c) 2019-2022 Packetcraft, Inc.  All rights reserved.
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

#ifndef HAPC_MAIN_H
#define HAPC_MAIN_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! HAPC profiles */
enum
{
  HAPC_PROFILE_HAUC,                      /*!< HAPC Hearing Aid Unicast Client */
  HAPC_PROFILE_HARC,                      /*!< HAPC Hearing Aid Remote Controller */
  HAPC_PROFILE_IAC,                       /*!< HAPC Immediate Alert Client */

  HAPC_PROFILE_MAX
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

typedef void (*hapcProcMsgCback_t)(wsfMsgHdr_t *pEvt);
typedef void (*hapcProcDmMsgCback_t)(dmEvt_t *pEvt);
typedef uint8_t (*hapcValueUpdate_t)(uint16_t *pHdlList, attEvt_t *pMsg);

/*! profile interface structure */
typedef struct
{
  hapcProcMsgCback_t  procMsg;                  /*!< Profiles's message process function */
  hapcValueUpdate_t   valueUpdate;              /*!< Profiles's value update function */
} hapcIf_t;

/*! \brief HAP Client connection control block */
typedef struct
{
  uint8_t             lastOpcode;               /*!< Last opcode sent to server (HA) */
  uint16_t            *pHasHdlList;             /*!< List of HAS attribute handles on server (HA) */
} hapcCcb_t;

/*! \brief HAP Client main control block */
typedef struct
{
  hapcCcb_t           connCb[DM_CONN_MAX];      /*!< Connection control block */
  hapcIf_t            *pIf[HAPC_PROFILE_MAX];   /*!< Profile interface */
  hapcCback_t         cback;                    /*!< Client application's HAPC callback */
} hapcCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern hapcCb_t hapcCb;

/**************************************************************************************************
  Globals
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

extern hapcCcb_t *hapcConnCbByCid(dmConnId_t connId);

#ifdef __cplusplus
};
#endif

#endif /* HAPC_MAIN_H */
