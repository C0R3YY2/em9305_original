////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/rtc/includes/rtc.h
///
/// @project    T9305
///
/// @brief      RTC driver
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2023, EM Microelectronic
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


#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "t9305_periph.h"
#include "errno.h"
#include "sleep_timer.h"

/**
 * @addtogroup drivers
 * @{
 * @defgroup rtc RTC Driver
 * @brief Driver for the RTC.
 * @{
*/

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @brief RTC reason a callback was called.
 */
typedef enum
{
    /// RTC Short Alarm event generated
    RTC_SHORT_ALARM      = 0u,
    /// RTC Alarm event generated
    RTC_ALARM            = 1u,
    /// RTC End of Day event generated
    RTC_END_OF_DAY       = 2u,
    /// RTC ready to use (including LF clock used in RTC)
    RTC_READY            = 3u,
} RTC_CallbackReason_t;

/**
 * @brief Type for any functions that are called due to a RTC event.
 * If the user wishes to post a QPC event from the RTC callback, special
 * care must be taken. The QACTIVE_POST_ISR macro must be used and all
 * posts must be wrapped by QK_ISR_ENTRY and QK_ISR_EXIT. For example:
 * @code
 * void RTC_Callback(RTC_CallbackReason_t reason)
 * {
 *     QK_ISR_ENTRY();
 *
 *     // Post an event to the task.
 *     QEventParams* pEvent = (QEventParams*)Q_NEW(QEventParams, RTC_EVENT_SIG);
 *     pEvent->params = (void*)reason;
 *     QACTIVE_POST(&AO_Task.super, (QEvt*)pEvent, NULL);
 *
 *     QK_ISR_EXIT();
 * }
 * @endcode
 * @note This callback is called in an interrupt context.
 * @param reason The reason this callback was called. See
 * @ref RTC_CallbackReason_t for more information.
 */
typedef void (*RTC_Callback_t)(RTC_CallbackReason_t reason);

/**
 * @brief RTC Sleep Timer Channel enumeration.
 * The RTC will use this sleep timer channel for comparison
 * with sleep timer counter.
 */
typedef enum
{
    /// Sleep Timer Channel 1
    SLEEP_TIMER_CHANNEL_1 = 1u,
    /// Sleep Timer Channel 2
    SLEEP_TIMER_CHANNEL_2 = 2u,
    /// Sleep Timer Channel 3
    SLEEP_TIMER_CHANNEL_3 = 3u,
} RTC_SleepTimerChannel_t;

/**
 * @brief RTC configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Sleep Timer channel used for RTC function (channel 1 to 3)
    RTC_SleepTimerChannel_t sleepTimerChannel;

    /// RTC Callback
    RTC_Callback_t callback;

    /// RTC Calibration enable
    bool calibrationEnable;
} RTC_Configuration_t;

/******************************************************************************\
 *  MACROS
\******************************************************************************/

/// Year size - normal year
#define RTC_NORMAL_YEAR_SIZE        (365u)

/// Year size - leap year
#define RTC_LEAP_YEAR_SIZE          (366u)

/// macro to check if year is leap year. Year range: 0..99 corresponding to
/// year 2000..2099
#define RTC_IS_LEAP_YEAR(year)  (0 == ((year) % 4))
/// macro to return number of days in given year. Year range: 0..99
/// corresponding to year 2000..2099
#define RTC_YEAR_SIZE(year)     (RTC_IS_LEAP_YEAR(year) ? RTC_LEAP_YEAR_SIZE : RTC_NORMAL_YEAR_SIZE)

/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/**
 * @brief RTC first day of the week.
 */
typedef enum
{
    /// RTC first day of the week Sunday
    RTC_FIRST_DAY_OF_THE_WEEK_SUNDAY = 6u,
    /// RTC first day of the week Monday
    RTC_FIRST_DAY_OF_THE_WEEK_MONDAY = 5u,
} RTC_FirstDayOfTheWeek_t;

/**
 * @brief RTC reason a callback was called.
 */
typedef enum
{
    /// RTC short alarm disabled
    RTC_SHORT_ALARM_DISABLE = 0u,
    /// RTC short alarm One shot mode
    RTC_SHORT_ALARM_ONE_SHOT = 1u,
    /// RTC short alarm periodic mode
    RTC_SHORT_ALARM_PERIODIC = 2u
} RTC_ShortAlarmMode_t;

/**
 * @brief RTC time counter structure
 */
typedef union
{
    /// timeCounter in fixed point, format 32.8
    uint64_t timeCounter;

    /// time bits access
    struct
    {
        /// Padding, not used
        uint64_t  pad        : 24;
        /// RTC time SubSeconds. Allowed range 0 up to 255. One LSB is 1/256sec.
        uint64_t  subSeconds : 8;
        /// RTC time Seconds. Allowed range 0..59
        uint64_t  seconds    : 32;
    } time;
} RTC_RefTimeCounter_t;

/**
 * @brief RTC time structure
 */
typedef struct
{
    /// RTC time Hours. Allowed range 0..23
    uint8_t  hours;
    /// RTC time Minutes. Allowed range 0..59
    uint8_t  minutes;
    /// RTC time Seconds. Allowed range 0..59
    uint8_t  seconds;
    /// RTC time SubSeconds. Allowed range 0 up to 255. One LSB is 1/256sec.
    uint8_t  subSeconds;
} RTC_Time_t;

/**
 * @brief RTC date structure
 */
typedef struct
{
    /// RTC date Year. Allowed range 2000..2099.
    uint16_t  year;
    /// RTC date Month. Allowed range 1..12
    uint8_t  month;
    /// RTC date Day. Allowed range 1..31
    uint8_t  day;
    /// RTC date WeekDay. Allowed range 1..7
    uint8_t  weekDay;
} RTC_Date_t;

/**
 * @brief RTC time and date structure
 */
typedef struct
{
    /// RTC date
    RTC_Date_t date;
    /// RTC time
    RTC_Time_t time;
} RTC_TimeDate_t;

/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

/// RTC Configuration
extern volatile RTC_Configuration_t gRTC_Config;

/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/

/**
 * @brief Add the RTC module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void RTC_RegisterModule(void);

/**
 * @brief Enable RTC function
 * @note It takes some time to fully initialize it and it is
 * indicated by posting event RTC_READY.
 */
void RTC_Enable(void);

/**
 * @brief Disable RTC
 * @note Alarm & Short Alarm are also disabled and will not occur after
 * calling RTC_Enable() again.
*/
void RTC_Disable(void);

/**
 * @brief Converts uint32_t to RTC_TimeDate_t structure.
 * @param pTime Current time expressed as number of seconds from
 * reference point (1.1.2000, 00:00:00).
 * @param pTimeDate Pointer to structure to store local time and date.
 * point.
 * @return RTC error code (see errno.h)
 */
int32_t RTC_TimeToTimeAndDate(const RTC_RefTimeCounter_t* pTime, RTC_TimeDate_t* pTimeDate);

/**
 * @brief Converts RTC_TimeDate_t structure to uint32_t.
 * @param pTimeDate Pointer to RTC_TimeDate_t structure containing time broken
 * into its component.
 * @param pTime Current time expressed as number of seconds from referecnce point
 * (1.1.2000, 00:00:00).
 * @return RTC error code
 */
int32_t RTC_TimeAndDateToTime(const RTC_TimeDate_t* pTimeDate, RTC_RefTimeCounter_t* pTime);

/**
 * @brief Set the first day of the week which can be Monday or Sunday.
 * @param fdotw first day of the week
 * @return RTC error code
*/
int32_t RTC_SetFirstDayOfTheWeek(RTC_FirstDayOfTheWeek_t fdotw);

/**
 * @brief Set RTC current time.
 * @param pTime Current time specified as number of seconds and sub second from reference
 * time point (1.1.2000, 00:00:00).
 * @return RTC error code
 *
 * @note RTC time is set to value specified in parameter at the time function
 * is called. Any delay caused by interrupts or other activity is not taken into
 * account and it is not compensated.
 */
int32_t RTC_SetTime(const RTC_RefTimeCounter_t* pTime);

/**
 * @brief Get RTC current time.
 * @param pTime Pointer to current time specified as number of seconds and sub second
 * from reference time point (1.1.2000, 00:00:00).
 * @return RTC error code
 */
int32_t RTC_GetTime(RTC_RefTimeCounter_t* pTime);

/**
 * @brief Set RTC current time and date.
 * @param pTimeDate Pointer to structure containing RTC time and date to set.
 *
 * @note RTC time is set to value specified in parameter at the time function
 * is called. Any delay caused by interrupts or other activity is not taken into
 * account and it is not compensated.
 * @return RTC error code
 */
int32_t RTC_SetTimeAndDate(const RTC_TimeDate_t* pTimeDate);

/**
 * @brief Get RTC current time and date.
 * @param pTimeDate Pointer to structure to store current RTC time and date.
 * @return RTC error code
 */
int32_t RTC_GetTimeAndDate(RTC_TimeDate_t* pTimeDate);

/**
 * @brief Set short alarm generated by RTC
 * @param period Period of interrupt. Period is in [ms] and can be in range
 * <1; 86400000>. Maximum value corresponds to 1 day (24*3600*100ms). If Period
 * value is out of range command is ignored.
 * @param mode Mode of short alarm operation.
 * @return RTC error code
 * @note Due to system latency it might be impossible to reach short period
 * of interrupts. This is even severe in case Sleep mode is used.
 */
int32_t RTC_SetShortAlarm(const uint32_t period, const RTC_ShortAlarmMode_t mode);

/**
 * @brief Disable short alarm generated by RTC
*/
void RTC_DisableShortAlarm(void);

/**
 * @brief Set RTC Alarm for given time.
 * @param pTime Time of alarm expressed as number of seconds and sub second
 * from reference point (1.1.2000, 00:00:00).
 * @return RTC error code
 *
 * @note RTC alarm is set to value specified in alarmTime. It is not checked
 * whether it is valid time thus user should ensure it is valid.
 */
int32_t RTC_SetAlarmTime(const RTC_RefTimeCounter_t* pTime);

/**
 * @brief Set RTC Alarm for given time and date
 * @param pTimeDate Pointer to structure containing RTC time and date to be used
 * for alarm setting.
 *
 * @note RTC alarm is set to value specified in pTimeDate. It is not checked
 * whether it is valid date/time thus user should ensure it is valid.
 * weekDay (part of date structure) is ignored.
 * @return RTC error code
 */
int32_t RTC_SetAlarmTimeAndDate(const RTC_TimeDate_t* pTimeDate);

/**
 * @brief Disable pending alarm
*/
void RTC_DisableAlarm(void);

/** @} */ // End of group rtc
/** @} */ // End of addtogroup drivers

#endif //_RTC_H
