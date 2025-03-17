/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Published Audio Capability service implementations.
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

#ifndef SVC_PAC_H
#define SVC_PAC_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAC_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*!< \brief Size of a Sink or Source PAC record */
#define PACS_PAC_REC_SIZE(cl, ml)   (7 + (cl) + (ml))

/*!< \brief Maximum number of the Sink and Source PAC records */
#ifndef PACS_PAC_MAX
#define PACS_PAC_MAX                4
#endif

/*!< \brief Length of the Sink or Source PAC characteristic value */
#define PACS_PAC_LEN                (1 + (PACS_PAC_MAX * PACS_PAC_REC_SIZE(GA_MAX_CODEC_LEN, GA_MAX_METADATA_LEN)))

/*!< \brief Maximum length of the Sink or Source PAC record */
#define PACS_MAX_PAC_LEN            255

/*!< \brief Length of the Sink or Source Audio Locations */
#define PACS_AUDIO_LOC_LEN          4

/**************************************************************************************************
  Handle Ranges
**************************************************************************************************/
/** \name PAC Service Handles
 *
 */
/**@{*/
#define PACS_START_HDL                0x0720             /*!< PAC start handle */
#define PACS_END_HDL                  (PACS_MAX_HDL - 1) /*!< PAC end handle */
/**@}*/

/**************************************************************************************************
  Handles
**************************************************************************************************/

/** \name PAC Service Handles
 *
 */
/**@{*/
/*! \brief PAC service handle */
enum
{
  PACS_SVC_HDL = PACS_START_HDL,    /*!< PAC service declaration */
  PACS_SNK_PAC_CH_HDL,              /*!< Sink PAC characteristic */
  PACS_SNK_PAC_HDL,                 /*!< Sink PAC value */
  PACS_SNK_PAC_CH_CCC_HDL,          /*!< Sink PAC CCCD */
  PACS_SNK_LOC_CH_HDL,              /*!< Sink audio locations characteristic */
  PACS_SNK_LOC_HDL,                 /*!< Sink audio locations value */
  PACS_SNK_LOC_CH_CCC_HDL,          /*!< Sink audio locations CCCD */
  PACS_SRC_PAC_CH_HDL,              /*!< Source PAC characteristic */
  PACS_SRC_PAC_HDL,                 /*!< Source PAC value */
  PACS_SRC_PAC_CH_CCC_HDL,          /*!< Source PAC CCCD */
  PACS_SRC_LOC_CH_HDL,              /*!< Source audio locations characteristic */
  PACS_SRC_LOC_HDL,                 /*!< Source audio locations value */
  PACS_SRC_LOC_CH_CCC_HDL,          /*!< Source audio locations CCCD */
  PACS_AV_AUD_CTX_CH_HDL,           /*!< Available Audio Contexts characteristic */
  PACS_AV_AUD_CTX_HDL,              /*!< Available Audio Contexts value */
  PACS_AV_AUD_CTX_CH_CCC_HDL,       /*!< Available Audio Contexts client characteristic configuration */
  PACS_SUP_AUD_CTX_CH_HDL,          /*!< Supported Audio Contexts characteristic */
  PACS_SUP_AUD_CTX_HDL,             /*!< Supported Audio Contexts value */
  PACS_SUP_AUD_CTX_CH_CCC_HDL,      /*!< Supported Audio Contexts client characteristic configuration */

  PACS_MAX_HDL                      /*!< PAC maximum handle */
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
void SvcPacAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcPacRemoveGroup(void);

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
void SvcPacCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* PAC_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_PAC_H */
