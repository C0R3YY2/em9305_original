/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      PAL board definition lookup.
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

#ifndef PAL_BOARD_H
#define PAL_BOARD_H

#if defined(BOARD_PCA10056)
#include "pal_board_pca10056.h"
#elif defined(BOARD_PCA10040)
#include "pal_board_pca10040.h"
#elif defined(BOARD_PCA10112)
#include "pal_board_pca10112.h"
#endif

#endif /* PAL_BOARD_H */
