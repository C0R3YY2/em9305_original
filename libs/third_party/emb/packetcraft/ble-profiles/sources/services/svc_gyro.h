/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example gyroscope sensor service implementation.
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

#ifndef SVC_GYRO_H
#define SVC_GYRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GYROSCOPE_SENSOR_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Gyroscope Sensor Service Handles
 *
 */
/**@{*/
#define GYRO_HANDLE_START  0x50                          /*!< Start handle. */
#define GYRO_HANDLE_END   (GYRO_HANDLE_END_PLUS_ONE - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Gyroscope service handles. */
enum
{
  GYRO_HANDLE_SVC = GYRO_HANDLE_START,    /*!< Service declaration. */

  GYRO_HANDLE_DATA_CHR,                   /*!< Data characteristic declaration. */
  GYRO_HANDLE_DATA,                       /*!< Data characteristic value. */
  GYRO_HANDLE_DATA_CLIENT_CHR_CONFIG,     /*!< Data characteristic CCCD. */
  GYRO_HANDLE_DATA_CHR_USR_DESCR,         /*!< Data characteristic user description. */

  GYRO_HANDLE_TEMPDATA_CHR,               /*!< Temporary data characteristic declaration. */
  GYRO_HANDLE_TEMPDATA,                   /*!< Temporary data characteristic value. */
  GYRO_HANDLE_TEMPDATA_CLIENT_CHR_CONFIG, /*!< Temporary data characteristic CCCD. */
  GYRO_HANDLE_TEMPDATA_CHR_USR_DESCR,     /*!< Temporary data characteristic user description. */

  GYRO_HANDLE_CONFIG_CHR,                 /*!< Configuration characteristic delcaration. */
  GYRO_HANDLE_CONFIG,                     /*!< Configuration characteristic value. */
  GYRO_HANDLE_CONFIG_CHR_USR_DESCR,       /*!< Configuration characteristic user description. */

  GYRO_HANDLE_PERIOD_CHR,                 /*!< Period characteristic declaration. */
  GYRO_HANDLE_PERIOD,                     /*!< Period characteristic value. */
  GYRO_HANDLE_PERIOD_CHR_USR_DESCR,       /*!< Period characteristic user description. */

  GYRO_HANDLE_END_PLUS_ONE                /*!< Maximum handle. */
};
/**@}*/

/**************************************************************************************************
 Macros
**************************************************************************************************/

/** \name Values for Config Attributes
 *
 */
/**@{*/
#define GYRO_ATT_CONFIG_DISABLE  0x00u /*!< Disable */
#define GYRO_ATT_CONFIG_ENABLE   0x01u /*!< Enable */
/**@}*/

/** \name Values for Period Attributes.
 *
 */
/**@{*/
#define GYRO_ATT_PERIOD_MAX      250u /*!< Maximum period */
#define GYRO_ATT_PERIOD_DEFAULT  100u /*!< Default period */
#define GYRO_ATT_PERIOD_MIN       10u /*!< Minimum period */
/**@}*/

/*! \brief Sizes of attributes. */
#define GYRO_SIZE_CONFIG_ATT       1u /*!< Configuration attribute size */
#define GYRO_SIZE_PERIOD_ATT       1u /*!< Period attribute size */
#define GYRO_SIZE_DATA_ATT         6u /*!< Data attribute size */
#define GYRO_SIZE_TEMPDATA_ATT     2u /*!< Temp data attribute size */
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
void SvcGyroAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcGyroRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcGyroCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* GYROSCOPE_SENSOR_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_GYRO_H */
