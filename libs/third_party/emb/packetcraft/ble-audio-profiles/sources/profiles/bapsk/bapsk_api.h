/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Broadcast Sink.
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
#ifndef BAPSK_API_H
#define BAPSK_API_H

#include "att_api.h"
#include "bap_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_BROADCAST_SINK
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
  dmSyncId_t          syncId;             /*!< \brief Sync identifier */
  uint16_t            syncHandle;         /*!< \brief Sync handle */
  uint8_t             bigHandle;          /*!< \brief BIG handle */
  uint8_t             basState;           /*!< \brief BAS state */
  bool                paStarted;          /*!< \brief true if PA started */
  uint8_t             numSubgrp;          /*!< \brief Number of subgroups used to group BISes present in the BIG */
  uint8_t             numBis;             /*!< \brief Total number of BISes in the BIG */
  uint8_t             nse;                /*!< \brief Number of Sub-Events in each BIS event in the BIG */
  bool                encrypt;            /*!< \brief true if BIS encryption enabled */
  dmSecBcastCode_t    bcastCode;          /*!< \brief Broadcast_Code for this sync id */
  uint32_t            ctrDly;             /*!< \brief Controller delay (in usec) */
} bapskBasCb_t;

/*! BIG control block structure type */
typedef struct
{
  uint8_t           bigHandle;            /*!< \brief BIG handle */
  dmSyncId_t        syncId;               /*!< \brief Sync identifier */
  uint8_t           numBis;               /*!< \brief Total number of BISes in the BIG sync */
  uint8_t           bis[DM_BIS_MAX];      /*!< \brief List of indices of BISes (in ascending order) */
  uint16_t          bisHandle[DM_BIS_MAX];/*!< \brief BIS handles */
  uint8_t           subgrp;               /*!< \brief Subgroup that BISes belong to */
  uint32_t          bisSync;              /*!< \brief BIS_Sync bitfield. Bit 0-30 = BIS_index[1-31] */
  bool              bigStarted;           /*!< \brief true if BIG sync started */
  bool              inUse;                /*!< \brief true if entry in use */
} bapskBigCb_t;

/*! BIS control block structure type */
typedef struct
{
  uint8_t           bigHandle;            /*!< \brief BIG that BIS belongs to */
  uint16_t          bisHandle;            /*!< \brief BIS handle */
  uint8_t           bisIndex;             /*!< \brief BIS_index */
  bapLtvCodecCfg_t  codecSpCfg;           /*!< \brief Codec specific configuration LTV for the BIS */
  bapskBasCb_t      *pBasCb;              /*!< \brief Pointer to Broadcast Audio Stream control block */
  bool              dpCreated;            /*!< \brief true if data path created for this direction */
  bool              vsDpConfig;           /*!< \brief true if vendor-specific data path configured */
  bool              inUse;                /*!< \brief true if enry in use */
} bapskBisCb_t;

/*! \brief BAP Broadcast Sink BAS update callback */
typedef void (*BapskBasCback_t)(uint8_t bigHandle, bapskBasCb_t *pBasCb, uint8_t basOldState,
                                bapskBigCb_t *pBigCb);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  BAP Broadcast Sink Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Synchronize with periodic advertising from the given Broadcast Source, and start
 *          receiving the Basic Broadcast Annoucements.
 *
 *  \param  advSid      Advertising SID.
 *  \param  advAddrType Advertiser address type.
 *  \param  advAddr     Advertiser address.
 *
 *  \return Sync identifier.
 */
/*************************************************************************************************/
dmSyncId_t BapskBasicAudioAnncSync(uint8_t advSid, uint8_t advAddrType, bdAddr_t advAddr);

/*************************************************************************************************/
/*!
 *  \brief  Establish Broadcast Audio Stream(s) by synchronizing to a given Broadcast Isochronous
 *          Group (BIG).
 *
 *  \param  syncId      Sync identifier.
 *  \param  baseLevel   Level of the node in the BASE structure.
 *  \param  subgrpId    Subgroup identifier in the BIG group.
 *  \param  bisSync     BIS_Sync bitfield. Bit 0-30 = BIS_index[1-31].
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapskBasEstablish(dmSyncId_t syncId, uint8_t baseLevel, uint8_t subgrpId, uint32_t bisSyn);

/*************************************************************************************************/
/*!
 *  \brief  Establish Broadcast Audio Stream(s) with no preference.
 *
 *  \param  syncId    Sync identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapskBasNoPrefEstablish(dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Disable Broadcast Audio Stream(s) by stopping synchronization to the BIG identified by
 *          the handle.
 *
 *  \param  bigHandle BIG handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapskBasDisable(uint8_t bigHandle);

/*************************************************************************************************/
/*!
 *  \brief  Release Broadcast Audio Stream(s) by stopping reception of the periodic advertising
 *          identified by the given sync identifier.
 *
 *  \param  syncId    Sync identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapskBasRelease(dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Delete a node with the given hierarchal level in the Broadcast Audio Source Endpoint
 *          (BASE) structure of the Basic Audio Announcements.
 *
 *  \param  syncId    Sync identifier.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void BapskBaseDelete(dmSyncId_t syncId, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId);

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
void BapskSetVsDataPathConfig(uint8_t dpId, uint8_t configLen, uint8_t *pConfig);

/*************************************************************************************************/
/*!
 *  \brief  Get the Presentation_Delay value in a Broadcast Audio Announcement.
 *
 *  \param  syncId     Sync identifier.
 *
 *  \return Presentation_Delay value.
 */
/*************************************************************************************************/
uint32_t BapskGetPresentationDelay(dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Get the Codec Specific Configuration for the specified BASE level in a Broadcast Audio
 *          Announcement.
 *
 *  \param  syncId    Sync identifier.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisId     BIS identifier in a subgroup.
 *  \param  adType    Data element type.
 *
 *  \return Pointer to the Codec Specific Configuration value if found. NULL otherwise.
 */
/*************************************************************************************************/
uint8_t *BapskGetCodecCfg(dmSyncId_t syncId, uint8_t baseLevel, uint8_t subgrpId, uint8_t bisId,
                          uint8_t adType);

/*************************************************************************************************/
/*!
 *  \brief  Get the Metadata of a given node in a Broadcast Audio Announcement.
 *
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  pLen      Pointer to Metdata length to be returned.
 *
 *  \return Pointer to the Metadata value if found. NULL otherwise.
 */
/*************************************************************************************************/
uint8_t *BapskGetMetadata(dmSyncId_t syncId, uint8_t baseLevel, uint8_t subgrpId, uint16_t *pLen);

/*************************************************************************************************/
/*!
 *  \brief  Get a data element in the Metadata of a given node in a Broadcast Audio Announcement.
 *
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  adType    Data element type.
 *
 *  \return Pointer to the Metadata value if found. NULL otherwise.
 */
/*************************************************************************************************/
uint8_t *BapskGetMetadataByType(dmSyncId_t syncId, uint8_t baseLevel, uint8_t subgrpId, uint8_t adType);

/*************************************************************************************************/
/*!
 *  \brief  Get the BIS indices in a Broadcast Audio Announcement.
 *
 *  \param  syncId    Sync identifier.
 *  \param  baseLevel Level of the node in the BASE structure.
 *  \param  subgrpId  Subgroup identifier in the group.
 *  \param  bisSync   BIS_Sync bitfield. Bit 0-30 = BIS_index[1-31].
 *  \param  pBisIndex List of indices of BISes (to be updated). Must be at least size of
 *                    BAP_MAX_BIS.
 *
 *  \return Total number of BISes found in the BIG.
 */
 /*************************************************************************************************/
uint8_t BapskGetBisIndices(dmSyncId_t syncId, uint8_t baseLevel, uint8_t subgrpId, uint32_t bisSync,
                           uint8_t *pBisIndex);

/*************************************************************************************************/
/*!
 *  \brief  Set the Broadcast Code for the given sync to decrypt the corresponding BIS(es).
 *
 *  \param  syncId       Sync identifier
 *  \param  pBcastCode   Broadcast code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapskSetBcastCode(dmSyncId_t syncId, uint8_t *pBcastCode);

/*************************************************************************************************/
/*!
 *  \brief  Set the Metadata that the application is looking for in a subgroup in addition to the
 *          Streaming Audio Contexts.
 *
 *  \param  len          Lenght of the Metadata.
 *  \param  pMetadata    Pointer to the Metadata specified by the app.
 *
 *  \return None.
 *
 *  \note   The caller is responsible for maintaining the memory that contains the Metadata.
 */
/*************************************************************************************************/
void BapskSetAdditionalMetadata(uint8_t len, uint8_t *pMetadata);

/*************************************************************************************************/
/*!
 *  \brief  Get the Metadata for a given subgroup within a BASE group.
 *
 *  \param  syncId       Sync identifier.
 *  \param  subgrp       Subgroup number within the BASE group.
 *  \param  pLen         Pointer to Metadata length to be updated.
 *
 *  \return Pointer to Metadata if subgroup found. NULL, otherwise.
 */
/*************************************************************************************************/
const uint8_t *BapskGetSubgrpMetadata(dmSyncId_t syncId, uint8_t subgrp, uint8_t *pLen);

/*************************************************************************************************/
/*!
 *  \brief  Enable automatic Broadcast Audio Stream (BAS) establishment.
 *
 *  \param  autoEst      true if automatic BAS establishment to be turned on.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapskEnableAutoBasEst(bool autoEst);

/*************************************************************************************************/
/*!
 *  \brief  Return the automatic Broadcast Audio Stream (BAS) establishment flag.
 *
 *  \return true if automatic BAS establishment is turned on. false, otherwise.
 */
/*************************************************************************************************/
bool BapskGetEnableAutoBasEst(void);

/*************************************************************************************************/
/*!
 *  \brief  BAP Broadcast Sink initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapskInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application Broadcast Source BAS callback function.
 *
 *  \param  cback    Application callback
 *
 *  \return none
 */
/*************************************************************************************************/
void BapskBasCbackRegister(BapskBasCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP Broadcast Sink of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapskProcDmMsg(dmEvt_t *pEvt);

/*! \} */    /* BASIC_AUDIO_PROFILE_BROADCAST_SINK */

#ifdef __cplusplus
};
#endif

#endif /* BAPSK_API_H */
