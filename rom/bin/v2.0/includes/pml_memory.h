////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml_memory.h
///
/// @project    T9305
///
/// @brief      PML driver memory.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2022, EM Microelectronic
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


#ifndef _PML_MEMORY_H
#define _PML_MEMORY_H


#include "types.h"
#include "pml.h"


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/**
 * @brief Persistent memory for the PML module.
 */
typedef struct
{
    /// Boot overhead time [hf clk]
    uint32_t overheadBootHfClk;
    /// Sleep overhead time [hf clk]
    uint32_t overheadSleepHfClk;
    /// Actual RC calibration constant for HF clk
    uint32_t actualRcCalibHfConst;
    /// Actual RC calibration constant for LF clk
    uint32_t actualRcCalibLfConst;
    /// last RC calibration (state of Sleep counter)
    uint32_t lastRcCalibration;
    /// wakeup time to be precised after rc calibration
    uint32_t wakeupTimeInHfClk[4];
    /// LF XTAL is connected
    uint8_t lfXtalIsPresent;
    /// LF XTAL is ready to use
    uint8_t lfXtalIsReady;
    /// Skip RC calibration and use XTAL calibration constants (freq)
    uint8_t rcCalibSkip;
    /// Automatic Sleep mode transition disabled
    uint8_t automaticSleepDisable;
    /// Flag indicating that first RC calibration already done
    bool firstRcCalidDone;
    /// reserved
    uint8_t reserved[2];
}  PML_PersistentMemory_t;



/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/


// This is defined as an extern to a uint32_t array to prevent it's use within
// the module. Using this within the module would make the memory address
// hardcoded within the ROM.
#ifndef PML_MEMORY_INTERNAL

/**
 * @brief Reference to the persistent memory for the PML module.
 * @note Do not use this directly.
 */
extern char gPML_PersistentMemory[];

#endif // PML_MEMORY_INTERNAL


#endif /* _PML_MEMORY_H */
