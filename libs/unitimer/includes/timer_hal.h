////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/unitimer/includes/timer_hal.h
///
/// @project    T9305
///
/// @brief      Timer hardware abstraction layer that is specific to the chip.
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2015-present EM Microelectronic-US Inc.
/// @cond
///
/// All rights reserved.
///
/// IMPORTANT - PLEASE CAREFULLY READ THE FOLLOWING SOURCE CODE LICENSE
/// AGREEMENT, WHICH IS LEGALLY BINDING.  THE SOURCE CODE MAY BE USED ONLY IF
/// YOU ACCEPT THE TERMS AND CONDITIONS OF THIS SOURCE CODE LICENSE AGREEMENT
/// (hereafter, the Agreement).
///
/// By Using this source code, you: (i) warrant and represent that you have
/// obtained all authorizations and other applicable consents required
/// empowering you toenter into and (ii) agree to be bound by the terms of this
/// Agreement on your own behalf and/or on behalf of your company.  If you do
/// not agree to this Agreement, then you are not permitted to Use this source
/// code, in whole or in part.
///
/// License Grant.  Pursuant to the terms in the accompanying software license
/// agreement and Terms of Use located at:
/// https://forums.emdeveloper.com/emassets/emus_sourcelicense.html and
/// https://forums.emdeveloper.com/emassets/emus_termsofuse.html  (the terms of
/// each are incorporated herein by this reference) and subject to the
/// restrictions, disclaimer and limitation of liability set forth below,
/// EM Microelectronic US Inc. (EM), grants strictly to you, without the right
/// to sublicense, a non-exclusive, non-transferable, revocable, limited
/// worldwide license to use the source code to modify or create derivative
/// works of the software program for the sole purpose of developing object
/// and executable versions that execute solely and exclusively on devices
/// manufactured by or for EM or your products that use or incorporate devices
/// manufactured by or for EM.
///
/// Restriction.  Without limiting any of the foregoing, the name
/// "EM Microelectronic-US Inc." or that of any of the EM Parties (as such term
/// is defined below) must not be Used (as such term is defined below) to
/// endorse or promote products derived from the source code without prior
/// written permission from an authorized representative of EM Microelectronic
/// US Inc. Unless authorized officers of the parties to this Agreement have
/// duly executed a special written contract specifically governing such Use,
/// in no event shall the source code be Used in or with: (i) life-critical
/// medical equipment, products or services, (ii) military, armament, nuclear or
/// aerospace applications or environments; or (iii) automotive applications
/// unless specifically designated by EM as automotive-grade.  In addition, you
/// understand and agree that you remain solely responsible for using your
/// independent analysis, evaluation, and judgment in designing and developing
/// your applications, products and services.
///
/// DISCLAIMER.  THE SOURCE CODE IS PROVIDED "AS IS" WITH ALL FAULTS, WITHOUT
/// ANY SUPPORT. EM MAKES NO WARRANTIES OR REPRESENTATIONS, EXPRESS, IMPLIED OR
/// STATUTORY, INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
/// A PARTICULAR PURPOSE OR USE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS OF
/// RESPONSES, RESULTS, LACK OF NEGLIGENCE AND LACK OF SECURITY.  EM DISCLAIMS
/// ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET POSSESSION, AND NON-
/// INFRINGEMENT OF ANY THIRD PARTY INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO
/// THE SOURCE CODE OR YOUR USE OF THE SOURCE CODE.  THE ENTIRE RISK AS TO THE
/// QUALITY AND PERFORMANCE OF THE SOURCE CODE IS WITH YOU. SHOULD THE SOURCE
/// CODE PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR
/// OR CORRECTION.  NOTHING CONTAINED IN THIS AGREEMENT WILL BE CONSTRUED AS A
/// WARRANTY OR REPRESENTATION BY ANY EM PARTIES TO MAINTAIN PRODUCTION OF ANY
/// EM DEVICE OR OTHER HARDWARE OR SOFTWARE WITH WHICH THE SOURCE CODE MAY BE
/// USED.  NO ORAL OR WRITTEN INFORMATION OR ADVICE GIVEN BY EM OR AN EM
/// AUTHORIZED REPRESENTATIVE WILL CREATE A WARRANTY
///
/// LIMITATION OF LIABILITY.  IN NO EVENT SHALL EM MICROELECTRONIC US INC., ITS
/// PARENT AND ITS AND THEIR RESPECTIVE AFFILIATES, SUBSIDIARIES, LICENSORS,
/// THIRD PARTY PROVIDERS, REPRESENTATIVES, AGENTS AND ASSIGNS (COLLECTIVLEY,
/// EM PARTIES) BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// PUNITIVE, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
/// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
/// EQUIPMENT, SYSTEMS, SOFTWARE, TECHNOLOGY, SERVICES, GOODS, CONTENT,
/// MATERIALS OR PROFITS; BUSINESS INTERRUPTION OR OTHER ECONOMIC LOSS OR ANY
/// CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF)
/// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
/// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
/// OUT OF THE USE OF THIS SOURCE CODE, INCLUDING ANY PORTION(S) THEREOF, EVEN
/// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  NOTWITHSTANDING ANYTHING ELSE
/// TO THE CONTRARY, IN NO EVENT WILL THE EM PARTIES AGGREGATE LIABILITY UNDER
/// THIS AGREEMENT OR ARISING OUT OF YOUR USE OF THE SOURCE CODE EXCEED ONE
/// HUNDRED U.S. DOLLARS (U.S. $100).
///
/// Because some jurisdictions do not allow the exclusion or limitation of
/// incidental or consequential damages or limitation on how long an implied
/// warranty lasts, the above limitations or exclusions may not apply to you.
///
/// Please refer to the accompanying software license agreement and Terms of Use
/// located at, respectively:
/// https://forums.emdeveloper.com/emassets/emus_sourcelicense.html and
/// https://forums.emdeveloper.com/emassets/emus_termsofuse.html  to better
/// understand all of your rights and obligations hereunder.  Except as
/// otherwise provided in this Agreement, all capitalized terms defined in said
/// software license agreement and Terms of Use shall have the meanings
/// ascribed to such terms therein.
/// @endcond
////////////////////////////////////////////////////////////////////////////////


#ifndef TIMER_HAL_H
#define TIMER_HAL_H


#include <config.h>
#include <timer.h>
#include <interrupts.h>
#include <irq.h>
#include <arc.h>

#ifndef CONFIGURABLE_TIMER
#define CONFIGURABLE_TIMER 0
#endif

/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/// Number of timer ticks (with no prescaler) that the code takes to
/// disable, clear, reset the limit, and re-start the timer and at a later
/// time clear the timer, update timer limits, and re-start the timer again.
#define TIMER_ENABLE_CODE_DELAY    (12 + 16)


/// Number of timer ticks (with no prescaler) that the code takes to start the
/// timer and read the system time during a restore.
#define TIMER_RESTORE_DELAY (-5)


/// Number of timer ticks (with no prescaler) that the code takes to
/// clear the timer, update timer limits and re-start the timer.
#define TIMER_INTERRUPT_CODE_DELAY (16)


/// Number of timer ticks (with no prescaler) that the code takes to
/// call the enable function, for the enable to execute, and for the
/// callback to be executed.  Note that the callback time is dependent on
/// the number of callbacks register with the timer.  This delay only
/// directly affects non-periodic timers.
#define TIMER_CODE_OVERHEAD_DELAY (460u + 890u)


/**
 * @brief Source of the timer clock.
 */
typedef enum
{
    /// The timer will run off the system oscillator.
    TIMER_CLOCK_SYSOSC = 0,

    /// The timer will run off the given GPIO.
    TIMER_CLOCK_GPIO = 1,
} Timer_ClockSource_t;


/// We have a 32-bit timer, 718x has a 16-bit timer; define the size here.
typedef uint32_t Timer_Value_t;


/// Allow for larger timeout values larger than the rollover value of the
/// the timer.
typedef uint64_t Timer_ValueLimit_t;


/// Define for a variable of the appropriate size to perform signed math.
typedef int64_t Timer_MathValue_t;


/// Forward declaration of the timer entry for the configuration structure.
struct Timer_Entry;


/// Timer configuration data structure.
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Size of the timer entry buffer.
    int8_t timerEntriesSize;

    /// If the driver is enabled.
    uint8_t enabled;

    /// Clock source for the timer.
    uint8_t clockSource;

    /// Prescaler for the timer.
    uint8_t prescaler;

    /// Frequency of the clock source (in Hz).
    uint32_t clockFrequency;

#if (0 != CONFIGURABLE_TIMER)
    /// The timer this driver uses.
    uint8_t activeTimer;

    /// Reserved bytes for alignment.
    uint8_t reserved[3];
#endif // (0 != CONFIGURABLE_TIMER)
} Timer_Configuration_t;


/**
 * @brief Prescaler value to divide the clock source by.
 */
typedef enum
{
    TIMER_PRESCALER_NONE = 0,
    TIMER_PRESCALER_2,
    TIMER_PRESCALER_4,
    TIMER_PRESCALER_8,
    TIMER_PRESCALER_16,
    TIMER_PRESCALER_32,
    TIMER_PRESCALER_64,
    TIMER_PRESCALER_128,
} Timer_Prescaler_t;


/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/


/// Reference to the timer configuration.
extern volatile Timer_Configuration_t gTimer_Config;


/******************************************************************************\
 *  INLINE FUNCTIONS
\******************************************************************************/


/**
 * @brief Get the hardware timer used by the driver.
 * @returns Timer 2 or 3.
 */
static ALWAYS_INLINE uint8_t TimerHal_GetHardwareTimer(void)
{
#if (0 != CONFIGURABLE_TIMER)
    return gTimer_Config.activeTimer;
#else
    return 2;
#endif // (0 != CONFIGURABLE_TIMER)
}


/**
 * @brief The number of timer ticks lost when the timer is enabled before it
 *   starts counting.
 * @param timer Timer to get the latency for.
 * @returns Number of timer ticks lost waiting for the timer to enable.
 */
static ALWAYS_INLINE uint8_t TimerHal_GetLatency(uint8_t timer)
{
    return 3;
}


/**
 * @brief Determine the frequency, in Hz, of the system oscillator.
 * @returns The frequency of the oscillator in Hz.
 */
static ALWAYS_INLINE uint32_t TimerHal_GetSystemOscillatorFrequency(void)
{
    return gTimer_Config.clockFrequency;
}


/**
 * @brief Determine the frequency, in Hz, of the timer oscillator.
 * @param timer Timer to get the frequency for.
 * @returns The frequency of the oscillator in Hz.
 */
static ALWAYS_INLINE uint32_t TimerHal_GetTimerOscillatorFrequency(
    uint8_t timer)
{
    uint32_t config;
    uint32_t freq;

    switch(timer)
    {
        case 2:
            config = UTIM->RegUniTimer2Cfg.r32;
            break;
        case 3:
            config = UTIM->RegUniTimer3Cfg.r32;
            break;
        default:
            return 0;
    }

    // The frequency comes from the same configuration structure entry
    // for both the system oscillator and the external GPIO.
    freq = TimerHal_GetSystemOscillatorFrequency();

    return freq >> GET_UT2_PRESCALER_SEL(config);
}


/**
 * @brief Set the timer limit, in counts, of the specified timer.
 * @param timer The hardware timer to modify.
 * @param limit The new limit for the specified timer.
 */
static ALWAYS_INLINE void TimerHal_SetLimit(uint8_t timer, Timer_Value_t limit)
{
    switch(timer)
    {
        case 2:
            UTIM->RegUniTimer2Limit.r32 = limit;
            break;
        case 3:
            UTIM->RegUniTimer3Limit.r32 = limit;
            break;
        default:
            break;
    }
}


/**
 * @brief Get the timer limit, in counts, of the specified timer.
 * @param timer The hardware timer to read.
 * @returns The current limit of the specified timer.
 */
static ALWAYS_INLINE Timer_Value_t TimerHal_GetLimit(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            return UTIM->RegUniTimer2Limit.r32;
        case 3:
            return UTIM->RegUniTimer3Limit.r32;
        default:
            break;
    }

    return 0;
}


/**
 * @brief Get the current count of the specified timer.
 * @param timer The hardware timer to read.
 * @returns The current count of the specified timer.
 */
static ALWAYS_INLINE Timer_Value_t TimerHal_GetCount(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            return UTIM->RegUniTimer2Count.r32;
        case 3:
            return UTIM->RegUniTimer3Count.r32;
        default:
            break;
    }

    return 0;
}


/**
 * @brief Reset the specified timer's count.
 * @param timer The hardware timer to clear.
 */
static ALWAYS_INLINE void TimerHal_Clear(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            UTIM->RegUniTimer2Ctrl.r32 |= UT2_CLEAR(1);
            break;
        case 3:
            UTIM->RegUniTimer3Ctrl.r32 |= UT3_CLEAR(1);
            break;
        default:
            break;
    }
}


/**
 * @brief Start the specified timer.
 * @param timer The hardware timer to start.
 */
static ALWAYS_INLINE void TimerHal_Start(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            UTIM->RegUniTimer2Ctrl.r32 = UT2_ENABLE(1) | UT2_START_SW(1);
            break;
        case 3:
            UTIM->RegUniTimer3Ctrl.r32 = UT3_ENABLE(1) | UT3_START_SW(1);
            break;
        default:
            break;
    }
}


/**
 * @brief Stop the specified timer.
 * @param timer The hardware timer to stop.
 */
static ALWAYS_INLINE void TimerHal_Stop(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            UTIM->RegUniTimer2Ctrl.r32 = UT2_ENABLE(1);
            break;
        case 3:
            UTIM->RegUniTimer3Ctrl.r32 = UT3_ENABLE(1);
            break;
        default:
            break;
    }
}


/**
 * @brief Disable the timer IRQ and all other tasks.
 * @param timer The hardware timer to disable.
 * @returns Context to be used when interrupts are enabled again.
 * @sa TimerHal_EnableInterrupts
 */
static ALWAYS_INLINE uint32_t TimerHal_DisableInterrupts(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            IRQ_Mask(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Mask(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }

    uint32_t context = IRQ_DisableAndSaveInterrupts();
    IRQ_EnableInterruptsSetThreshold(PRIORITY_2);

    return context;
}


/**
 * @brief Enable the timer IRQ and all other tasks.
 * @param timer The hardware timer to enable.
 * @param context Context that was save when interrupts were disabled.
 * @sa TimerHal_DisableInterrupts
 */
static ALWAYS_INLINE void TimerHal_EnableInterrupts(uint8_t timer,
    uint32_t context)
{
    switch(timer)
    {
        case 2:
            IRQ_Unmask(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Unmask(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }

    IRQ_RestoreInterrupts(context);
}


/**
 * @brief Unmask the timer IRQ.
 * @param timer The hardware timer to enable.
 */
static ALWAYS_INLINE void TimerHal_UnmaskInterrupts(uint8_t timer)
{
    uint32_t context = IRQ_DisableAndSaveInterrupts();

    switch(timer)
    {
        case 2:
            IRQ_Unmask(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Unmask(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }

    IRQ_RestoreInterrupts(context);
}


/**
 * @brief Disable interrupts for the specified timer.
 * @param timer The hardware timer to disable interrupts for.
 */
static ALWAYS_INLINE void TimerHal_DisableInterrupt(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            IRQ_Disable(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Disable(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }
}


/**
 * @brief Enable interrupts for the specified timer.
 * @param timer The hardware timer to enable interrupts for.
 */
static ALWAYS_INLINE void TimerHal_EnableInterrupt(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            IRQ_Enable(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Enable(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }
}


/**
 * @brief Clear interrupts for the specified timer.
 * @param timer The hardware timer to clear interrupts for.
 */
static ALWAYS_INLINE void TimerHal_ClearInterrupt(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            IRQ_Clear(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Clear(IRQ_GROUP_UNIVERSAL_TIMER, IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }
}


/**
 * @brief Check if an interrupt is pending for the specified timer.
 * @param timer The hardware timer to check.
 * @returns true if the timer interrupt is pending, false otherwise.
 */
static ALWAYS_INLINE bool TimerHal_IsInterruptPending(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            return IRQ_IsPending(IRQ_GROUP_UNIVERSAL_TIMER,
                IRQ_UT2_FULL_VAL_STS_MASK);
        case 3:
            return IRQ_IsPending(IRQ_GROUP_UNIVERSAL_TIMER,
                IRQ_UT3_FULL_VAL_STS_MASK);
        default:
            return false;
    }
}


/**
 * @brief Number of timer ticks (with no prescaler) of restore code delay.
 *
 * @returns Number of timer ticks (with no prescaler) that the code takes to
 * start the timer and get the system time when it was started.
 */
static ALWAYS_INLINE int32_t TimerHal_GetRestoreDelay(void)
{
    int32_t restoreDelay = 0u;

    if(TIMER_CLOCK_SYSOSC == gTimer_Config.clockSource)
    {
        restoreDelay = TIMER_RESTORE_DELAY >> gTimer_Config.prescaler;
    }

    return restoreDelay;
}


/**
 * @brief Number of timer ticks (with no prescaler) of interrupt code delay.
 *
 * @returns Number of timer ticks (with no prescaler) that the code takes to
 * disable, clear, reset the limit, and re-start the timer and at a later
 * time clear the timer, update timer limits, and re-start the timer again.
 */
static ALWAYS_INLINE uint32_t TimerHal_GetInterruptCodeDelay(void)
{
    uint32_t codeDelay = 0u;

    if(TIMER_CLOCK_SYSOSC == gTimer_Config.clockSource)
    {
        codeDelay = TIMER_INTERRUPT_CODE_DELAY >> gTimer_Config.prescaler;
    }

    return codeDelay;
}


/**
 * @brief Number of timer ticks(with no prescaler) of enable code delay.
 *
 * @returns Number of timer ticks (with no prescaler) that the code takes to
 * clear the timer, update timer limits and re-start the timer.
 */
static ALWAYS_INLINE uint32_t TimerHal_GetEnableCodeDelay(void)
{
    uint32_t codeDelay = 0u;

    if(TIMER_CLOCK_SYSOSC == gTimer_Config.clockSource)
    {
        codeDelay = TIMER_ENABLE_CODE_DELAY >> gTimer_Config.prescaler;
    }

    return codeDelay;
}



/**
 * @brief Number of timer ticks(with no prescaler) of enable code delay.
 *
 * @returns Number of timer ticks (with no prescaler) that the code takes to
 * clear the timer, update timer limits and re-start the timer.
 */
static ALWAYS_INLINE uint32_t TimerHal_GetCodeOverheadDelay(void)
{
    uint32_t codeDelay = 0u;

    if(TIMER_CLOCK_SYSOSC == gTimer_Config.clockSource)
    {
        codeDelay = TIMER_CODE_OVERHEAD_DELAY >> gTimer_Config.prescaler;
    }

    return codeDelay;
}



/**
 * @brief Trigger the timer interrupt through software.
 * @param timer The hardware timer to trigger the interrupt for.
 */
static ALWAYS_INLINE void TimerHal_TriggerInterrupt(uint8_t timer)
{
    switch(timer)
    {
        case 2:
            IRQ_Trigger(IRQ_GROUP_UNIVERSAL_TIMER,
                IRQ_UT2_FULL_VAL_STS_MASK);
            break;
        case 3:
            IRQ_Trigger(IRQ_GROUP_UNIVERSAL_TIMER,
                IRQ_UT3_FULL_VAL_STS_MASK);
            break;
        default:
            break;
    }
}

/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Module initilization function.
 */
void Timer_Init(void);


/**
 * @brief Module save function.
 */
void Timer_Save(void);


/**
 * @brief Module restore function.
 */
void Timer_Restore(void);



#endif /* TIMER_HAL_H */
