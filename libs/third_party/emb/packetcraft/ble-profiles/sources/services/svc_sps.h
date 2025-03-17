/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Serial port service implementation.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_SPS_H
#define SVC_SPS_H

#include "wsf_types.h"
#include "wsf_assert.h"
#include "util/bstream.h"
#include "att_api.h"
#include "svc_sps_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup SERIAL_PORT_SERVICE
 *  \{ */

/**************************************************************************************************
Configuration
**************************************************************************************************/
/*! Flow control settings. */
#define SPP_TX_DATA_ENABLED  0
#define SPP_TX_DATA_DISABLED 1
#define SPP_RX_DATA_ENABLED  0
#define SPP_RX_DATA_DISABLED 1

/*! Max transfer data length. */
#define SPS_TX_DATA_ATT_PAYLOAD_LEN 150
#define SPS_RX_DATA_ATT_PAYLOAD_LEN 150
#define SPS_TX_READ_ATT_PAYLOAD_LEN 150
#define SPS_RX_READ_ATT_PAYLOAD_LEN 150

WSF_CT_ASSERT(SPS_TX_DATA_ATT_PAYLOAD_LEN <= ATT_MAX_MTU);
WSF_CT_ASSERT(SPS_RX_DATA_ATT_PAYLOAD_LEN <= ATT_MAX_MTU);
WSF_CT_ASSERT(SPS_TX_READ_ATT_PAYLOAD_LEN <= ATT_MAX_MTU);
WSF_CT_ASSERT(SPS_RX_READ_ATT_PAYLOAD_LEN <= ATT_MAX_MTU);

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Arm Ltd. Proprietary Service Handles
 *
 */
/**@{*/
#define SPS_START_HDL               0x4000              /*!< Start handle. */
#define SPS_END_HDL                 (SPS_MAX_HDL - 1)   /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Proprietary Service Handles. */
enum
{
  SPS_SVC_HDL = SPS_START_HDL,      /*!< Proprietary Service Declaration. */

  SPS_TX_DATA_CH_HDL,               /*!< SPS TX Data Characteristic Declaration. */
  SPS_TX_DATA_HDL,                  /*!< SPS TX Data Characteristic Value. */
  SPS_TX_DATA_CH_CCC_HDL,           /*!< SPS TX Data CCCD. */

  SPS_RX_DATA_CH_HDL,               /*!< SPS RX Data Characteristic Declaration. */
  SPS_RX_DATA_HDL,                  /*!< SPS RX Data Characteristic Value. */
  SPS_RX_DATA_CH_CCC_HDL,           /*!< SPS RX Data CCCD. */

  SPS_TX_READ_CH_HDL,               /*!< SPS TX Read Characteristic Declaration. */
  SPS_TX_READ_HDL,                  /*!< SPS TX Read Characteristic Value. */
  SPS_TX_READ_CH_CCC_HDL,           /*!< SPS TX Read CCCD. */

  SPS_RX_READ_CH_HDL,               /*!< SPS RX Read Characteristic Declaration. */
  SPS_RX_READ_HDL,                  /*!< SPS RX Read Characteristic Value. */
  SPS_RX_READ_CH_CCC_HDL,           /*!< SPS RX Read CCCD. */

  SPS_MAX_HDL                       /*!< Maximum handle. */
};
/**@}*/

/**************************************************************************************************
  Global Declarations
**************************************************************************************************/

extern const uint8_t spsTxDataUuid[ATT_128_UUID_LEN];          /*!< SP TX Data Characteristic. */
extern const uint8_t spsRxDataUuid[ATT_128_UUID_LEN];          /*!< SP RX Data Characteristic. */
extern const uint8_t spsTxReadUuid[ATT_128_UUID_LEN];          /*!< SP TX Read Characteristic. */
extern const uint8_t spsRxReadUuid[ATT_128_UUID_LEN];          /*!< SP RX Read Characteristic. */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services from the attribute server.
 */
/*************************************************************************************************/
void SvcSpsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 */
/*************************************************************************************************/
void SvcSpsRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register a write callback functions for the ATT Group.
 *
 *  \param  writeCback   Write callback function.
 */
/*************************************************************************************************/
void SvcSpsRegister(attsWriteCback_t writeCback);

/*! \} */    /* SERIAL_PORT_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_SPS_H */
