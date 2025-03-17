/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Broadcast Source.
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
#ifndef BAPSC_API_H
#define BAPSC_API_H

#include "att_api.h"
#include "bap_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_BROADCAST_SOURCE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief BAP Broadcast Audio Stream (BAS) control block */
typedef struct
{
  uint8_t             advHandle;          /*!< \brief Advertising handle */
  uint8_t             bigHandle;          /*!< \brief BIG handle */
  uint8_t             basState;           /*!< \brief BAS state */
  bool                paStarted;          /*!< \brief true if PA started */
  uint32_t            ctrDly;             /*!< \brief Controller delay (in usec) */
} bapscBasCb_t;

/*! BIG control block structure type */
typedef struct
{
  uint8_t           bigHandle;            /*!< BIG handle */
  uint8_t           advHandle;            /*!< Advertising handle */
  uint8_t           numBis;               /*!< Number of BISes in BIG */
  uint16_t          bisHandle[DM_BIS_MAX];/*!< BIS handles */
  bool              bigStarted;           /*!< true if BIG started */
  bool              inUse;                /*!< true if entry in use */
} bapscBigCb_t;

/*! BIS control block structure type */
typedef struct
{
  uint8_t           bigHandle;            /*!< \brief BIG that BIS belongs to */
  uint16_t          bisHandle;            /*!< \brief BIS handle */
  uint8_t           bisIndex;             /*!< \brief BIS_index */
  bapLtvCodecCfg_t  codecSpCfg;           /*!< \brief Codec specific configuration LTV for the BIS */
  bapscBasCb_t      *pBasCb;              /*!< \brief Pointer to Broadcast Audio Stream control block */
  bool              dpCreated;            /*!< \brief true if data path created for this direction */
  bool              vsDpConfig;           /*!< \brief true if vendor-specific data path configured */
  bool              inUse;                /*!< \brief true if enry in use */
} bapscBisCb_t;

/*! \brief BAP Broadcast Source BAS update callback */
typedef void (*BapscBasCback_t)(uint8_t bigHandle, const bapscBasCb_t *pBasCb, uint8_t basOldState);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  BAP Broadcast Source Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Look up an BAS control block for a given advertising handle.
 *
 *  \param  advHandle    Advertising handle.
 *
 *  \return Pointer to BAS control block.
 */
/*************************************************************************************************/
bapscBasCb_t *BapscBasCbByAdvHandle(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Return the BAS state for a given advertising handle.
 *
 *  \param  advHandle    Advertising handle.
 *
 *  \return BAS state.
 */
/*************************************************************************************************/
uint8_t BapscBasState(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Determine the number of bytes required to encode a Broadcast Audio Announcement.
 *
 *  \param  advHandle Advertising handle of the announcement data.
 *
 *  \return Number of bytes required to encode an announcement.
 */
/*************************************************************************************************/
uint8_t BapscGetEncodeLen(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Encode the Broadcast Audio Source Endpoint (BASE) structure for a Broadcast Audio
 *          Announcement.
 *
 *  \param  advHandle Advertising handle of the announcement data.
 *  \param  pBuf      Buffer to hold BASE structure.
 *  \param  bufLen    Length of the Broadcast Audio Announcement data buffer maintained by the
 *                    Application.
 *
 *  \return Number of bytes written to pBuf.
 */
/*************************************************************************************************/
uint8_t BapscBaseEncode(uint8_t advHandle, uint8_t *pBuf, uint8_t bufLen);

/*************************************************************************************************/
/*!
 *  \brief  Get the list of BIS handles in a given BIG.
 *
 *  \param  bigHandle   BIG identifier.
 *  \param  pBisHandle  List of BIS handles to be returned. Must be at least size of DM_BIS_MAX.
 *
 *  \return Number of BISes if BIG found. 0, otherwise.
 */
/*************************************************************************************************/
uint8_t BapscGetBisHandle(uint8_t bigHandle, uint16_t *pBisHandle);

/*************************************************************************************************/
/*!
 *  \brief  Add a Group node in a Broadcast Audio Announcement.
 *
 *  \param  advHandle  Advertising handle.
 *
 *  \return Position of the Group node.
 */
/*************************************************************************************************/
uint8_t BapscAddGroup(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Add a Subgroup node in a Broadcast Audio Announcement.
 *
 *  \param  advHandle  Advertising handle.
 *
 *  \return Position of the node in the list of subgroups.
 */
/*************************************************************************************************/
uint8_t BapscAddSubgroup(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Add a BIS node to the specified subgroup in a Broadcast Audio Announcement.
 *
 *  \param  advHandle  Advertising handle.
 *  \param  subgrpId   Parent subgroup identifier.
 *
 *  \return Position of the node in the list of BISes in the subgroups.
 */
/*************************************************************************************************/
uint8_t BapscAddBis(uint8_t advHandle, uint8_t subgrpId);

/*************************************************************************************************/
/*!
 *  \brief  Configure (or reconfigure) the Presentation_Delay value in a Broadcast Audio Announcement.
 *
 *  \param  advHandle  Advertising handle.
 *  \param  presentDly Presentation_Delay value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscPresentationDelayConfig(uint8_t advHandle, uint32_t presentDly);

/*************************************************************************************************/
/*!
 *  \brief  Configure (or reconfigure) the Codec_ID value for the specified BASE level in a Broadcast
 *          Audio Announcement.
 *
 *  \param  advHandle Advertising handle.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *  \param  pCodecId  Pointer to Codec_ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscCodecIdConfig(uint8_t advHandle, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId,
                        bapCodecId_t *pCodecId);

/*************************************************************************************************/
/*!
 *  \brief  Configure (or reconfigure) the Codec Specific Configuration for the specified BASE
 *          level in a Broadcast Audio Announcement.
 *
 *  \param  advHandle Advertising handle.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added or updated, false otherwise.
 */
/*************************************************************************************************/
bool BapscCodecCfgConfig(uint8_t advHandle, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId,
                         uint8_t adType, uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Configure (or reconfigure) the Metadata for the specified BASE level in a Broadcast
 *          Audio Announcement.
 *
 *  \param  advHandle Advertising handle.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added or updated, false otherwise.
 */
/*************************************************************************************************/
bool BapscMetadataConfig(uint8_t advHandle, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId,
                         uint8_t adType, uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Update the Metadata for the specified BASE level in a Broadcast Audio Announcement.
 *
 *  \param  advHandle Advertising handle.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *  \param  adType    Advertising data element type.
 *  \param  len       Length of the value.
 *  \param  pValue    Pointer to the value.
 *
 *  \return true if the element was successfully added or updated, false otherwise.
 */
/*************************************************************************************************/
bool BapscMetadataUpdate(uint8_t advHandle, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId,
                         uint8_t adType, uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Start Extended Advertising (EA) that contains Broadcast Audio Announcements, which
 *          associate PA with broadcast Audio Streams.
 *
 *  \param  advHandle Advertising handle.
 *  \param  advSid    Advertsing SID.
 *  \param  len       Length of advertising data.
 *  \param  pData     Pointer to advertising data.
 *  \param  bufLen    Length of advertising data buffer maintained by Application.  Minimum
 *                    length is 31 bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBcastAudioAnncStart(uint8_t advHandle, uint8_t advSid, uint16_t len, uint8_t *pData,
                              uint16_t bufLen);

/*************************************************************************************************/
/*!
 *  \brief  Start Periodic Advertising (PA) that contains Basic Audio Announcements which
 *          expose broadcast Audio Stream parameters defined as the Broadcast Audio Source Endpoint
 *          (BASE) structure.
 *
 *  \param  advHandle Advertising handle.
 *  \param  len       Length of advertising data.
 *  \param  pData     Pointer to advertising data.
 *  \param  bufLen    Length of advertising data buffer maintained by Application.  Minimum
 *                    length is 31 bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBasicAudioAnncStart(uint8_t advHandle, uint16_t len, uint8_t *pData, uint16_t bufLen);

/*************************************************************************************************/
/*!
 *  \brief  Update Basic Audio Announcements which expose broadcast Audio Stream parameters
 *          defined as the Broadcast Audio Source Endpoint (BASE) structure.
 *
 *  \param  advHandle Advertising handle.
 *  \param  len       Length of advertising data.
 *  \param  pData     Pointer to advertising data.
 *  \param  bufLen    Length of advertising data buffer maintained by Application.  Minimum
 *                    length is 31 bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBasicAudioAnncUpdate(uint8_t advHandle, uint16_t len, uint8_t *pData, uint16_t bufLen);

/*************************************************************************************************/
/*!
 *  \brief  Start or resume broadcast Audio Stream transmission by establishing the specified
 *          Broadcast Audio Stream(s).
 *
 *  \param  bigHandle   BIG identifier.
 *  \param  advHandle   Used to identify the periodic advertising train.
 *  \param  pBigParams  Pointer to BIG parameters.
 *  \param  pBcastCode  Session key used to encrypt and decrypt BIS payloads. NULL if BIS not encrypted.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBasEstablish(uint8_t *pBigHandle, uint8_t advHandle, const BapBigParams_t *pBigParams,
                       uint8_t *pBcastCode);

/*************************************************************************************************/
/*!
 *  \brief  Disable a broadcast Audio Stream for the given BIG handle.
 *
 *  \param  bigHandle BIG handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBasDisable(uint8_t bigHandle);

/*************************************************************************************************/
/*!
 *  \brief  Release a broadcast Audio Stream for the given advertising handle.
 *
 *  \param  advHandle Advertising handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBasRelease(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Stop the Broadcast Audio Announcements.
 *
 *  \param  advHandle Advertising handle of the announcement data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscBcastAudioAnncStop(uint8_t advHandle);

/*************************************************************************************************/
/*!
 *  \brief  Delete a node with the given hierarchal level in the Broadcast Audio Source Endpoint
 *          (BASE) structure of the Basic Audio Announcements.
 *
 *  \param  advHandle Advertising handle.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapscBaseDelete(uint8_t advHandle, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId);

/*************************************************************************************************/
/*!
 *  \brief  Set the configuration data for a vendor-specific data transport path.
 *
 *  \param  dpId      Data path ID.
 *  \param  configLen Length of vendor-specific configuration data.
 *  \param  pConfig   Vendor-specific configuration data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscSetVsDataPathConfig(uint8_t dpId, uint8_t configLen, uint8_t *pConfig);

/*************************************************************************************************/
/*!
 *  \brief  BAP Broadcast Source initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapscInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application Broadcast Source BAS callback function.
 *
 *  \param  cback    Application callback
 *
 *  \return none
 */
/*************************************************************************************************/
void BapscBasCbackRegister(BapscBasCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP Broadcast Source of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapscProcDmMsg(dmEvt_t *pEvt);

/*! \} */    /* BASIC_AUDIO_PROFILE_BROADCAST_SOURCE */

#ifdef __cplusplus
};
#endif

#endif /* BAPSC_API_H */
