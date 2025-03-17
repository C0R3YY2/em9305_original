/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Audio interface common definition file.
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

#ifndef AIF_DEFS_H
#define AIF_DEFS_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef AIF_MAX_STREAM
/*! \brief  Maximum number of streams. */
#define AIF_MAX_STREAM      4
#endif

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  Message opcode. */
typedef enum
{
  /* Stream Control */
  AIF_OPCODE_START_STREAM   = 0x0000,
  AIF_OPCODE_STOP_STREAM    = 0x0001,
  AIF_OPCODE_DATA_IN        = 0x0002,
  AIF_OPCODE_DATA_OUT       = 0x0003,
  AIF_OPCODE_DATA_IN_REQ    = 0x0004,

  /* Amp Control */
  AIF_OPCODE_SET_VOLUME     = 0x0100,
  AIF_OPCODE_SET_GAIN       = 0x0101,

  /* Device Management */
  AIF_OPCODE_CUR_TIME_REQ   = 0x0200,
  AIF_OPCODE_CUR_TIME_RSP   = 0x0201,
  AIF_OPCODE_CODEC_CAP_REQ  = 0x0202,
  AIF_OPCODE_CODEC_CAP_RSP  = 0x0203,
  AIF_OPCODE_BUTTON_ACTION  = 0x0204,
  AIF_OPCODE_SET_LED_STATE  = 0x0205
} AifOpcode_t;

/*** Stream Control ***/

/*! \brief  Audio stream start. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t id;              /*!< Stream ID. */
  uint16_t dir;             /*!< Direction. */
  uint16_t chMask;          /*!< Channel mask. */
  uint16_t codecId;         /*!< Codec ID. */
  uint16_t presDlyMs;       /*!< Presentation delay in milliseconds. */
  uint32_t syncDelayUsec;   /*!< Synchronization delay in microseconds. */
} AifStartStream_t;

/*! \brief  Audio stream stop. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t id;              /*!< Stream ID. */
  uint16_t dir;             /*!< Direction. */
} AifStopStream_t;

/*! \brief  Audio data input (audio subsystem to connectivity subsystem). */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t id;              /*!< Stream ID. */
  uint16_t len;             /*!< SDU buffer length. */
  uint32_t pktCtr;          /*!< Packet counter. */
  uint32_t ts;              /*!< Packet timestamp. */
  /* uint8_t buf[]; */      /* Buffer follows header. */
} AifDataIn_t;

/*! \brief  Audio data output (connectivity subsystem to audio subsystem). */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t id;              /*!< Stream ID. */
  uint16_t len;             /*!< SDU buffer length. */
  uint32_t pktCtr;          /*!< Packet counter. */
  uint32_t ts;              /*!< Packet timestamp. */
  /* uint8_t buf[]; */      /* Buffer follows header. */
} AifDataOut_t;

/*! \brief  Audio data input request (audio subsystem to connectivity subsystem). */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t id;              /*!< Stream ID. */
  uint16_t len;             /*!< SDU buffer length. */
  uint32_t ts;              /*!< Exepcted packet timestamp. */
} AifDataInReq_t;

/*** Amp Control ***/

/*! \brief  Set volume. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  int8_t  volume;          /*!< Volume (-127 to 127 dB). */
} AifSetVolume_t;

/*! \brief  Set input gain. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  int8_t  gain;            /*!< Gain (-127 to 127 dB). */
} AifSetGain_t;

/*** Device Management ***/

/*! \brief  Get current time request. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t codecId;         /*!< Codec ID. */
} AifGetCurTimeReq_t;

/*! \brief  Get current time response. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint8_t  time;            /*!< Current time. */
} AifGetCurTimeRsp_t;

/*! \brief  Get codec capability request. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t codecId;         /*!< Codec ID. */
} AifGetCodecCapReq_t;

/*! \brief  Button action. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint8_t btnId;            /*!< Button ID. */
  uint8_t pos;              /*!< Button position. */
} AifButtonAction_t;

/*! \brief  Set LED state. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint8_t ledId;            /*!< LED ID. */
  bool state;             /*!< LED state (true - on, false - off). */
} AifSetLedState_t;

/*! \brief  Get codec capability response. */
typedef struct
{
  uint16_t opcode;          /*!< AIF command opcode. */
  uint16_t codecId;         /*!< Codec ID. */
  uint32_t sampFreq;        /*!< Sampling frequency. */
  uint16_t numChan;         /*!< Channel count. */
  uint16_t _reserved;       /*!< Reserved for future use. */
  uint32_t frmDurUsec;      /*!< Frame duration in microseconds. */
  uint16_t minFrmLen;       /*!< Minimum frame length. */
  uint16_t maxFrmLen;       /*!< Maximum frame length. */
  uint32_t minDlyOutUsec;   /*!< Minimum output delay in microseconds. */
  uint32_t maxDlyOutUsec;   /*!< Maximum output delay in microseconds. */
  uint32_t minDlyInUsec;    /*!< Minimum input delay in microseconds. */
  uint32_t maxDlyInUsec;    /*!< Maximum input delay in microseconds. */
} AifGetCodecCapRsp_t;

#ifdef __cplusplus
};
#endif

#endif /* AIF_DEFS_H */
