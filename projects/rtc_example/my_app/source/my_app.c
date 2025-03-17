////////////////////////////////////////////////////////////////////////////////
///
/// @file       my_app.c
///
/// @project    T9305
///
/// @brief      Application
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
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

#include "my_app.h"

#include <common.h>
#include <my_app_signals.h>
#include <my_app_task.h>
#include <printf.h>
#include <qf_port.h>
#include <rtc.h>
#include <types.h>

/******************************************************************************\
 *  Constants
\******************************************************************************/

/// Demo of Short Alarm Periodic
#define DEMO_STEP_PERIODIC_ALARM           0
/// Demo of Alarm after a delta time
#define DEMO_STEP_WITH_DELTA               1
/// Demo of End of Day Event
#define DEMO_STEP_SET_TIME_BEFORE_MIDNIGHT 2
/// Demo of disabling an alarm
#define DEMO_STEP_DISABLE_ALARM            3
/// Demo of weekdays
#define DEMO_STEP_WEEKDAYS                 4

/******************************************************************************\
 *  Types
\******************************************************************************/

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/// Week days string when Monday is the first day of the week
static const char gcWeekDayStr[7][10] = { "Monday",   "Tuesday", "Wednesday",
                                          "Thursday", "Friday",  "Saturday",
                                          "Sunday" };

/// Current RTC Demo step
static uint8_t gMyApp_DemoStep = 0;

/// Counter for alarm to go to next Demo step
static uint8_t gMyApp_AlarmCounter = 0;

/******************************************************************************\
 *  Private functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

MyApp_EvtStates_t MyApp_HandleEvent(MyApp_Signals_t signal, void *pParams)
{
    // Let's assume the event will be handled successfully.
    MyApp_EvtStates_t ret = MY_APP_EVT_ST_HANDLED;

    // Handle the event.
    switch (signal)
    {
        case MY_APP_INIT_SIG:
        {
            printf("EM Microelectronic\r\n");

            printf("EM9305 RTC Demo\r\n");

            RTC_Enable();

            /* Write your application initialization code here. */
        }
        break;

        case MY_APP_ERROR_SIG:
        {
            // Error!
            ret = MY_APP_EVT_ST_ERROR;
        }
        break;

        case RTC_EVENT_SIG:
        {
            RTC_CallbackReason_t reason = (RTC_CallbackReason_t)pParams; //lint !e549 Suspicious cast

            RTC_TimeDate_t timeDate;
            RTC_RefTimeCounter_t refTime;

            if (reason == RTC_READY)
            {
                printf("RTC Ready Event\r\n");

                // Set current time
                timeDate.date.day     = 23;
                timeDate.date.month   = 11;
                timeDate.date.year    = 2023;
                timeDate.date.weekDay = 4; // Was a thursday in Switzerland.
                timeDate.time.hours   = 11;
                timeDate.time.minutes = 37;
                timeDate.time.seconds = 0;
                timeDate.time.subSeconds = 0;

                printf("Set Time and Date 23.11.2023 11:37:00\r\n");
                if (RTC_SetTimeAndDate(&timeDate) != 0)
                {
                    printf("Invalid Time & Date\r\n");
                }

                printf("Get Time and Date ");
                (void)RTC_GetTimeAndDate(&timeDate);
                printf("%02d.%02d.%04d %02d:%02d:%02d\r\n",
                    timeDate.date.day, timeDate.date.month, timeDate.date.year,
                    timeDate.time.hours, timeDate.time.minutes, timeDate.time.seconds);

                timeDate.date.day     = 23;
                timeDate.date.month   = 11;
                timeDate.date.year    = 2023;
                timeDate.date.weekDay = 4; // Was a thursday in Switzerland.
                timeDate.time.hours   = 11;
                timeDate.time.minutes = 37;
                timeDate.time.seconds = 5;
                timeDate.time.subSeconds = 0;

                printf("Set Alarm with Time and Date 23.11.2023 11:37:05\r\n");
                if (RTC_SetAlarmTimeAndDate(&timeDate) != 0)
                {
                    printf("Invalid time & date\r\n");
                }
            }
            else
            {
                switch (reason)
                {
                    case RTC_SHORT_ALARM:
                        printf("RTC Short Alarm Event\r\n");
                        break;
                    case RTC_ALARM:
                        printf("RTC Alarm Event\r\n");
                        break;
                    case RTC_END_OF_DAY:
                        printf("RTC End of Day Event\r\n");
                        break;
                    default:
                        printf("RTC Unknown reason !!!\r\n");
                        break;
                }

                (void)RTC_GetTimeAndDate(&timeDate);
                (void)RTC_GetTime(&refTime);

                // Printf RTC Event Time
                printf("%02d.%02d.%04d %02d:%02d:%02d - %d + (%d / 256) sec since 01.01.2000\r\n",
                    timeDate.date.day, timeDate.date.month, timeDate.date.year,
                    timeDate.time.hours, timeDate.time.minutes, timeDate.time.seconds,
                    refTime.time.seconds, refTime.time.subSeconds);

                switch (gMyApp_DemoStep)
                {
                    case DEMO_STEP_PERIODIC_ALARM:
                    {
                        if (gMyApp_AlarmCounter == 0)
                        {
                            printf("Set Short Alarm Periodic with 1000 ms\r\n");
                            (void)RTC_SetShortAlarm(1000, RTC_SHORT_ALARM_PERIODIC);
                        }

                        gMyApp_AlarmCounter++;
                        if (gMyApp_AlarmCounter >= 4)
                        {
                            gMyApp_AlarmCounter = 0;
                            gMyApp_DemoStep++;

                            printf("Disable Short Alarm\r\n");
                            RTC_DisableShortAlarm();

                            // Next Demo step
                            (void)RTC_GetTime(&refTime);
                            refTime.time.seconds += 5;

                            printf("Set Alarm in 5 seconds\r\n");
                            (void)RTC_SetAlarmTime(&refTime);
                        }
                        break;
                    }

                    case DEMO_STEP_WITH_DELTA: // Alarm with delta from current time
                    {
                        gMyApp_AlarmCounter++;
                        if (gMyApp_AlarmCounter >= 4)
                        {
                            gMyApp_AlarmCounter = 0;
                            gMyApp_DemoStep++;

                            printf("Set Short Alarm Periodic with 250 ms\r\n");
                            (void)RTC_SetShortAlarm(250, RTC_SHORT_ALARM_PERIODIC);
                        }
                        else
                        {
                            (void)RTC_GetTime(&refTime);

                            printf("Set Alarm in 2 seconds\r\n");
                            refTime.time.seconds += 2;
                            (void)RTC_SetAlarmTime(&refTime);
                        }
                        break;
                    }

                    case DEMO_STEP_SET_TIME_BEFORE_MIDNIGHT: // End of Day Event
                    {
                        gMyApp_AlarmCounter++;
                        if (gMyApp_AlarmCounter >= 4)
                        {
                            gMyApp_AlarmCounter = 0;
                            gMyApp_DemoStep++;

                            printf("Disable Short Alarm\r\n");
                            RTC_DisableShortAlarm();

                            // Set current time just before midnight
                            timeDate.date.day     = 23;
                            timeDate.date.month   = 11;
                            timeDate.date.year    = 2023;
                            timeDate.date.weekDay = 4; // Was a thursday in Switzerland.
                            timeDate.time.hours   = 23;
                            timeDate.time.minutes = 59;
                            timeDate.time.seconds = 50;
                            timeDate.time.subSeconds = 0;

                            printf("Set Time and Date 10 seconds before midnight !\r\n");
                            if (RTC_SetTimeAndDate(&timeDate) != 0)
                            {
                                printf("Invalid Time & Date\r\n");
                            }
                        }
                        break;
                    }

                    case DEMO_STEP_DISABLE_ALARM:
                    {
                        (void)RTC_GetTime(&refTime);

                        printf("Set Alarm in 3 seconds...\r\n");
                        refTime.time.seconds += 3;
                        (void)RTC_SetAlarmTime(&refTime);

                        printf("Wait 1 second\r\n");
                        COMMON_WaitUs(1000);

                        printf("Disable Alarm, Event shall not be triggered !\r\n");
                        RTC_DisableAlarm();

                        gMyApp_DemoStep++;
                        printf("Set Short Alarm One Shot in 500 ms\r\n");
                        (void)RTC_SetShortAlarm(500, RTC_SHORT_ALARM_ONE_SHOT);
                        break;
                    }

                    case DEMO_STEP_WEEKDAYS:
                    {
                        // Monday 4.12.2023
                        timeDate.date.day     = 4;
                        timeDate.date.month   = 12;
                        timeDate.date.year    = 2023;
                        timeDate.date.weekDay = 1;
                        timeDate.time.hours   = 23;
                        timeDate.time.minutes = 59;
                        timeDate.time.seconds = 50;
                        timeDate.time.subSeconds = 0;

                        (void)RTC_SetFirstDayOfTheWeek(RTC_FIRST_DAY_OF_THE_WEEK_MONDAY);

                        (void)RTC_SetTimeAndDate(&timeDate);
                        printf("%s %02d.%02d.%04d\r\n", gcWeekDayStr[timeDate.date.weekDay - 1],
                                                        timeDate.date.day,
                                                        timeDate.date.month,
                                                        timeDate.date.year);

                        for (uint8_t i = 0; i < 6; i++)
                        {
                            (void)RTC_GetTime(&refTime);
                            refTime.time.seconds += 86400; // Add 1 day
                            (void)RTC_SetTime(&refTime);
                            (void)RTC_GetTimeAndDate(&timeDate);
                            printf("%s %02d.%02d.%04d %s\r\n", gcWeekDayStr[timeDate.date.weekDay - 1],
                                                            timeDate.date.day,
                                                            timeDate.date.month,
                                                            timeDate.date.year);
                        }

                        gMyApp_DemoStep++;
                        break;
                    }
                }
            }
        }
        break;

        /* Add other cases to handle other application signals here. */

        default:
        {
            // Unknown event!
            ret = MY_APP_EVT_ST_UNKNOWN;
        }
        break;
    }

    return ret;
}
