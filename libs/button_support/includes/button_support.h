////////////////////////////////////////////////////////////////////////////////
///
/// @file       button_support.h
///
/// @project    T9305
///
/// @brief      Button interface using GPIOs.
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


#ifndef _BUTTON_SUPPORT_H
#define _BUTTON_SUPPORT_H

#include <types.h>
#include "gpio.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Operational button states. */
typedef enum
{
  BUTTON_STATE_UP,                        /*!< Button up */
  BUTTON_STATE_DOWN,                      /*!< Button down */
  BUTTON_STATE_SHORT,                     /*!< Button short press */
  BUTTON_STATE_MED,                       /*!< Button medium press */
  BUTTON_STATE_LONG,                      /*!< Button long press */
  BUTTON_STATE_XLONG,                     /*!< Button extra long press */
  BUTTON_STATE_MAX
} ButtonState_t;

/*! \brief      Physical Button state. */
typedef enum
{
  PHYS_BUTTON_STATE_HIGH = GPIO_STATE_HIGH,   /*!< abstract the GPIO state */
  PHYS_BUTTON_STATE_LOW  = GPIO_STATE_LOW,    /*!< abstract the GPIO state */
  PHYS_BUTTON_STATE_INVALID,          /*!< Button position is invalid. */
} ButtonPhysState_t;


/*! \brief      Button callback signature. */
typedef void (*ButtonCback_t)(uint8_t btnId, ButtonState_t state);


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/**
 * @brief Initialize the button system.
 */
void ButtonSysInit(void);


/*****************/
/* Event Buttons */
/*****************/
/* Application is notified of button events when button is released */

/**
 * @brief Initialize an event reporting button.
 * @note This function initializes a button/gpio mapping.
 * @param id GPIO pin to map the button to.
 * @param open_polarity Polarity of button when not pressed.
 * @param Cback Callback when button released.
 */
void ButtonEventInit(uint8_t id, ButtonPhysState_t open_polarity, ButtonCback_t Cback);

/**
 * @brief Unbound a button/GPIO mapping.
 * @note Works for either event or position buttons.
 */
 void ButtonDeInit(uint8_t id);

/**
 * @brief Override the default event time for a button.
 * @note Times apply to all buttons.
 * @param ms_ticks Time in ms counts for the button event.
 */
void SetButtonShortTime(uint16_t ms_ticks);
void SetButtonMediumTime(uint16_t ms_ticks);
void SetButtonLongTime(uint16_t ms_ticks);
void SetButtonXlongTime(uint16_t ms_ticks);

/**
 * @brief Get the event time for the button events.
 * @return ms_ticks Time in ms counts for the button event.
 */
uint16_t GetButtonShortTime(void);
uint16_t GetButtonMediumTime(void);
uint16_t GetButtonLongTime(void);
uint16_t GetButtonXlongTime(void);

/**
 * @brief Register a notification callback for a button event transition.
 * @note This is a notification of state change only, useful for LED indications, etc.
 * The button should be setup as an event button prior to this call.
 * @param id GPIO pin to map the button to.
 * @param Cback Callback for a state change notification.
 * @param force Forces the notification callback at event callback time.
 */
void ButtonEventNotification(uint8_t id, ButtonCback_t Cback, bool force);

/******************/
/* Polled Buttons */
/******************/
/* Application implements all button state logic */
/* GPIOs cannot be mapped to both event and position buttons */

/**
 * @brief Initialize a position reporting button.
 * @note This function initializes a button/gpio mapping.
 * @param id GPIO pin to map the button to.
 * @param open_polarity Polarity of button when not pressed.
 * @param Cback Callback when button polled.
 */
void ButtonPositionInit(uint8_t id, ButtonPhysState_t open_polarity, ButtonCback_t Cback);

/**
 * @brief Manually read the poistion of a button.
 * @param id GPIO button to read.
 * @return The button position.
 */
ButtonState_t ButtonGetPosition(uint8_t id);

/**
 * @brief Manually read the state of a button.
 * @param id GPIO button to read.
 * @return The button state.
 */
ButtonState_t ButtonGetState(uint8_t id);

/**
 * @brief Override the default button time count.
 * @note BEWARE - All default times/tick counts are based on a 1ms time.
 * This impacts the poll time for state/event buttons too.
 * @param freq  Specifies the frequency, in Hz, to run the timer.
 */
 void ButtonPollTime(float freq);

 /**
 * @brief Override the default button debounce time.
 * @param us_time Time in usec for the button debounce.
 */
 void ButtonDebounceTime(uint16_t us_time);

#endif /* _BUTTON_SUPPORT_H */
