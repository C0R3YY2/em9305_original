////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_hci_vs_api.h
///
/// @project    T9305
///
/// @brief      EM Bleu Extensions over PacketCraft Host HCI Vendor Specific API
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

#ifndef __EMB_HCI_VS_API_H
#define __EMB_HCI_VS_API_H

#include <stdint.h>
#include "emb_dm_api.h"
#include <ll_api.h>
#include <lhci_api.h>

#define USE_EMB_EXTENSIONS_HCIVS    1

/*************************************************************************************************/
/*!
 *  \brief  Return the connection event counter for a given connection handle. If handle is not
 *          associated with an active connection, return 0
 *
 *  \param handle Connection Handle
 *  \return Connection Event Counter Value
 */
/*************************************************************************************************/
uint16_t EMB_HciVsReadConnEventCounter(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Get the latest anchor time for a slave connection
 *
 *  \param  handle Connection handle.
 *  \param  pAnchorTimeUs pointer to location to store the anchor time to.
 *  \param  pEventCounter pointer to location to store the event counter that
 *          is associated with the given anchor time.
 *  \return LL status code, LL_SUCCESS on success.
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
 */
/*************************************************************************************************/
LlStatus_t EMB_HciVsGetConnSlaveAnchorTime(uint16_t handle, uint32_t *pAnchorTimeUs,
    uint16_t *pEventCounter);

/*************************************************************************************************/
/*!
 *  \brief      HCI Set Transmit Power command for Enhanced Power Control.
 *
 *  \param      connHandle    Connection handle.
 *  \param      reqState      LE State to apply the TX Power.
 *  \param      phyChange     Apply to All PHYs if value equal to 0xFF or apply to specific PHY.
 *  \param      txPower       TX power to set in dBm.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void EMB_HciVsLeSetTxPower(uint16_t connHandle, EMB_LEStates reqState, uint8_t phyChange, int8_t txPower);

/*************************************************************************************************/
/*!
 *  \brief  Return the missed connection event counter for a given connection handle. If handle is not
 *          associated with an active connection, return 0
 *
 *  \param handle Connection Handle
 *  \return Missed Connection Event Counter Value
 */
/*************************************************************************************************/
uint32_t EMB_HciVsReadMissedConnEventCounter(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Decode EMB Extensions vendor specific HCI command packet.
 *
 *  \param  pHdr    Unpacked command header.
 *  \param  pBuf    Packed HCI packet buffer.
 *
 *  Command processing for vendor specific commands.
 *
 *  \return true if opcode handled, false otherwise.
 */
/*************************************************************************************************/
bool EMB_lhciVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Enable the generation of LL_ADV_COMPLETE_IND. This indication is generated after
 *              every advertisement (legacy advertising).
 *
 *  \param      enable  Set to TRUE to enable advertising complete reports, FALSE to disable.
 *
 */
/*************************************************************************************************/
void EMB_HciVsAdvCompleteIndEnable(uint8_t enable);

/*************************************************************************************************/
/*!
 *  \brief      Set connectionless Tx CTE parameters in the Link Layer
 *
 *  \param      advHandle             Advertising handle.
 *  \param      connlessCteTxParam    Connectionless CTE transmit parameters.
 *
 */
/*************************************************************************************************/
void EMB_HciVsSetConnlessCteTxParams(uint8_t advHandle, LlConnLessCteTxParam_t connlessCteTxParam);

/*************************************************************************************************/
/*!
 *  \brief      Enable connectionless Tx CTE in the Link Layer
 *
 *  \param      advHandle          Advertising handle.
 *  \param      enable             Enable flag.
 *
 */
/*************************************************************************************************/
void EMB_HciVsSetConnlessCteTxEnable(uint8_t advHandle, uint8_t enable);
#endif // __EMB_HCI_VS_API_H
