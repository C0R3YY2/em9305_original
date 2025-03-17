/*************************************************************************************************/
/*!
 *  \file   svc_esl.h
 *
 *  \brief  Example Electronic Shelf Label service implementation.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_ESL_H
#define SVC_ESL_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup ELECTRONIC_SHELF_LABEL_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Characteristic Data Length */
#define TBD 16

#define ESL_NUM_DISPLAYS          2U      /*!< Number of displays. */
#define ESL_NUM_SENSORS           2U      /*!< Number of sensors. */
#define ESL_NUM_LEDS              4U      /*!< Number of LEDs. */

#define ESL_KEY_MATERIAL_LEN          24U       /*!<Encription Key Material length. */
#define ESL_DISPLAY_INFO_STRUCT_LEN   5U        /*!< Display information structure length. */
#define ESL_SENSOR_INFO_STRUCT_LEN    5U        /*!< Sensor information structure length. */
#define ESL_CONTROL_POINT_QUEUE_LEN   0x80U     /*!< Control point length. */

#define ESL_ADDRESS_PAYLOAD_LEN               2U                            /*!< ESL address Value */
#define ESL_AP_SYNC_KEY_MATERIAL_PAYLOAD_LEN  ESL_KEY_MATERIAL_LEN                           /*!< ESL application sync key material Value */
#define ESL_RESP_KEY_MATERIAL_PAYLOAD_LEN     ESL_KEY_MATERIAL_LEN                           /*!< ESL response key material Value */
#define ESL_CURRENT_ABS_TIME_PAYLOAD_LEN      4U                            /*!< ESL current absolute time Value */
#define ESL_DISPLAY_INFO_PAYLOAD_LEN          (ESL_DISPLAY_INFO_STRUCT_LEN * ESL_NUM_DISPLAYS)   /*!< ESL display information Value */
#define ESL_IMAGE_INFO_PAYLOAD_LEN            4U                            /*!< ESL image information Value */
#define ESL_SENSOR_INFO_PAYLOAD_LEN           (ESL_SENSOR_INFO_STRUCT_LEN * ESL_NUM_SENSORS)     /*!< ESL sensor information Value */
#define ESL_LED_INFO_PAYLOAD_LEN              ESL_NUM_LEDS              /*!< ESL LED information Value */
#define ESL_CONTROL_POINT_PAYLOAD_LEN         ESL_CONTROL_POINT_QUEUE_LEN   /*!< ESL control point Value */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Electronic Shelf Label Service Handles
 *
 */
/**@{*/
#define ESL_START_HDL               0xA0U             /*!< Start handle. */
#define ESL_END_HDL                 (ESL_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Electronic Shelf Label Service Handles */
enum
{
  ESL_SVC_HDL = ESL_START_HDL,        /*!< Electronic shelf label service declaration */

  ESL_ADDRESS_CH_HDL,                 /*!< ESL address Characteristic Declaration */
  ESL_ADDRESS_HDL,                    /*!< ESL address Value */

  ESL_AP_SYNC_KEY_MATERIAL_CH_HDL,    /*!< ESL application sync key material Characteristic Declaration */
  ESL_AP_SYNC_KEY_MATERIAL_HDL,       /*!< ESL application sync key material Value */

  ESL_RESP_KEY_MATERIAL_CH_HDL,       /*!< ESL response key material Characteristic Declaration */
  ESL_RESP_KEY_MATERIAL_HDL,          /*!< ESL response key material Value */

  ESL_CURRENT_ABS_TIME_CH_HDL,        /*!< ESL current absolute time Characteristic Declaration */
  ESL_CURRENT_ABS_TIME_HDL,           /*!< ESL current absolute time Value */

  ESL_DISPLAY_INFO_CH_HDL,            /*!< ESL display information Characteristic Declaration */
  ESL_DISPLAY_INFO_HDL,               /*!< ESL display information Value */

  ESL_IMAGE_INFO_CH_HDL,              /*!< ESL image information Characteristic Declaration */
  ESL_IMAGE_INFO_HDL,                 /*!< ESL image information Value */

  ESL_SENSOR_INFO_CH_HDL,             /*!< ESL sensor information Characteristic Declaration */
  ESL_SENSOR_INFO_HDL,                /*!< ESL sensor information Value */

  ESL_LED_INFO_CH_HDL,                /*!< ESL LED information Characteristic Declaration */
  ESL_LED_INFO_HDL,                   /*!< ESL LED information Value */

  ESL_CONTROL_POINT_CH_HDL,           /*!< ESL control point Characteristic Declaration */
  ESL_CONTROL_POINT_HDL,              /*!< ESL control point Value */
  ESL_CONTROL_POINT_CCC_HDL,              /*!< ESL control point Value */

  ESL_MAX_HDL                         /*!< Maximum handle. */
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
void SvcEslAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcEslRemoveGroup(void);

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
void SvcEslCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* ELECTRONIC_SHELF_LABEL_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_ESL_H */
