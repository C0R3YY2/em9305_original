////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/transport/includes/transport.h
///
/// @project    T9305
///
/// @brief      Transport drivers
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021, EM Microelectronic
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


#ifndef _TRANSPORT_H
#define _TRANSPORT_H


#include <types.h>
#include <driver.h>
#include <config.h>


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/


/**
 * @brief Reason an event was generated.
 * @ingroup transport
 */
typedef Driver_Status_t Transport_CallbackReason_t
    __attribute__((deprecated("Use Driver_Status_t instead.")));


/// A receive transaction completed.
#define TRANSPORT_CALLBACK_RX    DRIVER_STATUS_RX
/// A transmit transaction completed.
#define TRANSPORT_CALLBACK_TX    DRIVER_STATUS_TX
/// An error occurred in the transport driver.
#define TRANSPORT_CALLBACK_ERROR DRIVER_STATUS_ERROR


/** @} */ /* End of Transport_CallbackReason_t */


/**
 * @brief Type for any functions that are called due to a transport event.
 * @param reason The reason this callback was called. See
 * @ref Transport_CallbackReason_t for more information.
 * @param pUserData Pointer to user defined data.
 * @ingroup transport
 */
typedef Driver_Callback_t Transport_TransactionCallback_t
    __attribute__((deprecated("Use Driver_Callback_t instead.")));


/**
 * @brief Table of function pointers for the active transport.
 * @ingroup transport
 */
typedef struct
{
    /// Restart the transport driver.
    bool (*restart)(void);

    /// Send data non-blocking function for transport driver.
    bool (*sendDataEx)(const void *pData, uint16_t dataLength,
        Driver_Callback_t pCallback, void *pUserData);

    /// Receive data non-blocking function for transport driver.
    bool (*receiveDataEx)(void *pData, uint16_t dataLength,
        Driver_Callback_t pCallback, void *pUserData);

    /// Block communication
    void (*blockCommunication)(bool blocking);

    /// Configuration for the transport driver.
    Config_ModuleConfiguration_t *pConfig;
} Transport_t;


/**
 * @brief Enumeration of possible transport modes.
 * @ingroup transport
 */
typedef enum
{
    /// UART transport mode.
    TRANSPORT_MODE_UART = 0,

    /// SPI transport mode.
    TRANSPORT_MODE_SPI,

    /// No transport used
    TRANSPORT_MODE_NONE,
} TransportMode_t;


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/


/**
 * @brief Pointer to the active transport function table.
 * @ingroup transport
 */
extern const Transport_t *gTransport_ActiveTransport;


/******************************************************************************\
 *  API FUNCTION PROTOTYPES
\******************************************************************************/


/**
 * @brief Set the active transport mode.
 * @param Transport mode to switch to.
 * @returns true if the transport mode was changed, false otherwise.
 * @ingroup transport
 */
bool Transport_SetActive(TransportMode_t mode);


/**
 * @brief Get the active transport mode.
 * @returns The active transport mode.
 * @ingroup transport
 */
TransportMode_t Transport_GetActive(void);


/**
 * @brief Get the configuration structure for the active transport.
 * @returns The configuration structure for the active transport.
 * @ingroup transport
 */
Config_ModuleConfiguration_t* Transport_GetConfig(void);


/**
 * @brief Get wake up pin number
 * @returns GPIO number which is used as wake up
 */
uint8_t Transport_GetWakeupPin( void );


/**
 * @brief Get wake up pin polarity
 * @returns True if wake up polarity is '1', false otherwise.
 */
bool Transport_GetWakeupPinPolarity( void );


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/


/**
 * @brief Restart the active transport driver.
 * @returns True if the transport driver restarted, false otherwise.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_Restart(void)
{
    return (*gTransport_ActiveTransport->restart)();
}

/**
 * @brief Send the data across the active transport.
 * @param pData Data to send across the active transport.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_SendDataEx(const void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback,
    void *pUserData)
{
    return (*gTransport_ActiveTransport->sendDataEx)(pData, dataLength,
        pCallback, pUserData);
}


/**
 * @brief Send the data across the Transport.
 * @param pData Data to send across the Transport.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion.
 * @param pUserData Data to pass to the callback upon error or completion.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_SendDataWithCallback(const void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    return Transport_SendDataEx(pData, dataLength, pCallback, pUserData);
}


/**
 * @brief Send the data across the Transport.
 * @param pData Data to send across the Transport.
 * @param dataLength Length (in bytes) of the data to send.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_SendData(const void *pData,
    uint16_t dataLength)
{
    return Transport_SendDataEx(pData, dataLength, NULL, NULL);
}


/**
 * @brief Receive data from the active transport.
 * @param pData Data to receive from the active transport.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_ReceiveDataEx(void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback,
    void *pUserData)
{
    return (*gTransport_ActiveTransport->receiveDataEx)(pData, dataLength,
        pCallback, pUserData);
}


/**
 * @brief Receive data from the Transport.
 * @param pData Data to receive from the Transport.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion.
 * @param pUserData Data to pass to the callback upon error or completion.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_ReceiveDataWithCallback(void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    return Transport_ReceiveDataEx(pData, dataLength, pCallback, pUserData);
}


/**
 * @brief Receive data from the Transport.
 * @param pData Data to receive from the Transport.
 * @param dataLength Length (in bytes) of the data to receive.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 * @ingroup transport
 */
static ALWAYS_INLINE bool Transport_ReceiveData(void *pData,
    uint16_t dataLength)
{
    return Transport_ReceiveDataEx(pData, dataLength, NULL, NULL);
}


/**
 * @brief Block incomming communication in active transport.
 * @param blocking Enable/disbale for blocking incomming communication.
 * @ingroup transport
 */
static ALWAYS_INLINE void Transport_BlockCommunication( bool blocking )
{
    (*gTransport_ActiveTransport->blockCommunication)(blocking);
}


/**
 * @brief Check if transport has some pending data
 * @returns True if there is some pending data, false otherwise.
 */
bool Transport_HasPendingData(void);

/**
 * @brief Check if transport is busy or not
 * @returns True if there is some communication (is busy), false otherwise.
 */
bool Transport_IsBusy(void);


#endif // _TRANSPORT_H
