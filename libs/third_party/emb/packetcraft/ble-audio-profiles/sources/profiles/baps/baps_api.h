/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Unicast Server.
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
#ifndef BAPS_API_H
#define BAPS_API_H

#include "att_api.h"
#include "bap_defs.h"
#include "svc_pac.h"
#include "svc_asc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief BAP Server ASE ID base (cannot be 0) */
#define BAPS_ASE_ID_BASE    0x01

/*! \brief BAP Server ASE IDs */
#define BAPS_SNK_ASE_1_ID   BAPS_ASE_ID_BASE
#define BAPS_SNK_ASE_2_ID   (BAPS_ASE_ID_BASE + 1)
#define BAPS_SRC_ASE_1_ID   (BAPS_ASE_ID_BASE + 2)
#define BAPS_SRC_ASE_2_ID   (BAPS_ASE_ID_BASE + 3)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief BAP Server codec configuration */
typedef struct
{
  uint8_t           aseId;                /*!< ASE ID */
  uint8_t           direction;            /*!< Direction of this ASE with respect to the server */
  uint8_t           targetLatency;        /*!< Target latency */
  uint8_t           targetPhy;            /*!< PHY parameter target to achieve the target latency value */
  bapCodecId_t      codecId;              /*!< Codec ID */
  uint8_t           codecCfgLen;          /*!< Length of the codec specific configuration */
  uint8_t           *pCodecCfg;           /*!< LTV-formatted codec specific Configuration */
} bapsCodecCfg_t;

/*! \brief BAP Server metadata configuration */
typedef struct
{
  uint8_t           aseId;                /*!< \brief ASE ID */
  uint8_t           metadataLen;          /*!< \brief Length of the Metadata field for this ASE */
  uint8_t           metadata[GA_MAX_METADATA_LEN];  /*!< \brief LTV-formatted Metadata for this ASE */
} bapsMetadataCfg_t;

/*! \brief BAP Server ASE control block */
typedef struct
{
  uint8_t           aseId;                /*!< \brief ASE ID */
  uint8_t           aseState;             /*!< \brief ASE state */
  uint8_t           direction;            /*!< \brief Direction of this ASE with respect to the server */
  bapCodecCfg_t     codecCfg;             /*!< \brief Codec configuration */
  bapQosCfg_t       qosCfg;               /*!< \brief QoS configuration */
  bapsMetadataCfg_t metadataCfg;          /*!< \brief Metadata configuration */
  bapQosSpec_t      *pQosSpec;            /*!< \brief Pointer to server's preferred QoS */
  uint16_t          cisHandle;            /*!< \brief CIS handle associated with this ASE */
} bapsAseCb_t;

/*! \brief BAP Server connection control block */
typedef struct
{
  uint16_t          handle;               /*!< Connection handle */
  bapsAseCb_t       aseCb[ASCS_AUDIO_STREAM_MAX]; /*!< ASE control block */
} bapsCcb_t;

/*! CIS control block structure type */
typedef struct
{
  uint8_t           cigId;                /*!< CIG that CIS belongs to */
  uint8_t           cisId;                /*!< CIS identifier */
  uint16_t          cisHandle;            /*!< CIS handle. */
  uint16_t          aclHandle;            /*!< ACL handle associated with this CIS */
  bool              cisEst;               /*!< true if CIS connection established */
  bapsAseCb_t       *pAcb[DM_ISO_NUM_DIR];      /*!< Pointer to ASE control block for this direction */
  bool              dpCreated[DM_ISO_NUM_DIR];  /*!< true if data path created for this direction */
  uint8_t           directionBits;        /*!< Directions being setup or removed */
  bool              vsDpConfig;           /*!< true if vendor-specific data path configured */
  bool              inUse;                /*!< true if entry in use */
} bapsCisCb_t;

/*! \brief BAP Server ASE Control Point (CP) structure */
typedef struct
{
  uint8_t           opcode;               /*!< \brief Opcode of client-initiated ASE Control operation causing this response */
  uint8_t           numAse;               /*!< \brief Total number of ASEs server is providing response for */
  bapAse_t          ase[ASCS_AUDIO_STREAM_MAX]; /*!< \brief List of ASEs */
} bapsAseCp_t;

/*!
 *  \brief BAP Server ASE update callback type.
 *
 *  \param  connId       DM connection identifier.
 *  \param  pAcb         Pointer to ASE control block.
 *  \param  aseOldState  ASE old state
 *
 *  \return None.
 *
 *  \note   This callback function calls the server application's callback when BAP Server ASE
 *          update is received.
 */
typedef void (*BapsAseCback_t)(dmConnId_t connId, bapsAseCb_t *pAcb, uint8_t aseOldState);

/*!
 *  \brief  Check if the requested vendor-specific codec configuration matches the server's
 *          codec specific capabilities.
 *
 *  \param  codecCapLen Length of server's codec specific capabilities.
 *  \param  pCodecCap   Pointer to server's codec specific capabilities.
 *  \param  pCodec      Pointer to requested codec specific configuration.
 *  \param  pReason     Reject reason (to be updated).
 *
 *  \return true if codec specific configurations match. false, otherwise.
 *
 *  \note   This callback function calls the server application's callback when a vendor-specific
 *          codec configuration is received on the server.
 */
/*************************************************************************************************/
typedef uint8_t (*BapsVsCodecCfgMatchCback_t)(uint8_t codecCapLen, uint8_t *pCodecCap, const bapsCodecCfg_t *pCodec,
                                              uint8_t *pReason);

/*! \brief BAP Server main control block */
typedef struct
{
  wsfQueue_t                  pacQueue;             /*!< CIS Queue of PAC records */
  bapsCcb_t                   connCb[DM_CONN_MAX];  /*!< Connection control block */
  bapsCisCb_t                 cisCb[DM_CIS_MAX];    /*!< CIS control block */
  bapsAseCp_t                 aseCp;                /*!< CIS ASE Control Point */
  uint8_t                     dpId;                 /*!< Data path id */
  uint8_t                     configLen;            /*!< Length of vendor-specific configuration data */
  uint8_t                     *pConfig;             /*!< Vendor-specific configuration data for data path being configured */
} bapsCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern bapsCb_t bapsCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  BAPS Function Declarations
**************************************************************************************************/

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
void BapsSetVsDataPathConfig(uint8_t dpId, uint8_t configLen, uint8_t *pConfig);

/*************************************************************************************************/
/*!
 *  \brief  BAP Server initialization.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Add a PAC record of the server when acting as Audio Sink or Audio Source.
 *
 *  \param  pPac        Pointer to a PAC structure.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsAddPac(bapsPac_t *pPac);

/*************************************************************************************************/
/*!
 *  \brief  Remove a PAC record from the BAP server.
 *
 *  \param  pPac        Pointer to a PAC structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsRemovePac(bapsPac_t *pPac);

/*************************************************************************************************/
/*!
 *  \brief  Set the supported Audio Locations of the server when acting as Audio Sink or Audio
 *          Source.
 *
 *  \param  direction     Audio direction.
 *  \param  locationBits  Audio location value bits.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsSetAudioLoc(uint8_t direction, uint32_t locationBits);

/*************************************************************************************************/
/*!
 *  \brief  Get the supported Audio Locations of the server when acting as Audio Sink or Audio
 *          Source.
 *
 *
 *  \return Audio location value bits.
 */
/*************************************************************************************************/
uint32_t BapsGetAudioLoc(uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Set the available audio contexts for reception or transmission.
 *
 *  \param  direction      Audio direction.
 *  \param  availCtxBits   Available audio contexts bitfield.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsSetAvailAudioCtx(uint8_t direction, uint16_t availCtxBits);

/*************************************************************************************************/
/*!
 *  \brief  Get the available audio contexts for reception or transmission.
 *
 *  \param  direction      Audio direction.
 *
 *  \return Available audio contexts bitfield.
 */
/*************************************************************************************************/
uint16_t BapsGetAvailAudioCtx(uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Send a notification for the available audio contexts for reception and transmission.
 *
 *  \param  connId     Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsNotifyAvailAudioCtx(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Set the supported audio contexts for reception or transmission.
 *
 *  \param  direction      Audio direction.
 *  \param  supCtxBits     Supported audio contexts bitfield.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsSetSupAudioCtx(uint8_t direction, uint16_t supCtxBits);

/*************************************************************************************************/
/*!
 *  \brief  Send a notification for the available audio contexts for reception and transmission.
 *
 *  \param  connId     Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsNotifySupAudioCtx(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Send an notification for the sink or source PAC record.
 *
 *  \param  connId     Connection identifier.
 *  \param  handle     ATT handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsNotifyPac(dmConnId_t connId, uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Send a notification for the sink or source audio location.
 *
 *  \param  connId     Connection identifier.
 *  \param  direction  Audio direction.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsNotifyAudioLoc(dmConnId_t connId, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Look up a CIS control block by ID.
 *
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return Pointer to CIS control block if found. NULL, otherwise.
 */
/*************************************************************************************************/
bapsCisCb_t *BapsCisCbById(uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Look up a CIS control block by handle.
 *
 *  \param  handle     CIS handle
 *
 *  \return Pointer to CIS control block if found. NULL, otherwise.
 */
/*************************************************************************************************/;
bapsCisCb_t *BapsCisCbByHandle(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Get the list of CIS handles in a given CIG ID.
 *
 *  \param  cigId       CIG identifier.
 *  \param  pCisHandle  List of CIS handles to be returned. Must be at least size of DM_CIS_MAX.
 *
 *  \return Number of CISes if CIG found. 0, otherwise.
 */
/*************************************************************************************************/
uint8_t BapsGetCisHandle(uint8_t cigId, uint16_t *pCisHandle);

/*************************************************************************************************/
/*!
 *  \brief  Setup an output (Controller to Host) data path on the BAP Client for the given CIS handle.
 *
 *  \param  cisHandle  CIS handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsSetupOutputDataPath(uint16_t cisHandle);

/*************************************************************************************************/
/*!
 *  \brief  Setup an input (Host to Controller) data path on the BAP Client for the given CIS.
 *
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsSetupInputDataPath(uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Remove input and output data path on the BAP Client for the given ASE.
 *
 *  \param  connId    DM connection ID.
 *  \param  pAcb      Pointer to ASE control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsRemoveDataPath(dmConnId_t connId, bapsAseCb_t *pAcb);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP Unicast server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP server of ATT messages.
 *
 *  \param  pMsg    message containing event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsProcAttMsg(wsfMsgHdr_t *pMsg);

/**************************************************************************************************
  BAPS ASE Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  BAP Server PAC initialization.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsPacInit(void);


/*************************************************************************************************/
/*!
 *  \brief  Called to register an application ASE update callback function.
 *
 *  \param  cback     Application ASE callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void BapsAscAseCbackRegister(BapsAseCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application vendor-specific codec configuration match callback
 *          function.
 *
 *  \param  cback     Application match VS codec configuration callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void BapsAseVsCodecCfgMatchCbackRegister(BapsVsCodecCfgMatchCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for PAC service.
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
uint8_t BapsPacReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                         attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for PAC service.
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
uint8_t BapsPacWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                          uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  BAP Server ASC initialization.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapsAscInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the BAP Server's priority used with EATT operations.
 *
 *  \param  priority  EATT priority.
 *
 *  \return none
 */
/*************************************************************************************************/
void BapsAscSetEattPriority(uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  ATTS read callback for ASE service.
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
uint8_t BapsAscReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                         attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for ASC service.
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
uint8_t BapsAscWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset,
                          uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Look up a connection control block by ID.
 *
 *  \param  connId    DM Connection ID.
 *
 *  \return Pointer to connection control block.
 */
/*************************************************************************************************/
bapsCcb_t *BapsConnCbById(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Look up a connection control block by handle.
 *
 *  \param  handle    Connection handle.
 *
 *  \return Pointer to connection control block. NULL, if not found.
 */
/*************************************************************************************************/
bapsCcb_t *BapsConnCbByHandle(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Look up an ASE control block for a by connection ID.
 *
 *  \param  connId    DM Connection ID.
 *
 *  \return Pointer to ASE control block.
 */
/*************************************************************************************************/
bapsAseCb_t *BapsAseCbByCid(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Look up an ASE control block for a given connection ID by ASE ID.
 *
 *  \param  connId  DM Connection ID.
 *  \param  aseId   ASE ID.
 *
 *  \return Pointer to control block.
 */
 /*************************************************************************************************/
bapsAseCb_t *BapsAseCbByAseId(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Look up an ASE control block for a connection by CIS identifier.
 *
 *  \param  connId    DM Connection ID.
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return Pointer to control block.
 */
/*************************************************************************************************/
bapsAseCb_t *BapsAseCbByCisId(dmConnId_t connId, uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Determine if another ASE with the same triplet of Direction, CIG_ID, and CIS_ID
 *          parameter values exists for a given client.
 *
 *  \param  connId    DM Connection ID.
 *  \param  aseId     ASE ID.
 *  \param  direction Direction.
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return true if another ASE found. false, otherwise.
 */
/*************************************************************************************************/
bool BapsAseConfigExists(dmConnId_t connId, uint8_t aseId, uint8_t direction, uint8_t cigId,
                         uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Look up an ASE control block for a connection by ASE State.
 *
 *  \param  connId    DM Connection ID.
 *  \param  aseState  ASE State.
 *
 *  \return Pointer to control block.
 */
 /*************************************************************************************************/
bapsAseCb_t *BapsAseCbByAseState(dmConnId_t connId, uint8_t aseState);

/*************************************************************************************************/
/*!
 *  \brief  Allocate an ASE control block.
 *
 *  \param  connId  DM Connection ID.
 *
 *  \return Pointer to control block or NULL if failure.
 */
 /*************************************************************************************************/
bapsAseCb_t *BapsAseCbAlloc(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Receiver Start Ready operation for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  pAcb       Pointer to ASE control block.
 *
 *  \return None.
 *
 *  \note   The Receiver Start Ready operation may be initiated autonomously by the Server if:
 *          - A CIS has been established and the Server is acting as Audio Sink for the ASE, and
 *          - The Server is ready to receive audio data transmitted by the Client.
 */
/*************************************************************************************************/
void BapsAseInitRxStartReadyOp(dmConnId_t connId, bapsAseCb_t *pAcb);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Receiver Stop Ready operation for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  pAcb       Pointer to ASE control block.
 *
 *  \return None.
 *
 *  \note   The Receiver Stop Ready operation may be initiated autonomously by the Server if:
 *          - The Server is acting as Audio Sink for the ASE, and
 *          - The Server is ready to stop consuming audio data transmitted by the Client.
 */
/*************************************************************************************************/
void BapsAseInitRxStopReadyOp(dmConnId_t connId, bapsAseCb_t *pAcb);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Disable operation for a given ASE ID.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE ID.
 *
 *  \return None.
*/
/*************************************************************************************************/
void BapsAseInitDisableOp(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Release operation for a given ASE ID.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE ID.
 *
 *  \return None.
 *
 *  \note   If the Server autonomously initiates the Release operation for an ASE, the Server:
 *          - Shall terminate CIS established by the Client for that ASE, and
 *          - Should remove any internal audio data paths that have been set up for the ASE
 *            that is in the Releasing state before terminating CIS.
 */
/*************************************************************************************************/
void BapsAseInitReleaseOp(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Released operation for an ASE in the Releasing state.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE Id.
 *  \param  caching    true if caching. false, otherwise.
 *
 *  \return None.
 *
 *  \note   The Released operation is initiated autonomously by the Server if:
 *          - The Release operation for an ASE has been completed and underlying CIS for the ASE
 *            has been torn down.
 *
 *  \note   Transitions ASE from Releasing state to Idle state or Codec Configured state depending
 *          on Server's preference for caching a codec configuration.
 */
/*************************************************************************************************/
void BapsAseInitReleasedOp(dmConnId_t connId, uint8_t aseId, bool caching);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Update Metadata operation for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE ID.
 *  \param  pMetadata  List of Metadata parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsAseInitUpdateMetadataOp(dmConnId_t connId, uint8_t aseId, bapMetadataParams_t *pMetadata);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Config Codec operation for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE ID.
 *  \param  pCodecCfg  Pointer to Codec Configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsAseInitConfigCodecOp(dmConnId_t connId, uint8_t aseId, bapsCodecCfg_t *pCodecCfg);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Update Metadata operation for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE ID.
 *  \param  pMetadata  List of Metadata parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsAseInitUpdateMetadataOp(dmConnId_t connId, uint8_t aseId, bapMetadataParams_t *pMetadata);

/*************************************************************************************************/
/*!
 *  \brief  Autonomously initiate the Config Codec operation for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  aseId      ASE ID.
 *  \param  pCodecCfg  Pointer to Codec Configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsAseInitConfigCodecOp(dmConnId_t connId, uint8_t aseId, bapsCodecCfg_t *pCodecCfg);

/*************************************************************************************************/
/*!
 *  \brief  Process link loss of a CIS for an ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  pCisCb     Pointer to CIS control block.
 *
 *  \return None.
 *
 *  \note   Transitions ASE from Releasing state to Idle state or Codec Configured state depending
 *          on Server's preference for caching a codec configuration.
 */
 /*************************************************************************************************/
void BapsAseProcCisLoss(dmConnId_t connId, bapsCisCb_t *pCisCb, bool caching);

/*************************************************************************************************/
/*!
 *  \brief   Process link loss of an LE-ACL for an ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  caching    true if caching. false, otherwise.
 *
 *  \return None.
 *
 *  \note   Transitions ASE from Releasing state to Idle state or Codec Configured state depending
 *          on Server's preference for caching a codec configuration.
 */
/*************************************************************************************************/
void BapsAseProcAclLoss(dmConnId_t connId, bool caching);

/*************************************************************************************************/
/*!
 *  \brief  Send out an ASE value notification for a given ASE.
 *
 *  \param  connId     DM connection ID.
 *  \param  handle     ASE attribute handle.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapsAscNotifyAse(dmConnId_t connId, uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  For testing purposes only.  Set Codec ID and Codec Configuration on the server.
 *
 *  \param  connId      DM connection ID.
 *  \param  handle      ASE attribute handle.
 *  \param  pCodecId    Pointer to Codec ID.
 *  \param  pCodecCfg   Pointer to Codec Configuration.
 *  \param  codecCfgLen Length of Codec Configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapsAscSetAseCodec(dmConnId_t connId, uint16_t handle, bapCodecId_t *pCodecId, uint8_t *pCodecCfg,
                        uint8_t codecCfgLen);

/*! \} */    /* BASIC_AUDIO_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* BAPS_API_H */
