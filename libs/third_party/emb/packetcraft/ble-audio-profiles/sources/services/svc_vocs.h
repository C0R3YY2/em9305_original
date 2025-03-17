/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Volume Offset Control Service implementation.
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

#ifndef SVC_VOCS_H
#define SVC_VOCS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup VOLUME_OFFSET_CONTROL_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Characteristic value lengths */
#define VOCS_STATE_LEN                  3
#define VOCS_LOC_LEN                    4
#define VOCS_CP_LEN                     4
#define VOCS_OUTPUT_DESC_MAX_LEN        64

/* Volume Offset control point opcodes */
#define VOCS_CP_OPCODE_SET_VOL_OFFSET   1

/* Volume Offset state parameter indicies */
#define VOCS_STATE_CH_CTR_IDX           2

/* Volume Offset error codes */
#define VOCS_ERR_INVALID_CHANGE_CTR     0x80
#define VOCS_ERR_OPCODE_NOT_SUPPORTED   0x81
#define VOCS_ERR_VALUE_OUT_OF_RANGE     0x82

/* Volume Offset error codes */
#define VOCS_OFFSET_MIN                 (-255)
#define VOCS_OFFSET_MAX                 (255)

/* CCCD Index Offsets */
#define VOCS_CCCD_IDX_STATE             0
#define VOCS_CCCD_IDX_LOCATION          1
#define VOCS_CCCD_IDX_DESCRIPTION       2

/* Number of CCCD attributes per service */
#define VOCS_NUM_CCCD                   3

/* Max supported secondary audio input control services for the device. */
/* TODO: A device can have multiple VOCS services. */
/*       Modify the max services as needed. */
#define VOCS_SVC_MAX                    2

/* Unknown VOCS service ID */
#define VOCS_SVC_UNKNOWN                0xFF

/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Volume Offset Control Service Handles
 *
 */
/**@{*/

/*!< \brief Start handle. */
#define VOCS_START_HDL                0x0600

/*!< \brief Number of VOCS handles. */
#define VOCS_NUM_HDL                  (VOCS_MAX_HDL_0 - VOCS_START_HDL)

/*!< \brief Base value handles. */
#define VOCS_STATE_HDL                VOCS_STATE_HDL_0
#define VOCS_LOCATION_HDL             VOCS_LOCATION_HDL_0
#define VOCS_CP_HDL                   VOCS_CP_HDL_0
#define VOCS_DESC_HDL                 VOCS_DESC_HDL_0

/* Calculate an value handle from a base value handle and a service id. */
#define VOCS_HANDLE_ID(base, index)   (base + VOCS_NUM_HDL * index)

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Volume Offset Control Service Handles macro */
#define VOCS_HANDLE_DEFS(_a, b)  \
enum \
{ \
  VOCS_SVC_HDL_##_a = b,        /*!< Volume Offset Control Service declaration */\
  VOCS_STATE_CH_HDL_##_a,       /*!< Volume Offset Control state characteristic */\
  VOCS_STATE_HDL_##_a,          /*!< Volume Offset Control state */\
  VOCS_STATE_CH_CCC_HDL_##_a,   /*!< Volume Offset Control state Client Characteristic Configuration Descriptor */\
  VOCS_LOCATION_CH_HDL_##_a,    /*!< Volume Offset Control location characteristic */\
  VOCS_LOCATION_HDL_##_a,       /*!< Volume Offset Control location */\
  VOCS_LOCATION_CH_CCC_HDL_##_a,/*!< Volume Offset Control location Client Characteristic Configuration Descriptor */\
  VOCS_CP_CH_HDL_##_a,          /*!< Volume Offset Control control point characteristic */\
  VOCS_CP_HDL_##_a,             /*!< Volume Offset Control control point */\
  VOCS_DESC_CH_HDL_##_a,        /*!< Volume Offset Control description characteristic */\
  VOCS_DESC_HDL_##_a,           /*!< Volume Offset Control description */\
  VOCS_DESC_CH_CCC_HDL_##_a,    /*!< Volume Offset Control description Client Characteristic Configuration Descriptor */\
  VOCS_MAX_HDL_##_a             /*!< Maximum handle. */\
};
/**@}*/

/*! \brief Volume Offset Control Service Handles */
/* TODO: A device can have multiple VOCS services.
 *       Add or remove handle definitions as needed. */
VOCS_HANDLE_DEFS(0, VOCS_START_HDL)
VOCS_HANDLE_DEFS(1, VOCS_MAX_HDL_0)

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
void SvcVocsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcVocsRemoveGroup(void);

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
void SvcVocsCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* VOLUME_OFFSET_CONTROL_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_VOCS_H */
