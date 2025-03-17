/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Hearing Access Profile Server (HAPS) Application Interface.
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
#ifndef HAPS_API_H
#define HAPS_API_H

#include "att_api.h"
#include "svc_has.h"
#include "baps_api.h"
#include "bapsk_api.h"
#include "bapsd_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup HEARING_ACCESS_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! HAPS profiles */
enum
{
  HAPS_PROFILE_HA,                        /*!< HAPS Hearing Aid */

  HAPS_PROFILE_MAX
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*!
 *  \brief HAP Server callback type.
 *
 *  \param  connId  DM connection identifier.
 *  \param  opcode  HAS opcode.
 *  \param  pRec    Pointer to preset record.
 *
 *  \return None.
 *
 *  \note   This callback function calls the server application's callback when a Hearing Aid Preset
 *          operation is received.
 */
typedef void (*hapsCback_t)(dmConnId_t connId, uint8_t opcode, hasPresetRec_t *pRec);

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**************************************************************************************************
  HAPS Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add a new preset record.
 *
 *  \param  len       Length of preset name.
 *  \param  pName     Pointer to preset name.
 *  \param  writable  true if preset name can be written by the client.
 *  \param  isAvail   true if preset is available.
 *
 *  \return Preset index if add successful. HAS_PRESET_INDEX_NONE, otherwise.
 */
/*************************************************************************************************/
uint8_t HapsHaAddPreset(uint8_t len, uint8_t *pName, bool writable, bool isAvail);

/*************************************************************************************************/
/*!
 *  \brief  Set a preset record as available or unavailable.
 *
 *  \param  index     Preset index.
 *  \param  avail     true if preset available.
 *
 *  \return true if availability set successful. false, otherwise.
 */
/*************************************************************************************************/
bool HapsHaSetPresetAvail(uint8_t index, bool avail);

/*************************************************************************************************/
/*!
 *  \brief  Set the name for a given preset.
 *
 *  \param  index     Preset index.
 *  \param  len       Length of name.
 *  \param  pName     Pointer to name.
 *
 *  \return true if name set successful. false, otherwise.
 */
/*************************************************************************************************/
bool HapsHaSetPresetName(uint8_t index, uint8_t len, uint8_t *pName);

/*************************************************************************************************/
/*!
 *  \brief  Delete the preset record for a given index.
 *
 *  \param  index     Preset index.
 *
 *  \return true if delete successful. false, otherwise.
 */
/*************************************************************************************************/
bool HapsHaDeletePreset(uint8_t index);

/*************************************************************************************************/
/*!
 *  \brief  Set the value of the Hearing Aid Features characteristic.
 *
 *  \param  features  Hearing Aid Features bits.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapsHaSetFeatures(uint8_t features);

/*************************************************************************************************/
/*!
 *  \brief  Set Active Preset to a given index.
 *
 *  \param  connId    Connection identifier.
 *  \param  index     Preset index.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapsHaSetActPreset(dmConnId_t connId, uint8_t index);

/*************************************************************************************************/
/*!
 *  \brief  Called to register application's HAP Server callback function.
 *
 *  \param  cback     Application's HAPS callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapsRegister(hapsCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the CAP Acceptor Unicast Client (AUC) subsystem.
 *
 *  \param  aseCback  Server application ASE callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the HAP server of ATT messages.
 *
 *  \param  pMsg    message containing event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void HapsProcAttMsg(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HAPS Hearing Aid (HA) role.
 *
 *  \param  aseCback  Server application ASE callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapsHaInit(BapsAseCback_t aseCback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HAPS CAP Broadcast Sink subsystem.
 *
 *  \param  skCback   Application's BAP Sink BAS callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapsCapabkInit(BapskBasCback_t skCback);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the HAPS CAP Scan Delegator subsystem.
 *
 *  \param  sdCback   Application's BAP SD BAS callback.
 *
 *  \return none.
 */
/*************************************************************************************************/
void HapsCapasdInit(BapsdBasCback_t sdCback);

/*! \} */    /* HEARING_ACCESS_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* HAPS_API_H */
