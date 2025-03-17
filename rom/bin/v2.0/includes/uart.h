////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/uart/includes/uart.h
///
/// @project    T9305
///
/// @brief      UART drivers
///
/// This peripheral is responsible of sending/receiving data through UART.
///
/// Callback function can be assigned to both RX and TX operations allowing
///  application to be notified once given number of bytes is sent/received.
///
/// Other functions to configure UART (speed, enable, modes, ...) are also
/// available.
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


#ifndef _UART_H
#define _UART_H


#include <driver.h>
#include <config.h>
#include <transport.h>
#include <t9305_periph.h>


/**
 * @addtogroup drivers
 * @{
 * @defgroup uart UART Driver
 * @brief UART driver.
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// Size of the transmit FIFO (in bytes).
#define UART_TX_FIFO_DEPTH (16)


/// Size of the receive FIFO (in bytes).
/// TODO: Check if 2 byte margin as in original FIFO size is OK.
#define UART_RX_FIFO_DEPTH (14)


/******************************************************************************\
 *  ENUMERATIONS / TYPE DEFINITIONS
\******************************************************************************/

/// UART status.
typedef enum
{
    /// Success, responded with expected data.
    UART_OK = 0,
    /// Cannot perform request due to its busy state.
    UART_BUSY,
    /// Cannot perform request due to an error.
    UART_BAD,
} UART_Status_t;


/**
 * @brief UART enable control.
 */
typedef uint8_t UART_Enable_t;


/// Disable both receiver and transmitter.
//lint -emacro((845), UART_DISABLE) argument to operator is certain to be 0 (in some cases)
#define  UART_DISABLE       (UART_RX_EN(FALSE) | UART_TX_EN(FALSE))
/// Enable receiver only, transmitter disabled.
#define  UART_ENABLE_RX     (UART_RX_EN(TRUE))
/// Enable transmitter only, receiver disabled.
#define  UART_ENABLE_TX     (UART_TX_EN(TRUE))
/// Enable both receiver and transmitter.
#define  UART_ENABLE        (UART_RX_EN(TRUE) | UART_TX_EN(TRUE))


 /******************************************************************************\
 *  STRUCTURES
\******************************************************************************/

/// UART configuration.
typedef union
{
    /// UART config byte.
    uint8_t configByte;

    /// bit access
    struct
    {
        /// UART MSBit mode.
        uint8_t msbFirst    : 1;
        /// UART parity enable.
        uint8_t parityMode  : 1;
        /// UART even parity.
        uint8_t evenParity  : 1;
        /// UART flow control enable.
        uint8_t flowCtrlEn  : 1;
        /// UART majority vote disable in oversampling mode.
        uint8_t majorityDis : 1;
        /// UART loop back enable.
        uint8_t loopBackEn  : 1;
        /// Padding bits.
        uint8_t reserved    : 2;
    } bits;
} UART_Config_t;


/// Data structure holding the information needed for a UART transaction.
typedef struct
{
    /// Pointer to the next byte of the data to be sent or received.
    uint8_t *pData;
    /// Pointer to user defined data to provide to the callback.
    void *pUserData;
    /// Function to call inside the UART ISR on an event.
    Driver_Callback_t pCallback;
    /// Number of bytes remaining to be sent/received for this transaction.
    uint16_t dataLength;
    /// Reserved for future use.
    uint8_t reserved[2];
} UART_Transaction_t;


/// UART configuration data structure.
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// Baud rate to set the UART to.
    uint32_t baudRate;

    /// RX transaction buffer.
    UART_Transaction_t *pRxBuffer;

    /// TX transaction buffer.
    UART_Transaction_t *pTxBuffer;

    /// Size of the RX transaction buffer.
    uint16_t rxBufferSize;

    /// Size of the RX transaction buffer.
    uint16_t txBufferSize;

    /// Default configuration bits for the UART.
    UART_Config_t configBits;

    /// If the UART module is enabled.
    UART_Enable_t enabled;

    /// Wake-up GPIO for the UART transport.
    uint8_t wakeUpGpio;

    /// Polarity of the wake-up GPIO.
    uint8_t wakeUpPolarity;

    /// DMA channel for TX operation.
    uint8_t  dmaChannelTx;

    /// DMA channel for RX operation.
    uint8_t  dmaChannelRx;
} UART_Configuration_t;


/******************************************************************************\
 *  REFERENCE TO GLOBAL
\******************************************************************************/


extern volatile UART_Configuration_t gUART_Config;
extern const Transport_t gUART_Transport;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Restart the UART driver.
 * @returns true if the restart was successful, false otherwise.
 *
 * This will flush the TX and RX FIFO and queues.
 * This will restart the UART block.
 */
bool UART_Restart(void);


/**
 * @brief Enable/disable UART receiver and/or transmitter.
 * @param enable Enable/disable for receiver/transmitter.
 * @returns UART_OK if the receiver and/or transmitter were successfuly
 * enabled/disabled.
 * @note This function does not do any configuration. Enabling should
 * normally be done after initialization is done (if not enabled as part
 * of init). If the receiver or transmitter was previously enabled and not
 * re-enabled by this function, it will be disabled after calling this function.
 */
UART_Status_t UART_Enable(UART_Enable_t enable);


/**
 * @brief Change the UART configuration.
 * @param config Configuration structure for UART.
 * @returns UART_OK if the UART configuration was changed.
 * @note UART configuration can be changed only if UART is not enabled.
 */
UART_Status_t UART_SetConfiguration(UART_Config_t config);


/**
 * @brief Block UART incomming communication by forcing RTS.
 * @param blocking Enable/disable for blocking incomming communication.
 */
void UART_BlockCommunication(bool blocking);


/**
 * @brief Set the baud rate manually.
 * @param clkDivider Clock divider for UART. Valid values: <1, 16383>.
 * @param oversampling Oversampling factor. Valid values: <4, 16>.
 * @returns UART_OK if the baud rate was set, UART_BUSY otherwise.
 * @note The UART baud rate can be set only if UART is not enabled. If UART is
 * running, the request is rejected.
 */
UART_Status_t UART_SetBaudRateEx(uint16_t clkDivider, uint8_t oversampling);


/**
 * @brief Set the baud rate.
 * @param baud Desired baud rate.
 * @returns Calculated baud rate or 0 if the UART is busy or the baud rate is
 * invalid (too high or too low).
 * @note The UART baud rate can be set only if UART is not enabled. If UART is
 * running, the request is rejected.
 */
uint32_t UART_SetBaudRate(uint32_t baud);


#if 1 == UART_INTERNAL


/**
 * @brief Initialize the UART ring buffers.
 * @param pRxBuffer Pointer to the first element in the receive ring buffer.
 * @param rxSize Number of elements in the receive ring buffer.
 * @param pTxBuffer Pointer to the first element in the transmit ring buffer.
 * @param txSize Number of elements in the transmit ring buffer.
 *
 * This method allows for changing the size of the ring buffer. Simply call this
 * method with a different buffer location and size inside of a patch.
 */
void UART_InitBuffers(UART_Transaction_t *pRxBuffer, uint16_t rxSize,
    UART_Transaction_t *pTxBuffer, uint16_t txSize);


#else /* !UART_INTERNAL */


/**
 * @brief Initialize the UART ring buffers.
 * @param pRxBuffer Pointer to the first element in the receive ring buffer.
 * @param rxSize Number of elements in the receive ring buffer.
 * @param pTxBuffer Pointer to the first element in the transmit ring buffer.
 * @param txSize Number of elements in the transmit ring buffer.
 *
 * This method allows for changing the size of the ring buffer. Simply call this
 * method with a different buffer location and size inside of a patch.
 */
void UART_InitBuffers(UART_Transaction_t *pRxBuffer, uint16_t rxSize,
    UART_Transaction_t *pTxBuffer, uint16_t txSize) __attribute__((deprecated(
    "This function is now internal and used during initialization.")));


#endif /* !UART_INTERNAL */


/**
 * @brief Send the data across the UART.
 * @param pData Data to send across the UART.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
bool UART_SendDataEx(const void *pData, uint16_t dataLength,
    Driver_Callback_t pCallback, void *pUserData);


/**
 * @brief Receive data from the UART.
 * @param pData Data to receive from the UART.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
bool UART_ReceiveDataEx(void *pData, uint16_t dataLength,
    Driver_Callback_t pCallback, void *pUserData);


/**
 * @brief Check if UART driver has some pending data.
 * @returns true if there are some pending data, false otherwise.
 */
bool UART_HasPendingData(void);


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/


#ifndef ROM_BUILD
/**
 * @brief Add the UART module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
static ALWAYS_INLINE void UART_RegisterModule(void)
{
    // Write to the module list the address of the UART config
    gConfig.pModuleConfig[CONFIG_MODULE_UART] = (Config_ModuleConfiguration_t*)&gUART_Config;
}
#endif


/**
 * @brief Send the data across the UART.
 * @param pData Data to send across the UART.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion.
 * @param pUserData Data to pass to the callback upon error or completion.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool UART_SendDataWithCallback(const void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    return UART_SendDataEx(pData, dataLength, pCallback, pUserData);
}


/**
 * @brief Send the data across the UART.
 * @param pData Data to send across the UART.
 * @param dataLength Length (in bytes) of the data to send.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool UART_SendData(const void *pData,
    uint16_t dataLength)
{
    return UART_SendDataEx(pData, dataLength, NULL, NULL);
}


/**
 * @brief Receive data from the UART.
 * @param pData Data to receive from the UART.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion.
 * @param pUserData Data to pass to the callback upon error or completion.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool UART_ReceiveDataWithCallback(void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    return UART_ReceiveDataEx(pData, dataLength, pCallback, pUserData);
}


/**
 * @brief Receive data from the UART.
 * @param pData Data to receive from the UART.
 * @param dataLength Length (in bytes) of the data to receive.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool UART_ReceiveData(void *pData,
    uint16_t dataLength)
{
    return UART_ReceiveDataEx(pData, dataLength, NULL, NULL);
}


/**
 * @brief Check if UART driver is busy.
 * @returns true if the UART is busy, false otherwise.
 */
static ALWAYS_INLINE bool UART_IsBusy(void)
{
    return 0u != (UART->RegUARTStat.r32 &
        (UART_RX_BUSY_MASK | UART_TX_BUSY_MASK));
}


/** @} */ // End of group uart
/** @} */ // End of addtogroup drivers


#endif /* _UART_H */
