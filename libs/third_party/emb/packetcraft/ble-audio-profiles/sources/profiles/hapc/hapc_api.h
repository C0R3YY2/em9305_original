/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Hearing Access Profile Client (HAPC) Application Interface.
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
#ifndef HAPC_API_H
#define HAPC_API_H

#include "att_api.h"
#include "bap_defs.h"
#include "bapc_api.h"
#include "cap_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup HEARING_ACCESS_PROFILE_CLIENT
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Length constants of HAS messages fixed length fields */
#define HAPC_HAP_CP_LEN            1     /*!< Hearing Aid Preset CP message length */
#define HAPC_HAP_PRESET_REC_LEN    3     /*!< Preset record length */
#define HAPC_HAF_LEN               1     /*!< Hearing Aid Features message length */
#define HAPC_API_LEN               1     /*!< Active Preset Index message length */

/*! \brief HAPC HARC application callback events */
#define HAPC_HEARING_AID_FEATS_NTF 0x0D  /*!< Hearing Aid Features notification */
#define HAPC_ACT_PRESET_INDEX_NTF  0x0E  /*!< Active Preset Index notification */

#define HAPC_OP_MAX                0x0F

/*! \brief Enumeration of handle indexes of HAS characteristics to be discovered */
enum
{
  HAPC_HAS_HAF_HDL_IDX,                  /*!< Hearing Aid Features */
  HAPC_HAS_HAF_CCC_HDL_IDX,              /*!< Hearing Aid Features client characteristic configuration */
  HAPC_HAS_HAP_CP_HDL_IDX,               /*!< Hearing Aid Preset Control Point */
  HAPC_HAS_HAP_CP_CCC_HDL_IDX,           /*!< Hearing Aid Preset Control Point client characteristic configuration */
  HAPC_HAS_API_HDL_IDX,                  /*!< Active Preset Index */
  HAPC_HAS_API_CCC_HDL_IDX,              /*!< Active Preset Index client characteristic configuration */

  HAPC_HAS_HDL_LIST_LEN                  /*!< \brief Handle list length */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief HAPC preset record */
typedef struct
{
  uint8_t               index;          /*!< Preset index */
  uint8_t               props;          /*!< Preset properties */
  uint8_t               len;            /*!< Length of reset name */
  uint8_t               *pName;         /*!< Human-readable name of preset */
} hapcPresetRec_t;

/*! \brief Data structure for \ref HAS_CP_OP_READ_PRESET_RSP. */
typedef struct
{
  uint8_t               isLast;         /*!< true if last notification or indication */
  hapcPresetRec_t       rec;            /*!< Preset record */
} hapcReadPresetRsp_t;

/*! \brief Data structure for \ref HAS_CP_OP_PRESET_CHANGED. */
typedef struct
{
  uint8_t               changeId;       /*!< Change id */
  uint8_t               isLast;         /*!< true if last notification or indication */
  uint8_t               index;          /*!< Preset index (not applicable only to HAS_CHANGE_ID_GEN_UPDATE) */
  uint8_t               prevIndex;      /*!< Previous index (applicable only to HAS_CHANGE_ID_GEN_UPDATE) */
  hapcPresetRec_t       rec;            /*!< Preset record (applicable only to HAS_CHANGE_ID_GEN_UPDATE) */
} hapcPresetChanged_t;

/*! \brief Data structure for \ref HAPC_HEARING_AID_FEATS_NTF. */
typedef struct
{
  uint8_t               features;       /*!< Hearing aid features */
} hapcHearingAidFeats_t;

/*! \brief Data structure for \ref HAPC_ACT_PRESET_INDEX_NTF. */
typedef struct
{
  uint8_t               index;          /*!< Active preset index */
} hapcActPresetIndex_t;

/*! \brief HAPC Preset event type */
typedef union
{
  hapcReadPresetRsp_t   readPresetRsp;  /*!< handles \ref HAS_CP_OP_READ_PRESET_RSP */
  hapcPresetChanged_t   presetChanged;  /*!< handles \ref HAS_CP_OP_PRESET_CHANGED */
  hapcHearingAidFeats_t hearingAidFeats;/*!< handles \ref HAPC_HEARING_AID_FEATS_NTF */
  hapcActPresetIndex_t  actPresetIndex; /*!< handles \ref HAPC_ACT_PRESET_INDEX_NTF */
} hapcPresetEvt_t;

/*!
 *  \brief HAP Client callback type.
 *
 *  \param  connId      DM connection identifier.
 *  \param  evt         HAPC event.
 *  \param  status      Event status.
 *  \param  pPresetEvt  Pointer to preset event.
 *
 *  \return None.
 *
 *  \note   This callback function calls the client application's callback when a Hearing Aid Preset
 *          operation is completed.
 */
typedef void (*hapcCback_t)(dmConnId_t connId, uint8_t evt, uint8_t status, hapcPresetEvt_t *pPresetEvt);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  HAPC Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for HAS service.  Note that pHdlList
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
void HapcHasDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set HAS service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can be used to restore handles after reestablishing a bond with a device.
 */
/*************************************************************************************************/
void HapcHasSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length HAPC_HAS_HDL_LIST_LEN.
 *          If the attribute handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return ATT_SUCCESS if handle is found, ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t HapcHasValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the HAPC HAUC of System events.
 *
 *  \param  pEvt    Pointer to the event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called to register application's HAPC callback.
 *
 *  \param  cback   Application's HAPC callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcRegister(hapcCback_t cback);

/**************************************************************************************************
  HAUC Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HAPC Hearing Aid Unicast Client (UAUC) role.
 *
 *  \param  aseCback    Application's BAP Client ASE callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapcHaucInit(BapcAseCback_t aseCback);

/**************************************************************************************************
  HARC Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initiate an HAP Read Presets Request procedure.
 *
 *  \param  connId     DM connection ID.
 *  \param  startIndex Start index.
 *  \param  numPresets Number of presets.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcHarcReadPresetsReq(dmConnId_t connId, uint8_t startIndex, uint8_t numPresets);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an HAP Write Preset Name operation.
 *
 *  \param  connId     DM connection ID.
 *  \param  startIndex Start index.
 *  \param  numPresets Number of presets.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcHarcWritePresetName(dmConnId_t connId, uint8_t index, uint8_t len, uint8_t *pName);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an HAP Set Active Preset (- Synchronized Locally) operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  index     Preset index.
 *  \param  syncLocal true if synchronized Locally operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcHarcSetActivePreset(dmConnId_t connId, uint8_t index, bool syncLocal);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an HAP Set Next Preset (- Synchronized Locally) operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  syncLocal true if synchronized Locally operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcHarcSetNextPreset(dmConnId_t connId, bool syncLocal);

/*************************************************************************************************/
/*!
 *  \brief  Initiate an HAP Set Previous Preset (- Synchronized Locally) operation.
 *
 *  \param  connId    DM connection ID.
 *  \param  syncLocal true if synchronized Locally operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcHarcSetPrevioustPreset(dmConnId_t connId, bool syncLocal);

/*************************************************************************************************/
/*!
 *  \brief  Read the Active Preset Index.
 *
 *  \param  connId    DM connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcHarcReadActPresetIndex(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HAPC Hearing Aid Remote Controller (HARC) role.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapcHarcInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HAPC CAP Broadcast Assistant subsystem.
 *
 *  \param  brsCback  Client application BRS callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapcCapcbaInit(BapbaBrsCback_t brsCback);

/**************************************************************************************************
  IAC Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for IAS service.  Note that pHdlList
 *          must point to an array of handles of length FMPL_IAS_HDL_LIST_LEN.  If discovery is
 *          successful the handles of discovered characteristics and descriptors will be set
 *          in pHdlList.
 *
 *  \param  connId    DM Connection ID.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcIacIasDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set IAS service and characteristic handles.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can be used to restore handles after reestablishing a bond with a device.
 */
/*************************************************************************************************/
void HapcIacIasSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Send an immediate alert to the peer device.
 *
 *  \param  connId    DM connection ID.
 *  \param  alert     Alert value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapcIacIasSendAlert(dmConnId_t connId, uint8_t alert);

/*! \} */    /* HEARING_ACCESS_PROFILE_CLIENT */

#ifdef __cplusplus
};
#endif

#endif /* HAPC_API_H */
