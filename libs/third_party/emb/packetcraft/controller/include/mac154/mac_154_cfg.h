/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      802.15.4 constant definitions.
 *
 *  Copyright (c) 2016-2018 ARM Ltd. All Rights Reserved.
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

#ifndef MAC_154_CFG_H
#define MAC_154_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  MAC options
**************************************************************************************************/

#define MAC_154_OPT_DISASSOC      1   /*!< Enable only if disassociation support required */
#define MAC_154_OPT_ORPHAN        1   /*!< Enable only if orphan scan/realignment support required */
#define MAC_154_PPL_TEST          1   /*!< Enable PPL testing */

#ifdef __cplusplus
};
#endif

#endif /* MAC_154_CFG_H */
