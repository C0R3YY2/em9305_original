////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/dma/includes/dma.h
///
/// @project    T9305
///
/// @brief      DMA drivers
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


#ifndef _DMA_H
#define _DMA_H


#include "aux_registers.h"


/**
 * @addtogroup drivers
 * @{
 * @defgroup dma DMA Driver
 * @brief Driver for the DMA.
 * @{
*/


/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/*
 * DMA enable/disable.
 */
/// Disable DMA.
#define DMA_DISABLE                    (false)
/// Enable DMA.
#define DMA_ENABLE                     (true)


/*
 * Number of channel definitions.
 */
/// Number of memory-mapped channel.
#define DMA_MEM_MAPPED_CHANNEL_COUNT   (2u)

/// Number of auxiliary-mapped channel.
#define DMA_AUX_MAPPED_CHANNEL_COUNT   (6u)

/// Number of channels (memory and auxiliary mapped).
#define DMA_CHANNEL_COUNT              (8u)


/*
 * Default peripherals' DMA channels.
 */
/// Radio TX DMA channel.
#define DMA_CHANNEL_RADIO_TX           (2u)
/// Radio RX DMA channel.
#define DMA_CHANNEL_RADIO_RX           (3u)
/// SPIS TX DMA channel.
// @todo LRC: to be shared with USB and optionally with UART
#define DMA_CHANNEL_SPIS_TX            (4u)
/// SPIS RX DMA channel.
// @todo LRC: to be shared with USB and optionally with UART
#define DMA_CHANNEL_SPIS_RX            (5u)
/// UART TX DMA channel.
// @todo LRC: Dedicated channel for UART or shared?
#define DMA_CHANNEL_UART_TX            (6u)
/// UART RX DMA channel.
// @todo LRC: Dedicated channel for UART or shared?
#define DMA_CHANNEL_UART_RX            (7u)
/// I2S TX DMA channel.
#define DMA_CHANNEL_I2S_TX             (6u)
/// I2S RX DMA channel.
#define DMA_CHANNEL_I2S_RX             (7u)


/*
 * DMA request type.
 */
/// DMA request single.
#define DMA_REQ_SINGLE                 (true)
/// DMA request regular.
#define DMA_REQ_REGULAR                (false)


/// DMA maximum transfer size.
#define DMA_MAX_TRANSFER_SIZE          (8192u)


/******************************************************************************\
 *  MACROS
\******************************************************************************/

/// Bit shift for the DMA channels.
#define DMA_CHANNEL_SHIFT(channel)     (0x01<<(channel))


/******************************************************************************\
 *  ENUMERATIONS
\******************************************************************************/

/// Trigger source selection.
typedef enum
{
    /// No periphery connected to DMA.
    DMA_REQ_SRC_NONE        = 0,
    /// Connect to SPIS RX FIFO.
    DMA_REQ_SRC_SPIS_RX     = 1,
    /// Connect to SPIS TX FIFO.
    DMA_REQ_SRC_SPIS_TX     = 2,
    /// Connect to UART RX FIFO.
    DMA_REQ_SRC_UART_RX     = 3,
    /// Connect to UART TX FIFO.
    DMA_REQ_SRC_UART_TX     = 4,
    /// Connect to Radio RX FIFO.
    DMA_REQ_SRC_RADIO_RX    = 5,
    /// Connect to Radio TX FIFO.
    DMA_REQ_SRC_RADIO_TX    = 6,
    /// Connect to USB endpoint 1 (control endpoint).
    DMA_REQ_SRC_USB_EP1     = 7,
    /// Connect to USB endpoint 2.
    DMA_REQ_SRC_USB_EP2     = 8,
    /// Connect to USB endpoint 3.
    DMA_REQ_SRC_USB_EP3     = 9,
    /// Connect to USB endpoint 4.
    DMA_REQ_SRC_USB_EP4     = 10,
    /// Connect to ADC SAR data valid.
    DMA_REQ_SRC_ADC         = 11,
    /// Connect to I2S RX.
    DMA_REQ_SRC_I2S_RX      = 12,
    /// Connect to I2S TX.
    DMA_REQ_SRC_I2S_TX      = 13,
} DMA_TriggerSource_t;


/// DMA Status.
typedef enum
{
    /// Success, responded with expected data.
    DMA_SUCCESS = 0,
    /// Cannot perform request due to its busy state.
    DMA_FAILED_BUSY,
    /// Cannot perform request due to an error.
    DMA_FAILED,
} DMA_Status_t;


/// DMA Priority.
typedef enum
{
    /// Priority Normal.
    DMA_PRIORITY_NORMAL = 0,
    /// Priority High.
    DMA_PRIORITY_HIGH,
    /// Priority Error.
    DMA_PRIORITY_ERROR,
} DMA_Priority_t;


/// DMA Busy Status.
typedef enum
{
    /// The DMA channel is available.
    DMA_OK = 0,
    /// The DMA channel is busy.
    DMA_BUSY,
    /// An error occured while checking the busy status.
    DMA_BUSY_ERROR,
} DMA_BusyStatus_t;


/// DMA Data Transfer Type (operation).
typedef enum
{
    /// Invalid Transfer.
    DMA_INVALID_TRANSFER = 0,
    /// Single Transfer.
    DMA_SINGLE_TRANSFER,
    /// Linked-list Transfer (auto-request).
    DMA_LINKED_LIST_TRANSFER_AR,
    /// Linked-list Transfer (manual-request).
    DMA_LINKED_LIST_TRANSFER_MR,
} DMA_Operation_t;


/// DMA Request Type.
typedef enum
{
    /// Auto Request.
    DMA_AUTO_REQUEST = 0,
    /// Manual Request.
    DMA_MANUAL_REQUEST,
} DMA_RequestType_t;


/// DMA Source and Destination Type.
typedef enum
{
    /// Memory to memory.
    DMA_MEM_TO_MEM = 0,
    /// Memory to Auxiliary.
    //DMA_MEM_TO_AUX, //unused
    /// Auxiliary to Memory.
    //DMA_AUX_TO_MEM, //unused
    /// Auxiliary to Auxiliary.
    //DMA_AUX_TO_AUX, //unused
} DMA_SrcDestType_t;


/// DMA Data Width and address increment.
typedef enum
{
    /// Data width is 1 byte and address increment is a byte.
    DMA_DW_BYTE_INC_BYTE = 0,
    /// Data width is 1 byte and address increment is 2 bytes.
    DMA_DW_BYTE_INC_2BYTES,
    /// Data width is 1 byte and address increment is 4 bytes.
    DMA_DW_BYTE_INC_4BYTES,
    /// Data width is 2 bytes and address increment is 2 bytes.
    DMA_DW_2BYTES_INC_2BYTES,
    /// Data width is 2 bytes and address increment is 4 bytes.
    DMA_DW_2BYTES_INC_4BYTES,
    /// Data width is 4 bytes and address increment is 4 bytes.
    DMA_DW_4BYTES_INC_4BYTES,
    /// Clear mode, initialize a region of memory to zero.
    DMA_DW_CLEAR_MODE,
} DMA_DataWidth_t;


/// DMA Adressing Mode.
typedef enum
{
    /// Source and destination address are not incremented.
    DMA_NO_INC = 0,
    /// Source address is incremented and destination address is not incremented.
    DMA_SRC_INC,
    /// Source address is not incremented and destination address is incremented.
    DMA_DEST_INC,
    /// Source address is incremented and destination address is incremented.
    DMA_INC,
} DMA_AdressingMode_t;


/// Transfer status type.
typedef enum {
    /// Transfer complete.
    DMA_CHANNEL_COMPLETE = 0,
    /// Transfer error.
    DMA_CHANNEL_ERROR,
    /// Channel inactive.
    DMA_CHANNEL_INACTIVE,
} DMA_ChannelTransferStatus_t;


/// Interrupt status type.
typedef enum {
    /// Interrupt raised.
    DMA_INTERRUPT_RAISED = 0,
    /// No interrupt.
    DMA_NO_INTERRUPT,
    /// Interrupt failed.
    DMA_INTERRUPT_FAILED,
} DMA_IrqStatus_t;


/******************************************************************************\
 *  STRUCTURES
\******************************************************************************/

/// DMA Control register.
typedef union {
    /// 32-bit access
    uint32_t ctrlBits;

    /// bit access
    struct PACKED_STRUCT {
        /// Type of data transfer (operation).
        DMA_Operation_t operation : 2;

        /// Request type.
        DMA_RequestType_t requestType : 1;

        /// Source and destination types (auxiliary or memory).
        DMA_SrcDestType_t srcDestType : 2;

        /// Data width and address increment.
        DMA_DataWidth_t dataWidth : 3;

        /// Size of the entire data transfer in bytes.
        uint16_t blockSize : 13;

        /// Number of DMA cells that can be transferred before the
        /// DMA controller arbitrates to determine which channel
        /// should next get access to the memory bus.
        uint8_t arbitration : 8;

        /// Enable/Disable interrupt when transfer is complete.
        bool enableInterrupt : 1;

        /// Addressing mode. Defines if the source and/or
        /// destination addresses should be incremented after
        /// a DMA cell transfer.
        DMA_AdressingMode_t adressingMode : 2;
    } bits;
} DMA_CTRLx_t;


/// DMA config data structure.
typedef struct
{
    /// Control.
    DMA_CTRLx_t ctrl;
    /// Source address.
    uint32_t sourceAddr;
    /// Destination address.
    uint32_t destinationAddr;
    /// Linked list address (necessary only if linked list mode is used).
    uint32_t linkedListAddr;
} DMA_Config_t;


/******************************************************************************\
 *  FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Enable/disable the DMA controller.
 * @param enable DMA_ENABLE or DMA_DISABLE.
 * @warning Disabling the DMA controller too early may prevent
 * outstanding DMA operations from completing.
 */
void DMA_Enable(bool enable);


/**
 * @brief Check if the DMA is enabled.
 * @returns True if DMA is enabled, false otherwise.
 */
bool DMA_IsEnabled(void);


/**
 * @brief Enable/disable a specific DMA channel.
 * @param channel The channel to enable/disable, from 0 to 7.
 * @param enable True to enable and false to disable.
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED.
 * @warning Disabling the DMA channel too early may prevent
 * outstanding DMA operations from completing.
 */
DMA_Status_t DMA_EnableChannel(uint8_t channel, bool enable);


/**
 * @brief Check if a specific DMA channel is enabled.
 * @param channel The channel, from 0 to 7.
 * @returns True if the channel is enabled, false otherwise.
 */
bool DMA_IsChannelEnabled(uint8_t channel);


/**
 * @brief Check if one or more DMA channel is enabled.
 * @returns True if at least one channel is enabled, false otherwise.
 */
bool DMA_IsOneChannelEnabled(void);


/**
 * @brief Enable/Disable the DMA done and DMA error interrupts.
 * @param enable True to enable and false to disable the interrupts.
 * @warning This function is not thread safe.
 */
void DMA_EnableInterrupts(bool enable);


/**
 * @brief Reset a specific DMA channel. It disables and
 * reset the internal state of the channel.
 * @param channel The channel, from 0 to 7.
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED
 */
DMA_Status_t DMA_ResetChannel(uint8_t channel);


/**
 * @brief Set the DMA configuration.
 * @param channel The DMA channel to configure, from 0 to 7.
 * @param pConfig A pointer to the DMA_Config_t structure.
 * @param pCallback The callback to call when the transfer
 * is done or an error occur. May be NULL if a callback
 * should not be called.
 * @returns The status of the operation.
 * @note The configuration should not be changed when the DMA is in use.
 */
DMA_Status_t DMA_SetConfiguration(uint8_t channel,
                                  DMA_Config_t* pConfig,
                                  void* pCallback);


/**
 * @brief Get the DMA configuration.
 * @param channel The DMA channel, from 0 to 7.
 * @param pConfig A pointer to a DMA_Config_t structure where the
 * configuration will be copied.
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED.
 */
DMA_Status_t DMA_GetConfiguration(uint8_t channel, DMA_Config_t* pConfig);


/**
 * @brief Set a priority to a specific DMA channel.
 * @param channel The channel, from 0 to 7.
 * @param priority The priority to set, DMA_PRIORITY_HIGH or
 * DMA_PRIORITY_NORMAL.
 * @note By default each channel is set to normal priority.
 * High priority channels are given precedence over normal priority
 * DMA channels. DMA channels of same priority are given equal access.
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED
 */
DMA_Status_t DMA_SetPriority(uint8_t channel, DMA_Priority_t priority);


/**
 * @brief Get the priority of a specific DMA channel.
 * @param channel The channel, from 0 to 7.
 * @returns DMA_PRIORITY_HIGH DMA_PRIORITY_NORMAL.
 * @note By default each channel is set to normal priority.
 */
DMA_Priority_t DMA_GetPriority(uint8_t channel);


/**
 * @brief Initiate a transfer on a specific DMA channel by software.
 * @param channel The channel, from 0 to 7.
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED.
 */
DMA_Status_t DMA_InitiateTransfer(uint8_t channel);


/**
 * @brief Get the busy status of a specific DMA channel.
 * @param channel The channel, from 0 to 7.
 * @returns DMA_OK or DMA_BUSY.
 */
DMA_BusyStatus_t DMA_GetBusyStatus(uint8_t channel);


/**
 * @brief Get the transfer status of a specific DMA channel.
 * @param channel The channel, from 0 to 7.
 * @returns DMA_CHANNEL_COMPLETE, DMA_CHANNEL_ERROR or DMA_CHANNEL_INACTIVE.
 */
DMA_ChannelTransferStatus_t DMA_GetTransferStatus(uint8_t channel);


/**
 * @brief Clear the error status of a specific DMA channel.
 * @param channel The channel, 0 to 7.
 * @warning The transfer status has to be read before clearing
 * the channel error.
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED
 */
DMA_Status_t DMA_ClearError(uint8_t channel);


/**
 * @brief Read the interrupt status of a specific DMA channel.
 * @param channel The channel, from 0 to 7.
 * @returns DMA_INTERRUPT_RAISED or DMA_NO_INTERRUPT.
 */
DMA_IrqStatus_t DMA_ReadInterrupt(uint8_t channel);


/**
 * @brief Clear the interrpt of a specific DMA channel.
 * @param channel The channel, from 0 to 7
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED
 */
DMA_Status_t DMA_ClearInterrupt(uint8_t channel);


/**
 * @brief Set DMA transfer request source (SW or HW trigger) for a DMA channel.
 * @param channel The channel to configure.
 * @param triggerSource The trigger source of the DMA transfer.
 * @param  singleRequest True if it is a single request, false if it is a normal
 * request (burst request).
 * @returns The status of the operation, DMA_SUCCESS or DMA_FAILED
 */
DMA_Status_t DMA_SetHwTriggerSource(uint8_t channel,
                                    DMA_TriggerSource_t triggerSource,
                                    bool singleRequest);


/******************************************************************************\
 *  INLINE API FUNCTIONS
\******************************************************************************/

/**
 * @brief Read the error status of all the DMA channels.
 * @returns The channels errors (channel0 = 0x1, channel1 = 0x2 ...).
 */
static ALWAYS_INLINE uint32_t DMA_ReadAllErrors(void)
{
    // Read the 16 most significant bits of the DMASTAT1 register
    return (ReadAUX(EM_REG_DMASTAT1) >> 16);
}


/**
 * @brief Clear the errors of all the DMA channels.
 */
static ALWAYS_INLINE void DMA_ClearAllErrors(void)
{
    // Clear all the errors by writing to DMASTAT1 register
    WriteAUX((uint32_t)(0xFFFF << 16), EM_REG_DMASTAT1);
}


/**
 * @brief Read the interrupt status of all the DMA channels.
 * @returns The channels interrupts (channel0 = 0x1, channel1 = 0x2 ...).
 */
static ALWAYS_INLINE uint32_t DMA_ReadAllInterrupts(void)
{
    // Read the DMACIRQ register
    return ReadAUX(EM_REG_DMACIRQ);
}


/**
 * @brief Clear the interrupts of all the DMA channels.
 */
static ALWAYS_INLINE void DMA_ClearAllInterrupts(void)
{
    // Clear all the interrupts by writing to DMACIRQ register
    WriteAUX(0xFFFF, EM_REG_DMACIRQ);
}


/** @} */ // End of group dma
/** @} */ // End of addtogroup drivers


#endif /* _DMA_H */
