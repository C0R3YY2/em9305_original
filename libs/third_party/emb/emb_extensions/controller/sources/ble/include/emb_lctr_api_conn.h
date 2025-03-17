////////////////////////////////////////////////////////////////////////////////
///
/// @file       emb_lctr_api_conn.h
///
/// @project    EMB
///
/// @brief      EM Bleu Extensions over Packetcraft Lower Controller APIs
///             regarding connections
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

#ifndef __EMB_LCTR_API_CONN_H
#define __EMB_LCTR_API_CONN_H

#include "lctr_api_conn.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/
/*!
 *  \brief  Get current connection event counter value of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return Connection Event Counter
 */
/*************************************************************************************************/
uint16_t EMB_LctrGetConnEventCounter(uint16_t handle);

/*************************************************************************************************/
/*!
 *  \brief  Get the latest anchor time for a slave connection
 *
 *  \param  handle Connection handle.
 *  \param  pAnchorTimeUs pointer to location to store the anchor time to.
 *  \param  pEventCounter pointer to location to store the event counter that
 *          is associated with the given anchor time.
 *  \return True on success, false on failure.
 *
 *  The anchor time is intended to provide information to be able to
 *  sync the central's time domain with the peripheral's. The anchor time is
 *  a timestamp provided by the protocol timer.
 *
 */
/*************************************************************************************************/
bool EMB_LctrGetConnSlaveAnchorTime(uint16_t handle, uint32_t *pAnchorTimeUs,
    uint16_t *pEventCounter);

/*************************************************************************************************/
/*!
 *  \brief  Get missed connection event counter value of a connection.
 *
 *  \param  handle      Connection handle.
 *
 *  \return Missed Connection Event Counter
 */
/*************************************************************************************************/
uint32_t EMB_LctrGetMissedConnEventCounter(uint16_t handle);

#endif // __EMB_LCTR_API_CONN_H
