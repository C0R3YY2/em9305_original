////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/aoad/includes/aoad_hal.h
///
/// @project    T9305
///
/// @brief      AOAD hardware abstraction layer.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2023 EM Microelectronic
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


#ifndef AOAD_HAL_H
#define AOAD_HAL_H


#include <t9305_periph.h>


/**
 * @addtogroup drivers
 * @{
 * @defgroup aoad_hal AOAD HAL Driver
 * @brief AOAD Driver for the AOAD.
 *
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/

/**
 * @brief Set AOAD Control
 * @param ctrl control bits for AOAD configuration
 */
static ALWAYS_INLINE void AOAD_HAL_SetCtrl(uint32_t ctrl)
{
    AOAD->RegAoadCtrl.r32 = ctrl;
}

/**
 * @brief Set AOAD Matrix Switching period
 * @param period period can be 0 = 1us or 1 = 2us
 */
static ALWAYS_INLINE void AOAD_HAL_SetMatrixSwitchingPeriod(uint8_t period)
{
    AOAD->RegAoadCtrl.r32 = (AOAD->RegAoadCtrl.r32 & ~AOAD_SW_PER_MASK) | AOAD_SW_PER(period);
}

/**
 * @brief Get AOAD Matrix Switching period
 * @return period period can be 0 = 1us or 1 = 2us
 */
static ALWAYS_INLINE uint8_t AOAD_HAL_GetMatrixSwitchingPeriod(void)
{
    return GET_AOAD_SW_PER(AOAD->RegAoadCtrl.r32);
}


/**
 * @brief Set AOAD Matrix Switching enable
 * @param enable enable the matrix switching
 */
static ALWAYS_INLINE void AOAD_HAL_SetMatrixSwitchingEnable(uint8_t enable)
{
    AOAD->RegAoadCtrl.r32 = (AOAD->RegAoadCtrl.r32 & ~AOAD_SW_EN_MASK) | AOAD_SW_EN(enable);
}

/**
 * @brief Set AOAD Switching count of antenna
 * @param number number of switch of antenna to be done
 */
static ALWAYS_INLINE void AOAD_HAL_SetSwitchingNumber(uint8_t number)
{
    AOAD->RegAoadCtrl.r32 = (AOAD->RegAoadCtrl.r32 & ~AOAD_SW_NUM_MASK) | AOAD_SW_NUM(number);
}

/**
 * @brief Set AOAD Matrix duration
 * @param duration duration of reference period time in number of 2us cycles
 */
static ALWAYS_INLINE void AOAD_HAL_SetMatrixDuration(uint8_t duration)
{
    AOAD->RegAoadCtrl.r32 = (AOAD->RegAoadCtrl.r32 & ~AOAD_REF_TIME_MASK) | AOAD_REF_TIME(duration);
}

/**
 * @brief Set AOAD Matrix Switch Control Mode
 * @param mode mode of control switch, can be SW or HW
 */
static ALWAYS_INLINE void AOAD_HAL_SetMatrixSwitchControlMode(uint8_t mode)
{
    AOAD->RegAoadCtrl.r32 = (AOAD->RegAoadCtrl.r32 & ~AOAD_CTRL_MODE_MASK) | AOAD_CTRL_MODE(mode);
}

/**
 * @brief Set AOAD Matrix Enable
 * @param enable enable matrix
 */
static ALWAYS_INLINE void AOAD_HAL_SetMatrixEnable(uint8_t enable)
{
    AOAD->RegAoadCtrl.r32 = (AOAD->RegAoadCtrl.r32 & ~AOAD_EN_MASK) | AOAD_EN(enable);
}

/**
 * @brief Set AOAD GPIO Sel 0
 * @param gpio Defines which LUT bit is selected for GPIO0 to GPIO3
 */
static ALWAYS_INLINE void AOAD_HAL_SetGpioSel0(uint32_t gpio)
{
    AOAD->RegAoadGpioSel0.r32 = gpio;
}

/**
 * @brief Set AOAD GPIO Sel 1
 * @param gpio Defines which LUT bit is selected for GPIO4 to GPIO7
 */
static ALWAYS_INLINE void AOAD_HAL_SetGpioSel1(uint32_t gpio)
{
    AOAD->RegAoadGpioSel1.r32 = gpio;
}

/**
 * @brief Set AOAD GPIO Sel 2
 * @param gpio Defines which LUT bit is selected for GPIO8 to GPIO11
 */
static ALWAYS_INLINE void AOAD_HAL_SetGpioSel2(uint32_t gpio)
{
    AOAD->RegAoadGpioSel2.r32 = gpio;
}

/** @} */ // End of group aoad_hal
/** @} */ // End of addtogroup drivers


#endif /* AOAD_HAL_H */
