////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_dm_api.h
///
/// @project    EMB
///
/// @brief      EM Bleu Extensions over PacketCraft Host Device Manager API
///
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2024 EM Microelectronic
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

#ifndef __EMB_DM_API_H
#define __EMB_DM_API_H

#include <stdint.h>
#include <ll_api.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define USE_EMB_EXTENSIONS_DM   1

/**************************************************************************************************
  Data Types
**************************************************************************************************/
enum
{
    SET_POWER_SUCCESS               = 0,
    LEPC_UNSUPPORTED                = 1,
    APP_TX_POWER_CHANGE_NOT_ALLOWED = 2,
    MIN_POWER_UNSUPPORTED           = 3,
    MAX_POWER_NOT_ALLOWED           = 4,
    MAX_TX_POWER_ALREADY_SET        = 5,
    REQ_TX_POWER_NOT_ALLOWED        = 6,
    REQ_TX_POWER_ALREADY_SET        = 7
};
typedef uint8_t EMB_LEPCRetVal;

enum
{
    EMB_BLE_STATE_ADVERTISING,
    EMB_BLE_STATE_SCANNING,
    EMB_BLE_STATE_CONNECTED,
    EMB_BLE_STATE_ISOC_BROADCASTING,
    EMB_BLE_STATE_STANDBY,
    EMB_BLE_STATE_MAX
};
typedef uint8_t EMB_LEStates;

/*! \brief Connection identifier. */
typedef uint8_t dmConnId_t;

/*************************************************************************************************/
/*!
 *  \brief  Initialize Manual Power Control (PC) module.
 *
 *  \return None.
 */
/*************************************************************************************************/
void EMB_ManualPcInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Return the connection event counter for a given connection handle. If handle is not
 *          associated with an active connection, return 0
 *
 *  \param connId        Connection identifier
 *
 *  \return Connection Event Counter Value
 */
/*************************************************************************************************/
uint16_t EMB_DmConnReadConnEventCounter(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief      Enable Radio Transmit Power Management flag.
 *
 *  \param      None.
 *
 *  \return     None.
 *
 *  Enable the Radio Transmit Power Management flag. Even when Enhanced Power Control is disabled
 *  this flag needs to be enabled to allow changing transmit power by the user.
 */
/*************************************************************************************************/
void EMB_LlEnableRadioTxPowerMgmt(void);

/*************************************************************************************************/
/*!
 *  \brief      Check if LEPC feature bits are enabled in the stack
 *
 *  \param      None
 *
 *  \return     Return code True - Enabled, False - Disabled.
 */
/*************************************************************************************************/
bool EMB_DmIsLEPCEnabled(void);

/*************************************************************************************************/
/*!
 *  \brief      The API can only be used if the Enhanced Power control is disabled feature in the stack.
 *              Set Transmit power requested for Enhanced Power Control.
 *
 *  \param      connId           Connection identifier
 *  \param      reqState         BLE state for the requested power change.
 *  \param      phyChange        Requested change for certain PHY or all PHYs.
 *  \param      reqTxPower       Power Requested to be changed in dBm.
 *
 *  \return     EMB_LEPCRetVal   Return codes for success or failure.
 */
/*************************************************************************************************/
EMB_LEPCRetVal EMB_DmLEPCSetTxPower(dmConnId_t connId, EMB_LEStates reqState, uint8_t phyChange, int8_t reqTxPower);

/*************************************************************************************************/
/*!
 *  \brief      Set the absolute minimum and maximum Tx power to be used by the radio.
 *
 *
 *  \param      minTxPwr        Minimum Transmit power that can be used in dBm.
 *  \param      maxTxPwr        Maximum Transmit power that can be used in dBm.
 *
 *  \return     EMB_LEPCRetVal  Return codes for success or failure.
 *
 */
/*************************************************************************************************/
EMB_LEPCRetVal EMB_DmSetTxMinMaxPower(int8_t minTxPwr, int8_t maxTxPwr);

/*************************************************************************************************/
/*!
 *  \brief  Get the latest anchor time for a slave connection
 *
 *  \param  connId Connection identifier.
 *  \param  pAnchorTimeUs pointer to location to store the anchor time to.
 *  \param  pEventCounter pointer to location to store the event counter that
 *          is associated with the given anchor time.
 *  \return LL status code
 *
 *  The anchor time is intended to provide information to be able to
 *  sync the central's time domain with the peripheral's. The anchor time is
 *  a timestamp provided by the protocol timer.
 *
 *  \warning The anchor time is read from the context from the LCTR layer. In
 *  some states (missed connection event, etc...) the information in the context
 *  is not adequate to guarantee a correct anchor time. Therefore, it is
 *  imperative to check the return code. Additionally, the user app should check
 *  the anchor time and store the results regularly to be sure that the connection
 *  timing between central and peripheral can always be correlated without another
 *  call to this function, since it may fail until a connection event goes well.
 *
 */
/*************************************************************************************************/
LlStatus_t EMB_DmGetConnSlaveAnchorTime(dmConnId_t connId, uint32_t *anchorTimeUs,
    uint16_t *eventCounter);

/*************************************************************************************************/
/*!
 *  \brief  Return the missed connection event counter for a given connection handle. If handle is not
 *          associated with an active connection, return 0
 *
 *  \param connId        Connection identifier
 *
 *  \return Missed Connection Event Counter Value
 */
/*************************************************************************************************/
uint32_t EMB_DmConnReadMissedConnEventCounter(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief      Enable the generation of LL_ADV_COMPLETE_IND. This indication is generated after
                every advertisement (legacy advertising).
 *
 *  \param      enable  Set to TRUE to enable advertising complete reports, FALSE to disable.
 *
 */
/*************************************************************************************************/
void EMB_DmAdvCompleteIndEnable(uint8_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Set connectionless Tx CTE parameters
 *
 *  \param      advHandle             Advertising handle.
 *  \param      connlessCteTxParam    Connectionless CTE transmit parameters.
 *
 */
/*************************************************************************************************/
void EMB_DmSetConnlessCteTxParams(uint8_t advHandle, LlConnLessCteTxParam_t connlessCteTxParam);

/*************************************************************************************************/
/*!
 *  \brief      Enable connectionless Tx CTE
 *
 *  \param      advHandle          Advertising handle.
 *  \param      enable             Enable flag.
 *
 */
/*************************************************************************************************/
void EMB_DmSetConnlessCteTxEnable(uint8_t advHandle, uint8_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Enable connectionless CTE Transmitter Initialization
 *
 */
/*************************************************************************************************/
void EMB_DmConnLessCteTransmitterInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Enable connectionless CTE Receiver Initialization
 *
 */
/*************************************************************************************************/
void EMB_DmConnLessCteReceiverInit(void);
#endif // __EMB_DM_API_H
