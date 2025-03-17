////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/spi_slave/includes/spi_slave.h
///
/// @project    T9305
///
/// @brief      SPI slave drivers
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


#ifndef _SPI_SLAVE_H
#define _SPI_SLAVE_H


#include <driver.h>
#include <config.h>
#include <transport.h>
#include <t9305_periph.h>


/**
 * @addtogroup drivers
 * @{
 * @defgroup spis SPI Slave Driver
 * @brief SPI slave driver.
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/


/// Size of the transmit FIFO (in bytes).
#define SPIS_TX_FIFO_DEPTH (16)


/// Size of the receive FIFO (in bytes).
#define SPIS_RX_FIFO_DEPTH (16)


/******************************************************************************\
 *  ENUMERATIONS / TYPE DEFINITIONS
\******************************************************************************/


/// SPI slave status.
typedef enum
{
    /// Success, responded with expected data.
    SPIS_OK = 0,
    /// Cannot perform request due to its busy state.
    SPIS_BSY,
    /// Cannot perform request due to an error.
    SPIS_BAD,
} SPIS_Status_t;


/**
 * @brief SPI slave enable control.
 */
typedef uint8_t SPIS_Enable_t;


/// Disable the SPI slave module.
//lint -emacro((845), SPIS_DISABLE) argument to operator is certain to be 0
#define  SPIS_DISABLE       (SPIS_EN(FALSE))
/// Enable the SPI slave module.
#define  SPIS_ENABLE        (SPIS_EN(TRUE))


/**
 * @brief SPI slave mode control.
 */
typedef uint8_t SPIS_Mode_t;


/// Select the SPI slave HALF DUPLEX mode.
#define  SPIS_MODE_HALF       (FALSE)
/// Select the SPI slave FULL DUPLEX mode.
#define  SPIS_MODE_FULL       (TRUE)


/******************************************************************************\
 *  STRUCTURES
\******************************************************************************/


/// SPI slave configuration.
typedef union
{
    /// SPI slave config byte.
    uint8_t configByte;

    /// bit access
    struct
    {
        /// Send/receive data with the most significant bit (MSB) first.
        uint8_t msbFirst : 1;

        /// Clock polarity.
        uint8_t cpol     : 1;

        /// Clock phase.
        uint8_t cpha     : 1;

        /**
         * @brief Which SPI mode to use.
         *
         * Options are:
         *  - @ref SPIS_MODE_HALF Half duplex mode limits traffic to one
         *    direction at a time. This is the required mode for HCI.
         *  - @ref SPIS_MODE_FULL Full duplex mode allows traffic in both
         *    directions. Note that no information is provided to the SPI
         *    master as to the status of the FIFOs.
         */
        SPIS_Mode_t mode : 1;

        /// Padding bits.
        uint8_t reserved : 4;
    } bits;
} SPIS_Config_t;


/// Data structure for an SPI slave transaction.
typedef struct
{
    /// Pointer to the next byte of the data to be sent or received.
    uint8_t *pData;
    /// Pointer to user defined data to provide to the callback.
    void *pUserData;
    /// Function to call inside the SPI slave ISR on an event.
    Driver_Callback_t pCallback;
    /// Number of bytes remaining to be sent/received for this transaction.
    uint16_t dataLength;
    /// Reserved for future use.
    uint8_t reserved[2];
} SPIS_Transaction_t;


/// SPI slave configuration data structure.
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;

    /// RX transaction buffer.
    SPIS_Transaction_t *pRxBuffer;

    /// TX transaction buffer.
    SPIS_Transaction_t *pTxBuffer;

    /// Size of the RX transaction buffer.
    uint16_t rxBufferSize;

    /// Size of the RX transaction buffer.
    uint16_t txBufferSize;

    /// Default configuration bits for the SPI slave.
    SPIS_Config_t configBits;

    /// If the SPI slave module is enabled.
    SPIS_Enable_t enabled;

    /// Constrols the STS1 byte of the SPI transport.
    uint8_t sts1;

    /// DMA channel for TX operation.
    uint8_t  dmaChannelTx;

    /// DMA channel for RX operation.
    uint8_t  dmaChannelRx;
} SPIS_Configuration_t;


/******************************************************************************\
 *  REFERENCE TO GLOBAL
\******************************************************************************/

/// SPI configuration
extern volatile SPIS_Configuration_t gSPIS_Config;
/// SPI transport configuration
extern const Transport_t gSPIS_Transport;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Restart the SPI slave driver.
 * @returns true if the restart was successful, false otherwise.
 *
 * This will flush the TX and RX FIFO and queues.
 * This will restart the SPI slave block.
 */
bool SPIS_Restart(void);


/**
 * @brief Enable/disable SPI slave module.
 * @param enable If the module should be enabled or disabled.
 * @returns SPIS_OK if the module was successfuly enabled/disabled.
 * @note This function does not do any configuration. Enabling should
 * normally be done after initialization is done (if not enabled as part
 * of init).
 */
SPIS_Status_t SPIS_Enable(SPIS_Enable_t enable);


/**
 * @brief Change the SPI slave configuration.
 * @param config Configuration structure for the SPI slave.
 * @param sts1 Value to set the STS1 byte to.
 * @returns SPIS_OK if the SPI slave configuration was changed.
 * @note SPI slave configuration can be changed only if the SPI slave module
 * is not enabled.
 */
SPIS_Status_t SPIS_SetConfiguration(SPIS_Config_t config, uint8_t sts1);


/**
 * @brief Block SPI slave communication by forcing RDY to 0.
 * @param blocking Enable/disable for blocking communication.
 */
void SPIS_BlockCommunication(bool blocking);


#if 1 == SPIS_INTERNAL


/**
 * @brief Initialize the SPI slave ring buffers.
 * @param pRxBuffer Pointer to the first element in the receive ring buffer.
 * @param rxSize Number of elements in the receive ring buffer.
 * @param pTxBuffer Pointer to the first element in the transmit ring buffer.
 * @param txSize Number of elements in the transmit ring buffer.
 *
 * This method allows for changing the size of the ring buffer. Simply call this
 * method with a different buffer location and size inside of a patch.
 */
void SPIS_InitBuffers(SPIS_Transaction_t *pRxBuffer, uint16_t rxSize,
    SPIS_Transaction_t *pTxBuffer, uint16_t txSize);


#else /* !SPIS_INTERNAL */


/**
 * @brief Initialize the SPI slave ring buffers.
 * @param pRxBuffer Pointer to the first element in the receive ring buffer.
 * @param rxSize Number of elements in the receive ring buffer.
 * @param pTxBuffer Pointer to the first element in the transmit ring buffer.
 * @param txSize Number of elements in the transmit ring buffer.
 *
 * This method allows for changing the size of the ring buffer. Simply call this
 * method with a different buffer location and size inside of a patch.
 */
void SPIS_InitBuffers(SPIS_Transaction_t *pRxBuffer, uint16_t rxSize,
    SPIS_Transaction_t *pTxBuffer, uint16_t txSize) __attribute__((deprecated(
    "This function is now internal and used during initialization.")));


#endif /* !SPIS_INTERNAL */


/**
 * @brief Send the data to the SPI master (when the master requests it).
 * @param pData Data to send to the SPI master.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
bool SPIS_SendDataEx(const void *pData, uint16_t dataLength,
    Driver_Callback_t pCallback, void *pUserData);


/**
 * @brief Receive data from the SPI master (when the master sends it).
 * @param pData Data received from the SPI master.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
bool SPIS_ReceiveDataEx(void *pData, uint16_t dataLength,
    Driver_Callback_t pCallback, void *pUserData);


/**
 * @brief Set the STS1 byte for the SPI transport.
 * @param sts1 Value to set the STS1 byte to.
 */
void SPIS_SetStatusByte(uint8_t sts1);


/**
 * @brief Get the STS1 byte for the SPI transport.
 * @return Value used for the STS1 byte.
 */
uint8_t SPIS_GetStatusByte(void);


/**
 * @brief Check if SPI slave driver has some pending data.
 * @returns true if there are some pending data, false otherwise.
 */
bool SPIS_HasPendingData(void);


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/


#ifndef ROM_BUILD
/**
 * @brief Add the SPI slave module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
static ALWAYS_INLINE void SPIS_RegisterModule(void)
{
    // Write to the module list the address of the SPI slave config
    gConfig.pModuleConfig[CONFIG_MODULE_SPIS] = (Config_ModuleConfiguration_t*)&gSPIS_Config;
}
#endif


/**
 * @brief Send the data to the SPI master (when the master requests it).
 * @param pData Data to send to the SPI master.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion.
 * @param pUserData Data to pass to the callback upon error or completion.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool SPIS_SendDataWithCallback(const void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    bool value=SPIS_SendDataEx(pData, dataLength, pCallback, pUserData);
    return value;
}


/**
 * @brief Send the data to the SPI master (when the master requests it).
 * @param pData Data to send to the SPI master.
 * @param dataLength Length (in bytes) of the data to send.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool SPIS_SendData(const void *pData,
    uint16_t dataLength)
{
    bool value= SPIS_SendDataEx(pData, dataLength, NULL, NULL);
    return value;
}


/**
 * @brief Receive data from the SPI master (when the master sends it).
 * @param pData Data received from the SPI master.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion.
 * @param pUserData Data to pass to the callback upon error or completion.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool SPIS_ReceiveDataWithCallback(void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    bool value= SPIS_ReceiveDataEx(pData, dataLength, pCallback, pUserData);
    return value;
}


/**
 * @brief Receive data from the SPI master (when the master sends it).
 * @param pData Data received from the SPI master.
 * @param dataLength Length (in bytes) of the data to receive.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool SPIS_ReceiveData(void *pData,
    uint16_t dataLength)
{
    bool value= SPIS_ReceiveDataEx(pData, dataLength, NULL, NULL);
    return value;
}


/**
 * @brief Check if SPI slave driver is busy.
 * @returns true if the SPI slave is busy, false otherwise.
 */
static ALWAYS_INLINE bool SPIS_IsBusy(void)
{

    bool value= 0u != GET_SPIS_BUSY(SPIS->RegSPISStat.r32);
    return value;
}


/** @} */ // End of group spis
/** @} */ // End of addtogroup drivers


#endif /* _SPI_SLAVE_H */
