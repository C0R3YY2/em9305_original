/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Wireless Data Exchange service implementation.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_WDXS_H
#define SVC_WDXS_H

#include "util/bstream.h"
#include "att_api.h"
#include "wdx_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WIRELESS_DATA_EXCHANGE_SERVICE
 *  \{ */

/**************************************************************************************************
Configuration
**************************************************************************************************/

/*! Max file transfer data length */
#ifndef WDXS_FTD_PAYLOAD_MAX
#define WDXS_FTD_PAYLOAD_MAX ATT_DEFAULT_PAYLOAD_LEN
#endif

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Arm Ltd. Proprietary Service Handles
 *
 */
/**@{*/
#define WDXS_START_HDL               0x240              /*!< Start handle. */
#define WDXS_END_HDL                 (WDXS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Proprietary Service Handles */
enum
{
  WDXS_SVC_HDL = WDXS_START_HDL,     /*!< Proprietary Service Declaration */
  WDXS_DC_CH_HDL,                    /*!< WDX Device Configuration Characteristic Declaration */
  WDXS_DC_HDL,                       /*!< WDX Device Configuration Characteristic Value */
  WDXS_DC_CH_CCC_HDL,                /*!< WDX Device Configuration CCCD */
  WDXS_FTC_CH_HDL,                   /*!< WDX File Transfer Control Characteristic Declaration */
  WDXS_FTC_HDL,                      /*!< WDX File Transfer Control Characteristic Value */
  WDXS_FTC_CH_CCC_HDL,               /*!< WDX File Transfer Control CCCD */
  WDXS_FTD_CH_HDL,                   /*!< WDX File Transfer Data Characteristic Declaration */
  WDXS_FTD_HDL,                      /*!< WDX File Transfer Data Characteristic Value */
  WDXS_FTD_CH_CCC_HDL,               /*!< WDX File Transfer Data CCCD */
  WDXS_AU_CH_HDL,                    /*!< WDX Authentication Characteristic Declaration */
  WDXS_AU_HDL,                       /*!< WDX Authentication Characteristic Value */
  WDXS_AU_CH_CCC_HDL,                /*!< WDX Authentication CCCD */
  WDXS_MAX_HDL                       /*!< Maximum handle. */
};
/**@}*/

/**************************************************************************************************
  Global Declarations
**************************************************************************************************/

extern const uint8_t wdxsDcUuid[ATT_128_UUID_LEN];          /*!< WDX Device Configuration Characteristic */
extern const uint8_t wdxsFtcUuid[ATT_128_UUID_LEN];         /*!< WDX File Transfer Control Characteristic */
extern const uint8_t wdxsFtdUuid[ATT_128_UUID_LEN];         /*!< WDX File Transfer Data Characteristic */
extern const uint8_t wdxsAuUuid[ATT_128_UUID_LEN];          /*!< WDX Authentication Characteristic */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcWdxsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcWdxsRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register a write callback functions for the ATT Group.
 *
 *  \param  writeCback   Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcWdxsRegister(attsWriteCback_t writeCback);

/*! \} */    /* WIRELESS_DATA_EXCHANGE_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_WDXS_H */
