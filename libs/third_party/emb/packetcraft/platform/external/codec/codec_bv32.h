/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Audio codec interface file.
 *
*  Copyright (c) 2019-2021 Packetcraft, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef CODEC_BV32_H
#define CODEC_BV32_H

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

#define BV32_CODEC \
  { \
    .codecId     = PAL_CODEC_ID_BV32_15000US_16KHZ_16B_M, \
    .codingFmt   = HCI_ID_VS, \
    .frmMode     = PAL_CODEC_MODE_LEFT, \
    .name        = "BroadVoice 32: 15ms, 16kHz, 16bit, M", \
    .open        = CodecBv32Open, \
    .close       = CodecBv32Close, \
    .encode      = CodecBv32Encode, \
    .decode      = CodecBv32Decode, \
    .cap         = \
    { \
      .sampFreq       = 16000, \
      .frmDurUsec     = 15000, \
      .numChan        = 1, \
      .minFrmLen      = 70, \
      .maxFrmLen      = 70, \
      .minDlyOutUsec  = 950, \
      .maxDlyOutUsec  = 950, \
      .minDlyInUsec   = 5690, \
      .maxDlyInUsec   = 5690 \
    } \
  }

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Data Path */
void CodecBv32Open(uint8_t id, PalCodecDir_t dir, uint32_t sampFreq, uint32_t frmDurUsec);
void CodecBv32Close(uint8_t id);
uint16_t CodecBv32Encode(uint8_t id, uint8_t *pEncBuf, uint16_t len, int16_t *pPcmBuf);
bool CodecBv32Decode(uint8_t id, int16_t *pPcmBuf, const uint8_t *pEncBuf, uint16_t len);

/*! \} */    /* PAL_CODEC */

#ifdef __cplusplus
};
#endif

#endif /* CODEC_BV32_H */
