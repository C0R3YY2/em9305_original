/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  TMAP Unicast Sink Interface.
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

#ifndef TMAPCTU_API_H
#define TMAPCTU_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Warning, it must be the same as bapsAseCb_t */
#define tmapAseCb_t bapsAseCb_t
#define TmapAseCback_t BapsAseCback_t
/*! WSF message event starting value */
#define TMAP_CTU_MSG_START              0xA0

/*! WSF message event enumeration */
enum
{
  TMAP_CTU_TIMER_IND = TMAP_CTU_MSG_START,            /*! Timer expired */

  /* TODO: A device can have multiple coordinated set identification services.
    *       Add or remove timer events as needed. */
  TMAP_CTU_CSIS_LOCK_TIMER_IND_0,                 /*! CSIS Lock Timer expired */
  TMAP_CTU_CSIS_LOCK_TIMER_IND_1,                 /*! CSIS Lock Timer expired */

  TMAP_CTU_CSIS_PSRI_IND,                         /*! CSIS private set resolving key indication */
  TMAP_CTU_CSIS_ENC_SIRK_EVT_0,                   /*! Event for encryption of the SIRK */
  TMAP_CTU_CSIS_ENC_SIRK_EVT_1,                   /*! Event for encryption of the SIRK */
};

/*! TMAP CTU message type */
typedef union
{
  wsfMsgHdr_t     hdr;
  dmEvt_t         dm;
  attsCccEvt_t    ccc;
  attEvt_t        att;
} tmapctuMsg_t;

/*************************************************************************************************/
/*!
*  \brief  Profile initialization function.
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuVcpInit(void);

/*************************************************************************************************/
/*!
*  \brief  Start the vcp application.
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuVcpStart(void);

/*************************************************************************************************/
/*!
*  \brief  Start the micp application.
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuMicpStart(void);

/*************************************************************************************************/
/*!
*  \brief  Start the aicp application.
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuAicpStart(void);

/*************************************************************************************************/
/*!
*  \brief  Start the csips application.
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuCsipsStart(wsfHandlerId_t handlerId, uint8_t cfg0HandleOffset, uint8_t cfg1HandleOffset);

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMedProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process CCC state change.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtUMediaProcCccState(tmapctuMsg_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuVcpProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMicpProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuCsipsProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the TMAP CT Unicast Media subsystem.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void TmapCtUnicastMediaInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtUnicastMediaRegisterServRecord(void);

/*************************************************************************************************/
/*!
*  \brief  Set Audio Location
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtUMediaSetAudioLocation(uint8_t dir, uint8_t audLoc);

/*************************************************************************************************/
/*!
*  \brief  Set Audio Contexts Availability
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtUMediaSetAvailAudioCtx(uint8_t dir, uint8_t availAudCtx);

/*************************************************************************************************/
/*!
*  \brief  Set Audio Contexts support
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtUMediaSetSupAudioCtx(uint8_t dir, uint8_t supAudCtx);

/*************************************************************************************************/
/*!
 *  \brief  Application ATT callback.
 *
 *  \param  pEvt    ATT callback event
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtUnicastAttCback(attEvt_t* pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Application ATTS client characteristic configuration callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtUnicastCccCback(attsCccEvt_t* pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the TMAP CT Unicast subsystem.
 *
 *  \return none.
 */
 /*************************************************************************************************/
void TmapCtUnicastInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
*  \brief  Start the application.
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtUnicastRegisterServRecord(void);

/*************************************************************************************************/
/*!
*  \brief  TMAP process message event
*
*  \return None.
*/
/*************************************************************************************************/
void TmapProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  TMAP DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuDmCback(dmEvt_t* pDmEvt);

/*************************************************************************************************/
/*!
 *  \brief  The API interface for application to change local status
 *          The layer will select proper notification and send it to the peer device
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuVcpSetStatusChange(uint8_t serviceId, uint8_t opId);


/*************************************************************************************************/
/*!
*  \brief  The API is to config the default volume and mute status
*          The layer will select proper notification and send it to the peer device if exists
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuSetDefaultValues(uint8_t volume, bool muteMode, uint8_t persist);


/*************************************************************************************************/
/*!
 *  \brief  Profile initialization function.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Discover service for profile.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return true - finished discovering services. false - more services are to be discovered.
 */
 /*************************************************************************************************/
bool TmapCtuMcpDiscover(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Configure service for profile.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    APP_DISC_CFG_START or APP_DISC_CFG_CONN_START.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpConfigure(dmConnId_t connId, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Process a received ATT read response, notification, or indication.
 *
 *  \param  pMsg    Pointer to ATT callback event message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpValueUpdate(attEvt_t* pMsg);

/*************************************************************************************************/
/*!
*  \brief  TMAP MCP process message event
*
*  \return None.
*/
/*************************************************************************************************/
void TmapCtuMcpProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  TmapCtuMcpCPCommand.
 *
 *  \param  connId    Connection identifier.
 *  \param  opcode    The message defined by the MCP/MCS profile
 *  \param  parms     The parameters with the opcode defined by the MCP/MCS profile
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpCPCommand(dmConnId_t connId, uint8_t opcode, int32_t parms);

/*************************************************************************************************/
/*!
 *  \brief  Read the player name attribute.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadPlayerName(dmConnId_t connId);


/*************************************************************************************************/
/*!
 *  \brief  Read the player name attribute.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadPlayerName(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the icon URL attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadIconUrl(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the track title attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadTrackTitle(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the track duration attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadTrackDuration(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the track position attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadTrackPosition(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the playback speed attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadPlaybackSpeed(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the seek speed attribute.
 *
 *  \param  connId        Connection identifier.
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadSeekSpeed(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the play order attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadPlayOrder(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the play order supported attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadPlayOrderSupported(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the state attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadState(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the control point opcodes supported attribute.
 *
 *  \param  connId        Connection identifier.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadCtrlPtOpcodesSupported(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Read the Content Control ID (CCID) attribute.
 *
 *  \param  connId        Connection identifier.
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpReadCcid(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Write to the track position attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  position      Track position.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpWriteTrackPosition(dmConnId_t connId, int32_t position);

/*************************************************************************************************/
/*!
 *  \brief  Write to the playback speed attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  speed         Playback speed.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpWritePlaySpeed(dmConnId_t connId, int8_t speed);

/*************************************************************************************************/
/*!
 *  \brief  Write to the play order attribute.
 *
 *  \param  connId        Connection identifier.
 *  \param  speed         Play order.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMcpWritePlayOrder(dmConnId_t connId, uint8_t order);

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuCcpProcMsg(wsfMsgHdr_t* pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Discover service for profile.
 *
 *  \param  connId    Connection identifier.
 */
 /*************************************************************************************************/
bool TmapCtuCcpDiscover(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Configure service for profile.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    APP_DISC_CFG_START or APP_DISC_CFG_CONN_START.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuCcpConfigure(dmConnId_t connId, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Perform MICP unmute operation.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMicpUnmute(void);

/*************************************************************************************************/
/*!
 *  \brief  Perform MICP mute operation.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMicpMute(void);

/*************************************************************************************************/
/*!
 *  \brief  Perform MICP mute disable operation.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuMicpMuteDisabled(void);

/*************************************************************************************************/
/*!
 *  \brief  The API interface for application to change local status
 *          The layer will select proper notification and send it to the peer device
 *
 *  \return None.
 */
 /*************************************************************************************************/
void TmapCtuAicpSetStatusChange(uint8_t serviceId, uint8_t opId);


/*************************************************************************************************/
/*!
 *  \brief  Set the lock attribute value.
 *
 *  \param  svcId       Service Identifier
 *  \param  lock        Lock attribute value
 *
 *  \return none.
 */
 /*************************************************************************************************/
void TmapCtuCsipsSetLock(uint8_t svcId, uint8_t lock);

/*! \} */    /* TMAP CT Unicast API*/
#ifdef __cplusplus
};
#endif

#endif /* TMAPCTUS_API_H */
