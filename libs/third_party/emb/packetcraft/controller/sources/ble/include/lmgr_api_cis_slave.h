/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager connected isochronous stream slave interface file.
 *
 *  Copyright (c) 2013-2018 ARM Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.  All rights reserved.
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

#ifndef LMGR_API_CIS_SLAVE_H
#define LMGR_API_CIS_SLAVE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_ADV_MST
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief      Slave role device parameter definition. */
typedef struct
{
  uint8_t           maxNumCis;          /*!< Maximum number of CIS. */
} lmgrCisSlvCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
/* Initialization */
void LmgrMstInit(void);

/*! \} */    /* LL_LMGR_API_CIS_SLV */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_CIS_SLAVE_H */
