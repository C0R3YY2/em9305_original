////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/em_hw_api/includes/boot.h
///
/// @project    T9305
///
/// @brief      Boot functions
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

#ifndef _BOOT_H_
#define _BOOT_H_

#include "types.h"
#include "t9305_periph.h"
#include "pml.h"


/******************************************************************************\
 *  MACRO
\******************************************************************************/

#define BOOT_ACTION_FLAGS_GPIO_SHIFT        0                           /// Shift for GPIO action flags
#define BOOT_ACTION_FLAGS_GPIO_MASK         0x00000FFF                  /// Mask for GPIO action flags
#define GET_BOOT_ACTION_FLAGS_GPIO(reg)     (((reg)>>0)&0xFFFu)         /// Get GPIO action flags
#define SET_BOOT_ACTION_FLAGS_GPIO(x)       (((x)&0xFFFu)<<0)           /// Set GPIO action flags

#define BOOT_ACTION_FLAGS_SLEEP_SHIFT       16                          /// Shift for Sleep Timer action flags
#define BOOT_ACTION_FLAGS_SLEEP_MASK        0x000F0000                  /// Mask for Sleep Timer action flags
#define GET_BOOT_ACTION_FLAGS_SLEEP(reg)    (((reg)>>16)&0xFu)          /// Get Sleep Timer action flags
#define SET_BOOT_ACTION_FLAGS_SLEEP(x)      (((x)&0xFu)<<16)            /// Set Sleep Timer action flags

#define BOOT_ACTION_FLAGS_QDEC_SHIFT        24                          /// Shift for QDec action flags
#define BOOT_ACTION_FLAGS_QDEC_MASK         0x00700000                  /// Mask for QDec action flags
#define GET_BOOT_ACTION_FLAGS_QDEC(reg)     (((reg)>>24)&0x7u)          /// Get QDec action flags
#define SET_BOOT_ACTION_FLAGS_QDEC(x)       (((x)&0x7u)<<24)            /// Set QDec action flags

#define BOOT_ACTION_FLAG_GPIO0_MASK         0x00000001                  /// Mask for GPIO0 action flag
#define BOOT_ACTION_FLAG_GPIO1_MASK         0x00000002                  /// Mask for GPIO1 action flag
#define BOOT_ACTION_FLAG_GPIO2_MASK         0x00000004                  /// Mask for GPIO2 action flag
#define BOOT_ACTION_FLAG_GPIO3_MASK         0x00000008                  /// Mask for GPIO3 action flag
#define BOOT_ACTION_FLAG_GPIO4_MASK         0x00000010                  /// Mask for GPIO4 action flag
#define BOOT_ACTION_FLAG_GPIO5_MASK         0x00000020                  /// Mask for GPIO5 action flag
#define BOOT_ACTION_FLAG_GPIO6_MASK         0x00000040                  /// Mask for GPIO6 action flag
#define BOOT_ACTION_FLAG_GPIO7_MASK         0x00000080                  /// Mask for GPIO7 action flag
#define BOOT_ACTION_FLAG_GPIO8_MASK         0x00000100                  /// Mask for GPIO8 action flag
#define BOOT_ACTION_FLAG_GPIO9_MASK         0x00000200                  /// Mask for GPIO9 action flag
#define BOOT_ACTION_FLAG_GPIO10_MASK        0x00000400                  /// Mask for GPIO10 action flag
#define BOOT_ACTION_FLAG_GPIO11_MASK        0x00000800                  /// Mask for GPIO11 action flag
#define BOOT_ACTION_FLAG_SLEEP0_MASK        0x00010000                  /// Mask for SLEEP CHANNEL0 action flag
#define BOOT_ACTION_FLAG_SLEEP1_MASK        0x00020000                  /// Mask for SLEEP CHANNEL1 action flag
#define BOOT_ACTION_FLAG_SLEEP2_MASK        0x00040000                  /// Mask for SLEEP CHANNEL2 action flag
#define BOOT_ACTION_FLAG_SLEEP3_MASK        0x00080000                  /// Mask for SLEEP CHANNEL3 action flag

#define BOOT_ACTION_FLAG_SLEEP0_MASK        0x00010000                  /// Mask for SLEEP CHANNEL0 action flag
#define BOOT_ACTION_FLAG_SLEEP1_MASK        0x00020000                  /// Mask for SLEEP CHANNEL1 action flag
#define BOOT_ACTION_FLAG_SLEEP2_MASK        0x00040000                  /// Mask for SLEEP CHANNEL2 action flag


/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/


/**
 * @brief Boot action flags
 * @note Action flags trigger an action during the boot sequence.
 *       Flags are uint32 type.
 *              Bits  0-11 corresponds to GPIO0 - GPIO11
 *              Bits 12-15 corresponds to Sleep channel 0 - 3
 *              Bits 16-31 user flags
 *       Values:
 *          runHfXtal       - trigger HF Xtal
 *          updateProtTim   - trigger updating of Protocol timer
 * @ingroup BOOT
 */
typedef struct
{
    /// 0-10 GPIOs; 11-15 SleepChannels; 16-31 user
    uint32_t runHfXtal;
    /// 0-10 GPIOs; 11-15 SleepChannels; 16-31 user
    uint32_t updateProtTim;
} BOOT_ActionFlag_t;

/**
 * @brief Boot configuration structure
 * @ingroup BOOT
 */
typedef struct
{
    /// Boot action flag
    BOOT_ActionFlag_t actionFlags;
} BOOT_Config_t;

/**
 * @brief Module configuration
 * @ingroup BOOT
 */
typedef enum
{
    BOOT_CONFIG_TYPE_INIT       = 0,    /// Initialize modules after power up
    BOOT_CONFIG_TYPE_SAVE       = 1,    /// Save parameters before sleep
    BOOT_CONFIG_TYPE_RESTORE    = 2,    /// Restore parameters after sleep
} BOOT_ConfigType_t;


/******************************************************************************\
 *  PROTOTYPES
\******************************************************************************/

/**
 * Main boot function for ROM
 * - clears reset reason flags (POR,Sleep)
 * - call initialization of peripherals
 * - starts sleep timer
 * - continue protocol timer
 */
void BOOT_RomBootUp(void);

/**
 * @brief Check if the configuration mode is requested (GPIO5 toggles at 32kHz)
 * @return TRUE if configuration mode is requested, FALSE otherwise
 */
bool BOOT_IsConfigModeEntryRequested(void);

/**
 * Main boot function
 * - clears reset reason flags (POR,Sleep)
 * - call initialization of peripherals
 * - starts sleep timer
 * - continue protocol timer
 */
void BOOT_BootUp(void);

/**
 * @brief Prepare boot module for transition to sleep mode
 */
void BOOT_PrepareToSleepAndWakeUp( void );

/**
 * @brief Register selected action (will be executed) during booting-up after sleep.
 * @note Function registers/deregisters selected action which will be executed
 *       during booting up after sleep mode to requisted wake-up source.
 * @param actionType select action to registration
 * @param mask select source of wake-up by 16bits mask,
 *             GPIO (0-11), SLEEP_CHANNEL (12-15)
 * @param enable TRUE - action is applied to requested wake-up source
 *               FALSE - action is removed from requested wake-up source
 */
void BOOT_RegisterWakeUpAction(PML_WakeupActionType_t actionType, uint32_t mask, bool enable);

/**
 * @brief WAKE and IRQ action handler
 * @param actionMask input action mask (gpio and sleep timer events)
 */
void BOOT_IrqActionRoutine(uint32_t actionMask);

void BOOT_IRQHandlerPMLHFColdStart(void);

void BOOT_IRQHandlerProtoTimerSync(void);

/**
 * @brief Function check if all action were handled during boot sequence
 * @return TRUE all action are handled (done), FALSE otherwise
 */
bool BOOT_IsAllActionHandled(void);


#endif //_BOOT_H_
