/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Application framework hardware interfaces.
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
#ifndef APP_HW_H
#define APP_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup APP_FRAMEWORK_HW_API
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Heart rate measurement structure */
typedef struct
{
  uint16_t          *pRrInterval;         /*!< Array of RR intervals */
  uint8_t           numIntervals;         /*!< Length of RR interval array */
  uint16_t          energyExp;            /*!< Energy expended value */
  uint16_t          heartRate;            /*!< Heart rate */
  uint8_t           flags;                /*!< Heart rate measurement flags */
} appHrm_t;

/*! \brief Date and time structure */
typedef struct
{
  uint16_t          year;                 /*!< Year */
  uint8_t           month;                /*!< Month */
  uint8_t           day;                  /*!< Day */
  uint8_t           hour;                 /*!< Hour */
  uint8_t           min;                  /*!< Minutes */
  uint8_t           sec;                  /*!< Seconds */
} appDateTime_t;

/*! \brief Blood pressure measurement structure */
typedef struct
{
  appDateTime_t     timestamp;            /*!< Date-time */
  uint16_t          systolic;             /*!< Systolic pressure */
  uint16_t          diastolic;            /*!< Diastolic pressure */
  uint16_t          map;                  /*!< Mean arterial pressure */
  uint16_t          pulseRate;            /*!< Pulse rate */
  uint16_t          measStatus;           /*!< Measurement status */
  uint8_t           flags;                /*!< Flags */
  uint8_t           userId;               /*!< User ID */
} appBpm_t;

/*! \brief Weight scale measurement structure */
typedef struct
{
  appDateTime_t     timestamp;            /*!< Date-time */
  uint16_t          weight;               /*!< Weight */
  uint8_t           flags;                /*!< Weight measurement flags */
} appWsm_t;

/*! \brief Temperature measurement structure */
typedef struct
{
  appDateTime_t     timestamp;            /*!< Date-time */
  uint32_t          temperature;          /*!< Temperature */
  uint8_t           flags;                /*!< Flags */
  uint8_t           tempType;             /*!< Temperature type */
} appTm_t;

/*! \brief Pulse Oximeter continuous measurement structure */
typedef struct
{
  uint8_t         flags;            /*!< Flags */
  uint16_t        spo2;             /*!< SpO2PR-Spot-Check - SpO2 */
  uint16_t        pulseRate;        /*!< SpO2PR-Spot-Check - Pulse Rate */
  uint16_t        spo2Fast;         /*!< SpO2PR-Spot-Check Fast - SpO2 */
  uint16_t        pulseRateFast;    /*!< SpO2PR-Spot-Check Fast - Pulse Rate */
  uint16_t        spo2Slow;         /*!< SpO2PR-Spot-Check Slow - SpO2 */
  uint16_t        pulseRateSlow;    /*!< SpO2PR-Spot-Check Slow - Pulse Rate */
  uint16_t        measStatus;       /*!< Measurement Status */
  uint32_t        sensorStatus;     /*!< Device and Sensor Status */
  uint16_t        pulseAmpIndex;    /*!< Pulse Amplitude Index */
} appPlxCm_t;

/*! \brief Pulse Oximeter spot check measurement structure */
typedef struct
{
  uint8_t         flags;            /*!< Flags */
  uint16_t        spo2;             /*!< SpO2PR-Spot-Check - SpO2 */
  uint16_t        pulseRate;        /*!< SpO2PR-Spot-Check - Pulse Rate */
  appDateTime_t   timestamp;        /*!< Timestamp */
  uint16_t        measStatus;       /*!< Measurement Status */
  uint32_t        sensorStatus;     /*!< Device and Sensor Status */
  uint16_t        pulseAmpIndex;    /*!< Pulse Amplitude Index */
} appPlxScm_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name App Hardware Interface
 * Interface to emulated sensor of real world devices (e.g. battery, heart rate monitor,
 * blood pressure sensor, etc.)
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Read the battery level.  The battery level value returned in pLevel is the
 *          percentage of remaining battery capacity (0-100%).
 *
 *  \param  pLevel   Battery level return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwBattRead(uint8_t *pLevel);

/*************************************************************************************************/
/*!
 *  \brief  Set the battery level, for test purposes.
 *
 *  \param  level   Battery level (0-100%).
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwBattTest(uint8_t level);

/*************************************************************************************************/
/*!
 *  \brief  Perform a heart rate measurement.  Return the heart rate along with any RR interval
 *          data.
 *
 *  \param  pHrm   Heart rate measurement return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwHrmRead(appHrm_t *pHrm);

/*************************************************************************************************/
/*!
 *  \brief  Set the heart rate, for test purposes.
 *
 *  \param  heartRate Heart rate.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwHrmTest(uint8_t heartRate);

/*************************************************************************************************/
/*!
 *  \brief  Perform a blood pressure measurement.  Return the measurement data.
 *
 *  \param  intermed  true if this is an intermediate measurement.
 *  \param  pBpm      Blood pressure measurement return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwBpmRead(bool intermed, appBpm_t *pBpm);

/*************************************************************************************************/
/*!
 *  \brief  Perform a weight scale measurement.  Return the measurement data.
 *
 *  \param  pWsm      Weight scale measurement return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwWsmRead(appWsm_t *pWsm);

/*************************************************************************************************/
/*!
 *  \brief  Perform a temperature measurement.  Return the measurement data.
 *
 *  \param  intermed  true if this is an intermediate measurement.
 *  \param  pTm      Temperature measurement return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwTmRead(bool intermed, appTm_t *pTm);

/*************************************************************************************************/
/*!
 *  \brief  Set the temperature measurement units.
 *
 *  \param  units     CH_TM_FLAG_UNITS_C or CH_TM_FLAG_UNITS_F.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwTmSetUnits(uint8_t units);


/*************************************************************************************************/
/*!
 *  \brief  Set the weight measurement units.
 *
 *  \param  units     CH_WSM_FLAG_UNITS_KG or CH_WSM_FLAG_UNITS_LBS.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwWmSetUnits(uint8_t units);

/*************************************************************************************************/
/*!
 *  \brief  Perform a pulse oximeter measurement.
 *
 *  \param  pPlxcm   Pulse Oximeter measurement return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwPlxcmRead(appPlxCm_t *pPlxcm);


/*************************************************************************************************/
/*!
 *  \brief  Perform a pulse oximeter spot check measurement.
 *
 *  \param  pPlxscm   Pulse Oximeter measurement return value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppHwPlxscmRead(appPlxScm_t *pPlxscm);

/**@}*/

/*! \} */    /*! APP_FRAMEWORK_HW_API */

#ifdef __cplusplus
};
#endif

#endif /* APP_HW_H */
