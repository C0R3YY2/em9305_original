/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  TinyUSB configuration.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#include "pal_tinyusb.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Common configuration. */
#define CFG_TUSB_DEBUG            0
#define CFG_TUSB_DEBUG_PRINTF     WsfTrace
#define CFG_TUD_ENABLED           1

/* Device configuration. */
#define CFG_TUD_AUDIO             0
#define CFG_TUD_CDC               1
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
