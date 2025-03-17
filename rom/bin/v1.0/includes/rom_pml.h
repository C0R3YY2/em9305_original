////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/rom_pml.h
///
/// @project    T9305
///
/// @brief      Power Manager Logic driver in ROM
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020, EM Microelectronic
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


#ifndef _ROM_PML_H_
#define _ROM_PML_H_

#include <types.h>
#include <t9305_periph.h>


/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/**
 * @brief  HF clock frequency
 */
typedef enum
{
    PML_HF_CLK_FREQ_24MHZ = 0,      /**< 24MHz */
    PML_HF_CLK_FREQ_48MHZ = 1,      /**< 48MHz */
} PML_HfClkFrequency_t;


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * @brief Set HF clock frequency configuration.
 * @param freq HF clock frequency to be set.
 */
void PML_SetHfClkFrequency(PML_HfClkFrequency_t freq);


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

extern volatile uint16_t gPmlResetFlags;


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Get current HF clock frequency.
 * @returns Current HF clock frequency configuration.
 */
static ALWAYS_INLINE PML_HfClkFrequency_t PML_GetHfClkFrequency(void)
{
    if (1 == GET_CLK_SEL_FREQ(SYS->RegClkCfg.r32))
    {
        return PML_HF_CLK_FREQ_48MHZ;
    }
    else
    {
        return PML_HF_CLK_FREQ_24MHZ;
    }
}


/**
 * @brief Get reset flags which caused last chip reset
 * @return reset flags
 */
static ALWAYS_INLINE uint16_t PML_GetResetFlags()
{
    return gPmlResetFlags;
}


/**
 * @brief Get if the chip has booted from POR.
 * @return TRUE if the chip booted from POR, FALSE otherwise.
 */
static ALWAYS_INLINE uint8_t PML_DidBootFromPOR()
{
    return (0 != (PML_GetResetFlags() & PML_POR_FLG_MASK));
}


/**
 * @brief Get if the chip has booted from sleep.
 * @return TRUE if the chip booted from sleep, FALSE otherwise.
 */
static ALWAYS_INLINE uint8_t PML_DidBootFromSleep()
{
    return (0 != (PML_GetResetFlags() & PML_SLEEP_FLG_MASK));
}


/**
 * @brief Get if the chip has booted from reset.
 * @return TRUE if the chip booted from reset, FALSE otherwise.
 */
static ALWAYS_INLINE uint8_t PML_DidBootFromReset()
{
    return (0 != (PML_GetResetFlags() & ( PML_PWR_CHK_FLG_MASK |
                                          PML_SW_RST_FLG_MASK  |
                                          PML_WD_FLG_MASK      |
                                          PML_NVM_BO_FLG_MASK  |
                                          PML_DEEP_SLEEP_FLG_MASK )));
}


/**
 * @brief Get the state of the Configuration Mode flag.
 * @returns Return the state of the Configuration Mode flag.
 */
static ALWAYS_INLINE bool PML_GetConfigurationModeFlag(void)
{
    // Return RegPmlSysMode->PmlSpare(7).
    return (PML->RegPmlSysMode.r32 & 0x00800000u) != 0u;
}


/**
 * @brief Set the Configuration Mode flag.
 */
static ALWAYS_INLINE void PML_SetConfigurationModeFlag(void)
{
    // Set RegPmlSysMode->PmlSpare(7).
    PML->RegPmlSysMode.r32 |= 0x00800000u;
}


/**
 * @brief Clear the Configuration Mode flag.
 */
static ALWAYS_INLINE void PML_ClearConfigurationModeFlag(void)
{
    // Clear RegPmlSysMode->PmlSpare(7).
    PML->RegPmlSysMode.r32 &= ~0x00800000u;
}


#endif // _ROM_PML_H_
