////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/prot_timer/includes/prot_timer.h
///
/// @project    T9305
///
/// @brief      Protocol Timer drivers
///
/// Protocol timer is needed to keep time reference even when CPU is sleeping.
///  Protocol timer works together with Sleep timer to keep time reference
///  even if high frequency clock domain (Protocol timer) is switched off.
///  Switching between Protocol timer and Sleep timer is done automatically
///  during entering/leaving Sleep mode.
///
/// Protocol timer is generally free running timer with 8 compare channels. Each
///  compare channel can be configured independently.
///
/// Available commands to control Protocol timer are:
///  - Start timer
///  - Stop timer
///  - Clear timer
///  - Set full value of timer
///  - Get full value of timer
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


#ifndef _PROT_TIM_H
#define _PROT_TIM_H

#include <arc.h>
#include <types.h>
#include <t9305_periph.h>
#include <config.h>



/**
 * @defgroup prottim Protocol Timer Driver
 * @brief Driver for the Protocol Timer.
 * @ingroup drivers
 */


/**
 * @addtogroup prottim
 * @{
 */



/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/
#define PROT_TIM_COMPARE_CHANNELS   8   /**< Number of compare channels in Protocol Timer */
#define PROT_TIM_CAPTURE_CHANNELS   4   /**< Number of capture channels in Protocol Timer */
#define PROT_TIM_HW_EVENTS          8   /**< Number of HW events in capture channel in Protocol Timer */



/******************************************************************************\
 *  MACROS
\******************************************************************************/

/// Convert us in system time
#define PROT_TIMER_US_TO_SYSTEM_TIME(us) ((SystemTime_t)((SystemTime_t)(us) * CLOCK_FREQ_MHZ))
/// Convert ms in system time
#define PROT_TIMER_MS_TO_SYSTEM_TIME(ms) ((SystemTime_t)((SystemTime_t)(ms) * CLOCK_FREQ_MHZ * 1000u))

/// Convert system time in us
#define PROT_TIMER_SYSTEM_TIME_TO_US(systime) ((SystemTime_t)((SystemTime_t)(systime) / CLOCK_FREQ_MHZ))
/// Convert system time in ms
#define PROT_TIMER_SYSTEM_TIME_TO_MS(systime) ((SystemTime_t)(((SystemTime_t)(systime) / CLOCK_FREQ_MHZ) / 1000u))

/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief Protocol configuration data structure.
 * @ingroup prottim
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// dedicated channel of sleep timer for synchronous start/stop of PT
    uint8_t sleepChanSyncCaptPt;

    /// Recovery range to apply after resuming from Sleep mode.
    /// Events (compare IRQs) occuring in the past after resuming from Sleep
    /// mode would be manually triggered if in recovery range.
    uint16_t recoveryRangeUs;

} PROT_Configuration_t;


/**
 * @brief    Protocol Timer HW events
 * @details  HW event codes for Protocol Timer capture function. For other peripheries codes might be different.
 */
typedef enum
{
    PT_HW_EVENT_RF_RX_SYNC_WORD = 0,    /**< RF - RX Syncword received */
    PT_HW_EVENT_RF_RX_DONE,             /**< RF - RX packet received */
    PT_HW_EVENT_RF_TX_DONE,             /**< RF - TX packet sent */
    PT_HW_EVENT_GPIO_0,                 /**< GPIO - edge/level detected on pin 0. */
    PT_HW_EVENT_GPIO_1,                 /**< GPIO - edge/level detected on pin 1. */
    PT_HW_EVENT_GPIO_2,                 /**< GPIO - edge/level detected on pin 2. */
    PT_HW_EVENT_GPIO_3,                 /**< GPIO - edge/level detected on pin 3. */
    PT_HW_EVENT_GPIO_4                  /**< GPIO - edge/level detected on pin 4. */
} ProtTimer_HwEvents_t;


/**
 * @brief  Protocol Timer run state
 */
typedef enum
{
    PT_STATE_IDLE             = 0,  /**< Protocol Timer in Idle state. */
    PT_STATE_RUNNIG_SYNC_STOP = 1,  /**< Protocol timer running and waiting for trigger to stop. */
    PT_STATE_IDLE_SYNC_START  = 2,  /**< Protocol timer in Idle state and waiting for trigger to start. */
    PT_STATE_RUNNING          = 3   /**< Protocol Timer in Running state. */
} ProtTimer_RunState_t;


/**
 * @brief Store structure for Protocol Timer configuration state.
 */
typedef struct
{
    /// Protocol Timer configuration
    RegProtTimCfg_t             RegProtTimCfg;
    /// Protocol Timer counter value
    RegProtTimCount_t           RegProtTimCount;
    /// Protocol Timer counter value
    RegProtTimStat_t            RegProtTimStat;
    /// Protocol Timer status
    RegProtTimFullVal_t         RegProtTimFullVal;
    /// Protocol Timer full value
    RegProtTimLoadVal_t         RegProtTimLoadVal;
    /// Protocol Timer output compare configuration
    RegProtTimCompareCfg_t      RegProtTimCompareCfg;
    /// Protocol Timer output compare value - channel 0
    RegProtTimCompare0_t        RegProtTimCompare0;
    /// Protocol Timer output compare value - channel 1
    RegProtTimCompare1_t        RegProtTimCompare1;
    /// Protocol Timer output compare value - channel 2
    RegProtTimCompare2_t        RegProtTimCompare2;
    /// Protocol Timer output compare value - channel 3
    RegProtTimCompare3_t        RegProtTimCompare3;
    /// Protocol Timer output compare value - channel 4
    RegProtTimCompare4_t        RegProtTimCompare4;
    /// Protocol Timer output compare value - channel 5
    RegProtTimCompare5_t        RegProtTimCompare5;
    /// Protocol Timer output compare value - channel 6
    RegProtTimCompare6_t        RegProtTimCompare6;
    /// Protocol Timer output compare value - channel 7
    RegProtTimCompare7_t        RegProtTimCompare7;
    /// Protocol Timer input capture control
    RegProtTimCaptureCtrl_t     RegProtTimCaptureCtrl;
    /// Protocol Timer HW event synchronizer selection
    RegProtTimHWEvtSyncSel_t    RegProtTimHWEvtSyncSel;
    /// Protocol Timer input capture HW event selection
    RegProtTimCaptureHWEvtSel_t RegProtTimCaptureHWEvtSel;
    /// last stop time
    uint32_t                    stopTimeInSleepClk;
    /// last restart time
    uint32_t                    restartTimeInSleepClk;
} ProtTimer_HardwareState_t;


/**
 * @brief  Protocol Timer restore state
 */
typedef enum
{
    PT_RESTORE_STATE_IDLE = 0,              /**< Protocol Timer not started and adjusted. */
    PT_RESTORE_STATE_STARTED,               /**< Protocol Timer started (restored) but not adjusted yet. */
    PT_RESTORE_STATE_STARTED_AND_ADJUSTED,  /**< Protocol Timer started (restored) and adjusted. */
} ProtTimer_RestoreState_t;


/**
 * Result of the event time callback.
 */
typedef enum
{
    /// The system timestamp to the next event is valid.
    PT_EVENT_TIME_STAMP_VALID,
    /// The next event is not known; the system may sleep for an infinite time.
    PT_EVENT_TIME_INFINITE_TIME,
    /// The system may not go to sleep.
    PT_EVENT_TIME_SLEEP_FORBIDDEN,
} ProtTimer_EventTimeResult;


/**
 * Callback for software to notify the system of upcoming events.
 */
typedef struct _ProtTimer_EventTimeCallback_t
{
    /**
     * Callback to query when the next event will happen.
     * @param pSystemTimeOfEvent Pointer to the variable to store the timestamp
     *   of the next event in.
     * @returns Result of the event time callback.
     */
    ProtTimer_EventTimeResult (*pTimeToNextEvent)(
        SystemTime_t *pSystemTimeOfEvent);

    /// Internal pointer to the next callback.
    struct _ProtTimer_EventTimeCallback_t *pNext;
} ProtTimer_EventTimeCallback_t;


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/
/// Protocol configuration
extern volatile PROT_Configuration_t gPROT_Config;



/******************************************************************************\
 *  INIT/SAVE/RESTORE FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief ProtocolTimer init function
 */
void ProtTimer_Init( void );


/**
 * @brief Store configuration of Protocol Timer.
 *
 * This function stores Protocol Timer HW configuration into persistant memory.
 * This allows later on (after leaving sleep mode) to restore configuration.
 * @sa ProtTimer_RestoreConfig
 */
void ProtTimer_StoreConfig( void );


/**
 * @brief ReStore configuration of Protocol Timer.
 *
 * This function restores Protocol Timer HW configuration from previously stored
 * configuration in persistant memory.
 * This allows after leaving sleep mode to restore complete configuration of
 * Protocol Timer HW.
 *
 * @note configuration is in RAM persistent during sleep, but not during POR.
 * @note that not all can be restored namely:
 * - RegProtTimCapture0-3
 * @sa ProtTimer_StoreConfig
 */
void ProtTimer_RestoreConfig( void );



/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/


/**
 * @brief Start Protocol Timer.
 *
 * @note Protocol Timer is not cleared when started. Instead it continues
 * counting from previous value.
 */
static ALWAYS_INLINE void ProtTimer_Start( void )
{
    PTIM->RegProtTimCtrl.r32 = PT_START(1);
}


/**
 * @brief Stop Protocol Timer.
 *
 * @note Protocol Timer is not cleared when stopped. Value of Protocol Timer is
 * kept and can be read.
 */
static ALWAYS_INLINE void ProtTimer_Stop( void )
{
    PTIM->RegProtTimCtrl.r32 = PT_STOP(1);
}


/**
 * @brief Clear Protocol Timer.
 *
 * It clears Protocol Timer. After clearing Protocol Timer value is zero.
 */
static ALWAYS_INLINE void ProtTimer_Clear( void )
{
    PTIM->RegProtTimCtrl.r32 = PT_CLEAR(1);
}


/**
 * @brief Clear Protocol Timer divider.
 *
 * It clears (restart) Protocol Timer divider. After clearing divider status
 * value is zero and it counts to predefined divider value.
 * @sa ProtTimer_SetDivider
 * @sa ProtTimer_GetDivider
 */
static ALWAYS_INLINE void ProtTimer_DividerClear( void )
{
    PTIM->RegProtTimCtrl.r32 = PT_DIV_CLEAR(1);
}


/**
 * @brief Get current state of Protocol Timer.
 * @returns Current state of Protocol Timer.
 */
static ALWAYS_INLINE ProtTimer_RunState_t ProtTimer_GetCurrentState( void )
{
    return (ProtTimer_RunState_t)( (uint32_t)PTIM->RegProtTimStat.r32 );
}


/**
 * @brief Get current value of Protocol Timer.
 * @returns Current value of Protocol Timer
 *
 * It reads and returns current value of Protocol Timer.
 */
static ALWAYS_INLINE uint32_t ProtTimer_GetCurrentValue( void )
{
    return PTIM->RegProtTimCount.r32;
}


/**
 * @brief Set full value of Protocol Timer.
 * @param fullValue Full value to be set.
 *
 * It sets full value of Protocol Timer. Once full value is reached interrupt is
 * generated and Protocol Timer either stops to count or it reloads to zero and
 * then it continues. Protocol Timer counts from 0 to fullVal (number of ticks
 * between two interrupts is equal to fullValue+1). If fullValue is set to -1
 * Protocol Timer counts up to 0xFFFFFFFF and then it continues from zero.
 *
 */
static ALWAYS_INLINE void ProtTimer_SetFullValue( uint32_t fullValue )
{
    PTIM->RegProtTimFullVal.r32 = fullValue;
}


/**
 * @brief Get full value of Protocol Timer.
 * @returns Current Full value of Protocol Timer.
 */
static ALWAYS_INLINE uint32_t ProtTimer_GetFullValue( void )
{
    return PTIM->RegProtTimFullVal.r32;
}


/**
 * @brief Set compare value of given compare channel in Protocol Timer.
 * @param channel Compare channel number, allowed range <0:7>.
 * @param compareValue Compare value to be set.
 *
 * It sets compare value of given compare channel in Protocol Timer.
 * @note No check is performed if compare channel number is in valid range.
 */
static ALWAYS_INLINE void ProtTimer_SetCompareValue( uint8_t channel, uint32_t compareValue )
{
    volatile uint32_t *ptr = (uint32_t*)&PTIM->RegProtTimCompare0;
    ptr[channel] = compareValue;
}


/**
 * @brief Get compare value of given compare channel.
 * @param channel Compare channel number, allowed range <0:7>
 * @returns Compare value of given compare channel.
 *
 * It reads and returns compare value of given compare channel.
 * @note No check is performed if compare channel number is in valid range.
 */
static ALWAYS_INLINE uint32_t ProtTimer_GetCompareValue( uint8_t channel )
{
    volatile uint32_t *ptr = (uint32_t*)&PTIM->RegProtTimCompare0;
    return ptr[channel];
}


/**
 * @brief Capture current Protocol Timer value by SW.
 * @param channel Capture channel number, allowed range <0:3>
 *
 * It captures current Protocol Timer value into capture register specified by
 * capture channel.
 * @note No check is performed if capture channel number is in valid range.
 */
static ALWAYS_INLINE void ProtTimer_SWCapture( uint8_t channel )
{
    PTIM->RegProtTimCaptureCtrl.regs.PTCaptureSW = (uint8_t)(1u << channel);
}


/**
 * @brief Set HW capture event for given capture channel.
 * @param channel Capture channel number, allowed range <0:3>
 * @param hwEvent Selected HW event to trigger capture.
 *
 * It assigns specified HW event for given capture channel. Protol Timer current
 * value is then captured once specified HW event occurs.
 * @note No check is performed if capture channel number is in valid range.
 */
static ALWAYS_INLINE void ProtTimer_SetHWCaptureEvent( uint8_t channel, uint8_t hwEvent )
{
    PTIM->RegProtTimCaptureHWEvtSel.r8[channel] = hwEvent;
}


/**
 * @brief Enable/disable dual FF synchronization on HW event.
 * @param hwEventMask Mask for enabling/disabling dual FF synchronization
 *
 * It enables/disables dual FF synchronization for HW events used in Protocol
 * Timer. Each HW event is represented by one bit in mask. Dual FF synchronization
 * is enabled for those HW events which mask bit is set to 1.
 */
static ALWAYS_INLINE void ProtTimer_SetHWCaptureEventSync( uint8_t hwEventMask )
{
    PTIM->RegProtTimHWEvtSyncSel.r32 = hwEventMask;
}


/**
 * @brief Get captured value from given channel.
 * @param channel Capture channel number, allowed range <0:3>
 * @returns Captured value in given capture channel.
 *
 * It reads and returns previously captured value from specified capture channel.
 * @note No check is performed if capture channel number is in valid range.
 */
static ALWAYS_INLINE uint32_t ProtTimer_GetCapturedValue( uint8_t channel )
{
    volatile uint32_t *ptr = (uint32_t*)&PTIM->RegProtTimCapture0;
    return ptr[channel];
}


/**
 * @brief Return if protocol timer is running.
 * @returns true if Protocol Timer is running, false otherwise.
 */
static ALWAYS_INLINE bool ProtTimer_IsRunning( void )
{
    return (PTIM->RegProtTimStat.r32 & PT_RUNNING(1)) != 0;
}


/**
 * @brief Return if protocol timer is running.
 * @returns true if Protocol Timer is running, false otherwise.
 */
static ALWAYS_INLINE uint8_t ProtTimer_GetSleepTimerChannelForSync( void )
{
    return gPROT_Config.sleepChanSyncCaptPt;
}


/**
 * @brief Set divider factor.
 * @param divFactor Divider factor, allowed range <0:255>
 *
 * Protocol timer is running on 24MHz/divFactor except value 0.
 * Divider factor 0 is interpreted as 256.
 * With divider factor of 1 Protocol timer is running at full speed (24MHz).
 * With divider factor of 255 Protocol timer is running at 24MHz/255.
 * With divider factor of 0 (256) Protocol timer is running at 24MHz/256.
 * @note Divider factor shall be changed only if Protocol timer is not running.
 * No check of running Protocol timer is done.
 * Valid range is not checked, bigger values are presented as divFactor mod 256.
 */
void ProtTimer_SetDivider( uint8_t divFactor );


/**
 * @brief Get current divider factor.
 * @returns Current Divider factor.
 */
uint16_t ProtTimer_GetDivider( void );


/**
 * @brief Set Protocol Timer configuration.
 * @param autoReload Enable for Auto Reload mode.
 * @param syncStart Enable for synchronous start/stop.
 *
 * It enables/disables Auto Reload mode and Synchronous Start/Stop.
 * In Auto Reload mode Protocol Timer continues in counting from zero after
 * reaching Full Value.
 * Synchronous Start/Stop ensures that timer is started/sopped synchronously
 * with low frequency clock. This ensures that timing is correctly maintained in
 * sleep modes. This mode is required when used together with Sleep Timer.
 *
 * @note After changing Synchronous Start/Stop state SW needs to wait for at
 * least 4 clock cycles (4 NOP instructions) before starting/stopping Protocol
 * Timer.
 */
void ProtTimer_SetConfiguration( bool autoReload, bool syncStart );


/**
 * @brief Load Protocol Timer.
 * @param loadValue Value to be loaded into Protocol Timer.
 *
 * It preloads Protocol Timer with specified value. After preloading Protocol
 * Timer value is equal to load value and Protocol Timer continues from this
 * loaded value.
 * @note Protocol Timer can be preloaded even when running.
 */
void ProtTimer_Load( uint32_t loadValue );


/**
 * @brief Adjust Protocol Timer value.
 * @param adjustValue Value to be added into Protocol Timer current value.
 *
 * It adjusts Protocol Timer value with specified value. After adjusting
 * Protocol Timer value is equal to old_value + adjustValue.
 * Adjustment can be done in both dirrection (add/subtract).
 * @note Protocol Timer can be adjusted even when running.
 */
void ProtTimer_Adjust( uint32_t adjustValue );


/**
 * @brief Enable/disable given compare channel.
 * @param channel Compare channel number, allowed range <0:7>.
 * @param enable Compare channel enable/disable.
 *
 * It enables or disables given compare channel in Protocol Timer.
 * @note No check is performed if compare channel number is in valid range.
 * @note This function is not interrupt safe.
 */
void ProtTimer_SetCompareEnable( uint8_t channel, bool enable );


/**
 * @brief Check if the given compare channel is enabled/disabled.
 * @param channel Compare channel number, allowed range <0:7>.
 * @return True if the compare channel is enabled, false otherwise.
 *
 * It returns true if the given compare channel in Protocol Timer is enabled.
 * @note No check is performed if compare channel number is in valid range.
 */
bool ProtTimer_GetCompareEnable( uint8_t channel);


/**
 * @brief Enable/disable HW trigger for RF from given compare channel.
 * @param channel Compare channel number, allowed range <0:7>.
 * @param enable Compare channel enable/disable.
 *
 * It enables/disables given compare channel in Protocol Timer to generate
 * HW trigger for RF.
 * @note No check is performed if compare channel number is in valid range.
 * @note This function is not interrupt safe.
 */
void ProtTimer_SetHwTriggerEnable( uint8_t channel, bool enable );


/**
 * @brief Enable/disable HW capture for given capture channel.
 * @param channel Capture channel number, allowed range <0:3>.
 * @param enable Capture channel enable/disable.
 *
 * It enables/disabled given capture channel in Protocol Timer to be triggered
 * by HW event.
 * @note No check is performed if capture channel number is in valid range.
 * @note This function is not interrupt safe.
 */
void ProtTimer_SetHWCaptureEnable( uint8_t channel, bool enable );


/**
 * @brief Update time when PT start synchronously started
 * @note Function also disables HW capture of sleep timer.
 */
void ProtTimer_UpdateRestartTime(void);


/**
 * @brief Start protocol timer after configuration restoring
 * @returns TRUE if Timer was started and needs adjustment, FALSE otherwise
 * @note It loads protocol timer by restore value and run it.
 */
bool ProtTimer_RestoreStart(void);


/**
 * @brief Adjust protocol timer after configuration restoring
 *
 * Protocol timer is adjusted by time spent when the system was in sleep mode
 *  or ptocol timer was disabled ().
 *
 * @note It must wait for PT run. Only then sleep timer will capture it's value
 *  (synchronously with PT start)
 * @note Then sleep clocks can be calculated and PT value can be adjusted
 * @note We could just read SleepTimer Count but based on code we do not know
 *  if PT is started now or next LF clock. Thus we would not know if to
 *  add to PT SleepClockDiff or SleepClockDiff-1
 */
void ProtTimer_RestoreAdjust(void);


/**
 * @brief Return if Protocol Timer was running when called
 * ProtTimer_StoreConfig()
 * @returns true if Protocol Timer was running when called
 *   ProtTimer_StoreConfig(), else otherwise.
 */
bool ProtTimer_WasRunning( void );


/**
 * @brief Restart Protocol Timer after Sleep
 */
void ProtTimer_RestartAfterSleep( void );


/**
 * Get an absolute timestamp of the system timer.
 * @returns the absolute timestamp of the system timer.
 */
SystemTime_t ProtTimer_GetSystemTime( void );


/**
 * Read a register and get the system timestamp for when that register was read.
 * @param pRegister Address of the register to read.
 * @param pDestination Pointer to the variable to store the register
 *   contents into.
 * @returns System timestamp when the register was read.
 */
SystemTime_t ProtTimer_GetTimestampedRegister(
    const volatile uint32_t *pRegister, uint32_t *pDestination);


/**
 * Get the system time plus a number of us.
 * @param timestamp The system timestamp to add to.
 * @param timeInUs The number of microseconds to add to the timestamp.
 * @returns System timestamp added with the number of microseconds
 */
SystemTime_t ProtTimer_AddUs(SystemTime_t timestamp, uint32_t timeInUs);


/**
 * Register a callback to be called by the system to query software of the next
 * event that was scheduled.
 * @param pCallback Pointer to the callback structure.
 * @returns true if the callback was registered, false otherwise.
 */
bool ProtTimer_RegisterEventTimeCallback(
    ProtTimer_EventTimeCallback_t *pCallback);


/**
 * Get a pointer to the first event time callback.
 * @returns Pointer to the first event time callback or NULL if there is none.
 */
ProtTimer_EventTimeCallback_t* ProtTimer_GetFirstEventTimeCallback(void);


/**
 * Get the time to the first event that is scheduled in the future.
 * @details Modules can register to the protocol timer module with the function
 *          @ref ProtTimer_RegisterEventTimeCallback. To get the time to the
 *          first event in the future, the registered "event time callbacks" are
 *          questioned one after the other.
 * @param[out] pTimeToFirstEvent Timestamp of the next event.
 * @param[out] pDeltaToFirstEvent Delta time to the next event.
 * @param[out] pQueryStart Reference timestamp for @p pDeltaToFirstEvent .
 * @retval PT_EVENT_TIME_STAMP_VALID    The time to the first event is returned
 *                                      in the parameters @p pTimeToFirstEvent ,
 *                                      @p pDeltaToFirstEvent and @p pQueryStart .
 * @retval PT_EVENT_TIME_INFINITE_TIME  No event are planned in the future.
 * @retval PT_EVENT_TIME_SLEEP_FORBIDDEN At least one module has an event in the
 *                                       future but the time of that event is not
 *                                       known.
 */
ProtTimer_EventTimeResult ProtTimer_GetTimeToFirstEvent(
    SystemTime_t *pTimeToFirstEvent, SystemTime_t *pDeltaToFirstEvent,
    SystemTime_t *pQueryStart);

/** @} (end addtogroup prottim) */

#endif //_PROT_TIM_H
