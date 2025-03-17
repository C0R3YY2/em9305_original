/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Audio Input Control Service implementation.
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

#ifndef SVC_AICS_H
#define SVC_AICS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup AUDIO_INPUT_CONTROL_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Chacteristic value lengths */
#define AICS_STATE_LEN                  4
#define AICS_GSA_LEN                    3
#define AICS_TYPE_LEN                   1
#define AICS_STATUS_LEN                 1
#define AICS_AICP_LEN                   3
#define AICS_AICP_MUTE_LEN              2
#define AICS_AICP_MODE_LEN              2
#define AICS_INPUT_DESC_MAX_LEN         64
#define AICS_CP_OPCODE_LEN              1
#define AICS_CP_CHANGE_COUNTER_LEN      1

/* Audio Input State parameter incidies */
#define AICS_STATE_GAIN_SETTING_IDX     0
#define AICS_STATE_MUTE_IDX             1
#define AICS_STATE_GAIN_MODE_IDX        2
#define AICS_STATE_CH_CTR_IDX           3

/* Audio Input mute modes */
#define AICS_STATE_MUTTED               0
#define AICS_STATE_NOT_MUTED            1

/* Audio Input Gain modes */
#define AICS_GAIN_MODE_MANUAL_ONLY      0
#define AICS_GAIN_MODE_AUTOMATIC_ONLY   1
#define AICS_GAIN_MODE_MANUAL           2
#define AICS_GAIN_MODE_AUTOMATIC        3

/* Audio Input Gain Setting parameter indicies */
#define AICS_GSA_UNITS_INDX             0
#define AICS_GSA_MIN_INDX               1
#define AICS_GSA_MAX_INDX               2

/* Audio Input Control Point opcodes */
#define AICS_AICP_OPCODE_SET_GAIN       1
#define AICS_AICP_OPCODE_UNMUTE         2
#define AICS_AICP_OPCODE_MUTE           3
#define AICS_AICP_OPCODE_MANUAL_MODE    4
#define AICS_AICP_OPCODE_AUTO_MODE      5

/* Audio Input error codes */
#define AICS_ERR_INVALID_CHANGE_CTR     0x80
#define AICS_ERR_OPCODE_NOT_SUPPORTED   0x81
#define AICS_ERR_MUTE_DISABLED          0x82
#define AICS_ERR_VALUE_OUT_OF_RANGE     0x83
#define AICS_ERR_GAIN_MODE_NOT_ALLOWED  0x84

/* CCCD Index Offsets */
#define AICS_CCCD_IDX_STATE             0
#define AICS_CCCD_IDX_STATUS            1
#define AICS_CCCD_IDX_DESCRIPTION       2

/* Number of CCCD attributes per service */
#define AICS_NUM_CCCD                   3

/* Max supported secondary audio input control services for the device. */
/* TODO: A device can have multiple AICS services.
 *       Modify the max services as needed. */
#define AICS_SVC_MAX                    2

/* Unknown AICS service ID */
#define AICS_SVC_UNKNOWN                0xFF

/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Audio Input Control Service Handles
 *
 */
/**@{*/

/*!< \brief Start handle. */
#define AICS_START_HDL                0x0800

/*!< \brief Number of AICS handles. */
#define AICS_NUM_HDL                  (AICS_MAX_HDL_0 - AICS_START_HDL)

/*!< \brief Base value handles. */
#define AICS_STATE_HDL                AICS_STATE_HDL_0
#define AICS_GSA_HDL                  AICS_GSA_HDL_0
#define AICS_TYPE_HDL                 AICS_TYPE_HDL_0
#define AICS_STATUS_HDL               AICS_STATUS_HDL_0
#define AICS_AICP_HDL                 AICS_AICP_HDL_0
#define AICS_AID_HDL                  AICS_AID_HDL_0

/* Calculate an value handle from a base value handle and a service id. */
#define AICS_HANDLE_ID(base, index)   (base + AICS_NUM_HDL * index)

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Audio Input Control Service Handles macro */
#define AICS_HANDLE_DEFS(_a, b)  \
enum \
{ \
  AICS_SVC_HDL_##_a = b,        /*!< Audio Input Control Service declaration */\
  AICS_STATE_CH_HDL_##_a,       /*!< Audio Input Control input state characteristic */\
  AICS_STATE_HDL_##_a,          /*!< Audio Input Control input state */\
  AICS_STATE_CH_CCC_HDL_##_a,   /*!< Audio Input Control input state Client Characteristic Configuration Descriptor */\
  AICS_GSA_CH_HDL_##_a,         /*!< Audio Input Control gain setting characteristic */\
  AICS_GSA_HDL_##_a,            /*!< Audio Input Control gain setting */\
  AICS_TYPE_CH_HDL_##_a,        /*!< Audio Input Control input type characteristic */\
  AICS_TYPE_HDL_##_a,           /*!< Audio Input Control input type */\
  AICS_STATUS_CH_HDL_##_a,      /*!< Audio Input Control input stauts characteristic */\
  AICS_STATUS_HDL_##_a,         /*!< Audio Input Control input stauts */\
  AICS_STATUS_CH_CCC_HDL_##_a,  /*!< Audio Input Control input stauts Client Characteristic Configuration Descriptor */\
  AICS_AICP_CH_HDL_##_a,        /*!< Audio Input Control audio input control point characteristic */\
  AICS_AICP_HDL_##_a,           /*!< Audio Input Control audio input control point */\
  AICS_AID_CH_HDL_##_a,         /*!< Audio Input Control audio input description characteristic */\
  AICS_AID_HDL_##_a,            /*!< Audio Input Control audio input description */\
  AICS_AID_CH_CCC_HDL_##_a,     /*!< Audio Input Control audio input description Client Characteristic Configuration Descriptor */\
  AICS_MAX_HDL_##_a             /*!< Maximum handle. */\
};
/**@}*/

/*! \brief Audio Input Control Service Handles */
/* TODO: A device can have multiple secondary AICS services.
 *       Add or remove handle definitions as needed. */
AICS_HANDLE_DEFS(0, AICS_START_HDL)
AICS_HANDLE_DEFS(1, AICS_MAX_HDL_0)

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
void SvcAicsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcAicsRemoveGroup(void);

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
void SvcAicsCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* AUDIO_INPUT_CONTROL_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_AICS_H */
