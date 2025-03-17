////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_rscps.h
///
/// @project    T9305
///
/// @brief      Running Speed and Cadence Profile Sensor
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023-2024, EM Microelectronic-Marin SA
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

#ifndef EM_RSCPS_H
#define EM_RSCPS_H

#include <wsf_timer.h>
#include <dm_api.h>
#include <wsf_os.h>

/**
 * @addtogroup emb_vs
 * @{
 * @defgroup em_rscps EM Running Speed and Cadence Profile Sensor
 * @brief Running Speed and Cadence Profile/Service.
 * @{
 */

/******************************************************************************\
 *  Types
\******************************************************************************/

/**
 * @brief Configurable parameters.
 */
typedef struct
{
    /// Measurement timer expiration period in ms.
    wsfTimerTicks_t period;
} RscpsCfg_t;


/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/**
 * @brief Initialize the Running Speed and Cadence profile sensor.
 * @param handerId WSF handler ID of the application using this service.
 * @param pCfg Configurable parameters.
 */
void RSCPS_Init(wsfHandlerId_t handlerId, RscpsCfg_t *pCfg);

/**
 * @brief Start periodic Running Speed and Cadence measurement.
 *        This function starts a timer to perform periodic measurements.
 * @param connId DM connection identifier.
 * @param timerEvt WSF event designated by the application for the timer.
 * @param rscsmCccIdx Index of RSC SM CCC descriptor in descriptor handle table.
 */
void RSCPS_MeasStart(dmConnId_t connId, uint8_t timerEvt, uint8_t rscsmCccIdx);

/**
 * @brief Stop periodic heart rate measurement.
 * @param connId DM connection identifier.
 */
void RSCPS_MeasStop(dmConnId_t connId);

/**
 * @brief Process received WSF message.
 * @param pMsg Event message.
 */
void RSCPS_ProcMsg(wsfMsgHdr_t *pMsg);

/** @} */ // End of group em_rscps
/** @} */ // End of addtogroup emb_vs

#endif // EM_RSCPS_H
