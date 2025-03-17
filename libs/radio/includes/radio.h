////////////////////////////////////////////////////////////////////////////////
///
/// @file       libs/radio/includes/radio.h
///
/// @project    T9305
///
/// @brief      Radio driver.
///
/// This peripheral is responsible for sending/receiving BLE packets over RF
///  interface.
///
/// Callback function can be assigned to both RX and TX transactions allowing
///  application to be notified once given transaction is finished.
///
/// Available commands for Radio are:
///  - Initialize Radio for BLE operation
///  - Start/stop Radio
///  - Configure Radio for given operation
///  - Check CRC result
///  - Apply whitening/de-whitening
///  - Encrypt/ Decrypt packet on the fly (in combination with AES co-processor,
///      short-cuts between radio event and AES-CCM command can be implemented)
///  - Calculate RSSI
///  - Set/get output power
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2020-2024, EM Microelectronic
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


#ifndef _RADIO_H
#define _RADIO_H

#include <config.h>
#include <types.h>
#include <t9305_periph.h>
#include <arc.h>
#include <radio_config.h>


/**
 * @defgroup radio Radio Driver
 * @brief Driver for the Radio.
 * @ingroup drivers
 */

/**
 * @addtogroup radio
 * @{
 */

/******************************************************************************\
 *  CONSTANTS
\******************************************************************************/

/**
 * @brief SyncWord for BLE advertiser
 * @verbatim
 * LSB                             MSB
 * 01101011 01111101 10010001 01110001
 * MSB                             LSB
 * 10001110 10001001 10111110 11010110
 * @endverbatim
 */
#define RF_SYNC_WORD_BLE_ADVERTISER     (0x8E89BED6)

/**
 * @brief SyncWord for BLE RF test mode
 * BLE SyncWord for RF test mode
 * @verbatim
 * LSB                             MSB
 * 10010100 10000010 01101110 10001110
 * MSB                             LSB
 * 01110001 01110110 01000001 00101001
 * @endverbatim
 */
#define RF_SYNC_WORD_BLE_TEST_MODE      (0x71764129)

/**
 * @brief CRC polynomial for BLE: x^24 + x^10 + x^9 + x^6 + x^4 + x^3 + x + 1
 */
#define RF_CRC_POLYNOMIAL_BLE           (0x80032D)

/**
 * @brief CRC polynomial length for BLE
 */
#define RF_CRC_POLYNOMIAL_LENGTH_BLE    (3)

/**
 * @brief CRC init value for BLE advertising
 */
#define RF_CRC_INIT_BLE_ADVERTISER      (0x555555)

/**
 * @brief CRC init value for BLE RF test mode
 */
#define RF_CRC_INIT_BLE_TEST_MODE       (0x555555)

/**
 * @brief Whitening polynomial for BLE: x^7 + x^4 + 1
 */
#define RF_WHITENING_POLYNOMIAL_BLE     (0x48)

/**
 * @brief RF controller timing granularity [us]
 */
#define RF_RFC_TIMING_GRN       (0.25)

/**
 * @brief T_IFS time from BLE spec [us]
 */
#define  RF_BLE_TIFS_TIME       (150)

/**
 * @brief Maximum RF output power: +10dBm
 */
#define  RF_POWER_MAX   (10)

/**
 * @brief Minimum RF output power: -57dBm
 */
#define  RF_POWER_MIN   (-57)

/**
 * @brief Minimum time in microseconds to set RF output power before RF operation.
 */
#define RF_APPLY_OUTPUT_POWER_TIME_MARGIN (50U)


/******************************************************************************\
 *  MACROS
\******************************************************************************/



/******************************************************************************\
 *  TYPE DEFINITION
\******************************************************************************/

/// RF mode
typedef enum
{
    /// BLE 1Msym/s, uncoded
    RF_MODE_BLE_1M              = 0,
    /// BLE 2Msym/s, uncoded
    RF_MODE_BLE_2M              = 1,
    /// BLE 1Msym/s, coded S=8
    RF_MODE_BLE_1M_CODED_S8     = 2,
    /// BLE 1Msym/s, coded S=2
    RF_MODE_BLE_1M_CODED_S2     = 3,
} RF_Mode_t;


/// Reason a callback was called.
typedef enum
{
    /// Transmit transaction completed (packet sent).
    RF_CALLBACK_TX = 0,
    /// Transmit transaction terminated.
    RF_CALLBACK_TX_TERMINATED,
    /// Buffer error occurred during transmit transaction.
    /// TX buffer was not re-initialized between two TX transactions.
    /// TX FIFO underflow
    RF_CALLBACK_TX_BUFFER_ERROR,

    /// Receive syncword completed (packet preamble + access address received).
    RF_CALLBACK_RX_SYNC,
    /// Receive transaction completed (packet received).
    RF_CALLBACK_RX,
    /// Receive transaction terminated due to timeout.
    RF_CALLBACK_RX_TIMEOUT,
    /// Receive transaction terminated.
    RF_CALLBACK_RX_TERMINATED,
    /// Packet received has incorrect CRC.
    RF_CALLBACK_RX_CRC_ERROR,
    /// Decoded packet length out of allowed range.
    RF_CALLBACK_RX_PDU_LENGTH_ERROR,
    /// Buffer error occurred during receive transaction.
    /// RX buffer was not re-initialized between two RX transactions.
    /// RX FIFO overflow.
    RF_CALLBACK_RX_BUFFER_ERROR,
    /// Packet received length is bigger than user buffer length.
    RF_CALLBACK_RX_PACKET_BIGGER_THAN_BUFFER,

    /// An error occurred in the RF driver.
    RF_CALLBACK_ERROR,
} RF_CallbackReason_t;


/// RF MAC time structure
typedef struct
{
    /// MAC time for BLE 1M (1Mbps)
    uint16_t    ble1M;
    /// MAC time for BLE 2M (2Mbps)
    uint16_t    ble2M;
    /// MAC time for BLE 1M Coded S=8
    uint16_t    ble1M_S8;
    /// MAC time for BLE 1M Coded S=2
    uint16_t    ble1M_S2;
} RF_MacTime_t;


/// RF configuration data structure.
typedef struct
{
    /// Base structure each module config must start with.
    Config_ModuleConfiguration_t moduleConfig;
    /// RF configuration table
    RF_config_t         *pRegConfig;
    /// RF configuration table size
    uint8_t             regConfigSize;
    /// RF MAC timing parameters
    RF_MacTiming_t      *pMacTiming;
    /// BLE CTE timing parameters
    RF_BleCteTiming_t   *pBleCteTiming;
    /// Rx FIFO limit
    uint8_t             rxFifoLimit;
    /// Tx FIFO limit
    uint8_t             txFifoLimit;
    /// User set RSSI offset
    //  int8 * 0.2 = dB of offset, note: signed value.
    int8_t              rssiOffset;
    /// Protocol used by radio driver, set optimized ISR functions
    uint8_t             protocol;
} RF_Configuration_t;


/**
 * @brief Type for any functions that are called due to a RF event.
 * @param reason The reason this callback was called. See
 * @ref RF_CallbackReason_t for more information.
 * @param transferBytes How many bytes were actually transferred.
 */
typedef void (*RF_TransactionCallback_t)( RF_CallbackReason_t reason,
    uint16_t transferBytes);


/// RF special test modes.
typedef enum
{
    /// Continuously modulated signal (PRBS9 sequence)
    RF_TEST_MODE_CM_PRBS9               = 0,
    /// Continuously modulated signal (PRBS15 sequence)
    RF_TEST_MODE_CM_PRBS15              = 1,

    /// Continuously modulated signal (sequence 0000000000000000)
    RF_TEST_MODE_CM_0000000000000000    = 2,
    /// Continuously modulated signal (sequence 1111111111111111)
    RF_TEST_MODE_CM_1111111111111111    = 3,
    /// Continuously modulated signal (sequence 0101010101010101)
    RF_TEST_MODE_CM_0101010101010101    = 4,
    /// Continuously modulated signal (sequence 0011001100110011)
    RF_TEST_MODE_CM_0011001100110011    = 5,
    /// Continuously modulated signal (sequence 0000111100001111)
    RF_TEST_MODE_CM_0000111100001111    = 6,
    /// Continuously modulated signal (sequence 0000000011111111)
    RF_TEST_MODE_CM_0000000011111111    = 7,
} RF_TestMode_t;


/// BLE CTE types
typedef enum
{
    /// AoA transmitter
    RF_BLE_CTE_TYPE_AOA_TX,
    /// AoA receiver with 1us slot
    RF_BLE_CTE_TYPE_AOA_RX_1US,
    /// AoA receiver with 2us slot
    RF_BLE_CTE_TYPE_AOA_RX_2US,
    /// AoD transmitter with 1us slot
    RF_BLE_CTE_TYPE_AOD_TX_1US,
    /// AoD transmitter with 2us slot
    RF_BLE_CTE_TYPE_AOD_TX_2US,
    /// AoD receiver
    RF_BLE_CTE_TYPE_AOD_RX,
} RF_BleCteType_t;

/// 802.15.4 Scan Mode
typedef enum
{
    /// Scan Mode waiting for 802.15.4 Syncword
    RF_15_4_SCAN_MODE_SYNCWORD = 0,
    /// Scan Mode with invalid Syncword
    RF_15_4_SCAN_MODE_NO_SYNCWORD = 1,
} RF_15_4_ScanMode_t;

/******************************************************************************\
 *  GLOBAL VARIABLES
\******************************************************************************/



/******************************************************************************\
 *  INIT/SAVE/RESTORE FUNCTION PROTOTYPES
\******************************************************************************/

/**
 * @brief Init radio driver including radio HW.
 *
 * This function is used to initialize radio driver and radio HW for BLE operation.
 */
void RF_Init(void);



/******************************************************************************\
 *  API FUNCTIONS
\******************************************************************************/


/**
 * @brief Add the RF module into the module config list
 * @note This function needs to be called before the modules are initialized
 */
void RF_RegisterModule(void);

/**
 * @brief Init radio HW.
 *
 * This function is used to initialize radio HW for BLE operation.
 * Radio initialization is according to RF IP guideline.
 */
void RF_InitHw(void);

/**
 * @brief Cause RF_SetMode to not set the RF output power.
 *
 * This function is used for Packetcraft RX operations requiring tight timing.
 * Setting the RF output power takes ~50us which is too long to receive T_MAFS.
 */
void RF_BypassSetRFPower(void);

/**
 * @brief Set radio into given operating mode.
 * @param rxMode Radio RX mode to be set. See @ref RF_Mode_t for supported
 * @param txMode Radio TX mode to be set. See @ref RF_Mode_t for supported
 * modes.
 *
 * This function is used to set RF into given operating mode.
 */
void RF_SetMode(RF_Mode_t rxMode, RF_Mode_t txMode);

/**
 * @brief Start radio in TX mode.
 *
 * This function is used to start radio in TX mode.
 */
void RF_StartTx(void);

/**
 * @brief Start radio in RX mode.
 *
 * This function is used to start radio in RX mode.
 */
void RF_StartRx(void);

/**
 * @brief Start radio in TX mode by HW trigger.
 *
 * This function is used to start radio in TX mode synchronously with external
 * HW trigger. HW trigger is an event from Protocol Timer.
 */
void RF_StartTxHw(void);

/**
 * @brief Start radio in RX mode by HW trigger.
 *
 * This function is used to start radio in RX mode synchronously with external
 * HW trigger. HW trigger is an event from Protocol Timer.
 */
void RF_StartRxHw(void);

/**
 * @brief Cancel radio start by HW trigger.
 *
 * This function is used to cancel starting radio by external HW trigger.
 */
static ALWAYS_INLINE void RF_StartHwDisable(void)
{
    // Disable RF start by HW trigger
    RFC->RegRfcCtrl.regs.RfcCtrlHw = RFC_START_HW_EN_R(0) | RFC_START_HW_TXNRX_R(0);
}

/**
 * @brief Stop Radio.
 *
 * This function is used to stop the Radio. Currently, this procedure does not
 * wait for the radio to be truly off. The RF hardware is expected to be fully
 * off maximum 4 microseconds after calling this procedure.
 * @note This Function currently blocks IRQ from Radio thus no notification or
 * callback is activated upon stopping Radio.
 */
void RF_Stop(void);

/**
 * @brief Returns if a syncword was received after the radio was stopped and restarted.
 * @note This function is used to ensure that the first interrupt that is received
 * after the radio is restarted is a syncword.
 * @returns true if a syncword was not received after the radio was stopped and restarted.
 */
bool RF_WaitForSyncwordAfterStop(void);

/**
 * @brief Clear the flag that a syncword was received after the radio was stopped and restarted.
 */
void RF_WaitForSyncwordAfterStopClear(void);

/**
 * @brief Set the flag that a syncword was received after the radio was stopped and restarted.
 */
void RF_WaitForSyncwordAfterStopSet(void);

/**
 * @brief Set RF channel.
 * @param rfChannel RF channel number (physical channel number).
 *
 * It sets RF channel according to provided RF channel.
 */
void RF_SetRfChannel(uint8_t rfChannel);

/**
 * @brief Set preamble pattern.
 * @param preamble Preamble pattern be used in RF packets.
 */
static ALWAYS_INLINE void RF_SetPreamble(uint8_t preamble)
{
    PP->RegPpPream.regs.PpPreamPattern = preamble;
}

/**
 * @brief Set syncword (access address).
 * @param syncword Syncword (access address) to be used in RF packets.
 */
static ALWAYS_INLINE void RF_SetSyncword(uint32_t syncword)
{
    // Syncword value
    PP->RegPpSyncWord0Lo.r32 = syncword;
}

/**
 * @brief Set CRC init value
 * @param crcInitValue CRC init value to be used for CRC calculation.
 */
static ALWAYS_INLINE void RF_SetCrcInitValue(uint32_t crcInitValue)
{
    // CRC init value
    PP->RegPpCrcInit0.r32 = crcInitValue;
}

/**
 * @brief Set whitening init value.
 * @param whiteningInitValue Whitening init value to be used for whitening.
 */
static ALWAYS_INLINE void RF_SetWhiteningInitValue(uint32_t whiteningInitValue)
{
    // Whitening init value
    PP->RegPpWhiteInit.r32 = whiteningInitValue;
}

/**
 * @brief Set whitening enable.
 * @param enable Enable/disable for whitening.
 */
void RF_SetWhiteningEnable(bool enable);

/**
 * @brief Set maximum packet length (limit of Length field).
 * @param maxPacketLength Maximum packet length. Only 9 bits are valid.
 *
 * In fixed length packet mode it is length to receive. In variable packet length
 * mode it defines maximum value of Length field in packet (PDU) to be accepted.
 * If Length field in variable packet length mode is bigger than limit RX
 * operation is stopped immediately.
 *
 * E.g. For legacy BLE 4.0 data packets maximum packet length should be set to
 * 27 bytes when encryption is not used and to 31 bytes when encryption is used.
 */
void RF_SetMaximumPacketLength(uint16_t maxPacketLength);

/**
 * @brief Set the maximum PDU Length
 * @param length Maximum PDU Length accepted by Packet Processor.
 * @note In case the packet is oversized, the reception is stopped by PP
 *       and an ISR with PDU Length status bit is set.
 * @deprecated Use RF_SetMaximumPacketLength()
*/
void RF_SetRxPDULength(uint8_t length);

/**
 * @brief Configure timeout for RX packet.
 * @param timeout RX timeout value - granularity 0.25us.
 * @param enable RX timeout enable.
 *
 * It configures (set timeout value and enable/disable timeout) for RX packet
 * which is applied during RX operation. Since length of RX packet might be
 * variable timeout is applied to syncword reception.
 * RX timeout is started when RF is started in RX mode and timeout is enabled.
 *
 * @warning RX timeout should be configured when radio is not running or at
 * least when radio is not in RX mode. Reconfiguring RX timeout during ongoing
 * radio RX operation might cause false IRQ generation.
 */
void RF_SetRxTimeout(uint32_t timeout, bool enable);

/**
 * @brief Check whether the Rx timeout has been set too late.
 *
 * This indicates whether the Rx timeout has been set after the Rx operation has
 * already started.
 * This check should be used immediately after a call to RF_SetRxTimeout().
 *
 * @note The PP flag (RX_ON) is used instead of the RFC flag (RX_ON) because the
 * RFC flag is already set when the Rx radio is ramping up.
 */
static ALWAYS_INLINE bool RF_WasRxTimeoutSetTooLate(void)
{
    bool value = (PP_STS_RX_ON(1) == (PP->RegPpSts.r32 & PP_STS_RX_ON(1)));
    return value;
}

/**
 * @brief Set MAC transitions.
 * @param rx2tx True: Rx->Tx transition, False: Rx->Rx transition.
 * @param tx2rx True: Tx->Rx transition, False: Tx->Tx transition.
 * This function set MAC transitions for Rx and Tx modes.
 */
static ALWAYS_INLINE void RF_SetMacTransition(bool rx2tx, bool tx2rx)
{
    // MAC transitions for Rx and Tx:
    // Rx->Tx/Rx
    // Tx->Rx/Tx
    RFC->RegRfcMacCfg.regs.RfcMacCfg =
        (uint8_t)(RFC_RX_MAC_TX_NRX_R((uint8_t)rx2tx) | RFC_TX_MAC_RX_NTX_R((uint8_t)tx2rx));
}

/**
 * @brief Set MAC transition enable.
 * @param rxMacEnable Enable/disable MAC transition from Rx mode.
 * @param txMacEnable Enable/disable MAC transition from Tx mode.
 * This function enable/disable MAC transitions. When enabled radio is
 * automatically started in given mode (Rx/Tx) after completing given operation
 * (Rx/Tx). This allows to implement automatic transition Rx->Tx->Rx->Tx...
 */
static ALWAYS_INLINE void RF_SetMacEnable(bool rxMacEnable, bool txMacEnable)
{
    // MAC transitions enable for Rx and Tx modes
    RFC->RegRfcMacCfg.regs.RfcMacEn =
        (uint8_t)(RFC_RX_MAC_EN_R(rxMacEnable) | RFC_TX_MAC_EN_R(txMacEnable));
}

/**
 * @brief Set MAC transition timing.
 * @param rxMacTime Rx->Tx/Rx MAC time.
 * @param txMacTime Tx->Rx/Tx MAC time.
 * @returns True if MAC timing can be met, False otherwise.
 * This function set MAC timing for Rx->Tx/Rx and Tx->Rx/Tx transitions.
 * Timing parameters are set with granularity 0.25us and requested MAC timing is
 * on the antenna/air. Internal timing is calculated from parameters and if
 * requested timing cannot be met (MAC time too short or too big ) False is
 * returned.
 *
 * @warning MAC transition timing depends on RX/TX PHY mode set (@ref RF_SetMode())
 * and on MAC transitions configured (RF_SetMacTransition()). Those must
 * be configured first before setting MAC transition timing.
 */
bool RF_SetMacTiming(uint16_t rxMacTime, uint16_t txMacTime);

/**
 * @brief Set external PA and LNA timing.
 * @param paTime Time needed for external PA to start, granularity=0.25us.
 * @param lnaTime Time needed for external LNA to start, granularity=0.25us.
 * This function set timing for external PA and external LNA.
 */
static ALWAYS_INLINE void RF_SetExternalPaLnaTiming(uint8_t paTime, uint8_t lnaTime)
{
    // PA and LNA timing
    RFC->RegRfcPaLnaTime.r32 =
        RFC_EXT_PA_START_TIME(paTime) | RFC_EXT_LNA_START_TIME(lnaTime);
}

/**
 * @brief Send the data across the RF.
 * @param pData Data to send across the RF.
 * @param dataLength Length (in bytes) of the data to send.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @returns true if the data was scheduled for transmission, false if other
 * data are already scheduled.
 * @note The data @em must remain valid until the transmit has completed.
 */
bool RF_SendData(const uint8_t *pData, uint16_t dataLength, RF_TransactionCallback_t pCallback);

/**
 * @brief Receive data from the RF.
 * @param pData Data to receive from the RF.
 * @param dataLength Maximum length (in bytes) of the data to store into buffer.
 * RX operation will be terminated if more data to be stored.
 * @param pCallback Callback to call upon error or completion. May be NULL
 * if a callback should not be called.
 * @returns true if data can be received into buffer, false if buffer contains
 * old data.
 * @note The destination @em must remain valid until the receive has completed.
 *
 * @todo RF: If more data to be stored...
 */
bool RF_ReceiveData(uint8_t *pData, uint16_t dataLength, RF_TransactionCallback_t pCallback);

/**
 * @brief Check if CRC is ok for received packet.
 * @returns true if CRC check passed, false otherwise.
 * @note It returns CRC status of last received packet. Once CRC status is returned
 * it is internally cleared. Value is refreshed once packet is received again.
 */
bool RF_IsCrcResultOk(void);

/**
 * @brief Get CI value (coding indicator) of last received packet.
 * @returns CI value (coding indicator) of last received packet.
 * This function returns CI value (coding indicator) as received for last
 * received packet (valid only when Coded PHY is used). CI value is in range
 * b00 - b11.
 */
static ALWAYS_INLINE uint8_t RF_GetCodedPhyCi(void)
{
    return (uint8_t)GET_PP_STS_RX_CODED_PHY_CI(PP->RegPpSts.r32);
}

/**
 * @brief Get the RSSI register value.
 * @returns The RSSI.
 */
static ALWAYS_INLINE uint16_t RF_GetRssiRegisterValue(void)
{
    return RFC->RegRfcRssi.r16[0] & 0x7FF;
}

/**
 * @brief Get the RSSI gain register value.
 * @returns The RSSI gain (from 0 to 5).
 */
static ALWAYS_INLINE uint8_t RF_GetRssiGainRegisterValue(void)
{
    return (uint8_t)(RFC->RegRfcRssi.r16[0] >> 11) & 0x7;
}

/**
 * @brief Get RSSI value of last received packet
 * @returns RSSI value in dBm units with resolution 0.1dBm for last received packet.
 *
 * @details
 * Examples of returned value and corresponding RSSI value [dBm]:
 * @verbatim
 * -------------------------------------
 * Returned value   RSSI [dBm]
 * -------------------------------------
 *  +101             +10.1dBm
 *   +12              +1.2dBm
 *   -78              -7.8dBm
 *  -542             -54.2dBm
 * -1045            -104.5dBm
 * -------------------------------------
 * @endverbatim
 */
int16_t RF_GetRssiValue(void);

/**
 * @brief Set RSSI Offset.
 * @param offset Requested RSSI offset index.  The index maps to a 0.2dB step.
 * Note that the offset is signed, so the range is -25.6dB to 25.4dB.  This offset
 * This offset will then be applied to the input power measurement calculation.
 */
void RF_SetRssiOffset(int8_t offset);

/**
 * @brief Set RF output power.
 * @param power Requested RF output power to be set in dBm units. If value is
 * bigger than maximum supported value then maximum supported value will be used.
 * If given value is not supported closest bigger value will be used.
 * @returns Estimated RF output power which will be set. It is estimation since
 * RF output power is changed later and battery voltage might change in between.
 *
 * It stores RF output power as requested in RF driver module and RF output
 * power is set during @ref RF_SetMode() execution.
 * @note In some cases requested RF output power cannot be set due to weak
 * battery thus lower RF output power is set and reported back.
 */
int8_t RF_SetOutputPower(int8_t power);

/**
 * @brief Get RF output power.
 * @param maxPower If TRUE, returns the maximum currently available RF output power
 * not accounting for the actually allowed maximum RF output power that is configured
 * in the device.1
 * If FALSE, returns the actual used RF output power.
 *
 * @returns RF output power in dBm units.
 *
 * @note Actually used RF output power might be different from previously
 * requested value since battery voltage could change in meantime.
 */
int8_t RF_GetOutputPower(bool maxPower);

/**
 * @fn bool RF_GetMaxAllowedOutputPower(int8_t *power)
 * @brief Returns the maximum allowed output power that is configured in the device.
 *
 * The maximum allowed output power is a value that is stored in a locked register. In
 * any case, requesting an RFoutput power greater than this value is rejected by the
 * hardware. In case no value has ever been written, then the maximum +10dBm power is
 * allowed.
 *
 * @param[out] power a pointer to an int8_t that will contain the power in dBm if found
 *                   in the radio power configuration table
 *
 * @returns true if the maximum allowed power in dBm has been found and in such a case,
 *          the power parameter contains the value in dBm, false otherwise.
 */
bool RF_GetMaxAllowedOutputPower(int8_t* power);

/**
 * @brief Start special RF test mode.
 * @param testMode RF test mode to run.
 * @returns true if given RF test mode was started.
 *
 * @note When starting RF special test mode some features are disabled or
 * reconfigured and not restored after stopping RF special test mode.
 * Thus before using Radio again in normal mode these features have to be
 * configured again if needed.
 */
bool RF_TestModeStart(RF_TestMode_t testMode);

/**
 * @brief Stop special RF test mode.
 * @returns true if given RF test mode was stopped.
 */
bool RF_TestModeStop(void);


/******************************************************************************\
 *  API FUNCTIONS - BLE SPECIFIC
\******************************************************************************/

/**
 * @brief Convert BLE channel into RF channel
 * @param  bleChannel BLE channel index
 * @returns RF channel index
 *
 * @details
 *  It converts BLE channel (advertising channel index or data channel index)
 *  into RF channel number.
 *
 * @verbatim
 * -------------------------------------------------------------------------------
 * RF        RF          Channel                     Data channel    Advertising
 * channel   frequency   Type                        index           channel index
 * -------------------------------------------------------------------------------
 * 0         2402 MHz    Advertising channel                         37
 * 1         2404 MHz    Data channel                0
 * 2         2406 MHz    Data channel                1
 * ...       ...         Data channels               ...
 * 11        2424 MHz    Data channel                10
 * 12        2426 MHz    Advertising channel                         38
 * 13        2428 MHz    Data channel                11
 * 14        2430 MHz    Data channel                12
 * ...       ...         Data channels               ...
 * 38        2478 MHz    Data channel                36
 * 39        2480 MHz    Advertising channel                         39
 * -------------------------------------------------------------------------------
 *@endverbatim
 */
uint8_t RF_BleChannel2RfChannel(uint8_t bleChannel);

/**
 * @brief Convert RF channel into BLE channel
 * @param rfChannel RF channel index
 * @returns BLE channel index
 *
 * @details
 *  It converts RF channel number into BLE channe index (advertising channel
 *  index or data channel index)
 *
 * @verbatim
 * -------------------------------------------------------------------------------
 * RF        RF          Channel                     Data channel    Advertising
 * channel   frequency   Type                        index           channel index
 * -------------------------------------------------------------------------------
 * 0         2402 MHz    Advertising channel                         37
 * 1         2404 MHz    Data channel                0
 * 2         2406 MHz    Data channel                1
 * ...       ...         Data channels               ...
 * 11        2424 MHz    Data channel                10
 * 12        2426 MHz    Advertising channel                         38
 * 13        2428 MHz    Data channel                11
 * 14        2430 MHz    Data channel                12
 * ...       ...         Data channels               ...
 * 38        2478 MHz    Data channel                36
 * 39        2480 MHz    Advertising channel                         39
 * -------------------------------------------------------------------------------
 * @endverbatim
 */
uint8_t RF_RfChannel2BleChannel(uint8_t rfChannel);

/**
 * @brief Do a Voltage Measurement to determine the
 * maximum allowed RF output power.
 * @note The voltage measurement is done each time the system wakes up. This function
 * shall be called by the stack when the system cannot goes to sleep periodically.
 * This function shall be called when there will be no RF activity for at least
 * 50 us.
 * @details Do voltage measurement if voltMonitorPeriod is elapsed
 * from the last voltage measurement.
 */
void RF_DoVoltageMeasurementIfTimeElapsed(void);

/**
 * @brief Set syncword for BLE operation.
 * @param syncword Syncword to be used in RF packets.
 *
 * It sets syncword (access address) for BLE operation. At the same time it
 * sets preamble according to syncword LSB.
 */
void RF_SetSyncwordBle(uint32_t syncword);

/**
 * @brief Set data whitening mode for BLE operation.
 * @param bleChannel BLE channel number.
 * @param whiteningEnable Enable/disable for whitening.
 *
 * It enables/disables data whitening mode. At the same time it initialize
 * LFSR for data whitening according to specified BLE channel.
 */
void RF_SetWhiteningModeBle(uint8_t bleChannel, bool whiteningEnable);

/**
 * @brief Set Key and Nonce for BLE encrypted operation.
 * @param pKey Pointer to Key to be set in RF and used for BLE packet
 * encryption/decryption.
 * @param pNonce Pointer to Nonce to be set in RF and used for BLE packet
 * encryption/decryption.
 *
 * @note Key and Nonce are required in little endian format. Key[0] is LSO and
 * Key[15] is MSO. Nonce[0] is LSO and Nonce[12] is MSO.
 */
void RF_SetEncryptionParametersBle(const uint8_t *pKey, const uint8_t *pNonce);

/**
 * @brief Set PacketCounter and Direction bit for BLE encrypted operation.
 * @param pNonce Pointer to Nonce containing PacketCounter and Direction bit to
 * be set in RF and used for BLE packet encryption/decryption.
 *
 * @note Nonce is required in little endian format. Nonce[0] is LSO and Nonce[4]
 * is MSO.
 * Only 5 bytes are required here since only PacketCounter and Direction bit is
 * set. Mapping is as following
 * Nonce[0]: PacketCounter octet 0 (LSO)
 * Nonce[1]: PacketCounter octet 1
 * Nonce[2]: PacketCounter octet 2
 * Nonce[3]: PacketCounter octet 3
 * Nonce[4]: PacketCounter octet 4 (MSO), Bit7: Direction bit
 */
void RF_SetEncryptionPacketCounterBle(const uint8_t *pNonce);

/**
 * @brief Enable/disable BLE packet encryption in RF communication.
 * @param rxEncEnable It enables decryption in RX operation when true otherwise
 * decryption is disabled.
 * @param txEncEnable It enables encryption in TX operation when true otherwise
 * encryption is disabled.
 */
void RF_SetEncryptionEnableBle(bool rxEncEnable, bool txEncEnable);

/**
 * @brief Check if MIC is ok for BLE received packet.
 * @returns true if MIC check passed, false otherwise.
 * @note It returns MIC check status of last received packet. Once MIC status is
 * returned it is internally cleared. Value is refreshed once encrypted packet
 * is received again.
 */
bool RF_IsMicResultOkBle(void);

/**
 * @brief Set MAC transition timing for BLE operation.
 * This function set MAC transition timing to meet BLE T_IFS for Rx->Tx/Rx or
 * Tx->Rx/Tx transitions.
 * Timing is set according to currently configured PHY mode for Rx/Tx operation
 * and according to currently configured MAC transition (Rx->Tx/Rx and Tx->Rx/Tx).
 */
static ALWAYS_INLINE void RF_SetMacTimingBle(void)
{
    uint16_t bleMacTime = (uint16_t)(RF_BLE_TIFS_TIME/RF_RFC_TIMING_GRN);

    (void)RF_SetMacTiming(bleMacTime, bleMacTime);
}

/**
 * @brief Enable/disable CTE
 * @param rxCteEnable True enable CTE in RX operation, false disable CTE in RX
 * operation.
 * @param txCteEnable True enable CTE in TX operation, false disable CTE in TX
 * operation.
 */
static ALWAYS_INLINE void RF_BleCteSetEnable(bool rxCteEnable, bool txCteEnable)
{
    PP->RegPpBleCteCfg.r32 =
        (PP->RegPpBleCteCfg.r32 & ~(PP_BLE_CTE_RX_EN_MASK | PP_BLE_CTE_TX_EN_MASK)) |
        (PP_BLE_CTE_RX_EN(rxCteEnable) | PP_BLE_CTE_TX_EN(txCteEnable));
}

/**
 * @brief Get number of IQ samples collected during CTE phase.
 * @returns Number of IQ samples collected.
 */
static ALWAYS_INLINE uint8_t RF_BleCteGetNumberSamples(void)
{
    uint8_t value = (uint8_t)GET_PP_BLE_CTE_SAMPLE_COUNT(PP->RegPpBleCteSts.r32);
    return value;
}

/**
 * @brief Set CTE Delta Rx time in clock cycle
 * @param rxSwitchTimeDelta Rx Switch Delta Time
 * @param rxSampleTimeDelta Rx Sample Delta Time
*/
void RF_SetCteDeltaRxTime(int16_t rxSwitchTimeDelta, int16_t rxSampleTimeDelta);

/**
 * @brief Set CTE Delta Tx time in clock cycle
 * @param txSwitchTimeDelta Tx Switch Delta Time
*/
void RF_SetCteDeltaTxTime(int16_t txSwitchTimeDelta);

/**
 * @brief Set a default callback for received data from the RF.
 * @param pDefaultRxCallback Default callback
 * @note If the RF_ReceiveData() function, which sets up a callback for the next
 * Rx transaction, is not called before data has been received, a default
 * callback can be called.
 * When the default callback is called, it can be considered has an error since
 * the RF transaction was not properly configured.
 */
void RF_SetDefaultRxCallback(RF_TransactionCallback_t pDefaultRxCallback);

/**
 * @brief Set a default callback after data was sent across the RF.
 * @param pDefaultTxCallback Default callback
 * @note If the RF_SendData() function, which sets up a callback for the next
 * Tx transaction, is not called before data has been sent, a default
 * callback can be called.
 * When the default callback is called, it can be considered has an error since
 * the RF transaction was not properly configured.
 */
void RF_SetDefaultTxCallback(RF_TransactionCallback_t pDefaultTxCallback);

/**
 * @brief Set 802.15.4 in transmit mode
 * @note This function need to be called before calling RF_ReceiveData() if
 * the last operation was not a 802.15.4 receive operation.
 */
void RF_802_15_4_SetTxMode(void);

/**
 * @brief Set 802.15.4 in receive mode
 * @note This function need to be called before calling RF_SendData() if
 * the last operation was not a 802.15.4 transmit operation.
 */
void RF_802_15_4_SetRxMode(void);

/**
 * @brief Set 802.15.4 channel
 * @note A total of 27 channels, numbered from 0 to 26,
 * are specified by IEEE 802.15. 4 across three unlicensed
 * operational frequency bands. Sixteen channels (11 to 26) are available
 * in the 2.4 GHz frequency band, ten in the 915 MHz frequency band,
 * and 868 MHz frequency band based system occupies one channel (see Fig. 4).
 * This radio driver support only channel from 11 to 26 in the 2.4 GHz band.
 * @param channel channel
 * @return true is channel is in range from 11 to 26
 */
bool RF_802_15_4_SetRfChannel(uint8_t channel);

/**
 * @brief Set 802.15.4 frequency
 * @note Frequency shall be from 2400 MHz (channel 11)
 * to 2475 MHz (channel 26)
 * @param frequency frequency
 * @return true if frequency is between 2405 and 2480 MHz
 */
bool RF_802_15_4_SetRfFrequency(uint16_t frequency);

/**
 * @brief Get Linq Quality Indicator
 * @return linq quality indicator value
 */
uint8_t RF_802_15_4_GetLinqQualityIndicator(void);

/**
 * @brief Start Channel Scan
 * @param mode scan mode can be with syncword or invalid syncword
 * @param freq frequency in MHz
 * @param rx_time rx time for channel scan without syncword
 * @param timeout timeout for rx operation
 * @param pData pointer on reception buffer for scan
 * @param dataLength length of data
 * @param pCallback Callback to call upon timeout or rx sync
 * @return scan running status following the scan mode desired
 */
bool RF_802_15_4_StartChannelScan(RF_15_4_ScanMode_t mode,
                                  uint16_t freq,
                                  uint16_t rx_time,
                                  uint32_t timeout,
                                  uint8_t* pData, uint16_t dataLength,
                                  RF_TransactionCallback_t pCallback);

/**
 * @brief Get RSSI after channel scan
 * @param pRadioRssi RSSI raw value from Radio
 * @param pAvgRssi Average RSSI computed by the driver
 */
void RF_802_15_4_GetRssiAfterChannelScan(uint32_t* pRadioRssi, int16_t* pAvgRssi);

/**
 * @brief Get RSSI after Channel Scan
 * @param pRssi Average RSSI during the Scan period for energy detection
 */
void RF_802_12_4_GetScanRssi(int16_t* pRssi);

/**
 * @brief Get all Radio RSSI (used for validation using DTM)
 * @param pRfRssi RAW RSSI during the Scan period for energy detection
 * @param pRssi Average RSSI during the Scan period for energy detection
 * @param pSlowRssi Slow RSSI during the Scan period for energy detection
 * @param pFastRssi Fast RSSI during the Scan period for energy detection
 */
void RF_802_12_4_GetRadioRssi(uint32_t* pRfRssi,  int16_t* pRssi,
                              uint8_t* pSlowRssi, uint8_t* pFastRssi);

/** @} (end addtogroup radio) */

#endif //_RADIO_H
