/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Hearing Access Service (HAS) service implementations.
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

#ifndef SVC_HAS_H
#define SVC_HAS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup HAS_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*!< \brief Hearing Aid Preset Control Point opcodes */
#define HAS_CP_OP_READ_PRESETS_REQ            0x01        /*!< Read Presets Request */
#define HAS_CP_OP_READ_PRESET_RSP             0x02        /*!< Read Preset Response */
#define HAS_CP_OP_PRESET_CHANGED              0x03        /*!< Preset Changed */
#define HAS_CP_OP_WRITE_PRESET_NAME           0x04        /*!< Write Preset Name */
#define HAS_CP_OP_SET_ACT_PRESET              0x05        /*!< Set Active Preset */
#define HAS_CP_OP_SET_NEXT_PRESET             0x06        /*!< Set Next Preset */
#define HAS_CP_OP_SET_PREV_PRESET             0x07        /*!< Set Previous Preset */
#define HAS_CP_OP_SET_ACT_PRESET_SYNC_LOCAL   0x08        /*!< Set Active Preset - Synchronized Locally */
#define HAS_CP_OP_SET_NEXT_PRESET_SYNC_LOCAL  0x09        /*!< Set Next Preset - Synchronized Locally */
#define HAS_CP_OP_SET_PREV_PRESET_SYNC_LOCAL  0x0A        /*!< Set Previous Preset - Synchronized Locally */

#define HAS_CP_OP_MAX                         0x0B

/*!< Values of ChangeId parameter for Preset Changed opcode (0x03) */
#define HAS_CHANGE_ID_GEN_UPDATE              0x00        /*!< Generic Update */
#define HAS_CHANGE_ID_PRESET_REC_DEL          0x01        /*!< Preset Record Deleted */
#define HAS_CHANGE_ID_PRESET_REC_AVAIL        0x02        /*!< Preset Record Available */
#define HAS_CHANGE_ID_PRESET_REC_UNAVAIL      0x03        /*!< Preset Record Unavailable */

#define HAS_CHANGE_ID_MAX                     0x04

/*!< \brief Hearing Aid Features characteristic value bits */
#define HAS_FEAT_BINAURAL_HEARING_AID_TYPE    (0x00<<0)   /*!< Binaural Hearing Aid type */
#define HAS_FEAT_MONAURAL_HEARING_AID_TYPE    (0x01<<0)   /*!< Monaural Hearing Aid type */
#define HAS_FEAT_BRANDED_HEARING_AID_TYPE     (0x02<<0)   /*!< Banded Hearing Aid type */
#define HAS_FEAT_PRESET_SYNC_SUP              (0x01<<2)   /*!< Preset Synchronization supported */
#define HAS_FEAT_INDEPENDENT_PRESETS          (0x01<<3)   /*!< List of preset records on server may be different from list of preset records in other server of Coordinated Set */
#define HAS_FEAT_DYNAMIC_PRESETS              (0x01<<4)   /*!< list of preset records may change */
#define HAS_FEAT_WRITABLE_PRESET_SUP          (0x01<<5)   /*!< Server supports writable preset records */
#define HAS_FEAT_RFU                          (0x02<<6)   /*!< RFU bits */

/*!< \brief Preset properties field value bits */
#define HAS_PRESET_PROP_NAME_WRITABLE         (0x01<<0)   /*!< Name of preset can be written by the client */
#define HAS_PRESET_PROP_IS_AVAILABLE          (0x01<<1)   /*!< Preset is available */

/*!< \brief Preset index none (0x00) is used to indicate that no preset is used */
#define HAS_PRESET_INDEX_NONE                 0x00

/*!< \brief Maximum number of the preset records */
#ifndef HAS_PRESET_REC_MAX
#define HAS_PRESET_REC_MAX                    8
#endif

/*!< \brief Maximum length of the human-readable name of the preset (1-40 octets) */
#ifndef HAS_PRESET_NAME_LEN_MAX
#define HAS_PRESET_NAME_LEN_MAX               20
#endif

/*!< \brief Length of Read Presets Request operation (StartIndex, and NumPresets) */
#define HAS_READ_PRESETS_REQ_LEN              (1 + 1)

/*!< \brief Length of preset record (Index, Properties, and Name) */
#define HAS_PRESET_RECORD_LEN                 (1 + 1 + HAS_PRESET_NAME_LEN_MAX)

/*!< \brief Length of Read Preset Response notification or indication (isLast, and PresetRecord) */
#define HAS_READ_PRESET_RSP_LEN               (1 + HAS_PRESET_RECORD_LEN)

/*!< \brief Length of Preset Changed operation fixed-length fields (ChangeId, and isLast) */
#define HAS_PRESET_CHANGED_COMMON_LEN         (1 + 1)

/*!< \brief Length of Generic Update ChangeId notification or indication (Opcode, ChangeId, isLast, PrevIndex, and PresetRecord) */
#define HAS_GEN_UPD_CHANGE_ID_LEN             (HAS_PRESET_CHANGED_COMMON_LEN + 1 + HAS_PRESET_RECORD_LEN)

/*!< \brief Length of Preset Record Deleted notification or indication (Opcode, ChangeId, isLast, and Index) */
#define HAS_PRESET_REC_DEL_LEN                (HAS_PRESET_CHANGED_COMMON_LEN + 1)

/*!< \brief Length of Preset Record Available notification or indication (Opcode, ChangeId, isLast, and Index) */
#define HAS_PRESET_REC_AVAIL_LEN              (HAS_PRESET_CHANGED_COMMON_LEN + 1)

/*!< \brief Length of Preset Record Unavailable notification or indication (Opcode, ChangeId, isLast, and Index) */
#define HAS_PRESET_REC_UNAVAIL_LEN            (HAS_PRESET_CHANGED_COMMON_LEN + 1)

/*!< \brief Write Preset Name operation fixed field length (Index) */
#define HAS_WRITE_PRESET_NAME_FIX_LEN         1

/*!< \brief Length of Write Preset Name operation (Opcode, Index, and Name) */
#define HAS_WRITE_PRESET_NAME_LEN             (HAS_WRITE_PRESET_NAME_FIX_LEN + HAS_PRESET_NAME_LEN_MAX)

/*!< \brief Length of Set Active Preset operation (Index) */
#define HAS_SET_ACTIVE_PRESET_LEN             1

/*!< \brief Length of Set Next Preset operation */
#define HAS_SET_NEXT_PRESET_LEN               0

/*!< \brief Length of Set Previous Preset operation */
#define HAS_SET_PREV_PRESET_LEN               0

/*!< \brief Maximum length of the Hearing Aid Preset Control Point characteristic value */
#define HAS_MAX_HAP_CP_LEN                    251

/*!< \brief Length of the Hearing Aid Preset Control Point characteristic value */
#define HAS_HAP_CP_LEN                        HAS_GEN_UPD_CHANGE_ID_LEN

/*!< \brief  ATT Application error codes */
#define HAS_ERR_INVALID_OPCODE                0x80  /*!< Invalid opcode has been used in control point operation */
#define HAS_ERR_WRITE_NAME_NOT_ALLOWED        0x81  /*!< Client executed Write Preset Name procedure with Index parameter corresponding to read-only preset record */
#define HAS_ERR_PRESET_SYNC_NOT_SUP           0x82  /*!< Client executed Synchronized Locally Hearing Aid Preset CP operation to server that doesn't support Preset Synchronization */
#define HAS_ERR_PRESET_OP_NOT_PSBL            0x83  /*!< Requested preset operation cannot be executed at this time */
#define HAS_ERR_INVALID_PARAM_LEN             0x84  /*!< Client requested to write valid opcode but with parameters of invalid length */

/**************************************************************************************************
  Handle Ranges
**************************************************************************************************/
/** \name HAS Service Handles
 *
 */
/**@{*/
#define HAS_START_HDL                   0x0750            /*!< HAS start handle */
#define HAS_END_HDL                     (HAS_MAX_HDL - 1) /*!< HAS end handle */
/**@}*/

/**************************************************************************************************
  Handles
**************************************************************************************************/

/** \name HAS Service Handles
 *
 */
/**@{*/
/*! \brief HAS service handle */
enum
{
  HAS_SVC_HDL = HAS_START_HDL,      /*!< HA service declaration */
  HAS_HAF_CH_HDL,                   /*!< Hearing Aid Features characteristic */
  HAS_HAF_HDL,                      /*!< Hearing Aid Features value */
  HAS_HAF_CH_CCC_HDL,               /*!< Hearing Aid Features client characteristic configuration */
  HAS_HAP_CP_CH_HDL,                /*!< Hearing Aid Preset Control Point characteristic */
  HAS_HAP_CP_HDL,                   /*!< Hearing Aid Preset Control Point value */
  HAS_HAP_CP_CH_CCC_HDL,            /*!< Hearing Aid Preset Control Point client characteristic configuration */
  HAS_API_CH_HDL,                   /*!< Active Preset Index characteristic */
  HAS_API_HDL,                      /*!< Active Preset Index value */
  HAS_API_CH_CCC_HDL,               /*!< Active Preset Index client characteristic configuration */

  HAS_MAX_HDL                       /*!< HAS maximum handle */
};
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief HAS preset record */
typedef struct
{
  uint8_t           index;          /*!< Preset index */
  uint8_t           props;          /*!< Preset properties */
  uint8_t           len;            /*!< Length of reset name */
  uint8_t           name[HAS_PRESET_NAME_LEN_MAX];  /*!< Human-readable name of preset */
} hasPresetRec_t;

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
void SvcHasAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcHasRemoveGroup(void);

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
void SvcHasCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* HAS_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* #define SVC_HAS_H */
