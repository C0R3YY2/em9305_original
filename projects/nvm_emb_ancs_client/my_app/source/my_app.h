////////////////////////////////////////////////////////////////////////////////
///
/// @file       app.h
///
/// @project    T9305
///
/// @brief      Application header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#ifndef MY_APP_H
#define MY_APP_H

#include <my_app_signals.h>

#define EMPTY_UID 0xFFFFFFFF

// This is the code passed around for a button press,
//   originated with a spacebar over the UART
#define BUTTON_PRESS_CODE 0x20

/******************************************************************************\
 *  Types
\******************************************************************************/

/// Application task event states enum.
typedef enum
{
    MY_APP_EVT_ST_HANDLED = 0,
    MY_APP_EVT_ST_UNKNOWN,
    MY_APP_EVT_ST_ERROR,
} MyApp_EvtStates_t;

// Call states
typedef enum
{
    MY_APP_CS_IDLE = 1,
    MY_APP_CS_INCOMING_CALL,
    MY_APP_CS_ACTIVE_CALL
} MyApp_CallState_t;

#define WORK_BUFF_SZ 32
// This is from the ANCS document Fig. 2-3 to retrieve only Attribute 2
#define TX_BUFF_SZ 8

/* Control block structure definition */
typedef struct
{
    wsfHandlerId_t      handlerId;
    dmConnId_t          connId;
    uint8_t             connUpdateAttempts;
    bool                doConnUpdate;
    bool                bondable;
    bool                bonded;
    uint8_t             state;
    uint8_t             rcvdKeys;               // Bitmask of keys received during pairing
    bdAddr_t            peerAddr;
    uint8_t             peerAddrType;
    bool                connected;              // Status of BLE connection to host
    wsfHandlerId_t      BLE_SubsysHandlerId;    // WSF handler ID
    uint8_t             txBuff[TX_BUFF_SZ];     // Holding for ANCS request message
    MyApp_CallState_t   callState;
    uint8_t             dataInput;
    uint32_t            msgUID;
    uint8_t             workBuff[WORK_BUFF_SZ]; // For some printf formatting
}  MyApp_Cb_t;

// Async ANCS event structure mapping
typedef struct
{
    uint8_t eventID;
    uint8_t eventFlags;
    uint8_t categoryID;
    uint8_t categoryCount;
    uint32_t notifUID;
} ANCS_Evt_t;

// Response to ANCS information request via attribute number
typedef struct
{
//  uint8_t commandID;      /* Always zero so leave it out */
    uint32_t notifUID;
    uint8_t  attrID;
    uint8_t  attrLen[2];
    uint8_t  attr[];
} ANCS_Info_Evt_t;

typedef struct
{
    wsfMsgHdr_t hdr;
    uint8_t     commandID;              // 2 to answer, 0 to request info
    uint8_t     notificationUID[4];
    // This can either contain a command (payload[0])
    // or info request (payload[0]) followed by length (payload[1]-payload[2])
    uint8_t     payload[3];
} ANCS_notif_action_t;

enum
{
    ANCS_CATEGORY_ID_OTHER,
    ANCS_CATEGORY_ID_INCOMING_CALL,     // 1
    ANCS_CATEGORY_ID_MISSED_CALL,
    ANCS_CATEGORY_ID_VOICE_MAIL,
    ANCS_CATEGORY_ID_SOCIAL,
    ANCS_CATEGORY_ID_SCHEDULE,
    ANCS_CATEGORY_ID_EMAIL,
    ANCS_CATEGORY_ID_NEWS,
    ANCS_CATEGORY_ID_HEALTH_AND_FITNESS,
    ANCS_CATEGORY_ID_BUSINESS_AND_FINANCE,
    ANCS_CATEGORY_ID_LOCATION,
    ANCS_CATEGORY_ID_ENTERTAINMENT,

    // It is undocumented but the ANCS server will send a notification
    // in this category with a UID for a call that was just accepted.
    // Use that UID to send a negative action to hang up the call.
    ANCS_CATEGORY_ID_RESERVED           // 12
};

enum
{
    ANCS_EVENT_ADDED,       // 0
    ANCS_EVENT_MODIFIED,
    ANCS_EVENT_REMOVED      // 2
};

#define ANCS_CMD_ID_GET_NOTIF_ATTR 0
#define ANCS_CMD_ID_PERFORM_NOTIF_ACTION 2

#define ANCS_ID_ACT_POS 0
#define ANCS_ID_ACT_NEG 1

enum
{
  ANCS_ATTR_APP_ID,

  // The following 3 Attribute IDs are followed by a 2 byte max length parameter
  ANCS_ATTR_TITLE,
  ANCS_ATTR_SUBTITLE,
  ANCS_ATTR_MESSAGE,

  ANCS_ATTR_MSG_SIZE,
  ANCS_ATTR_DATE,
  ANCS_ATTR_POS_ACT_LABEL,
  ANCS_ATTR_NEG_ACT_LABEL
};
extern uint8_t gCallState;

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/// Application event handler.
MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams);

#endif // MY_APP_H
