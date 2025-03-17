/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Audio interface (client role) definition file.
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

#ifndef AIFC_H
#define AIFC_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  Start stream received call signature. */
typedef void (*AifCStartStreamHandler_t)(uint16_t id, uint16_t dir, uint16_t chMask, uint16_t codecId, uint32_t intervalUsec, uint16_t presDlyMs);

/*! \brief  Stop stream received call signature. */
typedef void (*AifCStopStreamHandler_t)(uint16_t id, uint16_t dir);

/*! \brief  Data in request received call signature. */
typedef void (*AifCDataInRequestHandler_t)(uint16_t id, uint32_t ts, uint16_t len);

/*! \brief  Data output received call signature. */
typedef void (*AifCDataOutHandler_t)(uint16_t id, uint32_t pktCtr, uint32_t ts, uint8_t *pData, uint16_t actLen);

/*! \brief  Set volume received call signature. */
typedef void (*AifCSetVolumeHandler_t)(uint8_t volume);

/*! \brief  Get current time received call signature. */
typedef void (*AifCGetCurrentTimeHandler_t)(uint32_t time);

/*! \brief  Get current time received call signature. */
typedef void (*AifCGetCodecCapabilityRequestHandler_t)(uint16_t vsCodecId);

/*! \brief  Set input gain received call signature. */
typedef void (*AifCSetGainHandler_t)(uint8_t gain);

/*! \brief  Set LED state call signature. */
typedef void (*AifCSetLedStateHandler_t)(uint8_t ledId, bool state);

/*! \brief  AIF event handlers (service to client). */
typedef struct
{
  AifCStartStreamHandler_t    startStream;    /*!< Start stream event handler. */
  AifCStopStreamHandler_t     stopStream;     /*!< Stop stream event handler. */
  AifCDataInRequestHandler_t  dataInReq;      /*!< Data input request event handler. */
  AifCDataOutHandler_t        dataOut;        /*!< Data output event handler. */
  AifCSetVolumeHandler_t      setVolume;      /*!< Set volume event handler. */
  AifCGetCurrentTimeHandler_t getCurTimeRsp;  /*!< Get current time response handler. */
  AifCGetCodecCapabilityRequestHandler_t getCodecCapReq;
                                              /*!< Get codec capability request handler. */
  AifCSetGainHandler_t        setGain;        /*!< Set input gain event handler. */
  AifCSetLedStateHandler_t    setLedState;    /*!< Set LED state event handler. */
} AifCEventHandlers_t;

/**************************************************************************************************
  Function Declaration
**************************************************************************************************/

/* Initialization */
void AifCInit(const AifCEventHandlers_t *pEh);
void AifCDeInit(void);

/* Stream Control */
void AifCDataIn(uint16_t id, uint32_t pktCtr, uint32_t ts, const uint8_t *pData, uint16_t len);

/* Device Management */
void AifCCodecCapResponse(uint16_t codecId, uint32_t sampFreq, uint16_t numChan,
                          uint32_t frmDurUsec, uint16_t minFrmLen, uint16_t maxFrmLen,
                          uint32_t minDlyOutUsec, uint32_t maxDlyOutUsec,
                          uint32_t minDlyInUsec, uint32_t maxDlyInUsec);
/* Button action */
void AifCButtonAction(uint8_t btnId, uint8_t pos);

#ifdef __cplusplus
};
#endif

#endif /* AIFC_H */
