////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/sleep_timer/includes/sleep_timer.h
///
/// @project    T9305
///
/// @brief      Sleep Timer drivers
///
/// Sleep timer is needed to keep time reference even when CPU and Protocol
///  Timer is off. Sleep timer works together with Protocol timer to keep time
///  reference even if high frequency clock domain (Protocol timer) is switched
///  off. Switching between Protocol timer and Sleep timer is done automatically
///  during entering/leaving Sleep mode.
///
/// Sleep timer is generally free running timer with 4 compare channels. Each
///  compare channel can be configured independently.
///
/// Available commands to control Sleep Timer are:
///  - Start timer
///  - Stop timer
///  - Clear timer
///  - Get current value of timer
///  - Set compare value for given channel
///  - Get compare value for given channel
///  - SW and HW capture
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


#ifndef _SLEEP_TIM_H
#define _SLEEP_TIM_H

#include <types.h>
#include <t9305_periph.h>



/**
 * @addtogroup EM_Library
 * @{
 */

/**
 * @addtogroup SLEEP_TIMER
 * @brief Sleep Timer peripheral API
 * @{
 */



/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/
#define SLEEP_TIM_COMPARE_CHANNELS          4   /**< Number of compare channels in Sleep Timer */
#define SLEEP_TIM_CAPTURE_CHANNELS          4   /**< Number of capture channels in Sleep Timer */
#define SLEEP_TIM_CAPTURE_SW_CHANNELS       3   /**< Number of SW capture channels in Sleep Timer. Channel 0..2 */
#define SLEEP_TIM_CAPTURE_HW_PROT_CHANNELS  3   /**< Number of HW capture channels triggred by Protocol Timer in Sleep Timer. Channel 0..2 */
#define SLEEP_TIM_CAPTURE_HW_GPIO_CHANNELS  1   /**< Number of HW capture channels triggred by GPIO in Sleep Timer. Channel 3 */
#define SLEEP_TIM_HW_EVENTS                 2   /**< Number of HW events in capture channel in Sleep Timer */



/******************************************************************************\
 *  MACROS
\******************************************************************************/



/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief HW events from Protocol Timer for Sleep Timer
 */
typedef enum
{
    ST_HW_EVENT_PT_SYNC_START = 0,  /**< Protocol Timer - synchronous start */
    ST_HW_EVENT_PT_SYNC_STOP  = 1,  /**< Protocol Timer - synchronous stop */
} ST_HwEventsPT_t;



/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/



/******************************************************************************\
 *  INIT/SAVE/RESTORE FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Init function of Sleep Timer.
 */
void SleepTimer_Init(void);


/**
 * @brief Store configuration of Protocol Timer.
 *
 * This function stores Sleep Timer HW configuration into persistant memory.
 * This allows later on (after leaving sleep mode) to restore configuration.
 * @sa SleepTimer_RestoreConfig
 */
void SleepTimer_Store(void);


/**
 * @brief ReStore configuration of Sleep Timer.
 *
 * This function restores Sleep Timer HW configuration from previously stored
 * configuration in persistant memory.
 * This allows after leaving sleep mode to restore complete configuration of
 * Sleep Timer HW.
 *
 * @note Configuration is in RAM persistent during sleep, but not during POR.
 * @sa ProtTimer_StoreConfig
 */
void SleepTimer_ReStore(void);



/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/


/**
 * @brief
 *   Start Sleep Timer.
 *
 * @details
 *   Function starts Sleep Timer. Sleep Timer is not cleared when started
 *   (instead it continues counting from previous value).
 */
static ALWAYS_INLINE void SleepTimer_Start( void )
{
    PML->RegSleepTimCtrl.regs.STRunEn = ST_RUN_EN_R(1);
}


/**
 * @brief
 *   Stop Sleep Timer.
 *
 * @details
 *   Function stops Sleep Timer. Sleep Timer is not cleared when stopped
 *   (value of Sleep Timer is kept and can be read).
 */
static ALWAYS_INLINE void SleepTimer_Stop( void )
{
    PML->RegSleepTimCtrl.regs.STRunEn = ST_RUN_EN_R(0);
}


/**
 * @brief
 *   Load Sleep Timer.
 *
 * @details
 *   Function preloads value from SleepTimCompare0 into the Sleep Timer counter.
 */
static ALWAYS_INLINE void SleepTimer_Load( void )
{
    PML->RegSleepTimCtrl.regs.STCtrl = ST_LOAD_R(1);
}


/**
 * @brief
 *   Clear Sleep Timer.
 *
 * @details
 *   It clears Sleep Timer. After clearing Sleep Timer value is zero.
 *   Clearing may take some time since Sleep Timer is in LF clock domain.
 */
static ALWAYS_INLINE void SleepTimer_Clear( void )
{
    PML->RegSleepTimCtrl.regs.STCtrl = ST_CLEAR_R(1);
}


/**
 * @brief
 *   Determine if Sleep Timer is running.
 *
 * @details
 *   Determines if Sleep Timer is running or not.
 *
 * @returns true if Sleep Timer is running, false otherwise.
 */
static ALWAYS_INLINE bool SleepTimer_IsRunning( void )
{
    bool isRunning;
    isRunning = GET_ST_RUNNING_R(SYS->RegPmlSts.regs.STStat);
    // Read twice since it is in different clock domain and might be changing
    // during reading. Reading twice same value provide correct state.
    if (isRunning != GET_ST_RUNNING_R(SYS->RegPmlSts.regs.STStat))
    {
        isRunning = GET_ST_RUNNING_R(SYS->RegPmlSts.regs.STStat);
    }

    return isRunning;
}


/**
 * @brief
 *   Determine if Sleep Timer is busy.
 *
 * @details
 *   Determines if clear or preload operation was requested, but not yet executed
 *   because of resynchronization..
 *
 * @returns true if Sleep Timer is busy, false otherwise.
 */
static ALWAYS_INLINE bool SleepTimer_IsBusy( void )
{
    bool isBusy;
    isBusy = GET_ST_BUSY_R(SYS->RegPmlSts.regs.STStat);
    // Read twice since it is in different clock domain and might be changing
    // during reading. Reading twice same value provide correct state.
    if (isBusy != GET_ST_BUSY_R(SYS->RegPmlSts.regs.STStat))
    {
        isBusy = GET_ST_BUSY_R(SYS->RegPmlSts.regs.STStat);
    }

    return isBusy;
}


/**
 * @brief
 *   Get current value of Sleep Timer.
 *
 * @details
 *   It reads and returns current value of Sleep Timer.
 *
 * @return  Current value of Sleep Timer
 */
static ALWAYS_INLINE uint32_t SleepTimer_GetCurrentValue( void )
{
    uint32_t sleepValA;
    uint32_t sleepValB;

    do
    {
        sleepValA = PML->RegSleepTimCount.r32;
        sleepValB = PML->RegSleepTimCount.r32;
    } while(sleepValA != sleepValB);

    return sleepValA;
}


/**
 * @brief
 *   Enable compare channels.
 *
 * @details
 *   It enables compare channels in Sleep Timer. Each channel is represented
 *   by one bit in mask. All channels with assigned bit set to 1 are enabled.
 *   Channels with assigned bit set to 0 are not modified.
 *   This function is not interrupt safe.
 *
 * @param  enable  Enable mask for compare channels
 */
static ALWAYS_INLINE void SleepTimer_CompareEnable( uint8_t enable )
{
    PML->RegSleepTimCompareCfg.regs.STCompareEn |= ST_COMPARE_EN_R(enable);
}


/**
 * @brief
 *   Disable compare channels.
 *
 * @details
 *   It disables compare channels in Sleep Timer. Each channel is represented
 *   by one bit in mask. All channels with assigned bit set to 1 are disabled.
 *   Channels with assigned bit set to 0 are not modified.
 *   This function is not interrupt safe.
 *
 * @param  disable  Disable mask for compare channels
 */
static ALWAYS_INLINE void SleepTimer_CompareDisable( uint8_t disable )
{
    PML->RegSleepTimCompareCfg.regs.STCompareEn &= ST_COMPARE_EN_R(~disable);
}


/**
 * @brief
 *   Enable/disable given compare channel.
 *
 * @details
 *   It enables/disables given compare channel in Sleep Timer.
 *   No check is performed if compare channel number is valid value.
 *   This function is not interrupt safe.
 *
 * @param  channel  Compare channel number, allowed range <0:3>
 * @param  enable   Compare channel enable
 */
static ALWAYS_INLINE void SleepTimer_SetCompareEnable( uint8_t channel, bool enable )
{
    uint8_t enMask = (uint8_t)ST_COMPARE_EN_R(1u << channel);
    if (true == enable)
    {
        PML->RegSleepTimCompareCfg.regs.STCompareEn |= enMask;
    }
    else
    {
        PML->RegSleepTimCompareCfg.regs.STCompareEn &= ~enMask;
    }
}


/**
 * @brief
 *   Enable wake up from compare channels.
 *
 * @details
 *   It enables wake up from compare channels in Sleep Timer. Each channel is
 *   represented by one bit in mask. All channels with assigned bit set to 1
 *   are enabled. Channels with assigned bit set to 0 are not modified.
 *   This function is not interrupt safe.
 *
 * @param  enable  Enable mask for wake up from compare channels
 */
static ALWAYS_INLINE void SleepTimer_WakeUpEnable( uint8_t enable )
{
    PML->RegSleepTimCompareCfg.regs.STWakeUpEn |= ST_WAKE_UP_EN_R(enable);
}


/**
 * @brief
 *   Disable wake up from compare channels.
 *
 * @details
 *   It disables wake up from compare channels in Sleep Timer. Each channel is
 *   represented by one bit in mask. All channels with assigned bit set to 1
 *   are disabled. Channels with assigned bit set to 0 are not modified.
 *   This function is not interrupt safe.
 *
 * @param  disable  Disable mask for wake up from compare channels
 */
static ALWAYS_INLINE void SleepTimer_WakeUpDisable( uint8_t disable )
{
    PML->RegSleepTimCompareCfg.regs.STWakeUpEn &= ST_WAKE_UP_EN_R(~disable);
}


/**
 * @brief
 *   Enable/disable wake up from given compare channel.
 *
 * @details
 *   It enables/disables wake up from given compare channel in Sleep Timer.
 *   No check is performed if compare channel number is valid value.
 *   This function is not interrupt safe.
 *
 * @param  channel  Compare channel number, allowed range <0:3>
 * @param  enable   Wake up from given compare channel enable
 */
static ALWAYS_INLINE void SleepTimer_SetWakeUpEnable( uint8_t channel, bool enable )
{
    uint8_t enMask = (uint8_t)ST_WAKE_UP_EN_R(1u << channel);
    if (true == enable)
    {
        PML->RegSleepTimCompareCfg.regs.STWakeUpEn |= enMask;
    }
    else
    {
        PML->RegSleepTimCompareCfg.regs.STWakeUpEn &= ~enMask;
    }
}


/**
 * @brief
 *   Set compare value of given compare channel.
 *
 * @details
 *   It sets compare value of given compare channel.
 *   No check is performed if compare channel number is valid value.
 *
 * @param  channel       Compare channel number, allowed range <0:3>
 * @param  compareValue  Compare value
 */
void SleepTimer_SetCompareValue( uint8_t channel, uint32_t compareValue );


/**
 * @brief
 *   Get compare value of given compare channel.
 *
 * @details
 *   It reads and returns compare value of given compare channel.
 *   No check is performed if compare channel number is valid value.
 *
 * @param   channel  Compare channel number, allowed range <0:3>
 *
 * @return  Compare value of given compare channel
 */
static ALWAYS_INLINE uint32_t SleepTimer_GetCompareValue( uint8_t channel )
{
    volatile uint32_t *ptr = (uint32_t*)&PML->RegSleepTimCompare0;
    return ptr[channel];
}


/**
 * @brief
 *   Capture current Sleep Timer value by SW.
 *
 * @details
 *   It captures current Sleep Timer value into register specified by capture
 *   channel.
 *
 * @param  channel  Capture channel number, allowed range <0:2>
 *
 * @return true If the channel is valid for SW capture, false otherwise
 */
static ALWAYS_INLINE bool SleepTimer_SWCapture( uint8_t channel )
{
    if (SLEEP_TIM_CAPTURE_SW_CHANNELS <= channel)
    {
        return false;
    }
    PML->RegSleepTimCaptureCtrl.regs.STCaptureSW = (uint8_t)ST_CAPTURE_SW_R(1u << channel);
    return true;
}


/**
 * @brief
 *   Set HW capture event for given capture channel.
 *
 * @details
 *   It assigns specified HW event for given capture channel. Sleep Timer
 *   current value is then captured once specified HW event occurs.
 *
 * @param  channel  Capture channel number, allowed range <0:2>
 * @param  hwEvent  Selected HW event to trigger capture.
 */
static ALWAYS_INLINE bool SleepTimer_SetHWCaptureEvent( uint8_t channel, ST_HwEventsPT_t hwEvent )
{
    if (SLEEP_TIM_CAPTURE_HW_PROT_CHANNELS <= channel)
    {
        return false;
    }
    PML->RegSleepTimCaptureHWEvtSel.r8[channel] = (uint8_t)hwEvent;
    return true;
}


/**
 * @brief
 *   Configure HW capture event from GPIO
 *
 * @details
 *   It configures HW capture channel 3 in Sleep Timer triggered by GPIO. Configuration
 *   includes GPIO selection, edge polarity and debouncer.
 *
 * @param  gpio         GPIO to be used as input for HW trigger, allowed range <0:11>
 * @param  polarity     GPIO polarity making capture channel trigger.
 */
static ALWAYS_INLINE void SleepTimer_SetHWCaptureGPIOEvent( uint8_t gpio, bool polarity)
{
    PML->RegSleepTimCaptureCtrl.regs.STGPIOEvtCfg =
        (uint8_t)(ST_GPIO_SEL_R(gpio) | ST_GPIO_POL_R(polarity));
}


/**
 * @brief
 *   Enable/disable HW trigger for given capture channel.
 *
 * @details
 *   It enables/disabled given capture channel in Sleep Timer to be triggered
 *   by HW event.
 *   No check is performed if capture channel number is valid value.
 *   This function is not interrupt safe.
 *
 * @param   channel  Capture channel number, allowed range <0:3>
 * @param   enable   Capture channel enable
 */
static ALWAYS_INLINE void SleepTimer_SetHWCaptureEnable( uint8_t channel, bool enable )
{
    uint8_t enMask = (uint8_t)ST_CAPTURE_HW_R(1u << channel);
    if (true == enable)
    {
        PML->RegSleepTimCaptureCtrl.regs.STCaptureHW |= enMask;
    }
    else
    {
        PML->RegSleepTimCaptureCtrl.regs.STCaptureHW &= ~enMask;
    }
}


/**
 * @brief
 *   Get captured value from given channel.
 *
 * @details
 *   It reads and returns previously captured value from specified capture
 *   channel.
 *   No check is performed if capture channel number is valid value.
 *
 * @param   channel  Capture channel number, allowed range <0:3>
 *
 * @return  Captured value in given channel
 */
static ALWAYS_INLINE uint32_t SleepTimer_GetCapturedValue( uint8_t channel )
{
    volatile uint32_t *ptr = (uint32_t*)&PML->RegSleepTimCapture0;
    return ptr[channel];
}


/**
 * @brief Prepare sleep timer to be able to wakeup CPU
 * @note It also enables appropriate IRQ because wakeup must be handled as irq
 *       in active mode.
 * @param sleepClks sleep clocks to sleep
 * @param sleepChannel channel to set up
 */
void SleepTimer_SetWakeupTime(uint32_t sleepClks, uint8_t sleepChannel);


/**
 * @brief Disable sleep timer wake up
 * @note It also disables appropriate IRQ because.
 * @param sleepChannel channel to set up
 */
void SleepTimer_DisableWakeupTime(uint8_t sleepChannel);


/** @} (end addtogroup SLEEP_TIMER) */
/** @} (end addtogroup EM_Library) */

#endif //_SLEEP_TIM_H
