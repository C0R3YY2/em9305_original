/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Basic Audio Profile (BAP) Client.
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
#ifndef BAPC_API_H
#define BAPC_API_H

#include "att_api.h"
#include "bap_defs.h"
#include "svc_pac.h"
#include "svc_asc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BASIC_AUDIO_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name ASE Messages Length Fields
* Length constants of ASE messages fixed length fields
*/
/**@{*/
#define BAPC_AUDIO_CTX_LEN          4     /*!< Audio contexts message length */
#define BAPC_ASE_LEN                2     /*!< ASE message length */
#define BAPC_ASE_CP_LEN             1     /*!< ASE Control Point message length */
/**@}*/

/*! \brief Base value for CIG IDs */
#define BAPC_CIG_ID_BASE            0x00  /*!< CIG ID value starting base */

/*! \brief Base value for CIS IDs */
#define BAPC_CIS_ID_BASE            0x00  /*!< CIS ID value starting base */

/*! \brief Directions for CIS */
#define BAPC_CIS_DIR_MTS            0x00  /*!< Master to Slave CIS direction */
#define BAPC_CIS_DIR_STM            0x01  /*!< Slave to Master CIS direction */
#define BPAC_CIS_NUM_DIR            0x02  /*!< Number of CIS directions */

/*! \brief Enumeration of handle indexes of ASC characteristics to be discovered */
enum
{
  BAPC_ASC_SNK_ASE_1_HDL_IDX,             /*!< \brief Sink ASE #1 */
  BAPC_ASC_SNK_ASE_CCC_1_HDL_IDX,         /*!< \brief Sink ASE #1 client characteristic configuration */
  BAPC_ASC_SNK_ASE_2_HDL_IDX,             /*!< \brief Sink ASE #2 */
  BAPC_ASC_SNK_ASE_CCC_2_HDL_IDX,         /*!< \brief Sink ASE #2 client characteristic configuration */
  BAPC_ASC_SRC_ASE_1_HDL_IDX,             /*!< \brief Source ASE #1 */
  BAPC_ASC_SRC_ASE_CCC_1_HDL_IDX,         /*!< \brief Source ASE #1 client characteristic configuration */
  BAPC_ASC_SRC_ASE_2_HDL_IDX,             /*!< \brief Source ASE #2 */
  BAPC_ASC_SRC_ASE_CCC_2_HDL_IDX,         /*!< \brief Source ASE #2 client characteristic configuration */
  BAPC_ASC_ASE_CP_HDL_IDX,                /*!< \brief ASE Control Point */
  BAPC_ASC_ASE_CP_CCC_HDL_IDX,            /*!< \brief ASE Control Point client characteristic configuration */

  BAPC_ASC_HDL_LIST_LEN                   /*!< \brief Handle list length */
};

/*! \brief Enumeration of handle indexes of PAC characteristics to be discovered */
enum
{
  BAPC_PAC_SNK_PAC_HDL_IDX,               /*!< Sink PAC */
  BAPC_PAC_SNK_PAC_CCC_HDL_IDX,           /*!< Sink PAC client characteristic configuration */
  BAPC_PAC_SNK_AUD_LOC_HDL_IDX,           /*!< Sink Audio Locations */
  BAPC_PAC_SNK_AUD_LOC_CCC_HDL_IDX,       /*!< Sink Audio Locations client characteristic configuration */
  BAPC_PAC_SRC_PAC_HDL_IDX,               /*!< Source PAC */
  BAPC_PAC_SRC_PAC_CCC_HDL_IDX,           /*!< Source PAC client characteristic configuration */
  BAPC_PAC_SRC_AUD_LOC_HDL_IDX,           /*!< Source Audio Locations */
  BAPC_PAC_SRC_AUD_LOC_CCC_HDL_IDX,       /*!< Source Audio Locations client characteristic configuration */
  BAPC_PAC_AV_AUD_CTX_HDL_IDX,            /*!< Available Audio Contexts */
  BAPC_PAC_AV_AUD_CTX_CCC_HDL_IDX,        /*!< Available Audio Contexts client characteristic configuration */
  BAPC_PAC_SUP_AUD_CTX_HDL_IDX,           /*!< Supported Audio Contexts */
  BAPC_PAC_SUP_AUD_CTX_CCC_HDL_IDX,       /*!< Supported Audio Contexts client characteristic configuration */

  BAPC_PAC_HDL_LIST_LEN                   /*!< Handle list length */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief BAP Client metadata configuration */
typedef struct
{
  uint8_t           aseId;                /*!< \brief ASE ID */
  uint8_t           cisId;                /*!< \brief CIG_ID parameter value for this ASE (assigned by the client) */
  uint8_t           cigId;                /*!< \brief CIS_ID parameter value for this ASE (assigned by the client) */
  uint8_t           metadataLen;          /*!< \brief Length of the Metadata field for this ASE */
  uint8_t           metadata[GA_MAX_METADATA_LEN];  /*!< \brief LTV-formatted Metadata for this ASE */
} bapcMetadataCfg_t;

/*! \brief BAP Server ASE control block */
typedef struct
{
  uint8_t           aseId;                /*!< \brief ASE ID */
  uint8_t           aseState;             /*!< \brief ASE state */
  uint8_t           direction;            /*!< \brief Direction of this ASE with respect to the server */
  bapCodecCfg_t     codecCfg;             /*!< \brief Configured codec parameters */
  bapQosSpec_t      qosSpec;              /*!< \brief Server's preferred QoS configuration */
  bapQosCfg_t       qosCfg;               /*!< \brief Configured QoS parameters */
  bapcMetadataCfg_t metadataCfg;          /*!< \brief Metadata configuration */
  uint16_t          handle;               /*!< \brief Attribute handle */
  uint16_t          cisHandle;            /*!< \brief CIS handle associated with this ASE */
} bapcAseCb_t;

/*! \brief PAB Client PAC record structure */
typedef struct
{
  uint16_t          handle;               /*!< \brief Attribute handle */
  uint8_t           direction;            /*!< \brief Direction of audio with respect to server */
  bapCodecId_t      codecId;              /*!< \brief Codec ID */
  uint8_t           codecCapLen;          /*!< \brief Length of codec specific capabilities field */
  uint8_t           codecCap[GA_MAX_CODEC_LEN];     /*!< \brief Codec specific capabilities */
  uint8_t           metadataLen;          /*!< \brief Length of the metadata field */
  uint8_t           metadata[GA_MAX_METADATA_LEN];  /*!< \brief LTV-formatted metadata */
} bapcPacRec_t;

/*! \brief BAP Client connection control block */
typedef struct
{
  bapcPacRec_t      pacRec[PACS_PAC_MAX]; /*!< \brief List of PAC records discovered */
  uint32_t          snkAudioLocs;         /*!< \brief Sink audio locations */
  uint32_t          srcAudioLocs;         /*!< \brief Source audio locations */
  bapcAseCb_t       aseCb[ASCS_AUDIO_STREAM_MAX]; /*!< \brief ASE control block */
  uint16_t          snkAvAudCtx;          /*!< \brief Bitmask of available audio contexts for reception */
  uint16_t          srcAvAudCtx;          /*!< \brief Bitmask of available audio contexts for transmission */
  uint16_t          snkSupAudCtx;         /*!< \brief Bitmask of supported audio contexts for reception */
  uint16_t          srcSupAudCtx;         /*!< \brief Bitmask of supported audio contexts for transmission */
  uint16_t          *pAscHdlList;         /*!< \brief List of ASC attribute handles on server */
  uint16_t          *pPacHdlList;         /*!< \brief List of PAC attribute handles on server */
} bapcCcb_t;

/*! CIG control block structure type */
typedef struct
{
  uint8_t           cigId;                /*!< CIG identifier */
  uint8_t           numCis;               /*!< Number of CISes in CIG */
  uint8_t           cisId[DM_CIS_MAX];    /*!< List of CIS IDs in CIG */
  uint8_t           numCisCfg;            /*!< Total number of CISes in the CIG being added or modified. */
  uint8_t           cisIdCfg[DM_CIS_MAX]; /*!< List of CIS IDs being added or modified. */
  bool              inUse;                /*!< true if entry in use */
  bool              remove;               /*!< true if CIG to be removed after last CIS closes */
} bapcCigCb_t;

/*! CIS control block structure type */
typedef struct
{
  uint8_t           cigId;                /*!< CIG that CIS belongs to */
  uint8_t           cisId;                /*!< CIS identifier */
  uint16_t          cisHandle;            /*!< CIS handle. */
  dmConnId_t        connId;               /*!< ACL connection associated with this CIS */
  bool              cisEst;               /*!< true if CIS connection established */
  bapcAseCb_t       *pAcb[DM_ISO_NUM_DIR];      /*!< Pointer to ASE control block for this direction */
  bool              dpCreated[DM_ISO_NUM_DIR];  /*!< true if data path created for this direction */
  bool              vsDpConfig;           /*!< true if vendor-specific data path configured */
  bool              inUse;                /*!< true if entry in use */
} bapcCisCb_t;

/*!
 *  \brief BAP Client Audio Contexts update callback type.
 *
 *  \param  connId       DM connection identifier.
 *  \param  snkAvAudCtx  Bitmask of available audio contexts for reception.
 *  \param  srcAvAudCtx  Bitmask of available audio contexts for transmission.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Client Available
 *          Audio Contexts update is received.
 */
typedef void (*BapcAudioCtxCback_t)(dmConnId_t connId, uint16_t snkAudioCtx, uint16_t srcAudioCtx);

/*!
 *  \brief BAP Client ASE update callback type.
 *
 *  \param  connId       DM connection identifier.
 *  \param  pAcb         Pointer to ASE control block.
 *  \param  aseOldState  ASE old state.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Client ASE
 *          update is received.
 */
typedef void (*BapcAseCback_t)(dmConnId_t connId, bapcAseCb_t *pAcb, uint8_t aseOldState);

/*!
 *  \brief BAP Client ASE Control Point (CP) update callback type.
 *
 *  \param  connId       DM connection identifier.
 *  \param  opcode       ASE CP opcode.
 *  \param  numAse       Total number of ASEs server is providing response for.
 *  \param  pAse         List of ASEs.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Client ASE
 *          Control Point (CP) is received.
 */
typedef void (*BapcAseCpCback_t)(dmConnId_t connId, uint8_t opcode, uint8_t numAse, bapAse_t *pAse);

/*!
 *  \brief BAP Client PAC update callback type.
 *
 *  \param  connId       DM connection identifier.
 *  \param  direction    Audio direction.
 *  \param  pPac         Pointer to PAC record that was updated.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Client PAC
 *          update is received.
 */
typedef void (*BapcPacCback_t)(dmConnId_t connId, uint8_t direction, const bapcPacRec_t *pPac);

/*!
 *  \brief BAP Client PAC Audio Locations update callback type.
 *
 *  \param  connId       DM connection identifier.
 *  \param  direction    Audio direction.
 *  \param  audioLocs    Audio locations.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a BAP Client PAC
 *          Audio Locations update is received.
 */
typedef void (*BapcPacLocCback_t)(dmConnId_t connId, uint8_t direction, uint32_t audioLocs);

/*!
 *  \brief  Check if the server supports the desired vendor-specific codec specific configuration
 *          based on discovered codec specific capabilities.
 *
 *  \param  codecSpCfg    Desired codec specific configuration.
 *  \param  pPac          Pointer to PAC record containing codec specific capabilities.
 *
 *  \return BAP_SUCCESS if codec configuration supported. Error, otherwise.
 */
/*************************************************************************************************/
typedef uint8_t (*BapcVsCodecCfgSupCback_t)(const bapGenCodecSpCfg_t codecSpCfg, const bapcPacRec_t *pPac);

/*!
 *  \brief  Build vendor-specific codec configuration based on desired codec specific configuration
 *          and discovered codec specific capabilities.
 *
 *  \param  pLen          Pointer to codec configuration length to be updated.
 *  \param  pCodecCfg     Pointer to codec configuration to be built.
 *  \param  pCodecSpCfg   Pointer to desired codec specific configuration.
 *  \param  pPac          Pointer to PAC record containing codec specific capabilities.
 *  \param  audioChAlloc  Audio channel allocation.
 *
 *  \return Length of codec configuration.
 *
 *  \note   This callback function calls the client application's callback when the BAP Client
 *          requests a vendor-specific codec configuration on the server.
 */
typedef void (*BapcBuildVsCodecCfgCback_t)(uint8_t *pLen, uint8_t *pCodecCfg, const bapCodecSpCfg_t *pCodecSpCfg,
                                           const bapcPacRec_t *pPac, uint32_t audioChAlloc);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  BAPC Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Look up a CIG control block by ID.
 *
 *  \param  cigId     CIG identifier.
 *
 *  \return Pointer to CIG control block if found. NULL, otherwise.
 */
/*************************************************************************************************/
bapcCigCb_t *BapcCigCbById(uint8_t cigId);

/*************************************************************************************************/
/*!
 *  \brief  Look up a CIG control block by ID.
 *
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return Pointer to CIS control block if found. NULL, otherwise.
 */
/*************************************************************************************************/
bapcCisCb_t *BapcCisCbById(uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Look up a CIS control block by handle.
 *
 *  \param  handle    CIS handle
 *
 *  \return Pointer to CIS control block if found. NULL, otherwise.
 */
/*************************************************************************************************/
bapcCisCb_t *BapcCisCbByHandle(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Look up the connection ID by CIS handle.
 *
 *  \param  handle     CIS handle
 *
 *  \return Connection ID if found. DM_CONN_ID_NONE, otherwise.
 */
/*************************************************************************************************/
dmConnId_t BapcConnIdByCisHandle(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Check if a given CIG has been created.
 *
 *  \param  cigId     CIG identifier.
 *
 *  \return true if CIG found. false, otherwise.
 */
/*************************************************************************************************/
bool BapcCigCreated(uint8_t cigId);

/*************************************************************************************************/
/*!
 *  \brief  Get the number of CISes in a given CIG.
 *
 *  \param  cigId     CIG identifier.
 *
 *  \return Number of CISes if CIG found. 0, otherwise.
 */
/*************************************************************************************************/;
uint8_t BapcGetNumCis(uint8_t cigId);

/*************************************************************************************************/
/*!
 *  \brief  Get the list of CIS IDs in a given CIG.
 *
 *  \param  cigId     CIG identifier.
 *  \param  pCisId    List of CIS IDs to be returned. Must be at least size of DM_CIS_MAX.
 *
 *  \return Number of CISes if CIG found. 0, otherwise.
 */
/*************************************************************************************************/
uint8_t BapcGetCisId(uint8_t cigId, uint8_t *pCisId);

/*************************************************************************************************/
/*!
 *  \brief  Look up a CIS handle by ID.
 *
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return CIS handle if found. HCI_HANDLE_NONE, otherwise.
 */
/*************************************************************************************************/
uint16_t BapcCisHandleById(uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Look up the CIS handle associated with a given ASE ID.
 *
 *  \param  connId    DM Connection ID.
 *  \param  aseId     ASE ID.
 *
 *  \return CIS handle if found. HCI_HANDLE_NONE, otherwise.
 */
/*************************************************************************************************/
uint16_t BapcCisHandleByAseId(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  Check if the state of all CIS connections match a given established state.
 *
 *  \param  cigId     CIG identifier.
 *  \param  cisEst    CIS established state.
 *
 *  \return true if CIS established states match. false, otherwise.
 */
/*************************************************************************************************/
bool BapcAllCisEstMatch(uint8_t cigId, bool cisEst);

/*************************************************************************************************/
/*!
 *  \brief  Check if all datapath have been for a given CIG identifier.
 *
 *  \param  cigId     CIG identifier.
 *
 *  \return true of all datapath created. false, otherwise.
 */
/*************************************************************************************************/
bool BapcDatapathCreated(uint8_t cigId);

/*************************************************************************************************/
/*!
 *  \brief  Get the number of ASEs that are configured in a given state.
 *
 *  \param  connId    DM connection identifier.
 *  \param  aseState  ASE state.
 *
 *  \return Number of ASEs in a given state.
 */
/*************************************************************************************************/
uint8_t BapcGetNumAseByState(dmConnId_t connId, uint8_t aseState);

/*************************************************************************************************/
/*!
 *  \brief  Get the number of ASEs that are configured for a given direction.
 *
 *  \param  connId    DM connection identifier.
 *  \param  direction ASE direction.
 *
 *  \return Number of ASEs in a given state.
 */
/*************************************************************************************************/
uint8_t BapcGetNumAseByDirection(dmConnId_t connId, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Look up a BAP Client connection control block by connection ID.
 *
 *  \param  connId  DM Connection ID.
 *
 *  \return Pointer to connection control block.
 */
/*************************************************************************************************/
bapcCcb_t *BapcConnCbByCid(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Check if the server supports the required  codec configuration for a given audio stream
 *          configuration.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCodecId    Codec_ID.
 *  \param  direction   Number of audio stream configuration.
 *  \param  codecSpCfg  Codec specific configuration.
 *
 *  \return BAP_SUCCESS if audio configuration supported. Error, otherwise.
 */
 /*************************************************************************************************/
uint8_t BapcCodecCfgSupported(dmConnId_t connId, bapCodecId_t *pCodecId, uint8_t direction,
                              const bapGenCodecSpCfg_t codecSpCfg);

/*************************************************************************************************/
/*!
 *  \brief  Check if the server supports the required audio locations.
 *
 *  \param  connId      DM connection ID.
 *  \param  audChAlloc  Audio channel allocation.
 *  \param  direction   Audio direction.
 *
 *  \return BAP_SUCCESS if audio locations supported. Error, otherwise.
 */
/*************************************************************************************************/
uint8_t BapcAudioLocsSupported(dmConnId_t connId, uint32_t audChAlloc, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Return the ASE state for a given ASE ID on a connection.
 *
 *  \param  connId    DM Connection ID.
 *  \param  aseId     ASE ID.
 *
 *  \return ASE state if ASE found. BAP_ASE_SM_NUM_STATES, otherwise.
 */
/*************************************************************************************************/
uint8_t BapcAseState(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  Return the audio direction (with respect to the server) for a given ASE ID on a connection.
 *
 *  \param  connId    DM Connection ID.
 *  \param  aseId     ASE ID.
 *
 *  \return Audio direction if ASE found. BAP_AUDIO_DIR_NONE, otherwise.
 */
/*************************************************************************************************/
uint8_t BapcAseDirection(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  Request a codec configuration on the server for the given stream configuration.
 *
 *  \param  connId       DM connection ID.
 *  \param  direction    ASE direction with respect to the server.
 *  \param  pCodecId     Codec_ID.
 *  \param  numCfg       Number of stream configuration.
 *  \param  pStreamCfg   List of stream configuration.
 *  \param  pAseId       List of ASE IDs configured (to be returned). Must be at least size of
 *                       ASCS_AUDIO_STREAM_MAX.
 *
 *  \return Number of ASEs to be configured by Config Codec operation.
 */
 /*************************************************************************************************/
uint8_t BapcAseConfigCodecStream(dmConnId_t connId, uint32_t location, bapCodecId_t *pCodecId,
                                 uint8_t numStream, bapStream_t *pStream);

/*************************************************************************************************/
/*!
 *  \brief  Request a codec configuration on the server.
 *
 *  \param  connId          DM connection ID.
 *  \param  numSnkAudChans  Number of sink audio channels needed by app.
 *  \param  numSrcAudChans  Number of Source audio channels needed by app.
 *  \param  pAseId          List of ASE IDs configured (to be returned). Must be at least size of
 *                          ASCS_AUDIO_STREAM_MAX.
 *
 *  \return Number of ASEs to be configured by Config Codec operation.
 */
 /*************************************************************************************************/
uint8_t BapcAseConfigCodec(dmConnId_t connId, uint8_t numSnkAudChans, uint8_t numSrcAudChans, uint8_t *pAseId);

/*************************************************************************************************/
/*!
 *  \brief  Request an isochronous stream configuration on the server and to assign an identifier
 *          to the isochronous stream and to an Audio Group which is then coupled to an ASE via
 *          the ASE ID.
 *
 *  \param  connId        DM connection ID.
 *  \param  pCigParams    CIG parameters.
 *  \param  presentDelay  Presentation_Delay value supported (in usec).
 *
 *  \return Number of ASEs to be configured by Config QoS operation.
 */
 /*************************************************************************************************/
uint8_t BapcAseConfigQos(dmConnId_t connId, const BapCigParams_t *pCigParams, uint32_t presentDelay);

/*************************************************************************************************/
/*!
 *  \brief  Eequest the server to enable an ASE, to inform the server of Active_Content values,
 *          representing the Content_Type values intended for usage on the ASE, and to assign a
 *          list of Content Control IDs (CCIDs) to the ASE (where applicable).
 *
 *  \param  connId    DM connection ID.
 *
 *  \return Number of ASEs to be enabled by Enable operation.
 */
/*************************************************************************************************/
uint8_t BapcAseEnable(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Informs a server acting as Audio Source that the client (acting as Audio Sink) is ready
 *          to consume audio data transmitted by the server.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return Number of ASEs to start receiving by Rx Start Ready operation.
 */
/*************************************************************************************************/
uint8_t BapcAseRxStartReady(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Disable operation is used to request the server to disable an ASE.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return Number of ASEs to be disabled by Disable operation.
 */
/*************************************************************************************************/
uint8_t BapcAseDisable(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Inform a server acting as Audio Source that the client (acting as Audio Sink) is ready
 *          to stop consuming audio data transmitted by the server.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return Number of ASEs to stop receiving by Rx Stop Ready operation.
 */
/*************************************************************************************************/
uint8_t BapcAseRxStopReady(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Update Metadata operation is used to provide the server with Metadata to be applied to
 *          an ASE.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return Number of ASE metadata to be updated by Update Metadata operation.
 */
/*************************************************************************************************/
uint8_t BapcAseUpdateMetadata(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Release operation is used to request the server to release an ASE and all resources
 *          associated with that ASE.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return Number of ASEs to be released by Release operation.
 */
/*************************************************************************************************/
uint8_t BapcAseRelease(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Update the Metadata for the specified ASE identifier.
 *
 *  \param  connId    DM connection identifier.
 *  \param  aseId     ASE identifier.
 *  \param  mdType    Metadata type.
 *  \param  len       Length of the value.
 *  \param  pValue    Pointer to the value.
 *  \param  clear     true to clear Metadata before setting it.
 *
 *  \return true if the element was successfully added, updated or deleted. false otherwise.
 */
/*************************************************************************************************/
bool BapcMetadataUpdate(dmConnId_t connId, uint8_t aseId, uint8_t mdType, uint8_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Get the number of audio channels needed for a given audio direction.
 *
 *  \param  direction     ASE direction with respect to the server.
 *  \param  numCfg        Number of audio stream configuration.
 *  \param  pStreamCfg    List of audio stream configuration.
 *
 *  \return Number of audio channels.
 */
/*************************************************************************************************/
uint8_t BapcGetNumAudChans(uint8_t direction, uint8_t numStream, bapStream_t *pStream);

/*************************************************************************************************/
/*!
 *  \brief  Set the parameters of the CISes that are associated with the given CIG ID.
 *
 *  \param  pCigId        Pointer to CIG ID.
 *  \param  pCigParams    CIG parameters.
 *  \param  pConnId       DM connection IDs.
 *  \param  numConn       Number of open connections.
 *  \param  useUnidirect  true to use only unidirectional CISes.
 *
 *  \return Number of CISes to be configured with CIG. 0 if no CISes to configure CIG for.
 */
/*************************************************************************************************/
uint8_t BapcConfigCisCig(uint8_t *pCigId, const BapCigParams_t *pCigParams, dmConnId_t *pConnId,
                         uint8_t numConn, bool useUnidirect);

/*************************************************************************************************/
/*!
 *  \brief  Remove CIG and all CISes associated with the given CIG ID.
 *
 *  \param  connId    DM Connection ID.
 *  \param  pCigId    Pointer to CIG ID to be removed.
 *  \param  pAcb      Pointer to ASE control block.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapcRemoveCisCig(dmConnId_t connId, uint8_t *pCigId, bapcAseCb_t *pAcb);

/*************************************************************************************************/
/*!
 *  \brief  Open CISes that have been configured.
 *
 *  \return Number of CISes to open.
 */
 /*************************************************************************************************/
uint8_t BapcOpenCis(void);

/*************************************************************************************************/
/*!
 *  \brief  Setup an output (Controller to Host) data path on the BAP Client for the given CIS handle.
 *
 *  \param  cisHandle CIS handle.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcSetupOutputDataPath(uint16_t cisHandle);

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
void BapcSetupInputDataPath(uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  Remove input and output data path on the BAP Client for the given ASE.
 *
 *  \param  pAcb      Pointer to ASE control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcRemoveDataPath(bapcAseCb_t *pAcb);

/*************************************************************************************************/
/*!
 *  \brief  Close a CIS for the given CIS ID.
 *
 *  \param  cigId     CIG identifier.
 *  \param  cisId     CIS identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcCloseCis(uint8_t cigId, uint8_t cisId);

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
void BapcSetVsDataPathConfig(uint8_t dpId, uint8_t configLen, uint8_t *pConfig);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application vendor-specific codec configuration supported callback
 *          function.
 *
 *  \param  cback     Application VS codec configuration supported callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void BapcVsCodecCfgSupCbackRegister(BapcVsCodecCfgSupCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application build vendor-specific codec configuration callback
 *          function.
 *
 *  \param  cback     Application build VS codec configuration callback.
 *
 *  \return none
 */
/*************************************************************************************************/
void BapcBuildVsCodecCfgCbackRegister(BapcBuildVsCodecCfgCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  BAP Client initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the BAP Client of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapcProcDmMsg(dmEvt_t *pEvt);

/**************************************************************************************************
  BAPC ASE Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for ASE service.  Note that pHdlList
 *          must point to an array of handles of length BAPS_ASE_HDL_LIST_LEN.  If discovery is
 *          successful the handles of discovered characteristics and descriptors will be set
 *          in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set ASC service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can be used to restore handles after restablishing a bond with a device.
 */
/*************************************************************************************************/
void BapcAscSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Requests a Sink ASE from the server.
 *
 *  \param  connId     Connection identifier.
 *  \param  handleIdx  Sink ASE attribute handle index.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapcAscGetSinkAse(dmConnId_t connId, uint8_t handleIdx);

/*************************************************************************************************/
/*!
 *  \brief  Requests a Source ASE from the server.
 *
 *  \param  connId     Connection identifier.
 *  \param  handleIdx  Source ASE attribute handle index.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapcAscGetSourceAse(dmConnId_t connId, uint8_t handleIdx);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Configure Codec operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASEs in Config Codec operation.
 *  \param  pCodec    List of codec parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseConfigCodec(dmConnId_t connId, uint8_t numAse, bapCodecParams_t *pCodec);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Configure QoS operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Config QoS operation.
 *  \param  pQos      List of QoS configuration.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseConfigQos(dmConnId_t connId, uint8_t numAse, bapQosParams_t *pQos);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Enable operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Enable operation.
 *  \param  pMetadata List of Metadata parameters.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void BapcAscAseEnable(dmConnId_t connId, uint8_t numAse, bapMetadataParams_t *pMetadata);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Receiver Start Ready operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Receiver Start Ready operation.
 *  \param  pAseID    List of ASE IDs.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseRxStartReady(dmConnId_t connId, uint8_t numAse, uint8_t *pAseID);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Disable operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Disable operation.
 *  \param  pAseID    List of ASE IDs.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseDisable(dmConnId_t connId, uint8_t numAse, uint8_t *pAseID);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Receiver Stop Ready operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Enable operation.
 *  \param  pAseID    List of ASE IDs.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseRxStopReady(dmConnId_t connId, uint8_t numAse, uint8_t *pAseID);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Update Metadata operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Switch Content operation.
 *  \param  pMetadata List of Metadata parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseUpdateMetadata(dmConnId_t connId, uint8_t numAse, bapMetadataParams_t *pMetadata);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an ASE Release operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  numAse    Number of ASE IDs in Release operation.
 *  \param  pAseID    List of ASE IDs.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscAseRelease(dmConnId_t connId, uint8_t numAse, uint8_t *pAseID);

/*************************************************************************************************/
/*!
 *  \brief  Look up an ASE control block by connection ID.
 *
 *  \param  connId    DM Connection ID.
 *
 *  \return Pointer to ASE control block.
 */
/*************************************************************************************************/
bapcAseCb_t *BapcAseCbByCid(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Look up an ASE control block for a given connection by ASE ID.
 *
 *  \param  connId    DM Connection ID.
 *  \param  aseId     ASE ID.
 *
 *  \return Pointer to control block.
 */
/*************************************************************************************************/
bapcAseCb_t *BapcAseCbByAseId(dmConnId_t connId, uint8_t aseId);

/*************************************************************************************************/
/*!
 *  \brief  Look up an ASE control block for a given connection by attribute handle.
 *
 *  \param  connId    DM Connection ID.
 *  \param  handle    Attribute handle.
 *
 *  \return Pointer to ASE control block if found. NULL, otherwise.
 */
/*************************************************************************************************/
bapcAseCb_t *BapcAseCbByHandle(dmConnId_t connId, uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Look up an ASE control block for the given CIG and CIS identifiers.
 *
 *  \param  connId    DM Connection ID.
 *  \param  cigId     CIG ID.
 *  \param  cigId     CIS ID.
 *
 *  \return Pointer to ASE control block if found. NULL, otherwise.
 */
/*************************************************************************************************/
bapcAseCb_t *BapcAseCbByCigCisIds(dmConnId_t connId, uint8_t cigId, uint8_t cisId);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  Reset CIG/CIS IDs for a given ASE control block.
 *
 *  \param  pAcb      Pointer to ASE control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscResetCigCisId(bapcAseCb_t *pAcb);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  BAP Client ASE initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcAscInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length BAPC_ASE_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return ATT_SUCCESS if handle is found, ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t BapcAscValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called to register application ASE update callback functions
 *
 *  \param  aseCback    Application ASE callback
 *  \param  aseCpCback  Application ASE CP callback
 *
 *  \return none
 */
/*************************************************************************************************/
void BapcAscAseCbackRegister(BapcAseCback_t aseCback, BapcAseCpCback_t aseCpCback);

/**************************************************************************************************
  BAPC PAC Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for PAC service.  Note that pHdlList
 *          must point to an array of handles of length BAPS_PAC_HDL_LIST_LEN.  If discovery is
 *          successful the handles of discovered characteristics and descriptors will be set
 *          in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcPacDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set PAC service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can be used to restore handles after reestablishing a bond with a device.
 */
/*************************************************************************************************/
void BapcPacSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Requests a PAC record on the server when acting as Audio Sink or Audio Source.
 *
 *  \param  connId    Connection identifier.
 *  \param  direction Audio direction.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcGetPac(dmConnId_t connId, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Requests the supported Audio Locations on the server when acting as Audio Sink or Audio
 *          Source.
 *
 *  \param  connId    Connection identifier.
 *  \param  direction Audio direction.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcGetAudioLoc(dmConnId_t connId, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Returns the audio locations for the given audio direction.
 *
 *  \param  connId    Connection identifier.
 *  \param  direction Audio direction.
 *
 *  \return Audio locations.
 */
/*************************************************************************************************/
uint32_t BapcAudioLoc(dmConnId_t connId, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Requests the available audio contexts for reception or transmission on the server.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcGetAvailAudioCtx(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Requests the supported audio contexts for reception or transmission on the server.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcGetSupAudioCtx(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Set the supported Audio Locations on the server when acting as Audio Sink or Audio
 *          Source.
 *
 *  \param  connId        Connection identifier.
 *  \param  direction     Audio direction.
 *  \param  locationBits  Audio location value bits.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcSetAudioLoc(dmConnId_t connId, uint8_t direction, uint32_t locationBits);

/*************************************************************************************************/
/*!
 *  \brief  Look up an ASE control block by connection ID.
 *
 *  \param  connId    DM Connection ID.
 *
 *  \return Pointer to ASE control block.
 */
/*************************************************************************************************/
bapcPacRec_t *BapcPacRecsByCid(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Allocate a PAC record.
 *
 *  \param  connId    DM Connection ID.
 *  \param  handle    Attribute handle.
 *  \param  direction Audio direction.
 *
 *  \return Pointer to PAC record or NULL if failure.
 */
/*************************************************************************************************/
bapcPacRec_t *BapcPacRecAlloc(dmConnId_t connId, uint16_t handle, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Free a PAC record.
 *
 *  \param  pRec      Pointer to PAC record.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapcPacRecFree(bapcPacRec_t *pPac);

/*************************************************************************************************/
/*!
 *  \brief  Remove all PAC records for the given audio direction.
 *
 *  \param  connId    DM Connection ID.
 *  \param  handle    Attribute handle.
 *  \param  direction Audio direction.
 *
 *  \return none.
 */
/*************************************************************************************************/
void BapcRemovePacRec(dmConnId_t connId, uint16_t handle, uint8_t direction);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length BAPC_PAC_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return ATT_SUCCESS if handle is found, ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t BapcPacValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application Audio Contexts update callback functions
 *
 *  \param  avAudCtxCback   Application available audio contexts callback
 *  \param  supAudCtxCback  Application supported audio contexts callback
 *
 *  \return none
 */
/*************************************************************************************************/
void BapcPacAudioCtxCbackRegister(BapcAudioCtxCback_t avAudCtxCback,
                                  BapcAudioCtxCback_t supAudCtxCback);

/*************************************************************************************************/
/*!
 *  \brief  Called to register an application PAC update callback functions.
 *
 *  \param  pacCback    Application PAC callback
 *  \param  pacLocCback Application PAC Location callback
 *
 *  \return none
 */
/*************************************************************************************************/
void BapcPacCbackRegister(BapcPacCback_t pacCback, BapcPacLocCback_t pacLocCback);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  BAP Client PAC initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void BapcPacInit(void);

/*! \} */    /* BASIC_AUDIO_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* BAPS_API_H */
