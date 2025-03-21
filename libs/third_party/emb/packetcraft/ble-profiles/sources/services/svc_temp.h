/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example temperature sensor service implementation.
*
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_TEMP_H
#define SVC_TEMP_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup TEMPERATURE_SENSOR_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Temperature Service Handles
 *
 */
/**@{*/
#define TEMP_HANDLE_START  0x60                          /*!< Start handle. */
#define TEMP_HANDLE_END   (TEMP_HANDLE_END_PLUS_ONE - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Temperature service handles. */
enum
{
  TEMP_HANDLE_SVC = TEMP_HANDLE_START, /*!< Service declaration. */

  TEMP_HANDLE_DATA_CHR,                /*!< Data characteristic declaration. */
  TEMP_HANDLE_DATA,                    /*!< Data characteristic value. */
  TEMP_HANDLE_DATA_CLIENT_CHR_CONFIG,  /*!< Data characteristic CCCD. */
  TEMP_HANDLE_DATA_CHR_USR_DESCR,      /*!< Data characteristic user description. */

  TEMP_HANDLE_CONFIG_CHR,              /*!< Configruation characteristic declaration. */
  TEMP_HANDLE_CONFIG,                  /*!< Configruation characteristic value. */
  TEMP_HANDLE_CONFIG_CHR_USR_DESCR,    /*!< Configruation characteristic user description. */

  TEMP_HANDLE_PERIOD_CHR,              /*!< Period characteristc declaration. */
  TEMP_HANDLE_PERIOD,                  /*!< Period characteristc value. */
  TEMP_HANDLE_PERIOD_CHR_USR_DESCR,    /*!< Period characteristc user description. */

  TEMP_HANDLE_END_PLUS_ONE             /*!< Maximum handle. */
};
/**@}*/

/**************************************************************************************************
 Macros
**************************************************************************************************/

/** \name Configuration Attributes
 *
 */
/**@{*/
#define TEMP_ATT_CONFIG_DISABLE  0x00u  /*!< Disable */
#define TEMP_ATT_CONFIG_ENABLE   0x01u  /*!< Enable */
/**@}*/

/** \name Values for Period Attributes
 *
 */
/**@{*/
#define TEMP_ATT_PERIOD_MAX      250u  /*!< Maximum period */
#define TEMP_ATT_PERIOD_DEFAULT  100u  /*!< Default period */
#define TEMP_ATT_PERIOD_MIN       10u  /*!< Minimum period */
/**@}*/

/** \name Sizes of Attributes
 *
 */
/**@{*/
#define TEMP_SIZE_CONFIG_ATT  1u  /*!< Config Attribute size */
#define TEMP_SIZE_PERIOD_ATT  1u  /*!< Period Attribute size */
#define TEMP_SIZE_DATA_ATT    6u  /*!< Data Attribute size */
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
void SvcTempAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTempRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcTempCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* TEMPERATURE_SENSOR_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_TEMP_H */
