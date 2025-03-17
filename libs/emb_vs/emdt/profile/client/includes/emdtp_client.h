////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/emb_vs/emdt/profile/client/includes/emdtp_client.h
///
/// @project    T9305
///
/// @brief      EM Data Transfer Profile - Client side - header file.
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

#ifndef EMDTP_CLIENT_H
#define EMDTP_CLIENT_H

#include <att_api.h>

/**
 * @addtogroup emb_vs
 * @{
 * @defgroup emdtp_client EM Data Transfer Profile - Client side
 * @brief Proprietary profile definition example.
 * @{
 */

/******************************************************************************\
 *  Types
\******************************************************************************/

/** @brief EMM proprietary service  enumeration of handle indexes of characteristics to be discovered */
enum
{
    EMM_P_DAT_HDL_IDX,      // Proprietary data
    EMM_P_NA_CCC_HDL_IDX,   // Proprietary data client characteristic configuration descriptor
    EMM_P_HDL_LIST_LEN      // Handle list length
};

/** @brief Module configuration structure. */
typedef struct
{
    void   *pUserData;     /**< User-defined parameter that is returned when the callback is invoked. */
} EmdtpClient_Cfg_t;

/** @brief Modules events enumeration. */
typedef enum
{
    EMDTPCLIENT_TRANSFER_DATA_EVENT,    /**< Event generated when the application receives data from the remote data transfer service. */
    EMDTPCLIENT_TRANSFER_ENABLED_EVENT, /**< Event generated when the application successfully enables the data transfer service on the remote server. */
    EMDTPCLIENT_TRANSFER_DISABLED_EVENT /**< Event generated when the application successfully disables the data transfer service on the remote server. */
} EmdtpClient_EventType_t;

/** @brief Structure of the notification started event parameters. */
typedef struct
{
    /// Pointer to the att notification
    attEvt_t *pEvt;
} EmdtpClient_ValueNotifEvt_t;

typedef struct
{
    /// Pointer to the att notification
    attEvt_t *pEvt;
} EmdtpClient_WriteRspEvt_t;

/**
 * @brief Module event structure.
 */
typedef struct
{
    /** Type of profile event */
    EmdtpClient_EventType_t type;

    /** Parameters of the profile event */
    union
    {
        EmdtpClient_WriteRspEvt_t writeRsp;
        EmdtpClient_ValueNotifEvt_t notif;
    } params;
} EmdtpClient_Event_t;

/**
 * @brief Type definition of the module callback function.
 */
typedef void (*EmdtpClient_Cback_t)(EmdtpClient_Event_t *pEvt, void *pUserData);

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Initialize the EM Data Transfer Profile, client side.
 * @param pCfg Configuration of the module.
 * @param callback Callback function provided by the user.
 */
void EmdtpClient_Init(EmdtpClient_Cfg_t *cfg, EmdtpClient_Cback_t callback);

/**
 * @brief  Called by application to notify the EMDTP of System Events.
 * @param pMsg   Pointer to the Event
 */
void EmdtpClient_ProcMsg(wsfMsgHdr_t *pEvt);

/**
 * @brief Perform service and characteristic discovery for EM Datatransfer service.
 *        Parameter pHdlList must point to an array of length \ref EMM_P_HDL_LIST_LEN.
 *        If discovery is successful the handles of discovered characteristics and
 *        descriptors will be set in pHdlList.
 * @param pMsg Pointer to message.
 */
void EmdtpClient_Discover(dmConnId_t connId, uint16_t *pHdlList);

/**
 *  @brief Enable EM Data Transfer notifications.
 *  @param connId Connection identifier.
 */
void EmdtpClient_StartDataTransfer(dmConnId_t connId);

/**
 *  @brief Disable EM Data Transfer notifications.
 *  @param connId Connection identifier.
 */
void EmdtpClient_StopDataTransfer(dmConnId_t connId);

/**
 *  @brief Set service and characteristic handles.
 *  @param connId Connection identifier.
 *  @param pHdlList Characteristic handle list.
 *  @note This function can set restored handles after restablishing a link with a bonded device.
 */
void EmdtpClient_SetHandles(dmConnId_t connId, uint16_t* pHdlList);

/** @} */ // End of group emdtp_client
/** @} */ // End of addtogroup emb_vs

#endif /* EMDTP_CLIENT_H */
