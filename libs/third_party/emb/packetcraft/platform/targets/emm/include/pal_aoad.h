////////////////////////////////////////////////////////////////////////////////
///
/// @file       pal_aoad.h
///
/// @project    EMB
///
/// @brief      EMB AOAD Driver Abstraction
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

#ifndef PAL_AOAD_H
#define PAL_AOAD_H

#include "types.h"


/**
 * @brief Call platform set Look Up table for Antenna Switch
 * @param antIdRef antenna id for reference period
 * @param pAntId antenna id pattern
 * @param swPattLen length of antenna id pattern
 */
void PalAoADSetMatrixLookupTable(uint8_t antIdRef, const uint8_t *pAntId, uint8_t swPattLen);

/**
 * @brief Call platform Enable / Disable Antenna Switch matrix
 * @param antennaSwitchEnable enable antenna switching
 * @param aoaRx2us true if switching time period is 2 us
 * @param cteType CTE type, 0 = AoA, 1 = AoD 1 us, 2 = AoD 2 us
 * @param dataChannelPdu If true Data chanel PDU format is used to decode
 * CTEInfo field in packet, otherwise Advertising channel PDU format is used to
 * decode CTEInfo field in packet.
 */
void PalAoADAntennaSwitchEnable(bool antennaSwitchEnable, bool aoaRx2us,
                                     uint8_t cteType, bool dataChannelPdu);
#endif /* PAL_AOAD_H */
