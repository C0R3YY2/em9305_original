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

#include "types.h"
#include "driver.h"


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/**
 * @brief Table of function pointers for the active transport.
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

    /// Block communication.
    void (*blockCommunication)(bool blocking);

    /// Get wake up pin number.
    uint8_t (*getWakeupPin)(void);

    /// Get wake up pin polarity.
    bool (*getWakeupPinPolarity)(void);

    /// Check if transport has some pending data.
    bool (*hasPendingData)(void);

    /// Check if transport is busy or not.
    bool (*isBusy)(void);

    /// Check if sleep mode is allowed or not.
    bool (*isSleepAllowed)(void);
} Transport_t;


/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/

/**
 * @brief Pointer to the active transport function table.
 */
extern const Transport_t *gTransport_ActiveTransport;


/******************************************************************************\
 *  API FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Initialize the transport layer.
 * @param pTransport Pointer to the desired transport function table.
 */
void Transport_Init(const Transport_t *pTransport);


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Restart the active transport driver.
 * @returns True if the transport driver restarted, false otherwise.
 */
static ALWAYS_INLINE bool Transport_Restart(void)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->restart)
    {
        bool var = (*gTransport_ActiveTransport->restart)();
        return var;
    }
    else
    {
        return false;
    }
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
 */
static ALWAYS_INLINE bool Transport_SendDataEx(const void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback,
    void *pUserData)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->sendDataEx)
    {
        bool var = (*gTransport_ActiveTransport->sendDataEx)(pData, dataLength,
            pCallback, pUserData);
        return var;
    }
    else
    {
        return false;
    }
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
 */
static ALWAYS_INLINE bool Transport_SendDataWithCallback(const void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    bool var = Transport_SendDataEx(pData, dataLength, pCallback, pUserData);
    return var;
}

/**
 * @brief Send the data across the Transport.
 * @param pData Data to send across the Transport.
 * @param dataLength Length (in bytes) of the data to send.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool Transport_SendData(const void *pData,
    uint16_t dataLength)
{
    bool var = Transport_SendDataEx(pData, dataLength, NULL, NULL);
    return var;
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
 */
static ALWAYS_INLINE bool Transport_ReceiveDataEx(void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback,
    void *pUserData)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->receiveDataEx)
    {
        bool var = (*gTransport_ActiveTransport->receiveDataEx)(pData, dataLength,
            pCallback, pUserData);
        return var;
    }
    else
    {
        return false;
    }
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
 */
static ALWAYS_INLINE bool Transport_ReceiveDataWithCallback(void *pData,
    uint16_t dataLength, Driver_Callback_t pCallback, void *pUserData)
{
    bool var = Transport_ReceiveDataEx(pData, dataLength, pCallback, pUserData);
    return var;
}

/**
 * @brief Receive data from the Transport.
 * @param pData Data to receive from the Transport.
 * @param dataLength Length (in bytes) of the data to receive.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
static ALWAYS_INLINE bool Transport_ReceiveData(void *pData,
    uint16_t dataLength)
{
    bool var = Transport_ReceiveDataEx(pData, dataLength, NULL, NULL);
    return var;
}

/**
 * @brief Block incomming communication in active transport.
 * @param blocking Enable/disbale for blocking incomming communication.
 */
static ALWAYS_INLINE void Transport_BlockCommunication(bool blocking)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->blockCommunication)
    {
        (*gTransport_ActiveTransport->blockCommunication)(blocking);
    }
}

/**
 * @brief Get wake up pin number.
 * @returns GPIO number which is used as wake up.
 */
static ALWAYS_INLINE uint8_t Transport_GetWakeupPin(void)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->getWakeupPin)
    {
        uint8_t var = (*gTransport_ActiveTransport->getWakeupPin)();
        return var;
    }
    else
    {
        return 0xFF;    // GPIO_FUNCTION_NOT_MAPPED
    }
}

/**
 * @brief Get wake up pin polarity.
 * @returns True if wake up polarity is '1', false otherwise.
 */
static ALWAYS_INLINE bool Transport_GetWakeupPinPolarity(void)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->getWakeupPinPolarity)
    {
        bool var  = (*gTransport_ActiveTransport->getWakeupPinPolarity)();
        return var;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Check if transport has some pending data.
 * @returns True if there is some pending data, false otherwise.
 */
static ALWAYS_INLINE bool Transport_HasPendingData(void)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->hasPendingData)
    {
        bool var = (*gTransport_ActiveTransport->hasPendingData)();
        return var;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Check if transport is busy or not.
 * @returns True if there is some communication (is busy), false otherwise.
 */
static ALWAYS_INLINE bool Transport_IsBusy(void)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->isBusy)
    {
        bool var = (*gTransport_ActiveTransport->isBusy)();
        return var;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Check if sleep mode is allowed or not.
 * @returns True if sleep mode is allowed, false otherwise.
 */
static ALWAYS_INLINE bool Transport_IsSleepAllowed(void)
{
    if (gTransport_ActiveTransport &&
        gTransport_ActiveTransport->isSleepAllowed)
    {
        bool var = (*gTransport_ActiveTransport->isSleepAllowed)();
        return var;
    }
    else
    {
        return true;
    }
}


#endif // _TRANSPORT_H
