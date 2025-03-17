/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Demo Application for 802.15.4.
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

#ifndef DEMOAPP_H
#define DEMOAPP_H

#include "wsf_types.h"

/*************************************************************************************************/
/*!
 *  \fn     DemoAppInit
 *
 *  \brief  Initialization for 802.15.4 MAC Demo App.
 *
 *  \param  None.
 */
/*************************************************************************************************/
void DemoAppInit(void);

/*************************************************************************************************/
/*!
 *  \fn     DemoAppMain
 *
 *  \brief  Routine to be called in main loop.
 *
 *  \return true if event pending.
 *
 *  \param  None.
 */
/*************************************************************************************************/
bool DemoAppMain(void);

#endif
