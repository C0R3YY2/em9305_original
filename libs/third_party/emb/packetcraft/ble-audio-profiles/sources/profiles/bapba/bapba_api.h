/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Broadcast Assistant (BA).
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
#ifndef BAPBA_API_H
#define BAPBA_API_H

#include "att_api.h"
#include "bap_defs.h"
#include "svc_bas.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_BROADCAST_ASSISTANT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of BAS characteristics to be discovered */
enum
{
  BAPBA_BAS_CP_HDL_IDX,                   /*!< BAS Control Point */
  BAPBA_BAS_BRS_1_HDL_IDX,                /*!< BRS #1 */
  BAPBA_BAS_BRS_CCC_1_HDL_IDX,            /*!< BRS #1 client characteristic configuration */
  BAPBA_BAS_BRS_2_HDL_IDX,                /*!< BRS #2 */
  BAPBA_BAS_BRS_CCC_2_HDL_IDX,            /*!< BRS #2 client characteristic configuration */

  BAPBA_BAS_HDL_LIST_LEN                  /*!< Handle list length */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief BAP Broadcast Audio Source Endpoint (BASE) BIS info */
typedef struct
{
  uint8_t            bisIndex;             /*!< BIS_Index value */
  uint8_t            codecCfgLen;         /*!< Length of the codec specific configuration */
  uint8_t            codecCfg[GA_MAX_CODEC_LEN];     /*!< LTV-formatted codec specific configuration */
} bapbaBaseBis_t;

/*! \brief BAP Broadcast Audio Source Endpoint (BASE) Subgroup info */
typedef struct
{
  bapCodecId_t       codecId;             /*!< Codec_ID info */
  uint8_t            codecCfgLen;         /*!< Length of the codec specific configuration */
  uint8_t            codecCfg[GA_MAX_CODEC_LEN];     /*!< LTV-formatted codec specific configuration */
  uint8_t            metadataLen;         /*!< Length of the Metadata value */
  uint8_t            metadata[GA_MAX_METADATA_LEN];  /*!< LTV-formatted Metadata */
  uint8_t            numBis;              /*!< Number of BISes in Subgroup */

  /* Level 3 (BIS) info */
  bapbaBaseBis_t     bis[BASS_BIS_MAX];    /*!< \brief Subgroup info */
} bapbaBaseSubgrp_t;

/*! \brief BAP Broadcast Audio Source Endpoint (BASE) Group info */
typedef struct
{
  /* Level 1 (Group) */
  uint32_t    presentDly;                 /*!< Presentation_Delay */
  uint8_t     numSubgrp;                  /*!< Number of subgroups in Group */

  /* Level 2 (Subgroup) info */
  bapbaBaseSubgrp_t  subgrp[BASS_SUBGRP_MAX];  /*!< \brief Subgroup info */
} BapbaBaseGrp_t;

/*!
 *  \brief BAP Broadcast Assistant (BA) BRS callback type.
 *
 *  \param  connId      DM connection identifier.
 *  \param  srcId       Source identifier.
 *  \param  pBrsParams  Pointer to BRS parameters.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Broadcast Assistant
 *          (BA) BRS update is received.
 */
typedef void (*BapbaBrsCback_t)(dmConnId_t connId, uint8_t srcId, const bapBasBrsParams_t *pBrsParams);

/*!
 *  \brief BAP Broadcast Assistant (BA) operation callback
 *
 *  \param connId       DM connection ID.
 *  \param srcId        Source ID.
 *  \param opcode       Opcode of completed operation.
 *  \param status       Status of completed operation.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Broadcast Assistant
 *          (BA) operation is completed.
 */
typedef void (*BapbaOpCback_t)(dmConnId_t connId, uint8_t srcId, uint8_t opcode, uint8_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  BAP Broadcast Assistant (BA) BAS Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Look up the Broadcast Source information by the combined advertising address type,
 *          advertising SID, and Broadcast_ID fields.
 *
 *  \param  connId      DM Connection ID.
 *  \param  addrType    Advertiser address type.
 *  \param  advSid      Advertising SID.
 *  \param  broadcastId Broadcast_ID of the Broadcast Source.
 *  ]param  pSrcId      Source ID (to be returned).
 *
 *  \return BRS parameters for the Source ID if found. NULL, otherwise.
 */
/*************************************************************************************************/
const bapBasBrsParams_t *BapbaSrcInfoByBroadcastId(dmConnId_t connId, uint8_t addrType, uint8_t advSid,
                                                   uint32_t broadcastId, uint8_t *pSrcId);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a BAS Remote Scan Stop operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  handle    BAS Control Point attribute handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasRmtScanStart(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a BAS Remote Scan Stop operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  handle    BAS Control Point attribute handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasRmtScanStop(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a BAS Add Source operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  pAddSrc   Pointer to add source parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasAddSrc(dmConnId_t connId, bapBasAddSrc_t *pAddSrc);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a BAS Modify Source operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  pAddSrc   Pointer to modify source parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasModSrc(dmConnId_t connId, bapBasModSrc_t *pModSrc);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a BAS Set Broadcast_Code operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  pAddSrc   Pointer to set Broadcast_Code parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasSetBcastCode(dmConnId_t connId, bapBasSetBcastCode_t *pSetBcastCode);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a BAS Remove Source operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  pAddSrc   Pointer to modify source parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasRemSrc(dmConnId_t connId, bapBasRemSrc_t *pRemSrc);

/*************************************************************************************************/
/*!
 *  \brief  Decode the Broadcast Audio Source Endpoint (BASE) structure for a Basic Audio
 *          Announcement.
 *
 *  \param  pData     Pointer to the Basic Audio Announcement data.
 *  \param  len       Length of the Basic Audio Announcement data.
 *  \param  pBaseGrp  Pointer to the BASE Group (to be decoded from Basic Audio Announcement).
 *
 *  \return true if the BASE record decoded. false, otherwise.
 */
/*************************************************************************************************/
bool BapbaBaseDecode(uint8_t *pStream, uint8_t len, BapbaBaseGrp_t *pBaseGrp);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for ASE service.  Note that pHdlList
 *          must point to an array of handles of length BAPBA_BAS_HDL_LIST_LEN.  If discovery is
 *          successful the handles of discovered characteristics and descriptors will be set
 *          in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaBasDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set BAS service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can be used to restore handles after reestablishing a bond with a device.
 */
/*************************************************************************************************/
void BapcBasSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length BAPBA_BAS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return ATT_SUCCESS if handle is found, ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t BapbaBasValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP Broadcast Assistant of System events.
 *
 *  \param  pEvt   Pointer to the event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  BAP Broadcast Assistant (BA) BAS initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapbaInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to register application Broadcast Assistant BRS update and operation callback
 *          functions.
 *
 *  \param  brsCback  Application BRS callback.
 *  \param  opCback   Application operation callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void BapbaBrsCbackRegister(BapbaBrsCback_t brsCback, BapbaOpCback_t opCback);

/*! \} */    /* BASIC_AUDIO_PROFILE_BROADCAST_ASSISTANT */

#ifdef __cplusplus
};
#endif

#endif /* BAPBA_API_H */
