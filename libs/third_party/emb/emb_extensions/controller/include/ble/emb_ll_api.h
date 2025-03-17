////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_ll_api.h
///
/// @project    T9305
///
/// @brief      EM Bleu Extensions over Packetcraft Link Layer APIs
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

#ifndef __EMB_LL_API_H
#define __EMB_LL_API_H

#include <stdint.h>

#define USE_EMB_EXTENSIONS_LL    1

/*! \brief      EM Bleu specific LL runtime configuration parameters. */
typedef struct
{
  bool    enhancedPowerControl;               /*!< Enhanced Power Control enabled. */
} EmbSpecLlRtCfg_t;


/*************************************************************************************************/
/*!
 *  \brief      Get Connection Event Counter value of a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pEventCounter   Connection event counter.
 *
 *  \return     Link layer status code.
 *
 *  Get the current Connection Event Counter value of a connection.
 */
/*************************************************************************************************/
uint16_t EMB_LlGetConnEventCounter(uint16_t handle, uint16_t *pEventCounter);

/*************************************************************************************************/
/*!
 *  \brief  Get the latest anchor time for a slave connection
 *
 *  \param  handle Connection handle.
 *  \param  pAnchorTimeUs pointer to location to store the anchor time to.
 *  \param  pEventCounter pointer to location to store the event counter that
 *          is associated with the given anchor time.
 *  \return LL status code (LlStatus_t)
 *
 *  The anchor time is intended to provide information to be able to
 *  sync the central's time domain with the peripheral's. The anchor time is
 *  a timestamp provided by the protocol timer.
 *
 */
/*************************************************************************************************/
uint8_t EMB_LlGetConnSlaveAnchorTime(uint16_t handle, uint32_t *pAnchorTimeUs,
    uint16_t *pEventCounter);

/*************************************************************************************************/
/*!
 *  \brief      Get Missed Connection Event Counter value of a connection.
 *
 *  \param      handle          Connection handle.
 *  \param      pEventCounter   Missed connection event counter.
 *
 *  \return     Link Layer status code.
 *
 *  Get the Missed Connection Event Counter value of a connection.
 */
/*************************************************************************************************/
uint16_t EMB_LlGetMissedConnEventCounter(uint16_t handle, uint32_t *pEventCounter);

/*************************************************************************************************/
/*!
 *  \brief      Enable the generation of LL_ADV_COMPLETE_IND.
 *
 *  \param      enable  Set to TRUE to enable advertising complete reports, FALSE to disable.
 *
 *  This indication is generated after every advertisement (legacy advertising).
 */
/*************************************************************************************************/
void EMB_LLAdvCompleteIndEnable(uint8_t enable);

#endif // __EMB_LL_API_H
