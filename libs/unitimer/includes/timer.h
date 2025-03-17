////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/unitimer/includes/timer.h
///
/// @project    T9305
///
/// @brief      Universal timer driver.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (C) 2016 EM Microelectronic
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


#ifndef TIMER_H
#define TIMER_H


#include <driver.h>
#include <types.h>

/**
 * @defgroup unitimer Unitimer Driver
 * @brief Driver for the unitimer Timer.
 * @ingroup drivers
 */


/**
 * @addtogroup unitimer
 * @{
 */


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Add the unitimer module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void Timer_RegisterModule(void);


/**
 * @brief Schedule a callback function to be executed at a specified frequency.
 *
 * The accuracy of the timer is directly affected by the accuracy of the
 * high frequency clock source, and the universal timer does not set the
 * clock source.  The clock source can be set by the application in the
 * initialization function by calling the function, PML_SetHfClkSource.
 *
 * @param[in] frequency Specifies the frequency, in Hz, to run the timer.
 * @param[in] callbackFunction The function to call at the specified rate.
 * @param[in] pUserData Additional data to pass to the callback function.
 * @returns The timer polling index used to access all other timer functions.
 * Otherwise, returns -1 if timer is unschedulable.
 */
int8_t Timer_SchedulePolling(float frequency,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Schedule a callback function to be executed at a specified frequency.
 *
 * This function causes the timer to being counting immediately, and will not
 * coalesce with other timers running at the same or similar frequency. The
 * @ref Timer_SchedulePolling function should be used instead as it allows for
 * lower power consumption.
 *
 * @param[in] frequency Specifies the frequency, in Hz, to run the timer.
 * @param[in] callbackFunction The function to call at the specified rate.
 * @param[in] pUserData Additional data to pass to the callback function.
 * @returns The timer polling index used to access all other timer functions.
 * Otherwise returns -1 if timer is unschedulable.
 * @note The accuracy of the timer is directly affected by the accuracy of the
 * high frequency clock source.  The universal timer does not set the
 * clock source.  The clock source can be set by the application in the
 * initialization function by calling the function, PML_SetHfClkSource.
 * @sa Timer_SchedulePolling
 */
int8_t Timer_SchedulePollingImmediately(float frequency,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Execute a callback function after the specified time.
 *
 * This function starts immediately the timer without the call of @ref Timer_Enable.
 * The timer is automatically deleted after one execution of the callback function.
 *
 * @param[in] milliseconds Specifies the delay, in ms, to run the timer. This value shall be strictly greater than 0.
 * @param[in] callbackFunction The function to call after the specified time.
 * @param[in] pUserData The data to be pased to the callback function.
 * @returns The timer polling index used to access all other timer functions.
 * Otherwise, returns -1 if timer is unschedulable.
 * @note The callback function is executed only once.
 * @warning This function will trig a division by 0 exception in case the parameter `milliseconds` is zero.
 */
int8_t Timer_OneshotDelay(float milliseconds,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Execute a callback function after the specified time.
 *
 * To start the timer you must call @ref Timer_Enable.
 * The timer is disabled after one execution of the callback function.
 * It is possible to restart the timer by calling @ref Timer_Enable
 * The deletion of the timer is done with the call of @ref Timer_UnschedulePolling.
 *
 * @param[in] milliseconds Specifies the delay, in ms, to run the timer. This value shall be strictly greater than 0.
 * @param[in] callbackFunction The function to call after the specified time.
 * @param[in] pUserData The data to be pased to the callback function.
 * @returns The timer polling index used to access all other timer functions.
 * Otherwise, returns -1 if timer is unschedulable.
 *
 * @warning This function will trig a division by 0 exception in case the parameter `milliseconds` is zero.
 */
int8_t Timer_ExpressoDelay(float milliseconds,
    Driver_Callback_t callbackFunction, void *pUserData);


/**
 * @brief Removes a previously scheduled timer.
 * @param[in] pollIndex The timer index returned from the
 *   @ref Timer_SchedulePolling function.
 */
void Timer_UnschedulePolling(int8_t pollIndex);


/**
 * @brief Modify the poll interval of a timer.
 * @param[in] frequency Specifies the frequency, in Hz, to run the timer.
 * @param[in] pollIndex The already-scheduled timer index to modify.
 * @returns The timer polling index used to access all other timer functions.
 * Otherwise, returns -1 if timer is unschedulable.
 */
int8_t Timer_UpdatePolling(int8_t pollIndex, float frequency);


/**
 * @brief Retrieve the poll timer interval.
 * @param[in] pollIndex The already-scheduled timer index to read.
 * @returns The timer polling rate configured, in Hz, or 0 if no timer found.
 */
float Timer_GetPolling(int8_t pollIndex);


/**
 * @brief Begin counting the timer at the specified rate.
 * @param[in] pollIndex The already-scheduled timer index to enable.
 */
void Timer_Enable(int8_t pollIndex);


/**
 * @brief Stop the timer from counting.
 * @param[in] pollIndex The already-scheduled timer index to enable.
 */
void Timer_Disable(int8_t pollIndex);


/**
 * @brief Reset the time remaining on the specified timer.
 * @param[in] pollIndex The already-scheduled timer index to reset.
 */
void Timer_Reset(int8_t pollIndex);


/**
 * @brief Disable, reset, and then enable a timer to start it over.
 * @param[in] pollIndex The already-scheduled timer index to restart.
 */
void Timer_Restart(int8_t pollIndex);


/**
 * @brief Determines if the timer is enabled.
 * @param[in] pollIndex The timer index returned from the
 *   @ref Timer_SchedulePolling function.
 * @returns true if the timer is enabled, false otherwise.
 */
bool Timer_IsEnabled(int8_t pollIndex);


/**
 * @brief Returns the number of enabled timers.
 * @returns The number of enabled timers.
 */
uint8_t Timer_EnabledCount(void);


/**
 * @brief Get the number of system oscillator clock ticks until the next timer event.
 * @returns The number of system oscillator clock ticks until the next timer event.
 */
uint32_t Timer_ClocksToNextEvent(void);


/**
 *
 * @brief Resumes timer activities.
 *
 * @note This function shall be called in the NVM_ApplicationEntry() function.
 * It resumes any timer activity that was running before switching to sleep mode
 * so a previously running timer can keep working over sleep periods.
 * Calling this function when starting from POR or SW reset will have no effect.
 *
 */
void Timer_Resume(void);

#endif /* TIMER_H */

/** @} (end addtogroup unitimer) */
