/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Cycling Power Service Server implementation.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_CPS_H
#define SVC_CPS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup CYCLING_POWER_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name Cycling Power Feature Bits
 * Cycle Power Feature Bits of the Feature Characteristic.
 */
/**@{*/
#define CPP_PPBS_FEATURE_BIT            (1<<0)  /*!< Pedal Power Balance Supported Feature Mask Bit */
#define CPP_ATS_FEATURE_BIT             (1<<1)  /*!< Accumulated Torque Supported Feature Mask Bit */
#define CPP_WRDS_FEATURE_BIT            (1<<2)  /*!< Wheel Revolution Data Supported Feature Mask Bit */
#define CPP_CRDS_FEATURE_BIT            (1<<3)  /*!< Crank Revolution Data Supported Feature Mask Bit */
#define CPP_EMS_FEATURE_BIT             (1<<4)  /*!< Extreme Magnitudes Supported Feature Mask Bit */
#define CPP_EAS_FEATURE_BIT             (1<<5)  /*!< Extreme Angles Supported Feature Mask Bit */
#define CPP_TABDSAS_FEATURE_BIT         (1<<6)  /*!< Top and Bottom Dead Spot Angles Supported Feature Mask Bit */
#define CPP_AES_FEATURE_BIT             (1<<7)  /*!< Accumulated Energy Supported Feature Mask Bit */
#define CPP_OCIS_FEATURE_BIT            (1<<8)  /*!< Offset Compensation Indicator Supported Feature Mask Bit */
#define CPP_OCS_FEATURE_BIT             (1<<9)  /*!< Offset Compensation Supported Feature Mask Bit */
#define CPP_CPMCCM_FEATURE_BIT          (1<<10) /*!< Cycling Power Measurement Characteristic Content Masking Supported  Offset Compensation Supported  Feature Mask Bit */
#define CPP_MSLS_FEATURE_BIT            (1<<11) /*!< Multiple Sensor Locations Supported Feature Mask Bit */
#define CPP_CLAS_FEATURE_BIT            (1<<12) /*!< Crank Length Adjustment Supported Feature Mask Bit */
#define CPP_CHLAS_FEATURE_BIT           (1<<13) /*!< Chain Length Adjustment Supported Feature Mask Bit */
#define CPP_CHWAS_FEATURE_BIT           (1<<14) /*!< Chain Weight Adjustment Supported Feature Mask Bit */
#define CPP_SLAS_FEATURE_BIT            (1<<15) /*!< Span Length Adjustment Supported Feature Mask Bit */
#define CPP_SMC_FEATURE_BIT             (1<<16) /*!< Sensor Measurement Contex (0: FORCE, 1: TORQUE) */
#define CPP_IMDS_FEATURE_BIT            (1<<17) /*!< Instantaneous Measurement Direction Supported */
#define CPP_FCDS_FEATURE_BIT            (1<<18) /*!< Factory Calibration Date Supported Feature Mask Bit */
#define CPP_EOCS_FEATURE_BIT            (1<<19) /*!< Enhanced Offset Compensation Supported Feature Mask Bit */
#define CPP_DSS_FEATURE_BIT             (1<<20) /*!< Distribute System Support Feature Mask Bit */

/*! \brief TODO: Set to all supported features */
#define CPP_ALL_FEATURES                (0xff)  /*!< All Supported Feature Mask */
/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Cycling Power Service Handles
 *
 */
/**@{*/
#define CPS_START_HDL               0x0400            /*!< Start handle. */
#define CPS_END_HDL                 (CPS_MAX_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Cycling Power Service Handles */
enum
{
  CPS_SVC_HDL = CPS_START_HDL,        /*!< Cycling Power Server Service declaration */
  CPS_CPF_CH_HDL,                     /*!< Cycling Power Feature characteristic */
  CPS_CPF_HDL,                        /*!< Cycling Power Feature */
  CPS_CPM_CH_HDL,                     /*!< Cycling Power Measurement characteristic */
  CPS_CPM_HDL,                        /*!< Cycling Power Measurement */
  CPS_CPM_CH_CCC_HDL,                 /*!< Cycling Power Measurement Client Characteristic Configuration Descriptor */
  CPS_CPSL_CH_HDL,                    /*!< Cycling Power Sensor Location characteristic */
  CPS_CPSL_HDL,                       /*!< Cycling Power Sensor Location */
  CPS_MAX_HDL                         /*!< Maximum handle. */
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
void SvcCpsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCpsRemoveGroup(void);

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
void SvcCpsCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* CYCLING_POWER_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_CPS_H */
