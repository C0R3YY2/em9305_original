////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/emdt/profile/server/includes/emdtp_server.h
///
/// @project    T9305
///
/// @brief      EM Data Transfer Profile - Server side - header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic-Marin SA
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

#ifndef EMDTP_SERVER_H
#define EMDTP_SERVER_H

#include <att_api.h>
#include <emb_dm_api.h>
#include <stdint.h>
#include <wsf_os.h>

/**
 * @addtogroup emb_vs
 * @{
 * @defgroup emdtp_server EM Data Transfer Profile - Server side
 * @brief Proprietary profile definition example.
 * @{
 */

/******************************************************************************\
 *  Types
\******************************************************************************/

/** @brief Module configuration structure. */
typedef struct
{
    uint8_t dataCharCccIdx; /**< Index of the data characteristic in the client characteristic configuration table (attsCccSet_t) */
    void   *pUserData;      /**< User-defined parameter that is returned when the callback is invoked. */
} EmdtpServer_Cfg_t;

/** @brief Modules events enumeration. */
typedef enum
{
    EMDTPSERVER_NOTIFICATION_STARTED_EVENT, /**< Notification of the Data Characteristic started. */
    EMDTPSERVER_NOTIFICATION_STOPPED_EVENT, /**< Notification of the Data Characteristic stopped. */
    EMDTPSERVER_NOTIFICATION_QUEUED_EVENT, /**< Notification of the Data Characteristic queued. */
} EmdtpServer_EventType_t;

/** @brief Structure of the notification started event parameters. */
typedef struct
{
    dmConnId_t connId; /**< Connection ID */
} EmdtpServer_NotifStartedEvt_t;

/** @brief Structure of the notification stopped event parameters. */
typedef struct
{
    dmConnId_t connId; /**< Connection ID */
} EmdtpServer_NotifStoppedEvt_t;

/** @brief Structure of the notification queued event parameters. */
typedef struct
{
    uint8_t    attStatus; /**< ATT error codes or HCI error codes augmented by ATT_HCI_ERR_BASE */
    dmConnId_t connId; /**< Connection ID */
} EmdtpServer_NotifQueuedEvt_t;

/**
 * @brief Module event structure.
 */
typedef struct
{
    /** Type of profile event */
    EmdtpServer_EventType_t type;

    /** Parameters of the profile event */
    union
    {
        EmdtpServer_NotifStartedEvt_t notifStarted;
        EmdtpServer_NotifStoppedEvt_t notifStopped;
        EmdtpServer_NotifQueuedEvt_t  notifQueued;
    } params;
} EmdtpServer_Event_t;

/**
 * @brief Type definition of the module callback function.
 */
typedef void (*EmdtpServer_Cback_t)(EmdtpServer_Event_t *pEvt, void *pUserData);

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Initialize the EM Data Transfer Profile, server side.
 * @param pCfg Configuration of the module.
 * @param callback Callback function provided by the user.
 */
void EmdtpServer_Init(EmdtpServer_Cfg_t  *pCfg,
                      EmdtpServer_Cback_t callback);

/**
 * @brief Internal processing of some messages.
 * @param pMsg Pointer to message.
 */
void EmdtpServer_ProcMsg(wsfMsgHdr_t *pMsg);

/** @} */ // End of group emdtp_server
/** @} */ // End of addtogroup emb_vs

#endif /* EMDTP_SERVER_H */
