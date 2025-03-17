/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Definitions.
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
#ifndef BAP_DEFS_H
#define BAP_DEFS_H

#include "svc_pac.h"
#include "svc_bas.h"
#include "ga_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name ASE State Machine State
*  ASE state machine different state values.
*/
/**@{*/
enum
{
  BAP_ASE_SM_ST_IDLE,                           /*!< Idle state */
  BAP_ASE_SM_ST_CODEC_CFG,                      /*!< Codec Configured state */
  BAP_ASE_SM_ST_QOS_CFG,                        /*!< QoS Configured state */
  BAP_ASE_SM_ST_ENABLING,                       /*!< Enabling state */
  BAP_ASE_SM_ST_STREAMING,                      /*!< Streaming state */
  BAP_ASE_SM_ST_DISABLING,                      /*!< Disabling state */
  BAP_ASE_SM_ST_RELEASING,                      /*!< Releasing state */

  BAP_ASE_SM_NUM_STATES
};
/**@}*/


/** \name BAP Error codes
* BAP Error codes returned locally from BAP APIs.
*/
/**@{*/
#define BAP_SUCCESS                       0x00    /*!< Operation successful */
#define BAP_ERR_NO_PAC_REC                0x01    /*!< No PAC record has been discovered */
#define BAP_ERR_CODEC_ID_NOT_SUP          0x02    /*!< Codec_ID not supported by the server */
#define BAP_ERR_DIRECTION_NOT_SUP         0x03    /*!< Direction not supported by the server */
#define BAP_ERR_SAMPLE_FREQ_NOT_SUP       0x04    /*!< Sampling frequencies not supported by the server */
#define BAP_ERR_FRAME_DUR_NOT_SUP         0x05    /*!< Frame duration not supported by the server */
#define BAP_ERR_AUDIO_CH_NOT_SUP          0x06    /*!< Audio channels not supported by the server */
#define BAP_ERR_AUDIO_LOC_NOT_SUP         0x07    /*!< Audio location(s) not supported by the server */
#define BAP_ERR_OCT_PER_FRAME_RANGE       0x08    /*!< Octets per frame out of range */
#define BAP_ERR_OCT_PER_FRAME_NOT_SUP     0x09    /*!< Octets per frame not supported by the server */
#define BAP_ERR_FRAME_PER_SDU_NOT_SUP     0x0A    /*!< Codec frame blocks per SDU not supported by the server */
#define BAP_ERR_UNSUP_SNK_CTX             0x0B    /*!< Server doesn't support one or more of Sink Context Types */
#define BAP_ERR_UNSUP_SRC_CTX             0x0C    /*!< Server doesn't support one or more of Source Context Types */
#define BAP_ERR_UNAVAIL_ALL_SNK_CTX       0x0D    /*!< Server not available for all Sink Context Types */
#define BAP_ERR_UNAVAIL_ALL_SRC_CTX       0x0E    /*!< Server not available for all Source Context Types */
#define BAP_ERR_AVAIL_SUBSET_SNK_CTX      0x0F    /*!< Server only available for subset of Sink Context Types */
#define BAP_ERR_AVAIL_SUBSET_SRC_CTX      0x10    /*!< Server only available for subset of Source Context Types */
#define BAP_ERR_NO_CTX_TYPE_SPECIFIED     0x11    /*!< No Context Type specified */
/**@}*/

/** \name ASE Control Point Opcodes
*  ASE control point opcodes for all possible over-the-air ASE operations.
*/
/**@{*/
#define BAP_ASE_CP_OPCODE_NONE            0x00    /*!< No opcode */
#define BAP_ASE_CP_OPCODE_CFG_CODEC       0x01    /*!< Config Codec opcode */
#define BAP_ASE_CP_OPCODE_CFG_QOS         0x02    /*!< Config QoS opcode */
#define BAP_ASE_CP_OPCODE_ENABLE          0x03    /*!< Enable opcode */
#define BAP_ASE_CP_OPCODE_RX_START_READY  0x04    /*!< Receiver Start Ready opcode */
#define BAP_ASE_CP_OPCODE_DISABLE         0x05    /*!< Disable opcode */
#define BAP_ASE_CP_OPCODE_RX_STOP_READY   0x06    /*!< Receiver Stop Ready opcode */
#define BAP_ASE_CP_OPCODE_UPD_METADATA    0x07    /*!< Update Metadata opcode */
#define BAP_ASE_CP_OPCODE_RELEASE         0x08    /*!< Release opcode */

#define BAP_ASE_CP_OPCODE_MAX             0x09
/**@}*/

/** \name ASE Control Point Common Length
* Length of control point common fields (opcode and total number of ASEs)
*/
/**@{*/
#define BAP_ASE_CP_COMMON_LEN             2      /*!< Control point common length */
/**@}*/


/** \name ASE Control Point Opcode Length Fields
 * Length constants of opcodes fixed length fields (excluding number of ASEs)
 */
 /**@{*/
#define BAP_ASE_CP_CFG_CODEC_LEN          9       /*!< \brief Config Codec operation length */
#define BAP_ASE_CP_CFG_QOS_LEN            16      /*!< \brief Config QoS operation length */
#define BAP_ASE_CP_ENABLE_LEN             2       /*!< \brief Enable operation length */
#define BAP_ASE_CP_HANDSHAKE_LEN          1       /*!< \brief Handshake operation length */
#define BAP_ASE_CP_DISABLE_LEN            1       /*!< \brief Disable operation length */
#define BAP_ASE_CP_SWITCH_CONT_LEN        4       /*!< \brief Switch Content operation length */
#define BAP_ASE_CP_RELEASE_LEN            1       /*!< \brief Release operation length */
/**@}*/

/* ASE Control Point response codes */
#define BAP_ASE_RSP_CODE_SUCCESS          0x00    /*!< \brief Operation completed successfully */
#define BAP_ASE_RSP_CODE_UNSUP_OPCODE     0x01    /*!< \brief Unsupported opcode */
#define BAP_ASE_RSP_CODE_INVALID_LEN      0x02    /*!< \brief Invalid length */
#define BAP_ASE_RSP_CODE_INVALID_ASE_ID   0x03    /*!< \brief Invalid ASE ID */
#define BAP_ASE_RSP_CODE_INVALID_STATE    0x04    /*!< \brief Invalid ASE state machine transition */
#define BAP_ASE_RSP_CODE_INVALID_DIR      0x05    /*!< \brief Invalid ASE direction */
#define BAP_ASE_RSP_CODE_UNSUP_AUD_CAPS   0x06    /*!< \brief Unsupported Audio Capabilities */
#define BAP_ASE_RSP_CODE_UNSUP_VALUE      0x07    /*!< \brief Unsupported configuration parameter value */
#define BAP_ASE_RSP_CODE_REJ_VALUE        0x08    /*!< \brief Rejected configuration parameter value */
#define BAP_ASE_RSP_CODE_INVALID_VALUE    0x09    /*!< \brief Invalid configuration parameter value */
#define BAP_ASE_RSP_CODE_UNSUP_METADATA   0x0A    /*!< \brief Unsupported metadata */
#define BAP_ASE_RSP_CODE_REJ_METADATA     0x0B    /*!< \brief Rejected metadata */
#define BAP_ASE_RSP_CODE_INVALID_METADATA 0x0C    /*!< \brief Invalid metadata */
#define BAP_ASE_RSP_CODE_INSUF_RESOURCES  0x0D    /*!< \brief Insufficient resources */
#define BAP_ASE_RSP_CODE_UNSPECIFIED      0x0E    /*!< \brief Unspecified error */

/* ASE Control Point reasons */
#define BAP_ASE_REASON_SUCCESS            0x00    /*!< \brief Operation completed successfully */
#define BAP_ASE_REASON_CODEC_ID           0x01    /*!< \brief Codec ID  */
#define BAP_ASE_REASON_CODEC_CFG          0x02    /*!< \brief Codec specific configuration */
#define BAP_ASE_REASON_SDU_INT            0x03    /*!< \brief SDU interval */
#define BAP_ASE_REASON_FRAMING            0x04    /*!< \brief Framing */
#define BAP_ASE_REASON_PHY                0x05    /*!< \brief PHY */
#define BAP_ASE_REASON_MAX_SDU_SIZE       0x06    /*!< \brief Maximum SDU size */
#define BAP_ASE_REASON_RETRANS_NUM        0x07    /*!< \brief Retransmission number */
#define BAP_ASE_REASON_MAX_TRANS_LAT      0x08    /*!< \brief Max transport latency */
#define BAP_ASE_REASON_PRESENT_DELAY      0x09    /*!< \brief Presentation delay */
#define BAP_ASE_REASON_INVLD_ASE_CIS_MAP  0x0A    /*!< \brief Invalid ASE CIS mapping */

#define BAP_ASE_ID_ALL_ZERO               0x00    /*!< ASE ID value of 0x00 shall not be assigned by the server */

/** \name BAP Announcement Types
*  BAP Unicast Server advertising announcement types.
*/
/**@{*/
#define BAP_ANNC_TYPE_GEN                 0x00    /*!< General Announcement */
#define BAP_ANNC_TYPE_TGT                 0x01    /*!< Targeted Announcement */
/**@}*/

/** \name BAP Audio Directions
* BAP audio direction values with respect to the server.
*/
 /**@{*/
#define BAP_AUDIO_DIR_NONE                0x00    /*!< None */
#define BAP_AUDIO_DIR_SINK                0x01    /*!< Sink */
#define BAP_AUDIO_DIR_SOURCE              0x02    /*!< Source */
/**@}*/

/* Server support for Unframed ISOAL PDUs */
#define BAP_UNFRAME_ISOAL_PDU_SUP         0x00    /*!< Unframed ISOAL PDUs supported */
#define BAP_UNFRAME_ISOAL_PDU_NOT_SUP     0x01    /*!< Unframed ISOAL PDUs not supported */

/* Target Latency types */
#define BAP_TGT_LOW_LAT                   0x01    /*!< Target lower latency */
#define BAP_TGT_BAL_LAT_RELIAB            0x02    /*!< Target balanced latency and reliability */
#define BAP_TGT_HI_RELIAB                 0x03    /*!< Target higher reliability */

/* Preferred PHY bitfield */
#define BAP_PREF_PHY_NONE                 (0<<0)  /*!< No preference for particular PHY */
#define BAP_PREF_PHY_LE_1M_BIT            (1<<0)  /*!< LE 1M PHY preferred */
#define BAP_PREF_PHY_LE_2M_BIT            (1<<1)  /*!< LE 2M PHY preferred */
#define BAP_PREF_PHY_LE_CODED_BIT         (1<<2)  /*!< LE Coded PHY preferred */

/* PHY bitfield */
#define BAP_PHY_LE_1M_BIT                 (1<<0)  /*!< LE 1M PHY */
#define BAP_PHY_LE_2M_BIT                 (1<<1)  /*!< LE 2M PHY */
#define BAP_PHY_LE_CODED_BIT              (1<<2)  /*!< LE Coded PHY */

/* Target PHY types */
#define BAP_TGT_PHY_LE_1M                 0x01    /*!< LE 1M PHY */
#define BAP_TGT_PHY_LE_2M                 0x02    /*!< LE 2M PHY */
#define BAP_TGT_PHY_LE_CODED              0x03    /*!< LE Coded PHY */

/** \name PAC Audio Location Length
* Length constants of Sink or Source audio location fields
*/
/**@{*/
#define BAP_PAC_LOC_LEN                   4       /*!< \brief Sink or Source audio locations length */
/**@}*/

/** \name PAC Message Field Positions
* Position of fields in PAC record
*/
#define BAP_PAC_CODING_FMT_POS            0       /*!< \brief Position of coding format field */
#define BAP_PAC_CODEC_CFG_LEN_POS         BAP_CODEC_ID_SIZE /*!< \brief Position of codec configuration length field */
/**@}*/

/** \name PAC Configuration Element Indexes
* Codec specific Configuration or Metadata data element indexes.
*/
/**@{*/
#define BAP_CFG_LEN_IDX                   0       /*!< Configuration data element len */
#define BAP_CFG_TYPE_IDX                  1       /*!< Configuration data element type */
#define BAP_CFG_DATA_IDX                  2       /*!< Configuration data element data */
/**@}*/

/** \name Broadcast Audio Scan (BAS) Control Point Opcodes
*  BAS control point opcodes for all possible over-the-air ASE operations.
*/
/**@{*/
#define BAP_BAS_CP_OPCODE_RMT_SCAN_STOP   0x00    /*!< \brief Remote Scan Stopped opcode */
#define BAP_BAS_CP_OPCODE_RMT_SCAN_START  0x01    /*!< \brief Remote Scan Started opcode */
#define BAP_BAS_CP_OPCODE_ADD_SRC         0x02    /*!< \brief Add Source opcode */
#define BAP_BAS_CP_OPCODE_MOD_SRC         0x03    /*!< \brief Modify Source opcode */
#define BAP_BAS_CP_OPCODE_SET_BCAST_CODE  0x04    /*!< \brief Set Broadcast_Code opcode */
#define BAP_BAS_CP_OPCODE_REM_SRC         0x05    /*!< \brief Remove Source opcode */

#define BAP_BAS_CP_OPCODE_MAX             0x06
/**@}*/

/** \name BAS Control Point SyncInfo field Interval parameter values
* SyncInfo field Interval parameter unknown value
*/
/**@{*/
#define BAP_BAS_CP_PA_INT_UNKNOWN         0xFFFF  /*!< \brief PA interval unknown */
/**@}*/

/** \name Service_Data requirements for PAST procedure
* PAST Service_Data requirements
*/
/**@{*/
#define BAP_PAST_ADVA_MATCH_SRCA_EA       0x00    /*!< \brief AdvA in PAST matches Source_Address and AdvA in ADV_EXT_IND */
#define BAP_PAST_ADVA_MATCH_SRCA_NO_EA    0x01    /*!< \brief AdvA in PAST matches Source_Address but no AdvA in ADV_EXT_IND  */
#define BAP_PAST_ADVA_MATCH_NO_SRCA_EA    0x02    /*!< \brief AdvA in PAST doesn't match Source_Address but matches AdvA in ADV_EXT_IND  */
#define BAP_PAST_ADVA_MATCH_NO_SRCA_NO_EA 0x03    /*!< \brief AdvA in PAST doesn't match Source_Address and AdvA in ADV_EXT_IND  */
/**@}*/

/** \name BAS Control Point PA Sync operations
* PA sync operations
*/
/**@{*/
#define BAP_BAS_PA_SYNC_DONT_SYNC         0x00    /*!< Do not synchronize to PA  */
#define BAP_BAS_PA_SYNC_SYNC_PAST         0x01    /*!< Synchronize to PA - PAST available */
#define BAP_BAS_PA_SYNC_SYNC_NO_PAST      0x02    /*!< Synchronize to PA - PAST not available */
/**@}*/

/** \name Broadcast Receive State (BRS) PA sync states
* PA Sync states
*/
/**@{*/
#define BAP_BRS_PA_SYNC_ST_NOT_SYNC       0x00    /*!< Not synchronized to PA */
#define BAP_BRS_PA_SYNC_ST_INFO_REQ       0x01    /*!< SyncInfo Request  */
#define BAP_BRS_PA_SYNC_ST_SYNC           0x02    /*!< Synchronized to PA  */
#define BAP_BRS_PA_SYNC_ST_FAIL           0x03    /*!< Failed to synchronize to PA  */
#define BAP_BRS_PA_SYNC_ST_NO_PAST        0x04    /*!< No PAST  */
/**@}*/

/** \name BAS Control Point BIS Sync operations: 4-octet bitfield (Bit 0-30 = BIS_index[1-31])
* BIG sync operations
*/
/**@{*/
#define BAP_BAS_BIS_SYNC_DONT_SYNC        0x00    /*!< \brief Do not synchronize to BIS_index[x] */
#define BAP_BAS_BIS_SYNC_SYNC             0x01    /*!< \brief Synchronize to BIS_index[x] */
#define BAP_BAS_BIS_SYNC_NO_PREF          0xFFFFFFFF  /*!< \brief No preference */
/**@}*/

/** \name BRS BIS Sync states: 4-octet bitfield (Bit 0-30 = BIS_index[1-31])
* BIG Sync states
*/
/**@{*/
#define BAP_BRS_BIS_SYNC_ST_NOT_SYNC      0x00    /*!< \brief Not synchronized to BIS_index[x] */
#define BAP_BRS_BIS_SYNC_ST_SYNC          0x01    /*!< \brief Synchronized to BIS_index[x] */
#define BAP_BRS_BIS_SYNC_ST_FAIL          0xFFFFFFFF  /*!< \brief Failed to sync to BIG */
/**@}*/

/** \name BRS BIS Indexes (BIS_index[1-31])
* BIS Indexes
*/
/**@{*/
#define BAP_BRS_BIS_INDEX_MIN             1       /*!< Minimum BIS index */
#define BAP_BRS_BIS_INDEX_MAX             31      /*!< Maximum BIS index */
/**@}*/

/** \name BRS BIG Encryption values
* BIG Encryptions values
*/
/**@{*/
#define BAP_BRS_BIG_ENC_NOT_ENCRYPT       0x00    /*!< \brief Not encrypted */
#define BAP_BRS_BIG_ENC_BCAST_CODE_REQ    0x01    /*!< \brief Broadcast_Code required */
#define BAP_BRS_BIG_ENC_DECRYPT           0x02    /*!< \brief Decrypting */
#define BAP_BRS_BIG_ENC_BAD_CODE          0x03    /*!< \brief Incorrect encryption key */
/**@}*/

/** \name BAP Broadcast Audio Source (BAS) State Machine State
*  BAP BAS state machine different state values.
*/
/**@{*/
enum
{
  BAP_BAS_SM_ST_IDLE,                             /*!< Idle state */
  BAP_BAS_SM_ST_CONFIG,                           /*!< Configured state */
  BAP_BAS_SM_ST_STREAMING,                        /*!< Streaming state */

  BAP_BAS_SM_NUM_STATES
};
/**@}*/

/** \name BAP BASE hierarchal levels
* BAP Numerically hierarchal levels in the BASE structure
*/
/**@{*/
#define BAP_BASE_LEVEL_NONE               0x00    /*!< \brief Level none */
#define BAP_BASE_LEVEL_GRP                0x01    /*!< \brief Level 1: Group level */
#define BAP_BASE_LEVEL_SUBGRP             0x02    /*!< \brief Level 2: Subgroup level */
#define BAP_BASE_LEVEL_BIS                0x03    /*!< \brief Level 3: BIS level */
/**@}*/

/** \name BAP Level 1 Codec_ID
* BAP Codec_ID is the same as Level 1 Codec_ID
*/
/**@{*/
#define BAP_CODEC_ID_GRP                  0xFE
/**@}*/

/** \name BAP NULL
* BAP NULL node ID or memory block
*/
/**@{*/
#define BAP_NULL                          0xFF
/**@}*/

/** \name BAP Base value for BIG handles
* BAP BIG handle value starting base
*/
/**@{*/
#define BAP_BIG_HANDLE_BASE               0x00
/**@}*/

/** \name BAP Broadcast_ID length
* BAP length of Broadcast_ID field
*/
/**@{*/
#define BAP_BROADCAST_ID_LEN              3
/**@}*/

/** \name BAP Broadcast Audio Announcement Service Data length
* BAP length of Broadcast Audio Announcement Service Data
*/
/**@{*/
#define BAP_BCAST_AUD_ANNC_SVC_DATA_LEN   (ATT_16_UUID_LEN + BAP_BROADCAST_ID_LEN)
/**@}*/

/** \name BAP Codec_ID size
* BAP Size of Codec_ID field
*/
/**@{*/
#define BAP_CODEC_ID_SIZE                 5
/**@}*/

/** \name BAP Presentation Delay length
* BAP Length of Presentation_Delay in Basic Broadcast Announcements
*/
/**@{*/
#define BAP_PRESENT_DLY_LEN               3
/**@}*/

/** \name BAP BAS Memory block types
* BAP BAS different types of memory blocks
*/
/**@{*/
#define BAP_MEM_PRESENT_DLY               0       /*!< \brief Presentation delay memory */
#define BAP_MEM_NODE                      1       /*!< \brief Node memory */
#define BAP_MEM_CODEC_ID                  2       /*!< \brief Codec_ID memory */
#define BAP_MEM_LTV_DATA                  3       /*!< \brief LTV data memory */
/**@}*/

/** \name BAP Number of uint32_t bits
* BAP Number of bits in the uint32_t type
*/
/**@{*/
#define BAP_UINT32_BITS                   32
/**@}*/

/** \name BAP Maximum length of Codec Specific Configuration or Metadata data
* BAP Maximum length of the codec specific configuration or the metadata data
*/
/**@{*/
#if (GA_MAX_CODEC_LEN >= GA_MAX_METADATA_LEN)
#define BAP_MAX_LTV_LEN                   GA_MAX_CODEC_LEN
#else
#define BAP_MAX_LTV_LEN                   GA_MAX_METADATA_LEN
#endif
/**@}*/

/** \name BAP Maximum number of subgroups
* BAP Maximum number of subgroups supported
*/
/**@{*/
#define BAP_MAX_SUBGRP                    (BASS_GRP_MAX * BASS_SUBGRP_MAX)
/**@}*/

/** \name BAP Maximum number of BISes
* BAP Maximum number of BISes supported
*/
/**@{*/
#define BAP_MAX_BIS                       (BAP_MAX_SUBGRP * BASS_BIS_MAX)
/**@}*/

/** \name BAP Maximum number of Codecs
* BAP Maximum number of Codecs supported
*/
/**@{*/
#define BAP_MAX_CODEC                     (BASS_GRP_MAX * BASS_SUBGRP_MAX)
/**@}*/

/** \name BAP Maximum number of Codec Specific Configurations
* BAP Maximum number of Codec Specific Configurations supported
*/
/**@{*/
#define BAP_MAX_CODEC_CFG                 (BASS_GRP_MAX * (BASS_SUBGRP_MAX + BASS_BIS_MAX))
/**@}*/

/** \name BAP Maximum number of Metadata
* BAP Maximum number of Metadata supported
*/
/**@{*/
#define BAP_MAX_METADATA                  (BASS_GRP_MAX * BASS_SUBGRP_MAX)
/**@}*/

/** \name BAP Maximum length of CCID List
* BAP Maximum length of CCID List supported
*/
/**@{*/
#ifndef GA_METADATA_LEN_CCID
#define GA_METADATA_LEN_CCID              0x02
#endif
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief BAP CIG parameters */
typedef struct
{
  uint32_t          sduIntervalMToS;  /*!< The time interval between the start of consecutive SDUs from the master Host. */
  uint32_t          sduIntervalSToM;  /*!< The time interval between the start of consecutive SDUs from the slave Host. */
  uint8_t           sca;              /*!< Sleep clock accuracy. */
  uint8_t           packing;          /*!< Packing scheme. */
  uint8_t           framing;          /*!< Indicates the format of CIS Data PDUs.  */
  uint16_t          transLatMToS;     /*!< Maximum time, in milliseconds, for an SDU to be transported from the master Controller to slave Controller. */
  uint16_t          transLatSToM;     /*!< Maximum time, in milliseconds, for an SDU to be transported from the slave Controller to master Controller. */
  uint16_t          sduSizeMToS;      /*!< Maximum size of a data SDU from the master to the slave. */
  uint16_t          sduSizeSToM;      /*!< Maximum size of a data SDU from the slave to the master. */
} BapCigParams_t;

/*! \brief BAP BIG parameters */
typedef struct
{
  uint32_t          sduInterUsec;     /*!< Interval, in microseconds, of BIG SDUs */
  uint16_t          maxSdu;           /*!< Maximum size of an SDU */
  uint16_t          mtlMs;            /*!< Maximum time in milliseconds */
  uint8_t           rtn;              /*!< Retransmission number */
  uint8_t           phys;             /*!< Transmitter PHYs of packets */
  uint8_t           packing;          /*!< indicates the preferred method of arranging subevents of multiple BISes */
  uint8_t           framing;          /*!< Indicates the format for sending BIS Data PDUs */
} BapBigParams_t;

/*! \brief BAP ASE structure */
typedef struct
{
  uint8_t            aseId;           /*!< \brief ASE ID for this ASE */
  uint8_t            rspCode;         /*!< \brief Response code for ASE ID */
  uint8_t            reason;          /*!< \brief Reason for ASE ID */
} bapAse_t;

/*! \brief BAP codec specific configuration */
typedef struct
{
  uint8_t            sampleFreq;      /*!< Sampling frequency */
  uint8_t            frameDur;        /*!< Frame duration */
  uint16_t           octPerFrame;     /*!< Octets per codec frame */
  uint8_t            frameBlkPerSdu;  /*!< Codec frame blocks per SDU */
} bapCodecSpCfg_t;

/*! \brief  BAP generic codec specific configuration */
typedef void *bapGenCodecSpCfg_t;

/*! \brief BAP unicast metadata */
typedef struct
{
  uint16_t           desiredAudioCtx; /*!< Desired audio contexts */
  uint8_t            ccidList[GA_METADATA_LEN_CCID]; /*!< Array of CCID values */
} bapUcastMetadata_t;

/*! \brief BAP broadcast metadata */
typedef struct
{
  uint16_t           streamAudioCtx;  /*!< Streaming audio contexts */
  uint8_t            ccidList[GA_METADATA_LEN_CCID]; /*!< Array of CCID values */
  uint8_t            lang[GA_METADATA_LEN_LANGUAGE]; /*!< 3-byte, lower case language code as defined in ISO 639-3 */
  uint8_t            parentRating;    /*!< Bits 0-3 Value representing the parental rating */
  uint8_t            progInfoLen;     /*!< Length of program info */
  uint8_t            *pProgInfo;      /*!< Title and/or summary of Audio Stream content: UTF-8 format */
  uint8_t            uriLen;          /*!< Length of program info URI */
  uint8_t            *pUri;           /*!< UTF-8 formatted URL link used to present more information about Program Info */
  uint8_t            extMetadataLen;  /*!< Length of extended metadata */
  uint8_t            *pExtMetadata;   /*!< Extended metadata: Octet 0-1 = Extended Metadata Type, Octet 2-254 = Extended Metadata */
  uint8_t            vsMetadataLen;   /*!< Length of vendor-specific metadata */
  uint8_t            *pVsMetadata;    /*!< Vendor-specific metadata: Octet 0-1 = Company_ID, Octet 2-254 = Vendor-Specific Metadata */
} bapBcastMetadata_t;

/*! \brief BAP stream configuration */
typedef struct
{
  uint8_t                   direction;  /*!< Audio direction of the ASE */
  const bapGenCodecSpCfg_t  codecSpCfg; /*!< Codec specific configuration for the ASE */
  bapUcastMetadata_t        *pMetadata; /*!< Metadata for the ASE */
} bapStreamCfg_t;

/*! \brief BAP stream */
typedef struct
{
  bapStreamCfg_t     *pStreamCfg;     /*!< Audio stream configuration */
  uint8_t            aseId;           /*!< ASE_ID associated with audio stream configuration (will be set by BAPC) */
} bapStream_t;

/*! \brief BAP codec id */
typedef struct
{
  uint8_t            codingFmt;       /*!< \brief Coding Format */
  uint16_t           compId;          /*!< \brief Company ID (must be 0x0000 if 'codingFmt' not 0xFF) */
  uint16_t           vsCodecId;       /*!< \brief Vendor-defined codec ID (must be 0x0000 if 'codingFmt' not 0xFF) */
} bapCodecId_t;

/*! \brief BAP codec configuration */
typedef struct
{
  bapCodecId_t       codecId;         /*!< \brief Codec ID */
  uint8_t            targetLatency;   /*!< \brief Target latency */
  uint8_t            targetPhy;       /*!< \brief PHY parameter target to achieve the target latency value */
  uint8_t            codecCfgLen;     /*!< \brief Length of the codec specific configuration */
  uint8_t            codecCfg[GA_MAX_CODEC_LEN]; /*!< \brief LTV-formatted codec specific configuration */
} bapCodecCfg_t;

/*! \brief BAP codec configuration parameters */
typedef struct
{
  uint8_t            aseId;           /*!< \brief ASE ID */
  bapCodecCfg_t      codecCfg;        /*!< \brief Codec configuration */
} bapCodecParams_t;

/*! \brief BAP QoS configuration written by the client in the Config QoS operation */
typedef struct
{
  uint8_t            cisId;           /*!< CIG_ID parameter value for this ASE (assigned by the client) */
  uint8_t            cigId;           /*!< CIS_ID parameter value for this ASE (assigned by the client) */
  uint32_t           sduInterval;     /*!< SDU_Interval parameter value for this ASE */
  uint8_t            framing;         /*!< Framing parameter value for this ASE */
  uint8_t            phy;             /*!< PHY parameter value for this ASE */
  uint16_t           maxSdu;          /*!< Max_SDU parameter value for this ASE */
  uint8_t            retransNum;      /*!< Retransmission_Number parameter value for this ASE */
  uint16_t           maxTransLat;     /*!< Max_Transport_Latency parameter value for this ASE */
  uint32_t           presentDelay;    /*!< Presentation_Delay parameter value for this ASE */
} bapQosCfg_t;

/*! \brief BAP QoS configuration parameters */
typedef struct
{
  uint8_t            aseId;           /*!< ASE ID */
  bapQosCfg_t        qosCfg;          /*!< QoS configuration */
} bapQosParams_t;

/*! \brief BAP content info parameters */
typedef struct
{
  uint8_t            aseId;           /*!< ASE ID */
  uint8_t            metadataLen;     /*!< Length of the Metadata field for this ASE */
  uint8_t            *pMetadata;      /*!< Pointer to LTV-formatted Metadata for this ASE */
} bapMetadataParams_t;

/*! \brief Server's supported and preferred values for QoS parameters in Config Qos operation */
typedef struct
{
  uint8_t            framing;         /*!< \brief Server support for unframed ISOAL PDUs */
  uint8_t            prefPhy;         /*!< \brief Server preferred value for PHY param */
  uint8_t            prefRetransNum;  /*!< \brief Server preferred value for Retransmission_Number param (Range: 0x00-0xFF) */
  uint16_t           maxTransLat;     /*!< \brief Server preferred value for Max_Transport_Latency param in ms (Range: 0x0005-0x0FA0) */
  uint32_t           presentDelayMin; /*!< \brief Minimum server supported Presentation_Delay for ASE in us */
  uint32_t           presentDelayMax; /*!< \brief Maximum server supported Presentation_Delay for ASE in us */
  uint32_t           prefPresentDelayMin; /*!< \brief Server preferred minimum Presentation_Delay for ASE in us (0 = no preference). If nonzero, must be >= presentDelayMin */
  uint32_t           prefPresentDelayMax; /*!< \brief Server preferred maximum Presentation_Delay for ASE in us (0 = no preference). If nonzero, must be <= presentDelayMax */
} bapQosSpec_t;

/*! \brief BAP Server PAC PAC record structure */
typedef struct
{
  uint8_t            direction;       /*!< Direction of audio with respect to server */
  uint8_t            codecId;         /*!< Codec ID */
  uint8_t            sampleFreqBits;  /*!< Sampling frequency bitfield */
  uint8_t            codecLen;        /*!< Length of codec-specific parameters */
  uint8_t            *pCodecParams;   /*!< Pointer to codec-specific parameters */
  uint8_t            protectType;     /*!< Content protection type */
  uint8_t            protectLen;      /*!< Length of content protection type specific value */
  uint8_t            *pProtectValue;  /*!< Pointer to content protection type specific value */
} bapsPacRec_t;

/*! \brief BAP Server PAC structure */
typedef struct bapsPac_tag
{
  struct bapsPac_tag *pNext;          /*!< \brief For internal use only */
  uint8_t            direction;       /*!< \brief Audio direction */
  uint8_t            pacRecLen;       /*!< \brief Length of PAC record */
  uint8_t            *pPacRec;        /*!< \brief Pointer to PAC record */
  bapQosSpec_t       *pQosSpec;       /*!< \brief Pointer to Server's preferred QoS specification */
} bapsPac_t;

/*! \brief BAP Broadcast Audio Scan (BAS) subgroup info */
typedef struct
{
  uint32_t           bisSync;         /*!< \brief BIS (Broadcast Isochronous Stream) sync */
  uint8_t            metadataLen;     /*!< \brief Length of the Metadata parameter value */
  uint8_t            *pMetadata;      /*!< \brief LTV-formatted Metadata for Broadcast Source */
} bapBasSubgrp_t;

/*! \brief BAP Broadcast Audio Scan (BAS) Broadcast Source operation */
typedef struct
{
  uint8_t            srcId;           /*!< Source ID assigned by the server to a BRS characteristic */
  uint8_t            advAddrType;     /*!< Advertiser address type for Broadcast Source */
  bdAddr_t           advAddr;         /*!< Advertiser address for Broadcast Source */
  uint8_t            advSid;          /*!< Advertising SID of ADI of AUX_ADV_IND or LL_PERIODIC_SYNC_IND containing
                                           SyncInfo that points to PA transmitted by Broadcast Source */
  uint32_t           broadcastId;     /*!< Broadcast_ID of the Broadcast Source */
  uint8_t            paSync;          /*!< PA (Periodic Advertising) sync */
  uint16_t           paInterval;      /*!< SyncInfo field Interval parameter value */
} bapBasBcastSrc_t;

/*! \brief BAP Broadcast Audio Scan (BAS) Add Source operation */
typedef struct
{
  bapBasBcastSrc_t   bcastSrc;        /*!< \brief Broadcast source info */
  uint8_t            numSubgrp;       /*!< \brief Number of subgroups */
  bapBasSubgrp_t     subgrp[BAP_MAX_SUBGRP];  /*!< \brief Subgroup info */
} bapBasAddSrc_t;

/*! \brief BAP Broadcast Audio Scan (BAS) Modify Source operation */
typedef struct
{
  uint8_t            srcId;           /*!< \brief Source ID assigned by the server to a BRS characteristic */
  uint8_t            paSync;          /*!< \brief PA (Periodic Advertising) sync */
  uint16_t           paInterval;      /*!< \brief SyncInfo field Interval parameter value */
  uint8_t            numSubgrp;       /*!< \brief Number of subgroups */
  bapBasSubgrp_t     subgrp[BAP_MAX_SUBGRP];  /*!< \brief Subgroup info */
} bapBasModSrc_t;

/*! \brief BAP Broadcast Audio Scan (BAS) Remove Source operation */
typedef struct
{
  uint8_t            srcId;           /*!< Source ID assigned by the server to a BRS characteristic */
} bapBasRemSrc_t;

/*! \brief BAP Broadcast Audio Scan (BAS) Set Broadcast_Code operation */
typedef struct
{
  uint8_t            srcId;           /*!< Source ID assigned by the server to a BRS characteristic */
  uint8_t            *pBcastCode;     /*!< Broadcast_Code for Source ID assigned to a BRS characteristic */
} bapBasSetBcastCode_t;

/*! \brief BAP Broadcast Receive State (BRS) subgroup info */
typedef struct
{
  uint32_t           bisSync;         /*!< \brief BIS (Broadcast Isochronous Stream) sync */
  uint32_t           bisSyncState;    /*!< \brief BIS (Broadcast Isochronous Stream) sync state */
  uint8_t            metadataLen;     /*!< \brief Length of the Metadata parameter value */
  uint8_t            metadata[GA_MAX_METADATA_LEN];  /*!< \brief LTV-formatted Metadata */
} bapBrsSubgrp_t;

/*! \brief BAP BASS Broadcast Receive State (BRS) parameters */
typedef struct
{
  dmSyncId_t         syncId;          /*!< \brief Sync identifier */
  uint8_t            bigHandle;       /*!< \brief BIG handle */
  uint8_t            numBis;          /*!< \brief Total number of BISes in the BIG sync */
  uint8_t            bis[DM_BIS_MAX]; /*!< \brief List of indices of BISes (in ascending order) */
  uint8_t            srcAddrType;     /*!< \brief Source address type */
  bdAddr_t           srcAddr;         /*!< \brief Source address */
  uint8_t            srcAdvSid;       /*!< \brief Source advertising SID */
  uint32_t           broadcastId;     /*!< \brief Broadcast_ID of the Broadcast Source */
  uint8_t            paSync;          /*!< \brief PA (Periodic Advertising) sync */
  uint8_t            paSyncState;     /*!< \brief PA (Periodic Advertising) sync state */
  uint16_t           paInterval;      /*!< \brief SyncInfo field Interval parameter value */
  uint8_t            bigEncrypt;      /*!< \brief BIG (Broadcast Isochronous Group) encryption */
  dmSecBcastCode_t   bcastCode;       /*!< \brief Broadcast_Code for this Source ID */
  bool               encrypt;         /*!< \brief true if BIS encryption enabled */
  uint8_t            numSubgrp;       /*!< \brief Number of subgroups */
  bapBrsSubgrp_t     subgrp[BAP_MAX_SUBGRP];  /*!< \brief Subgroup info */
} bapBasBrsParams_t;

/* Codec Specific Configuration and Metadata containing length-type-value (LTV) structures */
typedef struct
{
  uint16_t            len;            /*!< Length of LTV data in bytes */
  uint8_t             data[BAP_MAX_LTV_LEN];/*!< LTV data buffer */
} bapLtvData_t;

typedef bapLtvData_t bapLtvCodecCfg_t;/*!< Codec Specific Configuration LTV structure */

/* Announcement tree node */
typedef struct
{
  uint8_t            id;              /*!< Identifier field */
  uint8_t            codecCfg;        /*!< Location of Codec Config in ltvDataTbl */
  uint8_t            metadata;        /*!< Location of Metadata ltvDataTbl */
  uint8_t            child;           /*!< Location of Child node in blockTbl */
  uint8_t            next;            /*!< Location of Next node in blockTbl */
} bapNode_t;

/* Announcement tree node for Level 3 BIS nodes */
typedef struct
{
  uint8_t            bisId;           /*!< BIS Identifier value - Invalid until set in BapskEncode */
  uint8_t            codecCfg;        /*!< Location of Codec Config in ltvDataTbl */
  uint8_t            reserved1;       /*!< Reserved */
  uint8_t            reserved2;       /*!< Reserved */
  uint8_t            next;            /*!< Location of Next BIS node in blockTbl */
} bapBis_t;

/* Announcement tree node for Level 2 Subgroup nodes */
typedef struct
{
  uint8_t            codecId;         /*!< 0xFE or Location of Codec ID in blockTbl */
  uint8_t            codecCfg;        /*!< Location of Codec Config in ltvDataTbl */
  uint8_t            metadata;        /*!< Location of Metadata ltvDataTbl */
  uint8_t            child;           /*!< Location of Child BIS node in blockTbl */
  uint8_t            next;            /*!< Location of Next Subgroup node in blockTbl */
} bapSubgrp_t;

/* Announcement tree node for Level 1 Group nodes */
typedef struct
{
  uint8_t            codecId;         /*!< Location of Codec ID in blockTbl */
  uint8_t            codecCfg;        /*!< Location of Codec Config in ltvDataTbl */
  uint8_t            metadata;        /*!< Location of Metadata ltvDataTbl */
  uint8_t            child;           /*!< Location of Child Subgroup node in blockTbl */
  uint8_t            presentDly;      /*!< Location of Presentation Delay in blockTbl */
} bapGroup_t;

/* Announcement tree node for Level 1 Group presentation delay value */
typedef struct
{
  uint32_t           presentDly;      /*!< Presentation Delay Value */
} bapPresentDly;

/* Union of bapskCb.blockTbl structures */
typedef union
{
  bapCodecId_t       codecId;         /*!< Codec_ID info */
  bapNode_t          node;            /*!< Node info */
  bapGroup_t         group;           /*!< Group info */
  bapSubgrp_t        subgroup;        /*!< Subgroup info */
  bapBis_t           bis;             /*!< BIS info */
  bapPresentDly      presentDly;      /*!< Presentation_Delay value */
} bapBlock_t;

/*! \} */    /* BASIC_AUDIO_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* BAP_DEFS_H */
