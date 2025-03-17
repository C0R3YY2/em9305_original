/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Audio interface (service role) definition file.
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

#ifndef AIFS_H
#define AIFS_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  Data input received call signature. */
typedef void (*AifSDataInHandler_t)(uint16_t id, uint32_t pktCtr, uint32_t ts, uint8_t *pData, uint16_t actLen);

/*! \brief  Codec capability received call signature. */
typedef void (*AifSCodecCapHandler_t)(uint16_t codecId, uint32_t sampFreq, uint16_t numChan,
                                      uint32_t frmDurUsec, uint16_t minFrmLen, uint16_t maxFrmLen,
                                      uint32_t minDlyOutUsec, uint32_t maxDlyOutUsec,
                                      uint32_t minDlyInUsec, uint32_t maxDlyInUsec);

/*! \brief  Button action call signature. */
typedef void (*AifSButtonActionHandler_t)(uint8_t btnId, uint8_t pos);

/*! \brief  AIF event handlers (client to service). */
typedef struct
{
  AifSDataInHandler_t    dataIn;    /*!< Data input event handler. */
  AifSCodecCapHandler_t  cap;       /*!< Codec capability handler. */
  AifSButtonActionHandler_t btn;    /*!< Button action handler. */
} AifSEventHandlers_t;

/**************************************************************************************************
  Function Declaration
**************************************************************************************************/

/* Initialization */
void AifSInit(const AifSEventHandlers_t *pEh);
void AifSDeInit(void);

/* Amp Control */
void AifSSetVolume(int8_t volume);
void AifSSetGain(int8_t gain);

/* Stream Control */
void AifSStartStream(uint16_t id, uint16_t dir, uint16_t chMask, uint16_t codecId,
                     uint32_t syncDelayUsec, uint16_t presDlyMs);
void AifSStopStream(uint16_t id, uint16_t dir);
void AifSDataOut(uint16_t id, uint32_t pktCtr, uint32_t ts, const uint8_t *pData, uint16_t len);
void AifSDataInRequest(uint16_t id, uint8_t *pData, uint16_t len, uint32_t ts);

/* Device Management */
void AifSDataInResponse(uint32_t time);
void AifCGetCodecCapabilityRequest(uint16_t codecId);
void AifSSetLedState(uint8_t ledId, bool state);

#ifdef __cplusplus
};
#endif

#endif /* AIFS_H */
