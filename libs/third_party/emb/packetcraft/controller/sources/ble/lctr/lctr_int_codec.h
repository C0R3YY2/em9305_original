/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller codec interface file.
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

#ifndef LCTR_INT_CODEC_H
#define LCTR_INT_CODEC_H

/*! \brief      lctrCodec data in callback. */
typedef void (*lctrCodecDataInCback_t)(uint16_t id, uint32_t pktCtr, uint32_t ts, uint8_t *pData,
                                       uint16_t actLen);
/*! \brief      lctrCodec data path direction. */
enum
{
  LCTR_CODEC_DIR_INPUT   = 0,        /*!< Input data path. */
  LCTR_CODEC_DIR_OUTPUT  = 1         /*!< Output data path. */
};
typedef uint8_t lctrCodecDir_t;

/*! \brief      lctrCodec Codec_ID specification structure. */
typedef struct
{
  uint8_t codingFmt;
  uint16_t compId;
  uint16_t vsCodecId;
} lctrCodecId_t;

/*! \brief      lctrCodec Codec Specific Configuration in LTV format. */
typedef struct
{
  uint8_t len;                     /*!< Codec configuration length. */
  uint8_t *pCfg;                   /*!< Pointer to codec configuration. */
} lctrCodecSpConfig_t;

/*! \brief      lctrCodec stream parameters. */
typedef struct
{
  lctrCodecDir_t         dir;                 /*!< Stream data direction. */
  uint32_t               pktCtr;              /*!< Initial packet counter value. */
  lctrCodecId_t          codecId;             /*!< Codec_ID. */
  lctrCodecSpConfig_t    codecConfig;         /*!< Codec Specific configuration. */
  uint32_t               syncDelayUsec;       /*!< Synchronization delay in microseconds. */
  lctrCodecDataInCback_t inCback;             /*!< Data input callback. */
  uint64_t               srcAddr;             /*!< Source device address. */
  uint64_t               dstAddr;             /*!< Destination device address. */
  bool                   triggerEncManually;  /*!< Trigger encode manually. */
} lctrCodecStreamParam_t;

/*! \brief      Start stream call signature. */
typedef bool (*lctrCodecStartStream_t)(uint16_t id, lctrCodecStreamParam_t *pParam);

/*! \brief      Stop stream call signature. */
typedef void (*lctrCodecStopStream_t)(uint16_t id, lctrCodecDir_t dir);

/*! \brief      Stream in data request call signature. */
typedef void (*lctrCodecStreamInReq_t)(uint16_t id, uint8_t *pData, uint16_t len, uint32_t sduRef);

/*! \brief      Stream out data call signature. */
typedef void (*lctrCodecStreamOut_t)(uint16_t id, const uint8_t *pBuf, uint16_t len, uint32_t sduRef);

/*! \brief      Codec event handlers. */
typedef struct
{
  lctrCodecStartStream_t  start;          /*!< Start stream. */
  lctrCodecStopStream_t   stop;           /*!< Stop stream. */
  lctrCodecStreamInReq_t  inReq;          /*!< Stream data input request. */
  lctrCodecStreamOut_t    out;            /*!< Stream data output. */
} lctrCodecHandlers_t;

#endif /* LCTR_INT_CODEC_H */
