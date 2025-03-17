/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Audio Stream Control (ASC) service implementations.
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

#ifndef SVC_ASC_H
#define SVC_ASC_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup ASC_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*!< \brief Maximum number of concurrent Audio Streams supported by the server for a single
 *          client.
 *
 *   \note The server limits the number of Sink and Source ASE characteristics exposed to the
 *         number of concurrent Audio Streams it can support for a single client.
 *
 *   \note When adjusting the number of concurrent Audio Streams, the number of Sink and Source
 *         ASE characteristics defined in the ASC service must be adjusted accordingly.
 */
#define ASCS_AUDIO_STREAM_MAX           4

/** \name ASE Characteristic Length Fields
 * Length constants of ASE characteristic fixed length fields
 */
 /**@{*/
#define ASCS_ASE_COMMON_LEN             2          /*!< \brief ASE common field length */
#define ASCS_ASE_ADD_PARAMS_CODEC_LEN   17         /*!< \brief ASE additional parameter length when for Codec Configured state */
#define ASCS_ASE_ADD_PARAMS_QOS_LEN     15         /*!< \brief ASE additional parameter length when for QoS Configured state */
#define ASCS_ASE_ADD_PARAMS_ENABLE_LEN  3          /*!< \brief ASE additional parameter length when for Enabling Configured state */
/**@}*/

/** \name ASE Control Point Characteristic Length Fields
 * Length constant of ASE Control Point characteristic fixed length field
 */
 /**@{*/
#define ASCS_ASE_CP_COMMON_LEN          2          /*!< ASE Control Point common field length */
/**@}*/

/** \name ASE Control Point Characteristic Length Fields
* Length constant of ASE Control Point characteristic response length fields
*/
/**@{*/
#define ASCS_ASE_CP_RSP_LEN            3          /*!< ASE Control Point response field length */
/**@}*/

/**@}*/
/*!< \brief Maximum length of the ASE characteristic value */
#define ASCS_MAX_ASE_LEN                251

/*!< \brief Maximum length of the ASE Control Point characteristic value */
#define ASCS_MAX_ASE_CP_LEN             251

/*!< \brief Length of the Available Audio Contexts characteristic value */
#define ASCS_AV_AUD_CTX_LEN             4

/*!< \brief Length of the ASE characteristic value */
#define ASCS_ASE_LEN                    (ASCS_ASE_COMMON_LEN + ASCS_ASE_ADD_PARAMS_CODEC_LEN + GA_MAX_CODEC_LEN)

/*!< \brief Length of the ASE Control Point characteristic value */
#define ASCS_ASE_CP_LEN                 (ASCS_ASE_CP_COMMON_LEN + (ASCS_ASE_CP_RSP_LEN * ASCS_AUDIO_STREAM_MAX))

/**************************************************************************************************
  Handle Ranges
**************************************************************************************************/
/** \name ASC Service Handles
 *
 */
/**@{*/
#define ASCS_START_HDL                0x0700              /*!< ASES start handle */
#define ASCS_END_HDL                  (ASCS_MAX_HDL - 1)  /*!< ASES end handle */
/**@}*/

/**************************************************************************************************
  Handles
**************************************************************************************************/

/** \name ASC Service Handles
 *
 */
/**@{*/
/*! \brief ASC service handle */
enum
{
  ASCS_SVC_HDL = ASCS_START_HDL,    /*!< \brief ASE service declaration */
  ASCS_ASE_CH_1_HDL,                /*!< \brief Sink ASE #1 characteristic */
  ASCS_SNK_ASE_1_HDL,               /*!< \brief Sink ASE #1 value */
  ASCS_SNK_ASE_CH_CCC_1_HDL,        /*!< \brief Sink ASE #1 client characteristic configuration */
  ASCS_SNK_ASE_CH_2_HDL,            /*!< \brief Sink ASE #2 characteristic */
  ASCS_SNK_ASE_2_HDL,               /*!< \brief Sink ASE #2 value */
  ASCS_SNK_ASE_CH_CCC_2_HDL,        /*!< \brief Sink ASE #2 client characteristic configuration */
  ASCS_SRC_ASE_CH_1_HDL,            /*!< \brief Source ASE #1 characteristic */
  ASCS_SRC_ASE_1_HDL,               /*!< \brief Source ASE #1 value */
  ASCS_SRC_ASE_CH_CCC_1_HDL,        /*!< \brief Source ASE #1 client characteristic configuration */
  ASCS_SRC_ASE_CH_2_HDL,            /*!< \brief Source ASE #2 characteristic */
  ASCS_SRC_ASE_2_HDL,               /*!< \brief Source ASE #2 value */
  ASCS_SRC_ASE_CH_CCC_2_HDL,        /*!< \brief Source ASE #2 client characteristic configuration */
  ASCS_ASE_CP_CH_HDL,               /*!< \brief ASE Control Point characteristic */
  ASCS_ASE_CP_HDL,                  /*!< \brief ASE Control Point value */
  ASCS_ASE_CP_CH_CCC_HDL,           /*!< \brief ASE Control Point client characteristic configuration */
  ASCS_MAX_HDL                      /*!< \brief ASES maximum handle */
};
/**@}*/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAscAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAscRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void SvcAscCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* ASE_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* #define SVC_ASC_H */
