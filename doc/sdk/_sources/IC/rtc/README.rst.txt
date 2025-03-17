Overview
--------

The RTC library provides an API to manage date and time ranging from the 1.1.2000 used as the reference date (origin) to the 31.12.2099.
The library supports day of the week number and let the user select the first day of the week which can be either Sunday or Monday depending on the countries.

One standard alarm can be configured to provide a synchronization event based on the elapsed time from the reference time or using a date and time in the future.

One short alarm can also be configured with a delay ranging from 1 ms to 1 day with a resolution of 1 ms in one shot or on a periodic basis.

When an alarm is configured, the RTC generates an event after the configured time has been reached. Beyond alarm related events, the RTC can also generate events in the following conditions:

* an RTC ready event when the clock source is ready to be used
* an End of Day event occurs each day at midnight


How does it work ?
^^^^^^^^^^^^^^^^^^

The RTC uses the sleep timer clocked by either LF-RC or LF-XTAL as per the user configuration.
When an alarm is setup, it configures the sleep timer comparator to generate a sleep timer interruption when the trigger criteria is met (number of elapsed seconds from the reference time or a date/time in the future).

There are 4 (0 to 3) available channels and one can be configured in the RTC configuration structure. By default and if no channel is explicitely configured, the RTC library uses channel 3.

When the RTC is clocked by the LF-RC, a calibration is done with HF-XTAL to compute the RTC frequency in order to increase the accuracy.

Otherwise, the LF-XTAL can be used instead of the LF-RC with or without calibration by configuring appropriately the PML configuration structure.

The code snippet below shows a typical configuration for both LF-XTAL without calibration (`USE_LF_XTAL` defined whether the calibration is needed or not) and LF-XTAL with calibration (`USE_LF_XTAL_WITH_CALIBRATION` defined for using the calibration).

.. code-block:: bash

  #ifdef USE_LF_XTAL
  // configure PML for using LF XTAL
  // Use LF-XTAL without calibration
    gPML_Config.lfClkSourceType     = (uint8_t)PML_LF_CLK_XTAL_EN;
    gPML_Config.lfClkFreqRatioHf    = 46875;    // 48MHz & 32.768kHz
    gPML_Config.lfClkFreqRatioLf    = 64;
    gPML_Config.lfClkSourceAccuracy = 150;      // 150ppm
    gPML_Config.rcCalibSkip         = true;     // Disable calibration
  #ifdef USE_LF_XTAL_WITH_CALIBRATION
  // configure PML for specifically using LF XTAL with calibration
  // Use LF-XTAL with calibration
    gPML_Config.lfClkSourceAccuracy = 50;       // 50ppm
    gPML_Config.rcCalibSkip         = false;    // Do not skip calibration
    gPML_Config.rcCalibHfLimit      = 0;
    gPML_Config.rcCalibLfLimit      = 38;       // 1200ms @32 kHz
    gPML_Config.rcCalibPeriod       = 16384;    // 500ms @32kHz
    gPML_Config.rcCalibCorrection   = 0;        // 0ppm
  #endif
  #endif

RTC Library registration
^^^^^^^^^^^^^^^^^^^^^^^^

In order to be used, the RTC library shall be registered in `NVM_ConfigModules()` function as shown in the code below. Note that the RTC header file `rtc.h` shall be included first.

.. code-block:: c

    #include "rtc.h"

    void NVM_ConfigModules(void)
    {
        // Register the modules.
        RTC_RegisterModule();

        ...
    }

RTC Events
^^^^^^^^^^

A callback shall be provided to the RTC configuration structure for RTC events notifications.
This callback provides the source of the RTC event which can be:

* short alarm

  * one shot
  * periodic

* standard alarm

  * one shot only

* end of day

  * event sent each day at midnight

* ready state

  * sent only once as soon as the RTC is ready to be used when the RTC clock source has been started

Before actually using the RTC or setting up an alarm, it is recommended to wait for the `RTC_Ready` event to be triggered.


To be able to process a notification, the user defined callback shall have the following prototype:

.. code-block:: c

  typedef void (*RTC_Callback_t)(RTC_CallbackReason_t reason);

  void RTC_Callback(RTC_CallbackReason_t reason)
  {

  }


The parameter `reason` is of type `RTC_CallbackReason_t` defined as follow:

.. code-block:: c

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

The following code example shows a typical way of setting up the RTC callback in the RTC configuration structure in the `NVM_ConfigModules()` function. As already mentioned, the sleep timer comparator is also selected and configured.

.. code-block:: c

  void NVM_ConfigModules(void)
  {
    ...

    gRTC_Config.callback = RTC_Callback;
    gRTC_Config.sleepTimerChannel = SLEEP_TIMER_CHANNEL_3;

    ...
  }

RTC Time structures
^^^^^^^^^^^^^^^^^^^

The RTC library is able to work with two different representations of the time.
All RTC functions that deal with time parameters accept parameters of the two representations.

Internally the RTC works with the elapsed time in second and subsecond based on a reference time of 1.1.2000 00:00:00. Based on that it computes a date & time and the day of the week number (1 to 7 included).
It is important to note that the day of the week depends on the day that has been configured as the first day of the week, either Sunday or Monday (the default).

The structure `RTC_RefTimeCounter_t` is a union of two types of variables. The `timeCounter` is a fixed point value containing the elapsed time from the reference time in both seconds and the subseconds. It helps to do math computations like adding or substracting time. The second time variable is a structure that contains two separated fields, the seconds and the subseconds. This is helpful accessing such information individually.

.. code-block:: c

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


The other way to represent the time is to use a structure for defining the time and another structure for defining the date. These two structures define the following data fields:

* time of the day (hours, minutes, seconds, subseconds)
* date (year, month, day)

The time structure `RTC_Time_t` is defined as follow:

.. code-block:: c

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


The date structure `RTC_Date_t` is defined as follow:

.. code-block:: c

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


All the above mentioned structures are gathered into one single structure that contains both the date and the time, as shown in the following code snippet:

.. code-block:: C

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


RTC Subsecond
^^^^^^^^^^^^^

The subsecond is a unsigned 8 bits value representing a fraction of one second.
One LSB is 1 / 256 = 3,906 ms.

RTC Weekday
^^^^^^^^^^^

The RTC is able to manage which day is considered the first day of the week. One RTC function is available to set the first day of the week which can be Monday or Sunday. Depending on this special day, all other days of the week are renumbered as follow:


.. list-table:: First day of the week (day_of_the_week_value)
   :widths: 40 40
   :header-rows: 1

   * - Monday
     - Sunday
   * - Monday = 1
     - Sunday = 1
   * - Tuesday = 2
     - Monday = 2
   * - Wednesday = 3
     - Tuesday = 3
   * - Thursday = 4
     - Wednesday = 4
   * - Friday = 5
     - Thursday = 5
   * - Saturday = 6
     - Friday = 6
   * - Sunday = 7
     - Saturday = 7

By default the RTC uses Monday as the first day of the week, but it can be changed to Sunday with `RTC_SetFirstDayOfTheWeek()` API. It takes one single parameter of type `RTC_FirstDayOfTheWeek_t` which is a simple enum type containing Monday and Sunday numerical representation as follow:

.. code-block:: c

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
   * @brief Set the first day of the week which can be Monday or Sunday.
   * @param fdotw first day of the week
   * @return RTC error code (see errno.h)
  */
  int32_t RTC_SetFirstDayOfTheWeek(RTC_FirstDayOfTheWeek_t fdotw);

RTC API return Error Codes
^^^^^^^^^^^^^^^^^^^^^^^^^^

Some RTC API return a signed 32 bits values error code that mimic the status code defined in the well known `errno.h` header file.
Consequently, the status code is `0` in case of success. Otherwise, a negative error is returned.


RTC Alarms
^^^^^^^^^^

As mentionned before, the RTC supports two types of alarms:

* the standard alarm with a date & time
* the short alarm which can be one shot or periodic with a range from 1 ms to 1 day

The RTC library can only manage one alarm of each type at the same time. Two (or more) alarms of the same type cannot be defined. Consequently, if a new standard alarm is set, it will replace the previous one that could have been previously configured. If a new short alarm is set, it will also replace the previously defined one of the same type.

Setting an Alarm
^^^^^^^^^^^^^^^^

An alarm can be set for a date & time in the future. When this moment is reached, the RTC generates the related RTC_ALARM event.

The following code snippet shows an example on how to configure a one shot alarm that will trigger the 23th of November, 2023 at 11:37:5:00.00.

.. code-block:: c

  RTC_TimeDate_t timeDate;

  timeDate.date.day     = 23;
  timeDate.date.month   = 11;
  timeDate.date.year    = 2023;
  timeDate.time.hours   = 11;
  timeDate.time.minutes = 37;
  timeDate.time.seconds = 5;
  timeDate.time.subSeconds = 0;

  RTC_SetAlarmTimeAndDate(&timeDate);


Moreover, an alarm can be set to trigger an event 30 seconds in the future from the current time. The principle is to read the elapsed time from the first of January 2000, add 30 seconds and use this new time to setup the alarm. The following code shows an example on how to configure such alarm:

.. code-block:: c

  RTC_RefTimeCounter_t refTime;

  // Read the elapsed time from the reference time
  RTC_GetTime(&refTime);
  // Add 30 seconds (RTC_RefTimeCounter_t type is useful for time computations)
  refTime.time.seconds += 30;
  // Configure a oneshot alarm this new time
  RTC_SetAlarmTime(&refTime);


In case an already configured alarm has been setup and prior to its event being triggered, it can be cancelled by using a dedicated API. In this case the RTC_ALARM event will not be generated.

.. code-block:: c

  RTC_DisableAlarm();


As already mentioned, an already configured alarm can be replaced by configuring a new one of the same type. In the example shown below, an alarm is set to be triggered 30 seconds in the future, and later in the code we increase this delay by 15 seconds. In this case the alarm is not raised when calling the second time RTC_SetAlarmTime(). The alarm event will be set 45 seconds later instead of 30 seconds.

.. code-block:: c

  RTC_RefTimeCounter_t refTime;

  RTC_GetTime(&refTime);
  refTime.time.seconds += 30;
  RTC_SetAlarmTime(&refTime);

  ...

  refTime.time.seconds += 15;
  RTC_SetAlarmTime(&refTime);

Setting a Short Alarm
^^^^^^^^^^^^^^^^^^^^^

The RTC supports short alarm and can be configured with a delay ranging from 1 ms to maximum 1 day with a precision of 1 ms.

In the example below, a one short alarm in configured with a delay of 50 ms. The RTC_SHORT_ALARM event then will be generated only once.

.. code-block:: c

  RTC_SetShortAlarm(50, RTC_SHORT_ALARM_ONE_SHOT);

In the example below, a periodic short alarm is configured with a delay of 1 minute. The RTC_SHORT_ALARM
event will be generated each minute until it is disabled or overriden by an other short alarm.

.. code-block:: c

  RTC_SetShortAlarm(1000, RTC_SHORT_ALARM_PERIODIC);

Like a standard alarm, a short alarm can be disabled. In the case of one shot, the alarm can be disabled before it has been generated. In case of a periodic alarm, it will stop it immediatly and stops emitting periodic events. This can simply be done by invoking the following function:

.. code-block:: c

  RTC_DisableShortAlarm();
