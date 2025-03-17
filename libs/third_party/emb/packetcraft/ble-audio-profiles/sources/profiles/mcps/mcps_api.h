/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Media Control Profile server.
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
#ifndef MCPS_API_H
#define MCPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MEDIA_CONTROL_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Media Control Profile Server service control block. */
typedef struct
{
  uint8_t             svcId;              /*! Media control service identifier */
  uint8_t             state;              /*! Media control state */
  uint32_t            trackId;            /*! Current track ID */
  int32_t             trackPos;           /*! Current track position */
  uint32_t            groupId;            /*! Current group ID */
  int8_t              seekSpeed;          /*! Seek speed */
  int8_t              playSpeed;          /*! Track play speed */
  uint16_t            playOrder;          /*! Track play order */
} mcpsMediaInfo_t;

/* Media Control Profile Server action function */
typedef uint8_t (*mcpsAct_t)(mcpsMediaInfo_t *pInfo, uint8_t opcode, int32_t parameter);

/* Media Control Profile Server action function set */
typedef struct
{
  mcpsAct_t           play;               /*! Media play action function */
  mcpsAct_t           pause;              /*! Media pause action function */
  mcpsAct_t           seek;               /*! Media seek action function */
  mcpsAct_t           stop;               /*! Media stop action function */
  mcpsAct_t           move;               /*! Media move action function */
  mcpsAct_t           segment;            /*! Media segment action function */
  mcpsAct_t           track;              /*! Media track action function */
  mcpsAct_t           group;              /*! Media group action function */
} mcpsActionTbl_t;

/* Media Control Profile Server get track duration callback */
typedef uint32_t(*mcpsGetTrackDur_t)(mcpsMediaInfo_t* pInfo);

/* Media Control Profile Server get track position callback */
typedef uint32_t (*mcpsGetTrackPos_t)(mcpsMediaInfo_t *pInfo);

/* Media Control Profile Server set track position callback */
typedef void (*mcpsSetTrackPos_t)(mcpsMediaInfo_t *pInfo, uint32_t pos);

/* Media Control Profile Server set playback speed callback */
typedef void (*mcpsSetPlaySpeed_t)(mcpsMediaInfo_t *pInfo, int8_t speed);

/* Media Control Profile Server set playback order callback */
typedef void (*mcpsSetPlayOrder_t)(mcpsMediaInfo_t *pInfo, uint16_t order);

/* Media Control Profile Server callback function set */
typedef struct
{
  mcpsGetTrackDur_t   getTrackDur;        /*! Callback to get the current track duration */
  mcpsGetTrackPos_t   getTrackPos;        /*! Callback to get the current track position */
  mcpsSetTrackPos_t   setTrackPos;        /*! Callback to set the current track position */
  mcpsSetPlaySpeed_t  setPlaySpeed;       /*! Callback to set the playback speed */
  mcpsSetPlayOrder_t  setPlayOrder;       /*! Callback to set the playback order */
} mcpsCbackTbl_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Send a track change notification.
 *
 *  \param svcId     Media control service index.
 *  \param priority  EATT priority.
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSendTrackChangeNtf(uint8_t svcId, uint8_t priority);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the media player name.
 *
 *  \param svcId     Media control service index.
 *  \param pName     Media player name (UTF-8 string).
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetMediaPlayerName(uint8_t svcId, const char *pName);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the media icon uri.
 *
 *  \param svcId     Media control service index.
 *  \param pUri      Media icon URI (URI array).
 *  \param len       Size of pUri in bytes.
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetMediaPlayerIconUri(uint8_t svcId, const char *pUri, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the track title.
 *
 *  \param svcId        Media control service index.
 *  \param pTitle       Track title (UTF-8 string).
 *  \param trackChanged true if track has changed.
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetTrackTitle(uint8_t svcId, const char *pTitle, bool trackChanged);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the track duration.
 *
 *  \param svcId        Media control service index.
 *  \param duration     Track duration (32-bit signed integer).
 *  \param trackChanged true if track has changed.
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetTrackDuration(uint8_t svcId, int32_t duration, bool trackChanged);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the track position.
 *
 *  \param svcId        Media control service index.
 *  \param position     Track position (32-bit signed integer).
 *  \param trackChanged true if track has changed.
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetTrackPosition(uint8_t svcId, int32_t position, bool trackChanged);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the track position without sending notification.
 *
 *  \param svcId        Media control service identifier.
 *  \param position     Track position (32-bit signed integer).
 *
 *  \return none.
 */
 /*************************************************************************************************/
void McpsSetTrackPositionWithoutNtf(uint8_t svcId, int32_t position);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the playback speed.
 *
 *  \param svcId     Media control service index.
 *  \param speed     Playback speed (8-bit signed integer).
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetPlaybackSpeed(uint8_t svcId, int8_t speed);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the seek speed.
 *
 *  \param svcId     Media control service index.
 *  \param speed     Seek speed (8-bit signed integer).
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetSeekSpeed(uint8_t svcId, int8_t speed);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the play order.
 *
 *  \param svcId     Media control service index.
 *  \param order     Play order
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetPlayOrder(uint8_t svcId, uint8_t order);

/*************************************************************************************************/
/*!
 *  \brief  Called to set the media control state.
 *
 *  \param svcId     Media control service index.
 *  \param state     Media control state.
 *
 *  \return none.
 */
/*************************************************************************************************/
void McpsSetState(uint8_t svcId, uint8_t state);

/*************************************************************************************************/
/*!
 *  \brief  Get a pointer to the service info structure for the given service ID.
 *
 *  \param svcId     Media control service index.
 *
 *  \return Pointer to the service info structure.
 */
/*************************************************************************************************/
mcpsMediaInfo_t *McpsGetSvcInfo(uint8_t svcId);

/*************************************************************************************************/
/*!
 *  \brief  Timeout function called by applicationt to simulate media player.
 *
 *  \param pInfo     Media control profile information.
 *  \param deltaMs   Change in milliseconds since last call.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpsSampleHandleTimeout(mcpsMediaInfo_t *pInfo, uint32_t deltaMs);

/*************************************************************************************************/
/*!
 *  \brief  Media Control Profile server registration of callback and action functions.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpsRegister(uint8_t svcId, uint8_t cccStart, const mcpsActionTbl_t *pActions, const mcpsCbackTbl_t *pCbacks);

/*************************************************************************************************/
/*!
 *  \brief  Media Control Profile server initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Media Control Profile server initialization - Example Application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpsSampleInit(uint8_t svcId, uint8_t cccStart);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Media Control Profile server of DM Events.
 *
 *  \param  pEvt   Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpsProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the MCP server of ATT messages.
 *
 *  \param  pMsg    message containing event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void McpsProcAttMsg(wsfMsgHdr_t *pMsg);

/*! \} */    /* MEDIA_CONTROL_PROFILE_SERVER */

/* Media control profile server test functions - for internal use only */
bool McpsTestCmpState(uint8_t svcId, uint8_t state);
bool McpsTestCmpTrackId(uint8_t svcId, uint32_t trackId);
bool McpsTestCmpGroupId(uint8_t svcId, uint32_t group);
bool McpsTestCmpSeekSpeed(uint8_t svcId, int8_t speed);
bool McpsTestCmpPlaySpeed(uint8_t svcId, int8_t speed);
bool McpsTestCmpPlayOrder(uint8_t svcId, uint8_t order);

#ifdef __cplusplus
};
#endif

#endif /* MCPS_API_H */
