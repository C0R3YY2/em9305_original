////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/usb/transport/usb_cdcacm.h
///
/// @project    T9305
///
/// @brief      USB CDCACM Transport driver
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

#ifndef _USB_CDCACM_H
#define _USB_CDCACM_H

#include "transport.h"
#include "driver.h"


/******************************************************************************\
 *  TYPE DEFINITIONS
\******************************************************************************/

/// Data structure for a USB CDCACM transaction.
typedef struct
{
    /// Pointer to the next byte of the data to be sent or received.
    uint8_t *pData;
    //uint8_t TxBufferIndex;      // index into the array for this data packet
    /// Pointer to user defined data to provide to the callback.
    void *pUserData;
    /// Function to call on a transaction event.
    Driver_Callback_t pCallback;
    /// Number of bytes remaining to be sent/received for this transaction.
    uint16_t dataLength;
    /// Reserved for future use.
    uint8_t reserved[2];
} USB_CDCACM_Transaction_t;


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/// USB CDCACM task event queue size.
#define USB_CDCACM_TASK_EVENT_QUEUE_SIZE        (10)

// Define an internal buffer for storing a copy of the PC stack packet otherwise
// the PC stack can randomly set their struct containing their packet to FREE
// which overwrites the 1st byte of the data buffer and corrupts the packet if
// it has not been sent, yet!  Need to have a copy for USB use.
#define EP_BUFFER_LEN                           (1024 * 4)  // 4KB
#define EP_MAX_BUFFERS                          (1)         //
#define EP_TEMP_BUFFER_SIZE                     (EP_BUFFER_LEN * EP_MAX_BUFFERS)

/******************************************************************************\
 *  REFERENCE TO GLOBAL
\******************************************************************************/

extern const Transport_t gUSB_Transport;

// Store a copy of the PC packet for USB's use
extern uint8_t gTX_EP_Buffer[EP_TEMP_BUFFER_SIZE];
// Note: we only track the next location to use
extern uint32_t gTX_EP_PushIndex;                 // buffer offset to copy data

/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Initialize the USB CDCACM driver and task.
 * @returns Status of the operation.
 */
bool USB_CDCACM_Init(void);

/**
 * @brief Start the USB CDCACM driver and task.
 * @returns Status of the operation.
 */
bool USB_CDCACM_Start(void);

/**
 * @brief Initialize the USB CDCACM ring buffers.
 * @param pRxBuffer Pointer to the first element in the receive ring buffer.
 * @param rxSize Number of elements in the receive ring buffer.
 * @param pTxBuffer Pointer to the first element in the transmit ring buffer.
 * @param txSize Number of elements in the transmit ring buffer.
 *
 * This method allows for changing the size of the ring buffer. Simply call this
 * method with a different buffer location and size inside of a patch.
 */
void USB_CDCACM_InitBuffers(
        volatile USB_CDCACM_Transaction_t *pRxBuffer, uint16_t rxSize,
        volatile USB_CDCACM_Transaction_t *pTxBuffer, uint16_t txSize);

/**
 * @brief Restart the USB device transport.
 * @returns true if the restart was successful, false otherwise.
 */
bool USB_CDCACM_Restart(void);

/**
 * @brief Send the data to the USB host.
 * @param pData Data to send to the USB host.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the data was buffered, false if the buffer is full.
 * @note The data @em must remain valid until the transmit has completed. As
 * such, the data can't be stored on the stack or it could be corrupted.
 */
bool USB_CDCACM_SendDataEx(const void *pData, uint16_t dataLength,
    Driver_Callback_t pCallback, void *pUserData);

/**
 * @brief Receive data from the USB host.
 * @param pData Data received from the USB host.
 * @param dataLength Length (in bytes) of the data to receive.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @param pUserData Data to pass along with the event and/or callback.
 * @returns true if the request was buffered, false if the buffer is full.
 * @note The destination @em must remain valid until the transmit has completed.
 * As such, the data can't be stored on the stack or it could be corrupted.
 */
bool USB_CDCACM_ReceiveDataEx(void *pData, uint16_t dataLength,
    Driver_Callback_t pCallback, void *pUserData);

/**
 * @brief Block USB device transport communication.
 * @param blocking Enable/disable for blocking communication.
 */
void USB_CDCACM_BlockCommunication(bool blocking);

/**
 * @brief Check if USB device transport has some pending data.
 * @returns true if there are some pending data, false otherwise.
 */
bool USB_CDCACM_HasPendingData(void);


#endif // _USB_CDCACM_H
