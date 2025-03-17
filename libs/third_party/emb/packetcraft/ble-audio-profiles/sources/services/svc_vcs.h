/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Volume Control Service implementation.
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

#ifndef SVC_VCS_H
#define SVC_VCS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup VOLUME_CONTROL_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Volume Control attribute lengths */
#define VCS_STATE_LEN                   3
#define VCS_FLAG_LEN                    1
#define VCS_CP_LEN                      3
#define VCS_CP_OPCODE_LEN               1
#define VCS_CP_CHANGE_COUNTER_LEN       1

/* Volume Control State parameter indicies */
#define VCS_STATE_VOL_IDX               0
#define VCS_STATE_MUTE_IDX              1
#define VCS_STATE_CH_CNTR_IDX           2

/* Volume Control Point opcodes */
#define VCS_CP_OPCODE_VOL_DWN           0
#define VCS_CP_OPCODE_VOL_UP            1
#define VCS_CP_OPCODE_UNMUTE_VOL_DWN    2
#define VCS_CP_OPCODE_UNMUTE_VOL_UP     3
#define VCS_CP_OPCODE_ABS_VOL           4
#define VCS_CP_OPCODE_UNMUTE            5
#define VCS_CP_OPCODE_MUTE              6

/* Volume Control Point relative volume opcode bit fields */
#define VCS_REL_VOL_UP_BIT              (1<<0)
#define VCS_REL_VOL_UNMUTE_BIT          (2<<0)

/* Mute disabled */
#define VCP_DISABLED                    2

/* Volume Control error codes */
#define VCS_ERR_INVALID_CHANGE_CTR      0x80
#define VCS_ERR_OPCODE_NOT_SUPPORTED    0x81

/* CCCD Index Offsets */
#define VCS_CCCD_IDX_STATE              0
#define VCS_CCCD_IDX_FLAGS              1

/* Number of CCCD attributes */
#define VCS_NUM_CCCD                    2

/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Volume Control Service Handles
 *
 */
/**@{*/
#define VCS_START_HDL                0x0520             /*!< Start handle. */
#define VCS_END_HDL                  (VCS_MAX_HDL - 1)  /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Volume Control Service Handles */
enum
{
  VCS_SVC_HDL = VCS_START_HDL,      /*!< Volume Control Service declaration */
  VCS_STATE_CH_HDL,                 /*!< Volume Control volume state characteristic */
  VCS_STATE_HDL,                    /*!< Volume Control volume state value */
  VCS_STATE_CH_CCC_HDL,             /*!< Volume Control volume state Client Characteristic Configuration Descriptor */
  VCS_CP_CH_HDL,                    /*!< Volume Control control point characteristic */
  VCS_CP_HDL,                       /*!< Volume Control control point value */
  VCS_FLAGS_CH_HDL,                 /*!< Volume Control volume flags characteristic */
  VCS_FLAGS_HDL,                    /*!< Volume Control volume flags value */
  VCS_FLAGS_CH_CCC_HDL,             /*!< Volume Control volume flags Client Characteristic Configuration Descriptor */
  VCS_AICS_INC_HDL_0,               /*!< Volume Control AICS include characteristic */
  VCS_AICS_INC_HDL_1,               /*!< Volume Control AICS include characteristic */
  VCS_VOCS_INC_HDL_0,               /*!< Volume Control VOCS include characteristic */
  VCS_VOCS_INC_HDL_1,               /*!< Volume Control VOCS include characteristic */
  VCS_MAX_HDL                       /*!< Maximum handle. */
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
void SvcVcsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcVcsRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcVcsCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* VOLUME_CONTROL_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_VCS_H */
