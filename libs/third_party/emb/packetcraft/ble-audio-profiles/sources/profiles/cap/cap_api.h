/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Common Audio Profile Application Interface.
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

#ifndef CAP_API_H
#define CAP_API_H

#include "att_api.h"
#include "svc_asc.h"
#include "bapba_api.h"
#include "bapc_api.h"
#include "baps_api.h"
#include "bapsc_api.h"
#include "bapsd_api.h"
#include "bapsk_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup COMMON_AUDIO_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Max number of coordinated sets in the common audio profile */
#ifndef CAP_SET_MAX
#define CAP_SET_MAX                   CSIPC_DB_MAX_SETS
#endif

/*! Maximum number of audio stream configuration */
#ifndef CAP_STREAM_CFG_MAX
#define CAP_STREAM_CFG_MAX            3
#endif

/*! Create/Update coordinated set error codes */
#define CAP_PROC_IN_PROGRESS          0xFC    /*!< A CAP procedure is in progress */
#define CAP_NO_SET_MEMBER             0xFD    /*!< The CAP coordinated set has no member */
#define CAP_INVALID_SET               0xFE    /*!< The CAP coordinated set is invalid */
#define CAP_INVALID_NUM_STREAM        0xFF    /*!< Invalid number of audio streams */

/*!< CAP Roles */
#define CAP_ROLE_ACCEPTOR             (1<<0)  /*!< CAP Acceptor role */
#define CAP_ROLE_INITIATOR            (1<<1)  /*!< CAP Initiator role */
#define CAP_ROLE_COMMANDER            (1<<2)  /*!< CAP Commander role */

/*!< CAP BAP Roles */
#define CAP_BAP_ROLE_CLIENT           (1<<0)  /*!< BAP Unicast Client role */
#define CAP_BAP_ROLE_SERVER           (1<<1)  /*!< BAP Unicast Server role */
#define CAP_BAP_ROLE_SOURCE           (1<<2)  /*!< BAP Broadcast Source role */
#define CAP_BAP_ROLE_SINK             (1<<3)  /*!< BAP Broadcast Sink role */
#define CAP_BAP_ROLE_BA               (1<<4)  /*!< BAP Broadcast Assistant role */
#define CAP_BAP_ROLE_SD               (1<<5)  /*!< BAP Scan Delegator role */

/*! CAP Procedures */
#define CAP_PROC_NONE                 0x00    /*!< CAP procedure none */

/*! CAP Initiator Procedures */
#define CAP_PROC_UCAST_AUDIO_START    0x01    /*!< CAP unicast audio start procedure */
#define CAP_PROC_UCAST_AUDIO_UPDATE   0x02    /*!< CAP unicast audio update procedure */
#define CAP_PROC_UCAST_AUDIO_STOP_DIS 0x03    /*!< CAP unicast audio stop (disable) procedure */
#define CAP_PROC_UCAST_AUDIO_STOP_REL 0x04    /*!< CAP unicast audio stop (release) procedure */
#define CAP_PROC_BCAST_AUDIO_START    0x05    /*!< CAP broadcast audio start procedure */
#define CAP_PROC_BCAST_AUDIO_UPDATE   0x06    /*!< CAP broadcast audio update procedure */
#define CAP_PROC_BCAST_AUDIO_STOP_DIS 0x07    /*!< CAP broadcast audio stop (disable) procedure */
#define CAP_PROC_BCAST_AUDIO_STOP_REL 0x08    /*!< CAP broadcast audio stop (release) procedure */

/*! CAP Commander Procedures */
#define CAP_PROC_BCAST_AUDIO_RX_START 0x10    /*!< CAP broadcast audio reception start procedure */
#define CAP_PROC_BCAST_AUDIO_RX_STOP  0x11    /*!< CAP broadcast audio reception stop procedure */
#define CAP_PROC_DISTRIB_BCAST_CODE   0x12    /*!< CAP Distribute Broadcast_Code procedure */
#define CAP_PROC_SET_ABS_VOL          0x13    /*!< CAP set absolute volume procedure */
#define CAP_PROC_SET_VOL_OFFSET       0x14    /*!< CAP set volume offset procedure */
#define CAP_PROC_SET_VOL_MUTE         0x15    /*!< CAP set volume mute procedure */
#define CAP_PROC_SET_MIC_MUTE         0x16    /*!< CAP set microphone mute procedure */
#define CAP_PROC_SET_MIC_GAIN_SETTING 0x17    /*!< CAP set microphone gain setting procedure */

/*! CAP BAS Control Point PA and BIS Sync operations */
#define CAP_BAS_PA_SYNC_NO_CHANGE     0xFF    /*!< No change to PA_Sync */
#define CAP_BAS_BIS_SYNC_NO_CHANGE    0xFFFFFFFE  /*!< No change to BIS_Sync */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Common Audio Profile state machine event. */
typedef struct
{
  wsfMsgHdr_t           hdr;                  /*!< Message header */
  uint8_t               svcId;                /*!< Service ID */
  uint8_t               capProc;              /*!< CAP procedure */
} capEvt_t;

/*! Common Audio Profile unicast audio start procedure complete event structure. */
typedef struct
{
  wsfMsgHdr_t           hdr;                  /*!< Message header */
  uint8_t               procId;               /*!< CAP procedure identifier */
  uint8_t               cigId;                /*!< CIG identifier */
} capUcastAudioStartCmpl_t;

/*! Common Audio Profile broadcast audio start procedure complete event structure. */
typedef struct
{
  wsfMsgHdr_t           hdr;                  /*!< Message header */
  uint8_t               procId;               /*!< CAP procedure identifier */
  uint8_t               bigHandle;            /*!< BIG handle */
} capBcastAudioStartCmpl_t;

/*! Common Audio Profile sync to PA event structure. */
typedef struct
{
  wsfMsgHdr_t           hdr;                  /*!< Message header */
  uint8_t               procId;               /*!< CAP procedure identifier */
  uint8_t               srcAddrType;          /*!< Source address type */
  bdAddr_t              srcAddr;              /*!< Source address */
} capSyncPa_t;

/*! Common Audio Profile application callback event. */
typedef struct
{
  wsfMsgHdr_t           hdr;                  /*!< Message header */
  uint8_t               procId;               /*!< CAP procedure identifier */
  union
  {
    capUcastAudioStartCmpl_t  ucastStartCmpl; /*!< Unicast audio start procedure complete */
    capBcastAudioStartCmpl_t  bcastStartCmpl; /*!< Broadcast audio start procedure complete */
    capSyncPa_t               syncPa;         /*!< Sync to PA */
  } evt;
} capCbackEvt_t;

/*! Audio configuration for each Acceptor */
typedef struct
{
  uint32_t              location;             /*!< Audio location */
  dmConnId_t            connId;               /*!< DM connection identifier */
  uint8_t               numStream;            /*!< Number of audio streams */
  bapStream_t           stream[ASCS_AUDIO_STREAM_MAX]; /*!< List of audio streams */
} capAcceptCfg_t;

/*! Unicast audio start procedure parameters */
typedef struct
{
  uint8_t               cigId;                /*!< Connected Isochronous Group (CIG) identifier */
  bapCodecId_t          codecId;              /*!< Codec_ID */
  const BapCigParams_t  *pCigParams;          /*!< BAP Client CIG parameters */
  uint32_t              presentDelay;         /*!< QoS presentation delay in usec */
  bool                  defaultToUnspecified; /*!< true to default to using "Unspecified" if Acceptor doesn't support "desiredAudioCtx" */
  capAcceptCfg_t        *pAcceptCfg[DM_CONN_MAX]; /*!< List of Acceptor configuration */
} capUcastAudioStartCfg_t;

/*! Unicast audio update procedure parameters */
typedef struct
{
  uint8_t               cigId;                /*!< Connected Isochronous Group (CIG) identifier */
  bool                  defaultToUnspecified; /*!< true to default to using "Unspecified" if Acceptor doesn't support "desiredAudioCtx" */
  capAcceptCfg_t        *pAcceptCfg[DM_CONN_MAX]; /*!< List of Acceptor configuration */
} capUcastAudioUpdateCfg_t;

typedef struct
{
  uint8_t               cigId;                /*!< Connected Isochronous Group (CIG) identifier */
  capAcceptCfg_t        *pAcceptCfg[DM_CONN_MAX];  /*!< List of Acceptor configuration */
} capUcastAudioStopCfg_t;

/*! Codec configuration for each BIS */
typedef struct
{
  /*! Level 3 (BIS) info */
  uint32_t              location;             /*!< Audio location */
} capBisCodecCfg_t;

/*! Audio configuration for each subgroup */
typedef struct
{
  /*! Level 2 (Subgroup) info */
  bapCodecId_t              codecId;          /*!< Codec_ID */
  const bapGenCodecSpCfg_t  codecCfg;         /*!< Codec specific configuration for the subgroup */
  bapBcastMetadata_t        *pMetadata;       /*!< Metadata for the subgroup */
  uint8_t                   numBis;           /*!< Number of BISes in the subgroup */

  /*! Level 3 (BISes) info */
  capBisCodecCfg_t          bisCodecCfg[BASS_BIS_MAX];  /*!< List of BIS codec configuration */
} capSubgrpCfg_t;

/*! Broadcast audio procedure configuration parameters */
typedef struct
{
  uint8_t               bigHandle;            /*!< Broadcast Isochronous Group (BIG) handle */
  const BapBigParams_t  *pBigParams;          /*!< BAP Broadcast Source BIG parameters */
  uint8_t               bcastCode[HCI_BC_LEN];/*!< Broadcast_Code used by the Broadcast Source */

  /*! Broadcast Audio Announcements */
  uint8_t               extAdvBufLen;         /*!< Length of extended advertising data buffer maintained by Application.  Minimum length is 31 bytes. */
  uint8_t               *pExtAdvData;         /*!< Extended advertising data to associate PA train with broadcast Audio Stream */

  /*! Basic Audio Announcements */
  uint8_t               perAdvBufLen;         /*!< Length of periodic advertising data buffer maintained by Application.  Minimum length is 31 bytes. */
  uint8_t               *pPerAdvData;         /*!< Periodic advertising data to expose broadcast Audio Stream parameters */

  /*! Level 1 (Group) info */
  uint8_t               advHandle;            /*!< Advertising handle */
  uint8_t               advSid;               /*!< Advertising SID */
  uint32_t              presentDelay;         /*!< QoS presentation delay in usec */
  uint8_t               numSubgrp;            /*!< Number of subgroups used to group BISes present in the BIG */

  /*! Level 2 (Subgroups) and Level 3 (BISes) info */
  capSubgrpCfg_t        *pSubgrpCfg[BASS_SUBGRP_MAX];  /*!< List of subgroup configuration */
} capBcastAudioCfg_t;

/*! Broadcast audio update procedure configuration parameters */
typedef struct
{
  /*! Basic Audio Announcements */
  uint8_t               perAdvBufLen;         /*!< Length of periodic advertising data buffer maintained by Application.  Minimum length is 31 bytes. */
  uint8_t               *pPerAdvData;         /*!< Periodic advertising data to expose broadcast Audio Stream parameters */

  /*! Level 1 (Group) info */
  uint8_t               advHandle;            /*!< Advertising handle */
  uint8_t               numSubgrp;            /*!< Number of subgroups used to group BISes present in the BIG */

  /*! Level 2 (Subgroups) and Level 3 (BISes) info; must contain all Subgroups */
  capSubgrpCfg_t        *pSubgrpCfg[BASS_SUBGRP_MAX];  /*!< List of subgroup configuration */
} capBcastAudioUpdateCfg_t;

/*! Broadcast audio stop procedure configuration parameters */
typedef struct
{
  uint8_t               bigHandle;            /*!< Broadcast Isochronous Group (BIG) handle */
  uint8_t               advHandle;            /*!< Advertising handle */
} capBcastAudioStopCfg_t;

/*! \brief CAP BASS Broadcast Audio Scan (BAS) subgroup info */
typedef struct
{
  uint8_t            numBis;                  /*!< Number of BISes in subgroup */
  uint8_t            metadataLen;             /*!< Length of the Metadata parameter value */
  uint8_t            *pMetadata;              /*!< LTV-formatted Metadata for Broadcast Source */
} capBasSubgrp_t;

/*! \brief CAP BASS Source info */
typedef struct
{
  bapBasBcastSrc_t   bcastSrc;                /*!< Broadcast source info */
  uint8_t            numSubgrp;               /*!< Number of subgroups */
  capBasSubgrp_t     subgrp[BAP_MAX_SUBGRP];  /*!< Subgroup info */
} capBasSrcInfo_t;

/*! Broadcast audio reception start procedure configuration parameters */
typedef struct
{
  capBasSrcInfo_t       srcInfo;              /*!< Source info */
  uint32_t              bisSync[DM_CONN_MAX]; /*!< BIS (Broadcast Isochronous Stream) sync for each Acceptor */
} capBcastAudioRxStartCfg_t;

/*! Broadcast audio reception stop procedure configuration parameters */
typedef struct
{
  uint8_t               advAddrType;          /*!< Advertiser address type for Broadcast Source */
  uint8_t               advSid;               /*!< Advertising SID of ADI of AUX_ADV_IND or LL_PERIODIC_SYNC_IND containing
                                                   SyncInfo that points to PA transmitted by Broadcast Source */
  uint32_t              broadcastId;          /*!< Broadcast_ID of the Broadcast Source */
  bool                  dontRemove;           /*!< true if Broadcast Source not be removed */
} capBcastAudioRxStopCfg_t;

/*! Distribute Broadcast_Code procedure configuration parameters */
typedef struct
{
  uint8_t               advAddrType;          /*!< Advertiser address type for Broadcast Source */
  uint8_t               advSid;               /*!< Advertising SID of ADI of AUX_ADV_IND or LL_PERIODIC_SYNC_IND containing
                                                   SyncInfo that points to PA transmitted by Broadcast Source */
  uint32_t              broadcastId;          /*!< Broadcast_ID of the Broadcast Source */
  uint8_t               bcastCode[HCI_BC_LEN];/*!< Broadcast_Code used by the Broadcast Source */
} capDistributeBcastCodeCfg_t;

/* Set volume offset procedure configuration parameters */
typedef struct
{
  uint8_t               vocsId;               /*!< VOCS service identifier */
  int16_t               volOffset;           /*!< Volume offset */
} capVolOffsetCfg_t;

/*! Set absolute volume procedure configuration parameter */
typedef uint8_t capAbsVolCfg_t;

/*! Set mute procedure configuration parameter */
typedef uint8_t capMuteCfg_t;

typedef struct
{
  uint8_t               aicsId;               /*!< AICS service identifier */
  int8_t                gain;                 /*!< Microphone gain setting */
} capGainCfg_t;

/*! CAP coordinated set identifier */
typedef uint8_t capSetId_t;

/*!
 *  \brief CAP callback type.
 *
 *  \param  pEvt            Pointer to callback event structure.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a CAP procedure is
 *          completed by CAP Initiator or CAP Commander, or CAP procedure is requested by CAP Acceptor.
 */
typedef void (*capCback_t)(capCbackEvt_t *pEvt);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Called to register a list of device IDs as a coordinated set in the CAP profile.
 *
 *  \param  pConnIdList     List of connection identifiers.
 *  \param  numConnIds      Number of connection identifiers in pConnIdList.
 *  \param  usesCsis        true if the set uses CSIS Locking.
 *
 *  \return Identifier for the set, CAP_INVALID_SET if no sets available, or CAP_PROC_IN_PROGRESS.
 *
 *  \note   This function cannot be called while a CAP procedure is in progress.
 */
/*************************************************************************************************/
capSetId_t CapCreateCoordinatedSet(dmConnId_t *pConnIdList, uint8_t numConnIds, bool usesCsis);

/*************************************************************************************************/
/*!
 *  \brief  Called to change the members of a coordinated set.
 *
 *  \param  setId           The set identifier.
 *  \param  pConnIdList     List of connection identifiers.
 *  \param  numConnIds      Number of connection identifiers in pConnIdList.
 *  \param  usesCsis        true if the set uses CSIS Locking.
 *
 *  \return true if an error occurred, else false.
 *
 *  \note   This function cannot be called while a CAP procedure is in progress.
 */
/*************************************************************************************************/
bool CapUpdateCoordinatedSet(capSetId_t setId, dmConnId_t *pConnIdList, uint8_t numConnIds,
                             bool usesCsis);

/*************************************************************************************************/
/*!
 *  \brief  Called to remove a coordinated set.
 *
 *  \param  setId           The set identifier.
 *
 *  \return true if an error occurred, else false.
 *
 *  \node   This function cannot be called while a CAP procedure is in progress.
 */
/*************************************************************************************************/
bool CapRemoveCoordinatedSet(capSetId_t setId);

/*************************************************************************************************/
/*!
 *  \brief  Cancels a procedure in process.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapCancelProcedure(void);

/*************************************************************************************************/
/*!
 *  \brief  Determine if all connected Acceptors in the set can support the required capabilities
 *          for the intended broadcast Audio Streams.
 *
 *  \param  setId         Coordinated Set identifier.
 *  \param  numSubgrp     Number of subgroup configuration.
 *  \param  p2pSubgrpCfg  Pointer to list of subgroup configuration.
 *
 *  \return BAP_SUCCESS if audio capabilities supported. Error, otherwise.
 *
 *  \note   This function cannot be called while a CAP procedure is in progress.
 */
/*************************************************************************************************/
uint8_t CapcAudioCapsSupported(capSetId_t setId, uint8_t numSubgrp, capSubgrpCfg_t **p2pSubgrpCfg);

/*************************************************************************************************/
/*!
 *  \brief  Determine if all connected Acceptors in the set can support the required audio locations
 *          for the intended broadcast Audio Streams.
 *
 *  \param  setId         Coordinated Set identifier.
 *  \param  numSubgrp     Number of subgroup configuration.
 *  \param  p2pSubgrpCfg  Pointer to list of subgroup configuration.
 *
 *  \return BAP_SUCCESS if audio locations supported. Error, otherwise.
 *
 *  \note   This function cannot be called while a CAP procedure is in progress.
 */
/*************************************************************************************************/
uint8_t CapcAudioLocsSupported(capSetId_t setId, uint8_t numSubgrp, capSubgrpCfg_t **p2pSubgrpCfg);

/*************************************************************************************************/
/*!
 *  \brief  Determine the BIS_Sync value for a given Acceptor based on its support for the required
 *          audio locations for the intended broadcast Audio Streams.
 *
 *  \param  connId        DM connection identifier.
 *  \param  numSubgrp     Number of subgroup configuration.
 *  \param  p2pSubgrpCfg  Pointer to list of subgroup configuration.
 *
 *  \return BIS_Sync value for the Acceptor.
 */
/*************************************************************************************************/
uint32_t CapcGetBisSync(dmConnId_t connId, uint8_t numSubgrp, capSubgrpCfg_t **p2pSubgrpCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the set VCP absolute volume procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  volume          Volume setting
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcSetVcpAbsoluteVolume(capSetId_t setId, uint8_t volume);

/*************************************************************************************************/
/*!
 *  \brief  Begins the set VCP volume offset procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  vocsId          VOCS service identifier.
 *  \param  volumeOffset    Volume offset.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcSetVcpVolumeOffset(capSetId_t setId, uint8_t vocsId, int16_t volumeOffset);

/*************************************************************************************************/
/*!
 *  \brief  Begins the set VCP mute/unmute procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  mute            true to mute, false to unmute.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcSetVcpMute(capSetId_t setId, bool unMute);

/*************************************************************************************************/
/*!
 *  \brief  Begins the MICP set mute setting procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  mute            MICP mute mode.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcSetMicpMute(capSetId_t setId, uint8_t mute);

/*************************************************************************************************/
/*!
 *  \brief  Begins the MICP set gain setting procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  aicsId          AICS service identifier.
 *  \param  gain            MICP gain setting.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcSetMicpGainSetting(capSetId_t setId, uint8_t aicsId, int8_t gain);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the BAS service handle list for a connection.
 *
 *  \param  connId    List of connection identifiers.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcSetPacHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Begins the broadcast audio reception start procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Broadcast audio reception start configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcBroadcastAudioRxStart(capSetId_t setId, capBcastAudioRxStartCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the broadcast audio reception stop procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Broadcast audio reception start configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcBroadcastAudioRxStop(capSetId_t setId, capBcastAudioRxStopCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the distribute Broadcast_Code procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Distribute Broadcast_Code configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapcDistributeBcastCode(capSetId_t setId, capDistributeBcastCodeCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Match up an Acceptor's availability, capabilities, and location for a given Acceptor
 *          audio configuration.
 *
 *  \param  setId         Coordinated Set identifier.
 *  \param  pAcceptCfg    Acceptor configuration to match.
 *  \param  pCodecId      Codec_ID.
 *  \param  defToUnspec   true to default to using 'Unspecified' if Acceptor doesn't support 'desiredAudioCtx'.
 *  \param  pConnId       Connection matching Acceptor configuration (to be returned).
 *  \param  pSnkCtxTypes  Acceptor's sink Context Type values (to be returned).
 *  \param  pSrcCtxTypes  Acceptor's source Context Type values (to be returned).
 *  \param  pErrCtxTypes  Unsupported or unavailable Context Type values in case of error (to be returned).
 *
 *  \return BAP_SUCCESS if match found. Error, otherwise.
 *
 *  \note   This function cannot be called while a CAP procedure is in progress.
 */
/*************************************************************************************************/
uint8_t CapiMatchAcceptCfg(capSetId_t setId, capAcceptCfg_t *pAcceptCfg, bapCodecId_t *pCodecId,
                           bool defToUnspec, dmConnId_t *pConnId, uint16_t *pSnkCtxTypes,
                           uint16_t *pSrcCtxTypes, uint16_t *pErrCtxTypes);

/*************************************************************************************************/
/*!
 *  \brief  Begins the unicast audio start procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Unicast audio start configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiUnicastAudioStart(capSetId_t setId, capUcastAudioStartCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the unicast audio update procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Unicast audio update configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiUnicastAudioUpdate(capSetId_t setId, capUcastAudioUpdateCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the unicast audio stop (disable) procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Unicast audio configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiUnicastAudioStopDisable(capSetId_t setId, capUcastAudioStopCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the unicast audio stop (release) procedure.
 *
 *  \param  setId           Coordinated Set identifier.
 *  \param  pCfg            Unicast audio configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiUnicastAudioStopRelease(capSetId_t setId, capUcastAudioStopCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the broadcast audio start procedure.
 *
 *  \param  pCfg            Broadcast audio configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiBroadcastAudioStart(capBcastAudioCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the broadcast audio update procedure. This procedure is used to update the Context
 *          Type values and/or CCID values associated with one or multiple broadcast Audio Streams.
 *
 *  \param  pCfg            Broadcast audio configuration.
 *
 *  \return None.
 *
 *  \note   This procedure shall only be used if the BAP Audio Configuration remains unchanged in
 *          the new use case. Otherwise, the Broadcast Audio Stop procedure and the Broadcast Audio
 *          Start procedure shall be used.
 */
/*************************************************************************************************/
void CapiBroadcastAudioUpdate(capBcastAudioUpdateCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the broadcast audio stop (disable) procedure.
 *
 *  \param  pCfg            Broadcast audio configuration.
 *
 *  \return None.
 *
 *  \Note   This procedure should be used if the app intends to restart the broadcast Audio Stream.
 */
 /*************************************************************************************************/
void CapiBcastAudioStopDisable(capBcastAudioStopCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Begins the broadcast audio stop (release) procedure.
 *
 *  \param  pCfg            Broadcast audio configuration.
 *
 *  \return None.
 *
 *  \Note   This procedure should be used if the app does not intend to restart the broadcast Audio
 *          Stream.
 */
 /*************************************************************************************************/
void CapiBcastAudioStopRelease(capBcastAudioStopCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Called by the CAP Client application to notify the profile of System events.
 *
 *  \param  pEvt   Pointer to the event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapClientProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called by the CAP Client and Server applications to notify the profile of System events.
 *
 *  \param  pEvt   Pointer to the event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Register a callback with CAP.
 *
 *  \param  cback         Application's CAP callback function.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void CapRegister(capCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Initiator profile.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapiInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Commander profile.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapcInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Acceptor profile.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapaInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Initiator Unicast Client (ICU) subsystem.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapiucInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Initiator Broadcast Source (IBS) subsystem.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapibsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Acceptor Unicast Server (AUS) subsystem.
 *
 *  \param  aseCback  Server application ASE callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapausInit(BapsAseCback_t aseCback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Acceptor Broadcast Sink (ABK) subsystem.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapabkInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Acceptor Scan Delegator (ASD) subsystem.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapasdInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Commander Broadcast Assistant (CBA) subsystem.
 *
 *  \param  brsCback  Client application BRS callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void CapcbaInit(BapbaBrsCback_t brsCback);

/*************************************************************************************************/
/*!
 *  \brief  Called to register application's BAP Client ASE update callback
 *          function with CAP Initiator.
 *
 *  \param  cback     Application's BAP Client ASE callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void CapiBapcAseCbackRegister(BapcAseCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Called to register application's BAP Sink and Scan Delegator (SD) BAS update callback
 *          functions with CAP Acceptor.
 *
 *  \param  skCback   Application's BAP Sink BAS callback.
 *  \param  sdCback   Application's BAP SD BAS callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void CapaBapBasCbackRegister(BapskBasCback_t skCback, BapsdBasCback_t sdCback);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the PAC service handle list for a connection.
 *
 *  \param  connId    List of connection identifiers.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiSetPacHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the ASC service handle list for a connection.
 *
 *  \param  connId    List of connection identifiers.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapiSetAscHandles(dmConnId_t connId, uint16_t *pHdlList);


/*************************************************************************************************/
/*!
 *  \brief  Set the Metadata that the application is looking for in a subgroup in addition to the
 *          Streaming Audio Contexts.
 *
 *  \param  len          Length of the Metadata.
 *  \param  pMetadata    Pointer to the Metadata specified by the app.
 *
 *  \return None.
 *
 *  \note   The caller is responsible for maintaining the memory that contains the Metadata.
 */
/*************************************************************************************************/
void CapaSetMetadataOfInterest(uint8_t len, uint8_t *pMetadata);

/*************************************************************************************************/
/*!
 *  \brief  Synchronize with PA to receive the Basic Broadcast Announcements.
 *
 *  \param  connId      DM connection ID.
 *  \param  advSid      Advertising SID.
 *  \param  advAddrType Advertiser address type.
 *  \param  advAddr     Advertiser address.
 *  \param  broadcastId Broadcast_ID of the Broadcast Source.
 *
 *  \return Sync identifier.
 */
 /*************************************************************************************************/
dmSyncId_t CapaDoBasicAudioAnncSync(dmConnId_t connId, uint8_t advSid, uint8_t addrType, bdAddr_t addr,
                                    uint32_t broadcastId);

/*************************************************************************************************/
/*!
 *  \brief  Toggle the BIGInfo advertising report reception filtering.
 *
 *  \param  syncId       Sync identifier (if known).
 *
 *  \return None.
 */
/*************************************************************************************************/
void CapaToggleBigInfoRx(dmSyncId_t syncId);

/*************************************************************************************************/
/*!
 *  \brief  Whether an extended, periodic or BIGInfo advertising report should be filtered.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return true if report should be filtered. false, otherwise.
 */
/*************************************************************************************************/
bool CapaReportFiltered(dmEvt_t *pDmEvt);

/*! \} */    /* COMMON_AUDIO_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* CAP_API_H */
