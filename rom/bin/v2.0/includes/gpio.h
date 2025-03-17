////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/gpio/includes/gpio.h
///
/// @project    T9305
///
/// @brief      GPIO driver.
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


#ifndef _GPIO_H
#define _GPIO_H

#include <arc.h>
#include "t9305_periph.h"
#include "config.h"
#include "gpio_memory.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup gpio GPIO Driver
 * @brief Driver for GPIO and Switch Matrix.
 *
 * This module contains all the code required to configure and control the GPIO
 * and Switch Matrix.
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @brief Indicates a function is not mapped to a GPIO input pin.
 * Returned by @ref GPIO_GetInputFunctionPin if a function is not mapped to
 * a GPIO input pin. Used by @ref GPIO_SetInputFunctionPin if a function should
 * not be mapped to a GPIO input pin.
 * @note Multiple functions can be mapped to the same GPIO input pins.
 */
#define GPIO_FUNCTION_NOT_MAPPED (0xFF)


/**
 * Get the mask for the given GPIO.
 * @param n The GPIO to get the mask for.
 */
#define GPIO_MASK(n) (1 << (n))


/// The total number of GPIO pins available.
#define NUM_GPIO_PINS (12u)


// State of a GPIO (high or low).
/// GPIO is or should be set high (logic 1).
#define GPIO_STATE_HIGH (1)
/// GPIO is or should be set low (logic 0).
#define GPIO_STATE_LOW  (0)


// Which edge to interrupt on.
/// Interrupt on the rising edge (transition from 0 to 1).
#define GPIO_POLARITY_RISING  (1)
/// Interrupt on the falling edge (transition from 1 to 0).
#define GPIO_POLARITY_FALLING (0)


// Pin mapping for the JTAG.
/// JTAG clock pin.
#define GPIO_PIN_JTAG_TDI                  (8u)
/// JTAG data out pin.
#define GPIO_PIN_JTAG_TDO                  (9u)
/// JTAG data in pin.
#define GPIO_PIN_JTAG_TCK                  (10u)
/// JTAG mode select pin.
#define GPIO_PIN_JTAG_TMS                  (11u)


// JTAG configuration.
/// JTAG 4 wires configuration.
#define JTAG_4WIRES                        (1u)
/// JTAG 2 wires configuration.
#define JTAG_2WIRES                        (2u)


//lint ++flb not referenced, treat as library code
#define GPIO_PIN_SPIS_CSN       (0)
#define GPIO_PIN_SPIS_SCK       (1)
#define GPIO_PIN_SPIS_MISO      (2)
#define GPIO_PIN_SPIS_MOSI      (3)
#define GPIO_PIN_SPIS_RDY       (4)

#define GPIO_PIN_UART_RXD       (6)
#define GPIO_PIN_UART_TXD       (7)
#define GPIO_PIN_UART_CTS       (8)
#define GPIO_PIN_UART_RTS       (10)
#define GPIO_PIN_UART_WU        (0)

#define GPIO_PIN_UART_CSP_RXD   (2)
#define GPIO_PIN_UART_CSP_TXD   (8)
#define GPIO_PIN_UART_CSP_CTS   (9)
#define GPIO_PIN_UART_CSP_RTS   (10)
#define GPIO_PIN_UART_CSP_WU    (0)

#define GPIO_MASK_SPIS_CSN      (GPIO_MASK(GPIO_PIN_SPIS_CSN))
#define GPIO_MASK_SPIS_SCK      (GPIO_MASK(GPIO_PIN_SPIS_SCK))
#define GPIO_MASK_SPIS_MISO     (GPIO_MASK(GPIO_PIN_SPIS_MISO))
#define GPIO_MASK_SPIS_MOSI     (GPIO_MASK(GPIO_PIN_SPIS_MOSI))
#define GPIO_MASK_SPIS_RDY      (GPIO_MASK(GPIO_PIN_SPIS_RDY))

#define GPIO_MASK_UART_RXD      (GPIO_MASK(GPIO_PIN_UART_RXD))
#define GPIO_MASK_UART_TXD      (GPIO_MASK(GPIO_PIN_UART_TXD))
#define GPIO_MASK_UART_RTS      (GPIO_MASK(GPIO_PIN_UART_RTS))
#define GPIO_MASK_UART_CTS      (GPIO_MASK(GPIO_PIN_UART_CTS))
#define GPIO_MASK_UART_WU       (GPIO_MASK(GPIO_PIN_UART_WU))

#define GPIO_MASK_UART_CSP_RXD  (GPIO_MASK(GPIO_PIN_UART_CSP_RXD))
#define GPIO_MASK_UART_CSP_TXD  (GPIO_MASK(GPIO_PIN_UART_CSP_TXD))
#define GPIO_MASK_UART_CSP_RTS  (GPIO_MASK(GPIO_PIN_UART_CSP_RTS))
#define GPIO_MASK_UART_CSP_CTS  (GPIO_MASK(GPIO_PIN_UART_CSP_CTS))
#define GPIO_MASK_UART_CSP_WU   (GPIO_MASK(GPIO_PIN_UART_CSP_WU))

#define GPIO_MASK_JTAG_TCK      (GPIO_MASK(GPIO_PIN_JTAG_TCK))
#define GPIO_MASK_JTAG_TDO      (GPIO_MASK(GPIO_PIN_JTAG_TDO))
#define GPIO_MASK_JTAG_TDI      (GPIO_MASK(GPIO_PIN_JTAG_TDI))
#define GPIO_MASK_JTAG_TMS      (GPIO_MASK(GPIO_PIN_JTAG_TMS))
//lint --flb


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// GPIO debounce time.
typedef enum
{
    /// Minimum debounce time of 32 us.
    GPIO_DEBOUNCE_TIME_32US   = 0,
    /// Minimum debounce time of 64 us.
    GPIO_DEBOUNCE_TIME_64US   = 1,
    /// Minimum debounce time of 128 us.
    GPIO_DEBOUNCE_TIME_128US  = 2,
    /// Minimum debounce time of 256 us.
    GPIO_DEBOUNCE_TIME_256US  = 3,
    /// Minimum debounce time of 512 us.
    GPIO_DEBOUNCE_TIME_512US  = 4,
    /// Minimum debounce time of 1024 us.
    GPIO_DEBOUNCE_TIME_1024US = 5,
    /// Minimum debounce time of 2048 us.
    GPIO_DEBOUNCE_TIME_2048US = 6,
    /// Minimum debounce time of 4096 us.
    GPIO_DEBOUNCE_TIME_4096US = 7,
} GPIO_DebounceTime_t;


/// Options for the debounce clock prescaler.
typedef enum
{
    /// Minimum debounce with a clock prescaler of 16.
    GPIO_DEBOUNCE_PRESCALER_16   = 0,
    /// Minimum debounce with a clock prescaler of 32.
    GPIO_DEBOUNCE_PRESCALER_32   = 1,
    /// Minimum debounce with a clock prescaler of 64.
    GPIO_DEBOUNCE_PRESCALER_64   = 2,
    /// Minimum debounce with a clock prescaler of 128.
    GPIO_DEBOUNCE_PRESCALER_128  = 3,
    /// Minimum debounce with a clock prescaler of 256.
    GPIO_DEBOUNCE_PRESCALER_256  = 4,
    /// Minimum debounce with a clock prescaler of 512.
    GPIO_DEBOUNCE_PRESCALER_512  = 5,
    /// Minimum debounce with a clock prescaler of 1024.
    GPIO_DEBOUNCE_PRESCALER_1024 = 6,
    /// Minimum debounce with a clock prescaler of 2048.
    GPIO_DEBOUNCE_PRESCALER_2048 = 7,
} GPIO_DebouncePrescaler_t;


/// Pin functions that may be assigned to a GPIO output pin.
/// @sa GPIO_SetOutputPinFunction
typedef enum
{
    /// Pin function for GPIO.
    GPIO_PIN_FUNC_OUT_GPIO           =  0,
    /// Pin function for SPI Slave MISO.
    GPIO_PIN_FUNC_OUT_SPIS_MISO      =  1,
    /// Pin function for SPI Slave Ready.
    GPIO_PIN_FUNC_OUT_SPIS_RDY       =  2,
    /// Pin function for UART TX.
    GPIO_PIN_FUNC_OUT_UART_TXD       =  3,
    /// Pin function for UART RTS.
    GPIO_PIN_FUNC_OUT_UART_RTS       =  4,
    /// Pin function for SPI Master SCK.
    GPIO_PIN_FUNC_OUT_SPIM_SCK       =  5,
    /// Pin function for SPI Master MOSI.
    GPIO_PIN_FUNC_OUT_SPIM_MOSI      =  6,
    /// Pin function for I2C Master SCK.
    GPIO_PIN_FUNC_OUT_I2C_SCK        =  7,
    /// Pin function for I2C Master SDA.
    GPIO_PIN_FUNC_OUT_I2C_SDA        =  8,
    /// Pin function for RF PA Enable.
    GPIO_PIN_FUNC_OUT_PA_ENABLE      =  9,
    /// Pin function for PML Pad Out.
    GPIO_PIN_FUNC_OUT_PML_PAD        = 12,
    /// Pin function for CTE.
    GPIO_PIN_FUNC_OUT_CTE            = 13,
    /// Pin function for IQ.
    GPIO_PIN_FUNC_OUT_IQ             = 14,
    /// Pin function for JTAG TDO.
    GPIO_PIN_FUNC_OUT_JTAG_TDO       = 15,
    /// Pin function for I2S SCLK
    GPIO_PIN_FUNC_OUT_I2S_SCLK       = 18,
    /// Pin function for I2S FSYNC
    GPIO_PIN_FUNC_OUT_I2S_FSYNC      = 19,
    /// Pin function for I2S SDATA
    GPIO_PIN_FUNC_OUT_I2S_SDATA      = 20,
    /// Pin function for I2S MCLK
    GPIO_PIN_FUNC_OUT_I2S_MCLK       = 21,
    /// Pin function for Timer 2 Out0.
    GPIO_PIN_FUNC_OUT_UNITIMER2_OUT0 = 24,
    /// Pin function for Timer 2 Out1.
    GPIO_PIN_FUNC_OUT_UNITIMER2_OUT1 = 25,
    /// Pin function for Timer 2 Out2.
    GPIO_PIN_FUNC_OUT_UNITIMER2_OUT2 = 26,
    /// Pin function for Timer 2 Out3.
    GPIO_PIN_FUNC_OUT_UNITIMER2_OUT3 = 27,
    /// Pin function for Timer 3 Out0.
    GPIO_PIN_FUNC_OUT_UNITIMER3_OUT0 = 28,
    /// Pin function for Timer 3 Out1.
    GPIO_PIN_FUNC_OUT_UNITIMER3_OUT1 = 29,
    /// Pin function for Timer 3 Out2.
    GPIO_PIN_FUNC_OUT_UNITIMER3_OUT2 = 30,
    /// Pin function for Timer 3 Out3.
    GPIO_PIN_FUNC_OUT_UNITIMER3_OUT3 = 31,
} GPIO_OutputPinFunction_t;


/// Functions that may be assigned to a GPIO input pin.
/// @sa GPIO_SetInputFunctionPin
typedef enum
{
    /// Pin function for SPI Slave CSN.
    GPIO_PIN_FUNC_IN_SPIS_CSN        =  0,
    /// Pin function for SPI Slave SCK.
    GPIO_PIN_FUNC_IN_SPIS_SCK        =  1,
    /// Pin function for SPI Slave MOSI.
    GPIO_PIN_FUNC_IN_SPIS_MOSI       =  2,
    /// Pin function for SPI Master MISO.
    GPIO_PIN_FUNC_IN_SPIM_MISO       =  3,
    /// Pin function for UART RX.
    GPIO_PIN_FUNC_IN_UART_RXD        =  4,
    /// Pin function for UART CTS.
    GPIO_PIN_FUNC_IN_UART_CTS        =  5,
    /// Pin function for Timer 2 Start.
    GPIO_PIN_FUNC_IN_UNITIMER2_START =  6,
    /// Pin function for Timer 2 Cap.
    GPIO_PIN_FUNC_IN_UNITIMER2_CAP   =  7,
    /// Pin function for Timer 3 Start.
    GPIO_PIN_FUNC_IN_UNITIMER3_START =  8,
    /// Pin function for Timer 3 Cap.
    GPIO_PIN_FUNC_IN_UNITIMER3_CAP   =  9,
    /// Pin function for Timer 2 Clock In.
    GPIO_PIN_FUNC_IN_UNITIMER2_CLK   = 10,
    /// Pin function for Timer 3 Clock In.
    GPIO_PIN_FUNC_IN_UNITIMER3_CLK   = 11,
    /// Pin function for I2S serial clock
    GPIO_PIN_FUNC_IN_I2S_SCLK        = 12,
    /// Pin function for I2S Frame sync
    GPIO_PIN_FUNC_IN_I2S_FSYNC       = 13,
    /// Pin function for I2S Serial data
    GPIO_PIN_FUNC_IN_I2S_SDATA       = 14,
    /// Number of functions that may be assigned to a GPIO input pin.
    GPIO_PIN_FUNC_IN_COUNT           = 15,
} GPIO_InputFunctionPin_t;


/// Bitmask for GPIO pins.
typedef enum
{
    /// Bitmask for GPIO pin 0.
    GPIO_MASK_PIN_0  = GPIO_MASK(0),
    /// Bitmask for GPIO pin 1.
    GPIO_MASK_PIN_1  = GPIO_MASK(1),
    /// Bitmask for GPIO pin 2.
    GPIO_MASK_PIN_2  = GPIO_MASK(2),
    /// Bitmask for GPIO pin 3.
    GPIO_MASK_PIN_3  = GPIO_MASK(3),
    /// Bitmask for GPIO pin 4.
    GPIO_MASK_PIN_4  = GPIO_MASK(4),
    /// Bitmask for GPIO pin 5.
    GPIO_MASK_PIN_5  = GPIO_MASK(5),
    /// Bitmask for GPIO pin 6.
    GPIO_MASK_PIN_6  = GPIO_MASK(6),
    /// Bitmask for GPIO pin 7.
    GPIO_MASK_PIN_7  = GPIO_MASK(7),
    /// Bitmask for GPIO pin 8.
    GPIO_MASK_PIN_8  = GPIO_MASK(8),
    /// Bitmask for GPIO pin 9.
    GPIO_MASK_PIN_9  = GPIO_MASK(9),
    /// Bitmask for GPIO pin 10.
    GPIO_MASK_PIN_10 = GPIO_MASK(10),
    /// Bitmask for GPIO pin 11.
    GPIO_MASK_PIN_11 = GPIO_MASK(11),
} GPIO_BitMask_t;

/// GPIO High Drive pins
typedef enum
{
    /// Pin 1 High Drive first bit of GPIOHighDrive in RegGPIOCfg
    GPIO_PIN1_HIGH_DRIVE = (1 << 0),
    /// Pin 2 High Drive second bit of GPIOHighDrive in RegGPIOCfg
    GPIO_PIN2_HIGH_DRIVE = (1 << 1),
    /// Pin 3 High Drive third bit of GPIOHighDrive in RegGPIOCfg
    GPIO_PIN3_HIGH_DRIVE = (1 << 2),
    /// USB GPIOs (6 & 7) High Drive bit 18 in RegGPIOCfg
    GPIO_USB_PINS_HIGH_DRIVE = (1 << 18),
} GPIO_HighDrivePin_t;

/**
 * @brief GPIO hardware state.
 */
typedef GPIO_PersistentMemory_t GPIO_HardwareState_t;


/**
 * @brief GPIO configuration data structure.
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Initial (boot) state of the GPIO hardware.
    GPIO_HardwareState_t hardwareState;
} GPIO_Configuration_t;


/******************************************************************************\
 *  GLOBAL VARIABLE REFERENCES
\******************************************************************************/

/// GPIO configuration data.
extern volatile GPIO_Configuration_t gGPIO_Config;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Set the minimum debounce time for all GPIO.
 * @param minTime The minimum debounce time in us.
 * @returns The minimum debounce time.
 */
uint16_t GPIO_SetMinimumDebounce(uint16_t minTime);


/**
 * @brief Get the minimum debounce time for all GPIO.
 * @returns The minimum debounce time in us.
 */
uint16_t GPIO_GetMinimumDebounce(void);


/**
 * @brief Set the desired GPIO output pin to perform the specified function.
 * @param gpio The GPIO output pin to set to the function of. This must be a
 * value less than @ref NUM_GPIO_PINS.
 * @param func The function for the GPIO output pin to perform.
 * @note Multiple GPIO output pins can be mapped to the same function.
 */
void GPIO_SetOutputPinFunction(uint8_t gpio, GPIO_OutputPinFunction_t func);


/**
 * @brief Get the function for the desired GPIO output pin.
 * @param gpio The GPIO output pin to set to get the function of. This must be
 * a value less than @ref NUM_GPIO_PINS.
 * @note Multiple GPIO output pins can be mapped to the same function.
 * @returns The function for the GPIO output pin.
 */
GPIO_OutputPinFunction_t GPIO_GetOutputPinFunction(uint8_t gpio);


/**
 * @brief Set the function to utilize the desired GPIO input pin.
 * @param gpio The GPIO input pin for the function to use. This must be a
 * value less than @ref NUM_GPIO_PINS.
 * @param func The function to set the GPIO input pin for.
 * @note Multiple functions can be mapped to the same GPIO input pins.
 */
void GPIO_SetInputFunctionPin(GPIO_InputFunctionPin_t func, uint8_t gpio);


/**
 * @brief Get the desired GPIO input pin for the function.
 * @param func The function to get the desired GPIO input pin of.
 * @note Multiple GPIO output pins can be mapped to the same function.
 * @returns The GPIO input pin for the desired function. This must be
 * a value less than @ref NUM_GPIO_PINS.
 */
uint8_t GPIO_GetInputFunctionPin(GPIO_InputFunctionPin_t func);


/**
 * @brief Enable the JTAG
 * @param mode JTAG_4WIRES or JTAG_2WIRES
 */
void GPIO_EnableJtag(uint8_t mode);


/**
 * @brief Disable the JTAG.
 */
void GPIO_DisableJtag(void);

/**
 * @brief Enable High Drive on GPIO
 * @param pin pin to enable high drive feature
 * @note Only GPIO 1,2,3 and 6,7 support High Drive feature
 */
void GPIO_EnableHighDrive(GPIO_HighDrivePin_t pin);

/**
 * @brief Disable High Drive on GPIO
 * @param pin pin to disable high drive feature
 * @note Only GPIO 1,2,3 and 6,7 support High Drive feature
 */
void GPIO_DisableHighDrive(GPIO_HighDrivePin_t pin);


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Read the state of all GPIO pins.
 * @note You need to wait a few clock cycles (about 4) before you sample the
 *  input after you enable pin as input using GPIO_EnableInput().
 * The least significant bit indicates the state of the first GPIO pin.
 * @returns The current state of the GPIO inputs.
 */
static ALWAYS_INLINE uint32_t GPIO_GetValues(void)
{
    return GPIO->RegGPIODataIn.r32;
}


/**
 * @brief Reads the specified GPIO input value.
 * @note You need to wait a few clock cycles (about 4) before you sample the
 *  input after you enable pin as input using GPIO_EnableInput().
 * @param gpio The GPIO input to read. This must be a value less than
 * @ref NUM_GPIO_PINS.
 * @retval TRUE  The GPIO input is high.
 * @retval FALSE The GPIO input is low.
 */
static ALWAYS_INLINE bool GPIO_GetValue(uint8_t gpio)
{
    return ((GPIO->RegGPIODataIn.r32 >> gpio) & 1) ? TRUE : FALSE;
}


/**
 * @brief Sets the state of all GPIO pins.
 * The least significant bit indicates the state of the first GPIO pin.
 * @param vals The desired state of the GPIO outputs.
 */
static ALWAYS_INLINE void GPIO_SetValues(uint32_t vals)
{
    GPIO->RegGPIODataOut.r32 = vals;
}


/**
 * @brief Set the specified GPIO pin high.
 * @param gpio The GPIO pin to set. This must be a value less than
 * @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_SetHigh(uint8_t gpio)
{
    GPIO->RegGPIODataOutSet.r32 = 1 << gpio;
}


/**
 * @brief Set the specified GPIO pin low.
 * @param gpio The GPIO pin to set. This must be a value less than
 * @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_SetLow(uint8_t gpio)
{
    GPIO->RegGPIODataOutClr.r32 = 1 << gpio;
}


/**
 * @brief Toggle the specified GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to toggle. This must be a value less than
 * @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_Toggle(uint8_t gpio)
{
    GPIO->RegGPIODataOut.r32 ^= (1 << gpio);
}


/**
 * @brief Enable the pull-up on the GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to enable the pull-up on. This must be a value
 * less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_EnablePullUp(uint8_t gpio)
{
    GPIO->RegGPIOPuEn.r32 |= 1 << gpio;
}


/**
 * @brief Disable the pull-up on the GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to disable the pull-up on. This must be a value
 * less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_DisablePullUp(uint8_t gpio)
{
    GPIO->RegGPIOPuEn.r32 &= ~(1 << gpio);
}


/**
 * @brief Enable the pull-down on the GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to enable the pull-down on. This must be a value
 * less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_EnablePullDown(uint8_t gpio)
{
    GPIO->RegGPIOPdEn.r32 |= 1 << gpio;
}


/**
 * @brief Disable the pull-up on the GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to disable the pull-up on. This must be a value
 * less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_DisablePullDown(uint8_t gpio)
{
    GPIO->RegGPIOPdEn.r32 &= ~(1 << gpio);
}


/**
 * @brief Enable the output driver of the specified GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to enable the output driver of. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_EnableOutput(uint8_t gpio)
{
    GPIO->RegGPIOOutputEn.r32 |= 1 << gpio;
}


/**
 * @brief Enable the input driver of the specified GPIO pin.
 * @note You need to wait a few clock cycles (about 4) before you sample the
 *  input. Failure to do this can produce an incorrect input value.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to enable the input driver of. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_EnableInput(uint8_t gpio)
{
    GPIO->RegGPIOInputEn.r32 |= 1 << gpio;
}


/**
 * @brief Disable the output driver of the specified GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to disable the output driver of. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_DisableOutput(uint8_t gpio)
{
    GPIO->RegGPIOOutputEn.r32 &= ~(1 << gpio);
}


/**
 * @brief Disable the input driver of the specified GPIO pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to disable the input driver of. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_DisableInput(uint8_t gpio)
{
    GPIO->RegGPIOInputEn.r32 &= ~(1 << gpio);
}


/**
 * @brief Enable the debouncer for the GPIO input pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to enable the debouncer for. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_EnableDebouncer(uint8_t gpio)
{
    PML->RegGPIOCfgDeb.r32 |= 1 << gpio;
}


/**
 * @brief Disable the debouncer for the GPIO input pin.
 * @note This function is not interrupt safe.
 * @param gpio The GPIO pin to disable the debouncer for. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_DisableDebouncer(uint8_t gpio)
{
    PML->RegGPIOCfgDeb.r32 &= ~(1 << gpio);
}


/**
 * @brief Set the minimum debounce time for all GPIO.
 * @param minTime The minimum debounce time.
 */
static ALWAYS_INLINE void GPIO_SetDebounceTime(GPIO_DebounceTime_t minTime)
{
    PML->RegGPIOCfgDeb.regs.GPIODebClock = (uint8_t)minTime;
}


/**
 * @brief Get the minimum debounce time for all GPIO.
 * @returns The minimum debounce time.
 */
static ALWAYS_INLINE GPIO_DebounceTime_t GPIO_GetDebounceTime(void)
{
    return (GPIO_DebounceTime_t)PML->RegGPIOCfgDeb.regs.GPIODebClock;
}


/**
 * @brief Set the minimum debounce time for all GPIO.
 * @param prescaler The minimum debounce time expressed as a clock prescaler.
 */
static ALWAYS_INLINE void GPIO_SetDebouncePrescaler(GPIO_DebouncePrescaler_t prescaler)
{
    PML->RegGPIOCfgDeb.regs.GPIODebClock = (uint16_t)prescaler;
}


/**
 * @brief Get the minimum debounce time for all GPIO.
 * @returns The minimum debounce time expressed as a clock prescaler.
 */
static ALWAYS_INLINE GPIO_DebouncePrescaler_t GPIO_GetDebouncePrescaler(void)
{
    return (GPIO_DebouncePrescaler_t)PML->RegGPIOCfgDeb.regs.GPIODebClock;
}


/**
 * @brief Set the desired GPIO pin to interrupt on the rising edge.
 * @param gpio The GPIO pin to set to the rising edge. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_SetPolarityRising(uint8_t gpio)
{
    GPIO->RegGPIOIRQPolarity.r32 &= ~(1 << gpio);
}


/**
 * @brief Set the desired GPIO pin to interrupt on the falling edge.
 * @param gpio The GPIO pin to set to the falling edge. This must be a
 * value less than @ref NUM_GPIO_PINS.
 */
static ALWAYS_INLINE void GPIO_SetPolarityFalling(uint8_t gpio)
{
    GPIO->RegGPIOIRQPolarity.r32 |= 1 << gpio;
}


/**
 * @brief Enable the JTAG (2 wires mode)
 */
static ALWAYS_INLINE void GPIO_EnableJtag2Wires(void)
{
    GPIO_EnableJtag(JTAG_2WIRES);
}


/**
 * @brief Enable the JTAG (4 wires mode)
 */
static ALWAYS_INLINE void GPIO_EnableJtag4Wires(void)
{
    GPIO_EnableJtag(JTAG_4WIRES);
}


/**
 * @brief Load the default GPIO configuration for SPI transport.
 * @note This function shall be called only in the function NVM_ConfigModules.
 */
static ALWAYS_INLINE void GPIO_LoadDefaultConfigSPI(void)
{
    gGPIO_Config.hardwareState.RegGPIOInputEn.r32 =
        GPIO_MASK_SPIS_CSN | GPIO_MASK_SPIS_SCK | GPIO_MASK_SPIS_MOSI;
    gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 =
        GPIO_MASK_SPIS_MISO | GPIO_MASK_SPIS_RDY;
    gGPIO_Config.hardwareState.RegGPIOOutSel0.regs.GPIOOutSel2 =
        (uint8_t)GPIO_PIN_FUNC_OUT_SPIS_MISO;
    gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel4 =
        (uint8_t)GPIO_PIN_FUNC_OUT_SPIS_RDY;
    gGPIO_Config.hardwareState.RegGPIOInpSel0.regs.GPIOInSelSSCSN =
        (uint8_t)GPIO_PIN_SPIS_CSN;
    gGPIO_Config.hardwareState.RegGPIOInpSel0.regs.GPIOInSelSSSCK =
        (uint8_t)GPIO_PIN_SPIS_SCK;
    gGPIO_Config.hardwareState.RegGPIOInpSel0.regs.GPIOInSelSSMOSI =
        (uint8_t)GPIO_PIN_SPIS_MOSI;
}


/**
 * @brief Load the default GPIO configuration for UART transport.
 * @note This function shall be called only in the function NVM_ConfigModules.
 */
static ALWAYS_INLINE void GPIO_LoadDefaultConfigUART(void)
{
    gGPIO_Config.hardwareState.RegGPIOInputEn.r32 =
        GPIO_MASK_UART_RXD | GPIO_MASK_UART_CTS |
        GPIO_MASK_UART_WU;
    gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 =
        GPIO_MASK_UART_TXD | GPIO_MASK_UART_RTS;
    gGPIO_Config.hardwareState.RegGPIOPuEn.r32 =
        GPIO_MASK_UART_RXD | GPIO_MASK_UART_TXD |
        GPIO_MASK_UART_RTS | GPIO_MASK_UART_CTS;
    gGPIO_Config.hardwareState.RegGPIOOutSel1.regs.GPIOOutSel7 =
        (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;
    gGPIO_Config.hardwareState.RegGPIOOutSel2.regs.GPIOOutSel10 =
        (uint8_t)GPIO_PIN_FUNC_OUT_UART_RTS;
    gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
        (uint8_t)GPIO_PIN_UART_RXD;
    gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTCTS =
        (uint8_t)GPIO_PIN_UART_CTS;
}


/**
 * @brief Load the default GPIO configuration for UART transport on CSP.
 * @note This function shall be called only in the function NVM_ConfigModules.
 */
static ALWAYS_INLINE void GPIO_LoadDefaultConfigUARTCSP(void)
{
    gGPIO_Config.hardwareState.RegGPIOInputEn.r32 =
        GPIO_MASK_UART_CSP_RXD | GPIO_MASK_UART_CSP_CTS |
        GPIO_MASK_UART_CSP_WU;
    gGPIO_Config.hardwareState.RegGPIOOutputEn.r32 =
        GPIO_MASK_UART_CSP_TXD | GPIO_MASK_UART_CSP_RTS;
    gGPIO_Config.hardwareState.RegGPIOPuEn.r32 =
        GPIO_MASK_UART_CSP_RXD | GPIO_MASK_UART_CSP_TXD |
        GPIO_MASK_UART_CSP_RTS | GPIO_MASK_UART_CSP_CTS;
    gGPIO_Config.hardwareState.RegGPIOOutSel2.regs.GPIOOutSel8 =
        (uint8_t)GPIO_PIN_FUNC_OUT_UART_TXD;
    gGPIO_Config.hardwareState.RegGPIOOutSel2.regs.GPIOOutSel10 =
        (uint8_t)GPIO_PIN_FUNC_OUT_UART_RTS;
    gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTRDX =
        (uint8_t)GPIO_PIN_UART_CSP_RXD;
    gGPIO_Config.hardwareState.RegGPIOInpSel1.regs.GPIOInSelUARTCTS =
        (uint8_t)GPIO_PIN_UART_CSP_CTS;
}


/** @} */ // End of group gpio
/** @} */ // End of addtogroup drivers


#endif /* _GPIO_H */
