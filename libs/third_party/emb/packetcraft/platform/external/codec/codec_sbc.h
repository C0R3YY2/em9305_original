/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Audio codec interface file.
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

#ifndef CODEC_SBC_H
#define CODEC_SBC_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup PAL_CODEC
 *  \{
 */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#if defined(NRF5340_XXAA_APPLICATION)

#define SBC_CODEC_VQ_JS \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_16KHZ_16B_JS, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_STEREO, \
    .name        = "Bluedroid SBC: 10ms, 16kHz, 16bit, JS", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 48, \
      .numChan        = 1, \
      .minFrmLen      = 160, \
      .maxFrmLen      = 160, \
      .minDlyOutUsec  = 1350, \
      .maxDlyOutUsec  = 1450, \
      .minDlyInUsec   = 1500, \
      .maxDlyInUsec   = 1600 \
    } \
  }

#define SBC_CODEC_VQ_L \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_16KHZ_16B_L, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_LEFT, \
    .name        = "Bluedroid SBC: 10ms, 16kHz, 16bit, L", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 48, \
      .numChan        = 1, \
      .minFrmLen      = 60, \
      .maxFrmLen      = 60, \
      .minDlyOutUsec  = 250, \
      .maxDlyOutUsec  = 350, \
      .minDlyInUsec   = 200, \
      .maxDlyInUsec   = 300 \
    } \
  }

#define SBC_CODEC_VQ_R \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_16KHZ_16B_R, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_RIGHT, \
    .name        = "Bluedroid SBC: 10ms, 16kHz, 16bit, R", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 48, \
      .numChan        = 1, \
      .minFrmLen      = 60, \
      .maxFrmLen      = 60, \
      .minDlyOutUsec  = 250, \
      .maxDlyOutUsec  = 350, \
      .minDlyInUsec   = 200, \
      .maxDlyInUsec   = 300 \
    } \
  }

#define SBC_CODEC_HQ_L \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_48KHZ_16B_L, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_LEFT, \
    .name        = "Bluedroid SBC: 10ms, 48kHz, 16bit, L", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 48000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 124, \
      .numChan        = 1, \
      .minFrmLen      = 155, \
      .maxFrmLen      = 155, \
      .minDlyOutUsec  = 450, \
      .maxDlyOutUsec  = 550, \
      .minDlyInUsec   = 550, \
      .maxDlyInUsec   = 650 \
    } \
  }

#define SBC_CODEC_HQ_R \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_48KHZ_16B_R, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_RIGHT, \
    .name        = "Bluedroid SBC: 10ms, 48kHz, 16bit, R", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 48000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 124, \
      .numChan        = 1, \
      .minFrmLen      = 155, \
      .maxFrmLen      = 155, \
      .minDlyOutUsec  = 450, \
      .maxDlyOutUsec  = 550, \
      .minDlyInUsec   = 550, \
      .maxDlyInUsec   = 650 \
    } \
  }

  #endif

#if defined(NRF52840_XXAA)

#define SBC_CODEC_VQ_JS \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_16KHZ_16B_JS, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_STEREO, \
    .name        = "Bluedroid SBC: 10ms, 16kHz, 16bit, JS", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 48, \
      .numChan        = 1, \
      .minFrmLen      = 160, \
      .maxFrmLen      = 160, \
      .minDlyOutUsec  = 1350, \
      .maxDlyOutUsec  = 1450, \
      .minDlyInUsec   = 1500, \
      .maxDlyInUsec   = 1600 \
    } \
  }

#define SBC_CODEC_VQ_L \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_16KHZ_16B_L, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_LEFT, \
    .name        = "Bluedroid SBC: 10ms, 16kHz, 16bit, L", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 48, \
      .numChan        = 1, \
      .minFrmLen      = 60, \
      .maxFrmLen      = 60, \

      .minDlyOutUsec  = 500, \
      .maxDlyOutUsec  = 600, \
      .minDlyInUsec   = 600, \
      .maxDlyInUsec   = 800 \
    } \
  }

#define SBC_CODEC_VQ_R \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_16KHZ_16B_R, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_RIGHT, \
    .name        = "Bluedroid SBC: 10ms, 16kHz, 16bit, R", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 48, \
      .numChan        = 1, \
      .minFrmLen      = 60, \
      .maxFrmLen      = 60, \
      .minDlyOutUsec  = 500, \
      .maxDlyOutUsec  = 600, \
      .minDlyInUsec   = 600, \
      .maxDlyInUsec   = 800 \
    } \
  }

#define SBC_CODEC_HQ_L \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_48KHZ_16B_L, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_LEFT, \
    .name        = "Bluedroid SBC: 10ms, 48kHz, 16bit, L", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 48000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 124, \
      .numChan        = 1, \
      .minFrmLen      = 155, \
      .maxFrmLen      = 155, \
      .minDlyOutUsec  = 1200, \
      .maxDlyOutUsec  = 1400, \
      .minDlyInUsec   = 1300, \
      .maxDlyInUsec   = 1500 \
    } \
  }

#define SBC_CODEC_HQ_R \
  { \
    .codecId     = PAL_CODEC_ID_SBC_10000US_48KHZ_16B_R, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_RIGHT, \
    .name        = "Bluedroid SBC: 10ms, 48kHz, 16bit, R", \
    .open        = CodecSbcOpen, \
    .close       = CodecSbcClose, \
    .encode      = CodecSbcEncode, \
    .decode      = CodecSbcDecode, \
    .cap         = \
    { \
      .sampFreq       = 48000, \
      .frmDurUsec     = 10000, \
      .bitRate        = 124, \
      .numChan        = 1, \
      .minFrmLen      = 155, \
      .maxFrmLen      = 155, \
      .minDlyOutUsec  = 1200, \
      .maxDlyOutUsec  = 1400, \
      .minDlyInUsec   = 1300, \
      .maxDlyInUsec   = 1500 \
    } \
  }

#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Data Path */
void CodecSbcOpen(uint8_t id, PalCodecDir_t dir, uint32_t sampFreq, uint32_t frmDurUsec,
                  uint32_t bitRate);
void CodecSbcClose(uint8_t id);
uint16_t CodecSbcEncode(uint8_t id, uint8_t *pEncBuf, uint16_t len, int16_t *pPcmBuf);
bool CodecSbcDecode(uint8_t id, int16_t *pPcmBuf, const uint8_t *pEncBuf, uint16_t len);

/*! \} */    /* PAL_CODEC */

#ifdef __cplusplus
};
#endif

#endif /* CODEC_SBC_H */
