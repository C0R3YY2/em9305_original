/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Codec interface file.
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

#ifndef PAL_CODEC_H
#define PAL_CODEC_H

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

/*! \brief      Macro for extracting codec type from codec ID. */
#define PAL_CODEC_GET_TYPE(id) ((id) & 0xFF00uL)

/**************************************************************************************************'
  Data Types
**************************************************************************************************/

/*! \brief      Audio data path direction. */
enum
{
  PAL_CODEC_DIR_INPUT   = 0,    /*!< Input data path. */
  PAL_CODEC_DIR_OUTPUT  = 1     /*!< Output data path. */
};
typedef uint8_t PalCodecDir_t;

/*! \brief      Audio Channel. */
enum
{
  PAL_CODEC_CH_LEFT     = 0,    /*!< Left channel. */
  PAL_CODEC_CH_RIGHT    = 1,    /*!< Right channel. */
  PAL_CODEC_NUM_CH              /*!< Total number of channel. */
};
typedef uint8_t PalCodecChan_t;

/*! \brief      Codec types. */
enum
{
  PAL_CODEC_TYPE_LC3     = 0,
  PAL_CODEC_TYPE_SBC     = 0xF000,
  PAL_CODEC_TYPE_OPUS    = 0xF100,
  PAL_CODEC_TYPE_BV32    = 0xF200,
  PAL_CODEC_TYPE_ANY     = 0xFE00,
  PAL_CODEC_TYPE_INVALID = 0xFF00,
};
typedef uint8_t PalCodecType_t;

/*! \brief      Codec IDs. */
enum
{
  /* Standard codecs. */
  PAL_CODEC_ID_LC3_10000US_48KHZ_16B_L        = PAL_CODEC_TYPE_LC3  | 0x00,
  PAL_CODEC_ID_LC3_10000US_48KHZ_16B_R        = PAL_CODEC_TYPE_LC3  | 0x01,
  PAL_CODEC_ID_LC3_10000US_16KHZ_16B_L        = PAL_CODEC_TYPE_LC3  | 0x02,
  PAL_CODEC_ID_LC3_10000US_16KHZ_16B_R        = PAL_CODEC_TYPE_LC3  | 0x03,
  PAL_CODEC_ID_LC3_7500US_48KHZ_16B_L         = PAL_CODEC_TYPE_LC3  | 0x04,
  PAL_CODEC_ID_LC3_7500US_48KHZ_16B_R         = PAL_CODEC_TYPE_LC3  | 0x05,
  PAL_CODEC_ID_LC3_7500US_16KHZ_16B_L         = PAL_CODEC_TYPE_LC3  | 0x06,
  PAL_CODEC_ID_LC3_7500US_16KHZ_16B_R         = PAL_CODEC_TYPE_LC3  | 0x07,
  PAL_CODEC_ID_LC3_7500US_48KHZ_16B_160KBPS_L = PAL_CODEC_TYPE_LC3  | 0x08,
  PAL_CODEC_ID_LC3_7500US_48KHZ_16B_160KBPS_R = PAL_CODEC_TYPE_LC3  | 0x09,
  PAL_CODEC_ID_LC3_5000US_48KHZ_16B_L         = PAL_CODEC_TYPE_LC3  | 0x0A,
  PAL_CODEC_ID_LC3_5000US_48KHZ_16B_R         = PAL_CODEC_TYPE_LC3  | 0x0B,
  PAL_CODEC_ID_LC3_2500US_48KHZ_16B_L         = PAL_CODEC_TYPE_LC3  | 0x0C,
  PAL_CODEC_ID_LC3_2500US_48KHZ_16B_R         = PAL_CODEC_TYPE_LC3  | 0x0D,
  /* Custom codecs. */
  PAL_CODEC_ID_SBC_10000US_16KHZ_16B_JS       = PAL_CODEC_TYPE_SBC  | 0x00,
  PAL_CODEC_ID_SBC_10000US_48KHZ_16B_JS       = PAL_CODEC_TYPE_SBC  | 0x01,
  PAL_CODEC_ID_SBC_10000US_16KHZ_16B_L        = PAL_CODEC_TYPE_SBC  | 0x02,
  PAL_CODEC_ID_SBC_10000US_16KHZ_16B_R        = PAL_CODEC_TYPE_SBC  | 0x03,
  PAL_CODEC_ID_SBC_10000US_48KHZ_16B_L        = PAL_CODEC_TYPE_SBC  | 0x04,
  PAL_CODEC_ID_SBC_10000US_48KHZ_16B_R        = PAL_CODEC_TYPE_SBC  | 0x05,
  PAL_CODEC_ID_OPUS_10000US_48KHZ_16B_JS      = PAL_CODEC_TYPE_OPUS | 0x00,
  PAL_CODEC_ID_BV32_15000US_16KHZ_16B_M       = PAL_CODEC_TYPE_BV32 | 0x00,
  /* Auxiliary. */
  PAL_CODEC_ID_INVALID                        = 0xFFFF,
};
typedef uint8_t PalCodecId_t;

/*! \brief      Codec info block. */
typedef struct
{
  uint8_t codecFmt;             /*!< Coding format. */
  uint16_t codecId;             /*!< Codec ID. */
} PalCodecInfo_t;

/*! \brief      Standard codec info block. */
typedef struct
{
  uint8_t codecId;              /*!< Codec ID. */
  uint8_t supTrans;             /*!< Supported transport mask. */
} PalCodecStdInfo_t;

/*! \brief      VS codec info block. */
typedef struct
{
  uint16_t compId;              /*!< Company ID. */
  uint16_t codecId;             /*!< Codec ID. */
  uint8_t  supTrans;            /*!< Supported transport mask. */
} PalCodecVsInfo_t;

/*! \brief      Buffer available call signature. */
typedef void (*PalCodecDataInCback_t)(uint16_t id, uint32_t pktCtr, uint32_t ts, uint8_t *pData, uint16_t actLen);

/*! \brief      Stream parameters. */
typedef struct
{
  PalCodecDir_t dir;            /*!< Stream data direction. */
  uint32_t      pktCtr;         /*!< Initial packet counter value. */
  uint16_t      chMask;         /*!< Channel mask. */
  uint16_t      codecId;        /*!< Codec ID. */
  uint32_t      syncDelayUsec;  /*!< Synchronization delay in microseconds. */
  PalCodecDataInCback_t inCback;/*!< Data input callback. */
} PalCodecStreamParam_t;

/*! \brief      Codec capability. */
typedef struct
{
  uint32_t sampFreq;            /*!< Sampling frequency. */
  uint32_t frmDurUsec;          /*!< Frame duration in microseconds. */
  uint32_t bitRate;             /*!< Bit rate. */
  uint16_t numChan;             /*!< Channel count. */
  uint16_t minFrmLen;           /*!< Minimum frame length. */
  uint16_t maxFrmLen;           /*!< Maximum frame length. */
  uint32_t minDlyOutUsec;       /*!< Minimum output delay in microseconds. */
  uint32_t maxDlyOutUsec;       /*!< Maximum output delay in microseconds. */
  uint32_t minDlyInUsec;        /*!< Minimum input delay in microseconds. */
  uint32_t maxDlyInUsec;        /*!< Maximum input delay in microseconds. */
} PalCodecCap_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Codec Information */

/*************************************************************************************************/
/*!
 *  \brief  Get supported codec list.
 *
 *  \param  codingFmt     Coding format, search key for \a info.
 *  \param  pNumInfo      Number of \a info elements, returns number of codecs found.
 *  \param  info          Codec list.
 */
/*************************************************************************************************/
void PalCodecGetSupported(uint8_t codingFmt, uint8_t *pNumInfo, PalCodecInfo_t info[]);

/*************************************************************************************************/
/*!
 *  \brief  Get codec capability.
 *
 *  \param  codecId       Vendor Specific Codec ID, search key for \a pCap.
 *  \param  pCap          Codec capability.
 *
 *  \return true if capability found, false otherwise.
 */
/*************************************************************************************************/
bool PalCodecGetCapability(PalCodecId_t codecId, PalCodecCap_t *pCap);

/* TODO Deprecate */

/*************************************************************************************************/
/*!
 *  \brief   Read local supported codecs.
 *
 *  \param   pNumStd     Input is size of \a stdCodecs and output is actual number of standard codecs.
 *  \param   stdCodecs   Standard codec info.
 *  \param   pNumVs      Input is size of \a vsCodecs and output is actual number of vendor specific codecs.
 *  \param   vsCodecs    Vendor specific codec info.
 *
 *  This function is called by HCI when the host invokes HCI_Read_Local_Supported_Codecs.
 */
/*************************************************************************************************/
void PalCodecReadLocalSupportedCodecs(uint8_t *pNumStd, PalCodecStdInfo_t stdCodecs[],
                                      uint8_t *pNumVs, PalCodecVsInfo_t vsCodecs[]);

/*************************************************************************************************/
/*!
 *  \brief   Read local supported codec capabilities.
 *
 *  \param   codingFmt   Coding format.
 *  \param   compId      Company ID.
 *  \param   vsCodecId   Vendor specific codec ID.
 *  \param   dir         Direction.
 *  \param   transType   Transport type.
 *  \param   pCapLen     Input is size of \a pCap and output is actual size copied into \a pCap.
 *  \param   pCap        Capability buffer return parameter.
 *
 *  \return  true if valid, false otherwise.
 *
 *  This function is called by HCI when the host invokes HCI_Read_Local_Supported_Codec_Capabilities.
 */
/*************************************************************************************************/
bool PalCodecReadLocalSupportedCodecCapabilities(uint8_t codingFmt, uint16_t compId, uint16_t vsCodecId, PalCodecDir_t dir, uint8_t transType,
                                                   uint8_t *pCapLen, uint8_t *pCap);

/*************************************************************************************************/
/*!
 *  \brief   Read local supported codecs.
 *
 *  \param   codingFmt   Coding format.
 *  \param   compId      Company ID.
 *  \param   vsCodecId   Vendor specific codec ID.
 *  \param   dir         Direction.
 *  \param   transType   Transport type.
 *  \param   pMinDly     Min_Controller_Delay return value.
 *  \param   pMaxDly     Max_Controller_Delay return value.
 *
 *  \return  true if valid, false otherwise.
 *
 *  This function is called by HCI when the host invokes HCI_Read_Local_Supported_Controller_Delay.
 */
/*************************************************************************************************/
bool PalCodecReadLocalSupportedControllerDelay(uint8_t codingFmt, uint16_t compId, uint16_t vsCodecId, PalCodecDir_t dir, uint8_t transType,
                                                 uint32_t *pMinDly, uint32_t *pMaxDly);

/*************************************************************************************************/
/*!
 *  \brief   Configure data path.
 *
 *  \param   dir         Direction.
 *  \param   vsCfgLen    Length of \a pVsCfg buffer.
 *  \param   pVsCfg      Vendor specific configuration buffer.
 *
 *  \return  true if valid, false otherwise.
 *
 *  This function is called by the LL when the host invokes HCI_Configure_Data_Path with a
 *  Data_Path_ID=1 (\a LL_ISO_DATA_PATH_VS).
 *
 *  The contents of pVsCfg contains a single byte which resolves the PCM buffer mode,
 *  \a palCodecMode_t.
 */
/*************************************************************************************************/
bool PalCodecConfigureDataPath(PalCodecDir_t dir, uint8_t vsCfgLen, const uint8_t *pVsCfg);

/* Audio Amplifier */
void PalCodecAmpInit(void);            /*!< TBD */

/*! \brief      TBD
 *
 *  \return     TBD
 */
uint8_t PalCodecAmpGetVolume(void);

/*! \brief      TBD
 *
 *  \param      vol         TBD
 */
void PalCodecAmpSetVolume(int8_t vol); /*!< TBD */
void PalCodecAmpVolumeUp(void);        /*!< TBD */
void PalCodecAmpVolumeDown(void);      /*!< TBD */
void PalCodecAmpMute(void);            /*!< TBD */
void PalCodecAmpUnmute(void);          /*!< TBD */

/* Audio Input */
void PalCodecInputInit(void);           /*!< TBD */
void PalCodecInputMute(void);           /*!< TBD */
void PalCodecInputUnmute(void);         /*!< TBD */

/*************************************************************************************************/
/*!
 *  \brief      Set input gain.
 *
 *  \param      gain     New input gain value in dB.
 */
/*************************************************************************************************/
void PalCodecInputSetGain(int8_t gain);

/* Data Path */

/*************************************************************************************************/
/*!
 * \brief       Initialize data path resources.
 */
/*************************************************************************************************/
void PalCodecDataInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Start I2S interface.
 *
 *  \param      mclk      TBD
 *  \param      lrclk     TBD
 */
/*************************************************************************************************/
void PalCodecDataConfig(uint32_t mclk, uint32_t lrclk);

/*************************************************************************************************/
/*!
 *  \brief      Start audio stream.
 *
 *  \param      id        Stream ID.
 *  \param      pParam    Stream parameters.
 *
 *  \return     true if successful, false otherwise.
 *
 *  This function is called by the LL when the host invokes HCI_LE_Setup_ISO_Data_Path with a
 *  Data_Path_ID=1 (\a LL_ISO_DATA_PATH_VS).
 */
/*************************************************************************************************/
bool PalCodecDataStartStream(uint16_t id, PalCodecStreamParam_t *pParam);

/*************************************************************************************************/
/*! \brief      Stop audio stream.
 *
 *  \param      id        Stream ID.
 *  \param      dir       TBD
 */
/*************************************************************************************************/
void PalCodecDataStopStream(uint16_t id, PalCodecDir_t dir);

/*************************************************************************************************/
/*! \brief      Get input audio data.
 *
 *  \param      id        TBD
 *  \param      pBuf      TBD
 *  \param      len       TBD
 *  \param      sduRef    TBD
 */
/*************************************************************************************************/
void PalCodecDataStreamIn(uint16_t id, uint8_t *pBuf, uint16_t len, uint32_t sduRef);

/*************************************************************************************************/
/*! \brief      Output audio data.
 *
 *  \param      id        TBD
 *  \param      pBuf      TBD
 *  \param      len       TBD
 *  \param      sduRef    TBD
 */
/*************************************************************************************************/
void PalCodecDataStreamOut(uint16_t id, const uint8_t *pBuf, uint16_t len, uint32_t sduRef);

/*! \} */    /* PAL_CODEC */

#ifdef __cplusplus
};
#endif

#endif /* PAL_CODEC_H */
