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
 * @defgroup gpio_def GPIO definitions
 * @{
 */


/**
 * @brief Options for the minimum debounce time.
 * @ingroup gpio_def
 * @{
 */
/// Minimum debounce time of 32 us.
#define GPIO_DEBOUNCE_TIME_32US   (0u)
/// Minimum debounce time of 64 us.
#define GPIO_DEBOUNCE_TIME_64US   (1u)
/// Minimum debounce time of 128 us.
#define GPIO_DEBOUNCE_TIME_128US  (2u)
/// Minimum debounce time of 256 us.
#define GPIO_DEBOUNCE_TIME_256US  (3u)
/// Minimum debounce time of 512 us.
#define GPIO_DEBOUNCE_TIME_512US  (4u)
/// Minimum debounce time of 1024 us.
#define GPIO_DEBOUNCE_TIME_1024US (5u)
/// Minimum debounce time of 2048 us.
#define GPIO_DEBOUNCE_TIME_2048US (6u)
/// Minimum debounce time of 4096 us.
#define GPIO_DEBOUNCE_TIME_4096US (7u)
/**
 * @}
 */


/**
 * @brief Options for the debounce clock prescaler.
 * @ingroup gpio_def
 * @{
 */
/// Minimum debounce with a clock prescaler of 16.
#define GPIO_DEBOUNCE_PRESCALER_16   (0u)
/// Minimum debounce with a clock prescaler of 32.
#define GPIO_DEBOUNCE_PRESCALER_32   (1u)
/// Minimum debounce with a clock prescaler of 64.
#define GPIO_DEBOUNCE_PRESCALER_64   (2u)
/// Minimum debounce with a clock prescaler of 128.
#define GPIO_DEBOUNCE_PRESCALER_128  (3u)
/// Minimum debounce with a clock prescaler of 256.
#define GPIO_DEBOUNCE_PRESCALER_256  (4u)
/// Minimum debounce with a clock prescaler of 512.
#define GPIO_DEBOUNCE_PRESCALER_512  (5u)
/// Minimum debounce with a clock prescaler of 1024.
#define GPIO_DEBOUNCE_PRESCALER_1024 (6u)
/// Minimum debounce with a clock prescaler of 2048.
#define GPIO_DEBOUNCE_PRESCALER_2048 (7u)
/**
 * @}
 */


/**
 * @brief Pin functions that may be assigned to a GPIO output pin.
 * @sa GPIO_SetOuputPinFunction
 * @ingroup gpio_def
 * @{
 */
/// Pin function for GPIO.
#define GPIO_PIN_FUNC_OUT_GPIO              (0u)
/// Pin function for SPI Slave MISO.
#define GPIO_PIN_FUNC_OUT_SPIS_MISO         (1u)
/// Pin function for SPI Slave Ready.
#define GPIO_PIN_FUNC_OUT_SPIS_RDY          (2u)
/// Pin function for UART TX.
#define GPIO_PIN_FUNC_OUT_UART_TXD          (3u)
/// Pin function for UART RTS.
#define GPIO_PIN_FUNC_OUT_UART_RTS          (4u)
/// Pin function for SPI Master SCK.
#define GPIO_PIN_FUNC_OUT_SPIM_SCK          (5u)
/// Pin function for SPI Master MOSI.
#define GPIO_PIN_FUNC_OUT_SPIM_MOSI         (6u)
/// Pin function for I2C Master SCK.
#define GPIO_PIN_FUNC_OUT_I2C_SCK           (7u)
/// Pin function for I2C Master SDA.
#define GPIO_PIN_FUNC_OUT_I2C_SDA           (8u)
/// Pin function for RF PA Enable.
#define GPIO_PIN_FUNC_OUT_PA_ENABLE         (9u)
/// Pin function for PML Pad Out.
#define GPIO_PIN_FUNC_OUT_PML_PAD           (12u)
/// Pin function for CTE.
#define GPIO_PIN_FUNC_OUT_CTE               (13u)
/// Pin function for IQ.
#define GPIO_PIN_FUNC_OUT_IQ                (14u)
/// Pin function for JTAG TDO.
#define GPIO_PIN_FUNC_OUT_JTAG_TDO          (15u)
/// Pin function for I2S SCLK
#define GPIO_PIN_FUNC_OUT_I2S_SCLK          (18u)
/// Pin function for I2S FSYNC
#define GPIO_PIN_FUNC_OUT_I2S_FSYNC         (19u)
/// Pin function for I2S SDATA
#define GPIO_PIN_FUNC_OUT_I2S_SDATA         (20u)
/// Pin function for I2S MCLK
#define GPIO_PIN_FUNC_OUT_I2S_MCLK          (21u)
/// Pin function for Timer 2 Out0.
#define GPIO_PIN_FUNC_OUT_UNITIMER2_OUT0    (24u)
/// Pin function for Timer 2 Out1.
#define GPIO_PIN_FUNC_OUT_UNITIMER2_OUT1    (25u)
/// Pin function for Timer 2 Out2.
#define GPIO_PIN_FUNC_OUT_UNITIMER2_OUT2    (26u)
/// Pin function for Timer 2 Out3.
#define GPIO_PIN_FUNC_OUT_UNITIMER2_OUT3    (27u)
/// Pin function for Timer 3 Out0.
#define GPIO_PIN_FUNC_OUT_UNITIMER3_OUT0    (28u)
/// Pin function for Timer 3 Out1.
#define GPIO_PIN_FUNC_OUT_UNITIMER3_OUT1    (29u)
/// Pin function for Timer 3 Out2.
#define GPIO_PIN_FUNC_OUT_UNITIMER3_OUT2    (30u)
/// Pin function for Timer 3 Out3.
#define GPIO_PIN_FUNC_OUT_UNITIMER3_OUT3    (31u)
/**
 * @}
 */


/**
 * @brief Functions that may be asigned a GPIO input pin.
 * @sa GPIO_SetInputFunctionPin
 * @ingroup gpio_def
 * @{
 */
/// Pin function for SPI Slave CSN.
#define GPIO_PIN_FUNC_IN_SPIS_CSN          (0u)
/// Pin function for SPI Slave SCK.
#define GPIO_PIN_FUNC_IN_SPIS_SCK          (1u)
/// Pin function for SPI Slave MOSI.
#define GPIO_PIN_FUNC_IN_SPIS_MOSI         (2u)
/// Pin function for SPI Master MISO.
#define GPIO_PIN_FUNC_IN_SPIM_MISO         (3u)
/// Pin function for UART RX.
#define GPIO_PIN_FUNC_IN_UART_RXD          (4u)
/// Pin function for UART CTS.
#define GPIO_PIN_FUNC_IN_UART_CTS          (5u)
/// Pin function for Timer 2 Start.
#define GPIO_PIN_FUNC_IN_UNITIMER2_START   (6u)
/// Pin function for Timer 2 Cap.
#define GPIO_PIN_FUNC_IN_UNITIMER2_CAP     (7u)
/// Pin function for Timer 3 Start.
#define GPIO_PIN_FUNC_IN_UNITIMER3_START   (8u)
/// Pin function for Timer 3 Cap.
#define GPIO_PIN_FUNC_IN_UNITIMER3_CAP     (9u)
/// Pin function for Timer 2 Clock In.
#define GPIO_PIN_FUNC_IN_UNITIMER2_CLK     (10u)
/// Pin function for Timer 3 Clock In.
#define GPIO_PIN_FUNC_IN_UNITIMER3_CLK     (11u)
/// Pin function for I2S serial clock
#define GPIO_PIN_FUNC_IN_I2S_SCLK          (12u)
/// Pin function for I2S Frame sync
#define GPIO_PIN_FUNC_IN_I2S_FSYNC         (13u)
/// Pin function for I2S Serial data
#define GPIO_PIN_FUNC_IN_I2S_SDATA         (14u)
/// Number of functions that may be asigned a GPIO input pin.
#define GPIO_PIN_FUNC_IN_COUNT             (15u)
/**
 * @}
 */


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


/**
 * @brief The total number of GPIO pins available.
 */
#define NUM_GPIO_PINS (12u)


/**
 * @brief State of a GPIO (high or low).
 * @{
 */
/// GPIO is or should be set high (logic 1).
#define GPIO_STATE_HIGH (1)
/// GPIO is or should be set low (logic 0).
#define GPIO_STATE_LOW  (0)
/**
 * @}
 */


/**
 * @brief Which edge to interrupt on.
 * @{
 */
/// Interrupt on the rising edge (transition from 0 to 1).
#define GPIO_POLARITY_RISING  (1)
/// Interrupt on the falling edge (transition from 1 to 0).
#define GPIO_POLARITY_FALLING (0)
/**
 * @}
 */


/**
 * @brief Bitmask for GPIO pins.
 * @{
 */
/// Bitmask for GPIO pin 0.
#define GPIO_MASK_PIN_0  (GPIO_MASK(0))
/// Bitmask for GPIO pin 1.
#define GPIO_MASK_PIN_1  (GPIO_MASK(1))
/// Bitmask for GPIO pin 2.
#define GPIO_MASK_PIN_2  (GPIO_MASK(2))
/// Bitmask for GPIO pin 3.
#define GPIO_MASK_PIN_3  (GPIO_MASK(3))
/// Bitmask for GPIO pin 4.
#define GPIO_MASK_PIN_4  (GPIO_MASK(4))
/// Bitmask for GPIO pin 5.
#define GPIO_MASK_PIN_5  (GPIO_MASK(5))
/// Bitmask for GPIO pin 6.
#define GPIO_MASK_PIN_6  (GPIO_MASK(6))
/// Bitmask for GPIO pin 7.
#define GPIO_MASK_PIN_7  (GPIO_MASK(7))
/// Bitmask for GPIO pin 8.
#define GPIO_MASK_PIN_8  (GPIO_MASK(8))
/// Bitmask for GPIO pin 9.
#define GPIO_MASK_PIN_9  (GPIO_MASK(9))
/// Bitmask for GPIO pin 10.
#define GPIO_MASK_PIN_10 (GPIO_MASK(10))
/// Bitmask for GPIO pin 11.
#define GPIO_MASK_PIN_11 (GPIO_MASK(11))
/**
 * @}
 */


/**
 * @brief Pin mapping for the JTAG.
 * @{
 */
/// JTAG clock pin.
#define GPIO_PIN_JTAG_TDI                  (8u)
/// JTAG data out pin.
#define GPIO_PIN_JTAG_TDO                  (9u)
/// JTAG data in pin.
#define GPIO_PIN_JTAG_TCK                  (10u)
/// JTAG mode select pin.
#define GPIO_PIN_JTAG_TMS                  (11u)
/**
 * @}
 */


/**
 * @brief JTAG configuration.
 * @{
 */
/// JTAG 4 wires configuration.
#define JTAG_4WIRES                        (1u)
/// JTAG data out pin.
#define JTAG_2WIRES                        (2u)
/**
 * @}
 */


/**
 * @} end of group gpio_def
 */


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @defgroup gpio_typedef GPIO Type definitions
 * @{
 */


/**
 * @brief GPIO hardware state.
 * @ingroup gpio
 */
typedef GPIO_PersistentMemory_t GPIO_HardwareState_t;


/**
 * @brief GPIO configuration data structure.
 * @ingroup gpio
 */
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Initial (boot) state of the GPIO hardware.
    GPIO_HardwareState_t hardwareState;
} GPIO_Configuration_t;


/**
 * @} end of group gpio_typedef
 */


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @defgroup gpio_fct GPIO Function prototypes
 * @{
 */


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
void GPIO_SetOutputPinFunction(uint8_t gpio, uint8_t func);


/**
 * @brief Get the function for the desired GPIO output pin.
 * @param gpio The GPIO output pin to set to get the function of. This must be
 * a value less than @ref NUM_GPIO_PINS.
 * @note Multiple GPIO output pins can be mapped to the same function.
 * @returns The function for the GPIO output pin.
 */
uint8_t GPIO_GetOutputPinFunction(uint8_t gpio);


/**
 * @brief Set the function to utilize the desired GPIO input pin.
 * @param gpio The GPIO input pin for the function to use. This must be a
 * value less than @ref NUM_GPIO_PINS.
 * @param func The function to set the GPIO input pin for.
 * @note Multiple functions can be mapped to the same GPIO input pins.
 */
void GPIO_SetInputFunctionPin(uint8_t func, uint8_t gpio);


/**
 * @brief Get the desired GPIO input pin for the function.
 * @param func The function to get the desired GPIO input pin of.
 * @note Multiple GPIO output pins can be mapped to the same function.
 * @returns The GPIO input pin for the desired function. This must be
 * a value less than @ref NUM_GPIO_PINS.
 */
uint8_t GPIO_GetInputFunctionPin(uint8_t func);


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
 * @} end of group gpio_fct
 */


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @defgroup gpio_inline GPIO inline function prototypes
 * @{
 */


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
static ALWAYS_INLINE void GPIO_SetDebounceTime(uint8_t minTime)
{
    PML->RegGPIOCfgDeb.regs.GPIODebClock = minTime;
}


/**
 * @brief Get the minimum debounce time for all GPIO.
 * @returns The minimum debounce time.
 */
static ALWAYS_INLINE uint8_t GPIO_GetDebounceTime(void)
{
    return (uint8_t)PML->RegGPIOCfgDeb.regs.GPIODebClock;
}


/**
 * @brief Set the minimum debounce time for all GPIO.
 * @param prescale The minimum debounce time expressed as a clock prescaler.
 */
static ALWAYS_INLINE void GPIO_SetDebouncePrescaler(uint8_t prescale)
{
    PML->RegGPIOCfgDeb.regs.GPIODebClock = prescale;
}


/**
 * @brief Get the minimum debounce time for all GPIO.
 * @returns The minimum debounce time expressed as a clock prescaler.
 */
static ALWAYS_INLINE uint8_t GPIO_GetDebouncePrescaler(void)
{
    return (uint8_t)PML->RegGPIOCfgDeb.regs.GPIODebClock;
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
static ALWAYS_INLINE void GPIO_EnableJtag2Wires()
{
    GPIO_EnableJtag(JTAG_2WIRES);
}


/**
 * @brief Enable the JTAG (4 wires mode)
 */
static ALWAYS_INLINE void GPIO_EnableJtag4Wires()
{
    GPIO_EnableJtag(JTAG_4WIRES);
}


/**
 * @} end of group gpio_inline
 */


/**
 * @} end of group gpio
 * @} end of addtogroup drivers
 */


#endif /* _GPIO_H */
