/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      PAL internal definitions for CTE Validation.
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

#ifndef PAL_RADIO2_CTE_H
#define PAL_RADIO2_CTE_H

#include "pal_types.h"
#include "pal_board.h"

/**************************************************************************************************
  Function pointer definitions
**************************************************************************************************/

/**
 * @brief Callback function to be called when IQ Samples are needed for validation.
 */
typedef void (*GetIQSamplesValidation_t)(int8_t* samp, uint8_t* pNumSamp, int8_t* pIBuf, int8_t* pQBuf);

/**
 * @brief Set a callback to provide IQ Samples for validation.
 * @note Instead of executing PAL code to get IQ Samples, it call the callback.
*/
void PalRadioCteSetIQSamplesCallback(GetIQSamplesValidation_t callback);

#endif /* PAL_RADIO2_CTE_H */
