////////////////////////////////////////////////////////////////////////////////
///
/// @file       led_support.h
///
/// @project    T9305
///
/// @brief      LED interface using GPIOs.
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

#ifndef _LED_SUPPORT_H
#define _LED_SUPPORT_H

#include "types.h"

/**
 * @defgroup led_support LED Support
 * @ingroup system
 * @brief LED Support
 * @note Assumptions: OFF = line driven low; ON = line driven high
 */


/**
 * @addtogroup system
 * @{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/******************************************************************************\
 *  Constants
\******************************************************************************/

/**
 * @brief LED counter update modes, used when the LED is flashing
 */
enum
{
  LED_COUNTER_IGNORE,   // don't update the counter
  LED_COUNTER_OVERWRITE,// overwite the count value
  LED_COUNTER_ADD       // add to the count value
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
/**
 * @brief Initialize the LED support internal variables.
 * @note Must be called before using LEDs.
 * @ingroup led_support
 */
void LedSysInit(void);

/**
 * @brief Setup the gpio pin for use as an LED.
 * @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 */
void LedInit(uint8_t gpio_pin);

/**
 * @brief Remove the gpio pin for use as an LED.
 * @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 */
void LedDeInit(uint8_t gpio_pin);

/**
 * @brief Set the LED flash time.
 * @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 * @param ms_time LED ON and OFF time in milisec.
 */
void LedFlashTime(uint8_t gpio_pin, uint16_t ms_time);

/* Control and Status */

/**
 * @brief Turn the LED ON.
 * @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 */
void LedOn(uint8_t gpio_pin);

/**
 * @brief Turn the LED OFF.
 * @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 */
void LedOff(uint8_t gpio_pin);

/**
 * @brief Toggle the LED state (ON -> OFF and OFF -> ON).
 * @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 */
void LedToggle(uint8_t gpio_pin);

/**
 *  @brief Flash the LED at the configured time and for "count" times.
 *  @param gpio_pin GPIO pin to use; is less than NUM_GPIO_PINS.
 *  @param count Number of times to flash, 0 = indefinite.
 *  @param mode If currently flashing (Ignore, Overwrite, Add) to the current count.
 */
void LedFlash(uint8_t gpio_pin, uint8_t count, uint8_t mode);

/**
 * @brief Turn LED off and disable the LED flash settings.
 * @param id GPIO pin to use; is less than NUM_GPIO_PINS.
 */
void LedFlashDisable(uint8_t id);

/**
 * @}
 */

#endif /* _LED_SUPPORT_H */
