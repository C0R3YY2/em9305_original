/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Scan Delegator (SD).
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
#ifndef BAPSD_API_H
#define BAPSD_API_H

#include "att_api.h"
#include "bap_defs.h"
#include "svc_bas.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_SCAN_DELEGATOR
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief  BAP Scan Delegator (SD) Broadcast Audio Scan (BAS) operations */
#define BAPSD_BAS_OP_RMT_SCAN_STOP          0x00  /*!< Remote Scan Stopped */
#define BAPSD_BAS_OP_RMT_SCAN_START         0x01  /*!< Remote Scan Started */
#define BAPSD_BAS_OP_START_SYNC_INFO_TMR    0x02  /*!< Start timer for receiving SyncInfo data from client */
#define BAPSD_BAS_OP_STOP_SYNC_INFO_TMR     0x03  /*!< Stop timer for receiving SyncInfo data from client */
#define BAPSD_BAS_OP_SYNC_PA                0x04  /*!< Synchronize to PA */
#define BAPSD_BAS_OP_STOP_SYNC_PA           0x05  /*!< Stop synchronization with PA */
#define BAPSD_BAS_OP_ENABLE_RX_BIG_INFO     0x06  /*!< Enable reception of BIG Info report */
#define BAPSD_BAS_OP_SYNC_BIS               0x07  /*!< Synchronize to BIS */
#define BAPSD_BAS_OP_SYNC_BIS_NO_PREF       0x08  /*!< Synchronize to BIS no preference */
#define BAPSD_BAS_OP_STOP_SYNC_BIS          0x09  /*!< Stop synchronization with BIS */
#define BAPSD_BAS_OP_SET_BCAST_CODE         0x0A  /*!< Set Broadcast_Code */
#define BAPSD_BAS_OP_REM_SRC                0x0B  /*!< Remove Source */

#define BAPSD_BAS_OP_MAX                    0x0C

/*! \brief  BAP Scan Delegator BRS Source ID base */
#define BAPSD_BRS_SRC_ID_BASE               0x00  /*!< Source ID base */

/*! \brief BAP Scan Delegator BRS Source IDs (range: 0x00-0xFF) */
#define BAPSD_BRS_SRC_1_ID                  BAPSD_BRS_SRC_ID_BASE
#define BAPSD_BRS_SRC_2_ID                  (BAPSD_BRS_SRC_ID_BASE + 1)
#define BAPSD_BRS_SRC_ID_NONE               0xFF

/*! \brief Remote Scan States */
enum
{
  BAPSD_RMT_SCAN_STATE_STOP = 0,                  /*!< Remote Scan Stopped */
  BAPSD_RMT_SCAN_STATE_START                      /*!< Remote Scan Started  */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief BAP Scan Delegator metadata configuration */
typedef struct
{
  uint8_t               srcId;                    /*!< \brief Source ID */
  uint8_t               metadataLen;              /*!< \brief Length of the Metadata field for this source */
  uint8_t               metadata[GA_MAX_METADATA_LEN];  /*!< \brief LTV-formatted Metadata for this source */
} bapsdMetadataCfg_t;

/*! \brief BAP Scan Delegator BASS Remote Scan operation */
typedef struct
{
  uint8_t               state;                    /*!< Remote scan state */
} bapsdBasRmtScan_t;

/* Union of all BAP Scan Delegator BAS messages */
typedef union
{
  bapsdBasRmtScan_t     rmtScan;                  /*!< Remote scan start/stop operation */
  bapBasAddSrc_t        addSrc;                   /*!< Add source operation */
  bapBasModSrc_t        modSrc;                   /*!< Modify source operation */
  bapBasRemSrc_t        remSrc;                   /*!< Remove source operation */
  bapBasSetBcastCode_t  setBcastCode;             /*!< Set Broadcast_Code operation */
} bapsdBasMsg_t;

/*!
 *  \brief BAP Scan Delegator (SD) BAS update callback type.
 *
 *  \param  connId          DM connection identifier.
 *  \param  srcId           Source identifier.
 *  \param  oper            BAS operation.
 *  \param  subgrp          Subgroup to synchronize with.
 *  \param  pBrsParams      Pointer to BRS parameters.
 *
 *  \return None.
 *
 *  \note   This callback function calls the server application's callback when a BAP Scan Delegator
 *          (SD) BAS update is received.
 */
typedef void (*BapsdBasCback_t)(dmConnId_t connId, uint8_t srcId, uint8_t oper,
                                uint8_t subgrp, bapBasBrsParams_t *pBrsParams);

/*!
 *  \brief BAP Scan Delegator (SD) event callback type.
 *
 *  \param  connId          DM connection identifier.
 *  \param  pEvt            Pointer to DM event.
 *  \param  pBrsParams      Pointer to BRS parameters.
 *
 *  \return None.
 *
 *  \note   This callback function calls the server application's callback when BAP Scan Delegator
 *          (SD) processes a DM event.
 */
typedef void (*BapsdEvtCback_t)(dmConnId_t connId, dmEvt_t *pEvt, bapBasBrsParams_t *pBrsParams);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Provide the server with information regarding a Broadcast Source.
 *
 *  \param  connId      DM connection ID.
 *  \param  addrType    Advertiser address type.
 *  \param  addr        Advertiser address.
 *  \param  advSid      Advertising SID.
 *  \param  broadcastId Broadcast_ID of the Broadcast Source.
 *  \param  syncId      Sync identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsdAddBcastSrc(dmConnId_t connId, uint8_t addrType, bdAddr_t addr, uint8_t advSid,
                      uint32_t broadcastId, dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Set the ist of indices corresponding to BIS(es) in a given Broadcast Isochronous Group
 *          (BIG) to be synchronized.
 *
 *  \param  bigHandle BIG handle.
 *  \param  syncId    Sync identifier.
 *  \param  numBis    Total number of BISes in the BIG.
 *  \param  pBis      List of indices of BISes (in ascending order).
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsdSetBisSync(uint8_t bigHandle, dmSyncId_t syncId, uint8_t subgrp, uint32_t bisIndex,
                     uint8_t numBis, uint8_t *pBis);

/*************************************************************************************************/
/*!
 *  \brief  Process a BIS Sync request which was originally made as part an Add Source operation.
 *
 *  \param  syncId    Sync identifier.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapsdProcBisSync(dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP Unicast Scan Delegator of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsdProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for BASS service.
 *
 *  \param  connId     Connection identifier.
 *  \param  handle     ATT handle.
 *  \param  operation  Operation selected.
 *  \param  offset     Offset to begin read from.
 *  \param  pAttr      Attribute to read from.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t BapsdBasReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                          attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for BASS service.
 *
 *  \param  connId     Connection identifier.
 *  \param  handle     ATT handle.
 *  \param  operation  Operation selected.
 *  \param  offset     Offset to begin write.
 *  \param  len        Length of write.
 *  \param  pValue     Pointer to buffer to write.
 *  \param  pAttr      Attribute to write to.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t BapsdBasWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                           uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  Find the subgroup that contains synchronized BIS(es).
 *
 *  \param  pBrsCb     Pointer to BRS control block.
 *
 *  \return Subgroup containing synchronized BIS(es). BAP_MAX_SUBGRP, otherwise.
 */
/*************************************************************************************************/
uint8_t BapsdFindSyncedSubgrp(const bapBasBrsParams_t *pBrsParams);

/*************************************************************************************************/
/*!
 *  \brief  Called after the server did not receive SyncInfo data from the client.
 *
 *  \param  connId     DM connection ID.
 *  \param  srcId      Source identifier.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapsdSyncInfoTimeout(dmConnId_t connId, uint8_t srcId);

/*************************************************************************************************/
/*!
 *  \brief  BAP Scan Delegator BAS initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsdInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application BAS update callback functions.
 *
 *  \param  cback    Application BAS callback
 *  \param  cback    Application event callback
 *
 *  \return none
 */
/*************************************************************************************************/
void BapsdBasCbackRegister(BapsdBasCback_t basCback, BapsdEvtCback_t evtCback);

/*************************************************************************************************/
/*!
 *  \brief  Send out a BRS value notification for a given BRS.
 *
 *  \param  connId     DM connection ID.
 *  \param  handle     BRS attribute handle.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapsdBasNotifyBrs(dmConnId_t connId, uint16_t handle);

/*! \} */    /* BASIC_AUDIO_PROFILE_SCAN_DELEGATOR */

#ifdef __cplusplus
};
#endif

#endif /* BAPSD_API_H */
