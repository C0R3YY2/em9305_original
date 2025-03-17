////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/pml/includes/pml_private.h
///
/// @project    T9305
///
/// @brief      PML driver internal function prototypes.
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


#ifndef _PML_PRIVATE_H
#define _PML_PRIVATE_H


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

// TODO 13.12.2022 define not used in the code. To be decided if it should be removed or not
#define PML_AUTO_SLEEP_ENABLE_HCI       (0x01u)
#define PML_AUTO_SLEEP_ENABLE_LL        (0x02u)


/******************************************************************************\
 *  INTERNAL FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief PML initialize
 */
void PML_Init(void);


/**
 * @brief PML store config before sleep
 */
void PML_StoreConfig(void);


/**
 * @brief PML restore config
 */
void PML_ReStoreConfig(void);

/**
 * @brief PML Enable LF-RC Low Power
 */
void PML_Enable_LF_RC_LP(void);

/**
 * @brief PML Disable LF-RC Low Power
 */
void PML_Disable_LF_RC_LP(void);

/**
 * @fn PML_GetLdoAntTrimLockConfig()
 * @brief This function reads the actual value used to configure the LDO ANT trim lock feature.
 * @return Returns the value used to configure the LDO ANT trim lock feature
 */
static ALWAYS_INLINE uint8_t PML_GetLdoAntTrimLockConfig(void)
{
    return ((uint8_t)((PML->RegPmlLockBits.r32 & PML_LDO_ANT_TRIM_LOCK_MASK) >> PML_LDO_ANT_TRIM_LOCK_SHIFT));
}

/**
 * @fn PML_GetTxPaPwrLockConfig()
 * @brief This function reads the actual value used to configure the PA PWR lock feature.
 * @return Returns the value used to configure the PA PWR lock feature
 */
static ALWAYS_INLINE uint8_t PML_GetTxPaPwrLockConfig(void)
{
    return ((uint8_t)((PML->RegPmlLockBits.r32 & PML_TX_PA_PWR_LOCK_MASK) >>  PML_TX_PA_PWR_LOCK_SHIFT));
}

#endif /* _PML_PRIVATE_H */
