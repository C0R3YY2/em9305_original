////////////////////////////////////////////////////////////////////////////////
///
/// @file       em_system_stack.h
///
/// @project    T9305
///
/// @brief      Header file for em_system mini stack
///
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

#ifndef EM_SYSTEM_STACK_H
#define EM_SYSTEM_STACK_H

#include "qf_port.h"
#include "radio.h"

/******************************************************************************\
 *  QPC Symbolic Constants
\******************************************************************************/
/// Add an offset to all signals to avoid QPC reserved signals.
#define FIRST_SIG_OFFSET             (Q_USER_SIG)
#define SENDPKTS_TX_SIG              (FIRST_SIG_OFFSET + (enum_t)0x01u)
#define SENDPKTS_STOP_SIG            (FIRST_SIG_OFFSET + (enum_t)0x02u)
#define SENDPKTS_START_SIG           (FIRST_SIG_OFFSET + (enum_t)0x03u)
#define SENDPKTS_RX_SIG              (FIRST_SIG_OFFSET + (enum_t)0x04u)
#define SENDPKTS_ADV_SIG             (FIRST_SIG_OFFSET + (enum_t)0x05u)
#define SENDPKTS_RX_RSSI_SIG         (FIRST_SIG_OFFSET + (enum_t)0x06u)
#define SENDPKTS_START_SCAN_15_4_SIG (FIRST_SIG_OFFSET + (enum_t)0x07u)
#define SENDPKTS_SCAN_15_4_PKT_SIG   (FIRST_SIG_OFFSET + (enum_t)0x08u)
#define NUM_TASKSENDPKTS_TASK_EVENTS 8u

/******************************************************************************\
 *  DTM defines
\******************************************************************************/
// symbolic constants
#define TEST_PKT_TYPE_PRBS9 0x00           /*!< Pseudo-Random bit sequence 9.    */
#define TEST_PKT_TYPE_0F 0x01              /*!< 00001111'b packet payload type.  */
#define TEST_PKT_TYPE_0F_DATA 0x0F         /*!< 00001111'b packet payload type.  */
#define TEST_PKT_TYPE_55 0x02              /*!< 01010101'b packet payload type.  */
#define TEST_PKT_TYPE_55_DATA 0x55         /*!< 01010101'b packet payload type.  */
#define TEST_PKT_TYPE_PRBS15 0x03          /*!< Pseudo-Random bit sequence 15.   */
#define TEST_PKT_TYPE_FF 0x04              /*!< 11111111'b packet payload type.  */
#define TEST_PKT_TYPE_FF_DATA 0xFF         /*!< 11111111'b packet payload type.  */
#define TEST_PKT_TYPE_00 0x05              /*!< 00000000'b packet payload type.  */
#define TEST_PKT_TYPE_00_DATA 0x00         /*!< 00000000'b packet payload type.  */
#define TEST_PKT_TYPE_F0 0x06              /*!< 11110000'b packet payload type.  */
#define TEST_PKT_TYPE_F0_DATA 0xF0         /*!< 11110000'b packet payload type.  */
#define TEST_PKT_TYPE_AA 0x07              /*!< 10101010'b packet payload type.  */
#define TEST_PKT_TYPE_AA_DATA 0xAA         /*!< 10101010'b packet payload type.  */
#define TEST_PKT_TYPE_802_15_4_A_DATA 0x08 /*!< 0x0102030405060708090A0B0C0D0E0F */
#define TEST_PKT_TYPE_802_15_4_B_DATA 0x09 /*!< 0x0123456789ABCDEF               */
#define TEST_PKT_TYPE_802_15_4_C_DATA 0x0A /*!< 0x102030405060708090A0B0C0D0E0F0 */
#define TEST_PKT_TYPE_802_15_4_BEACON_DATA 0x0B /*!< Beacon packet */

#define TEST_PKT_MAX_PLD_LENGTH 0xFFU /*< Maximum payload length: 1-255. */
#define PHY_BLE_1M 1u                 /*!< BLE 1Mbps PHY. */
#define PHY_BLE_2M 2u                 /*!< BLE 2Mbps PHY. */
#define PHY_BLE_CODED_S8 3u           /*!< BLE Coded PHY (data coding S8). */
#define PHY_BLE_CODED_S2 4u           /*!< BLE Coded PHY (data coding S2). */
#define PHY_802_15_4     5u           /*!< 802.15.4 PHY */
#define DTM_PROT_TIMER_CHAN                                                    \
    0x02u                  // protocol timer channel 2 is used for timing
                           // QPC events to send packets
#define DTM_HDR_LEN 2u     /*!< Direct Test Mode PDU header length. */
#define CRC_LEN 3u         /*!< CRC length. */
#define AA_LEN 4u          /*!< Access address length. */
#define PREAMBLE_LEN_1M 1u /*!< Preamble length (LE 1M PHY). */
#define PREAMBLE_LEN_2M 2u /*!< Preamble length (LE 2M PHY). */
#define PREAMBLE_LEN_CODED_BITS 10u /*!< Preamble length (LE Coded PHY). */
#define CI_LEN_BITS 2u        /*!< Coding indicator length (LE Coded PHY). */
#define TERM1_LEN_BITS 3u     /*!< TERM1 length (LE Coded PHY). */
#define TERM2_LEN_BITS 3u     /*!< TERM2 length (LE Coded PHY). */
#define BLE_US_PER_BYTE_1M 8u /*!< BLE PHY speed (LE 1M PHY). */
#define BLE_US_PER_BYTE_2M 4u /*!< BLE PHY speed (LE 2M PHY). */
#define BLE_US_PER_BYTE_CODED_S8                                               \
    64u                            /*!< BLE PHY speed (LE Coded PHY, S=8).     \
                                    */
#define BLE_US_PER_BIT_CODED_S8 8u /*!< BLE PHY speed (LE Coded PHY, S=8). */
#define BLE_US_PER_BYTE_CODED_S2                                               \
    16u                            /*!< BLE PHY speed (LE Coded PHY, S=2).     \
                                    */
#define BLE_US_PER_BIT_CODED_S2 2u /*!< BLE PHY speed (LE Coded PHY, S=2). */

#define BLE_SLOT_US 625U

// ---------------------------- 802.15.4 definitions -----------------------------
#define SFD_LEN_802_15_4      1
#define PREAMBLE_LEN_802_15_4 4
#define LEN_802_15_4          1
#define CRC_LEN_802_15_4      2
#define US_PER_BYTE_802_15_4  32
// -------------------------------------------------------------------------------

#define TX_MODE 1U
#define RX_MODE 2U
#define ADVERTISING_MODE 3U

#define SCAN_15_4_SYNCWORD_MODE 0U
#define SCAN_15_4_NO_SYNCWORD_MODE 1U

#define ADV_DATA_DEFAULT 0x00
#define ADV_DATA_LEN_DEFAULT 0x14U
#define ADV_INTERVAL_DEFAULT 0x000FU
#define ADV_START_CHAN_BLE 37
#define ADV_END_CHAN_BLE 39
#define ADV_INITIAL_DELAY 22500 /* ~1ms */
#define ADV_TYPE_DEFAULT 0x00 /*! Connectable & Scannable Undirected ADV */
#define BD_ADDR_NUM_BYTES 6U


#define BLE_WHITENING_ENABLED true
#define BLE_WHITENING_DISABLED false

// macros
#define MATH_DIV_625(n) ((uint32_t)(((uint64_t)(n)*UINT64_C(1717987)) >> 30))
/// Common type for all tasks.
typedef struct
{
    /// super
    QActive super;
} QL_Task_t;

/// Control data for tx packets QPC task.
typedef struct
{
    /// Mode
    uint8_t mode;
    /// RF channel
    uint8_t channel;
    /// Frequency offset from 2400 MHz
    uint8_t freq;
    /// length of packet payload
    uint8_t dataLength;
    /// Ppcket Payload
    uint8_t packetPayload;
    /// the physical layer
    uint8_t phy;
    /// the interval
    uint32_t interval;
    /// access address
    uint32_t syncword;
    /// RF output power
    int8_t outputPower;
    /// Rx Time for Scan
    uint16_t rx_time;
} emSystemStackControlBlock_t;

/******************************************************************************\
 *  Globals
\******************************************************************************/

// Internal. Prevents multiple tasks from being created.
extern bool gIsEmSystemStackInit;

/******************************************************************************\
 *  Public Functions
\******************************************************************************/
/**
 * @brief Inline util function for converting phy as an integre to a RF_Mode_t
 * for the radio driver
 * @param phy: PHY as integer as it would be passed into the HCI command.
 **/
RF_Mode_t getPhyMode(uint8_t phy);

/**
 * @brief Inline util function for creating pseudo random 9 payload. Adapted
 * from packetcraft DTM
 * @param pBuf: pointer to the packet payload buffer
 * @param len: length of payload buffer
 **/
void llPrbs9Payload(uint8_t *pBuf, uint8_t len);

/**
 * @brief util function for determining packet interval based on length
 * and phy. Adapted from packetcraft DTM
 * @param len: length of payload buffer
 * @param phy: requested phy: either 1M, 2M coded S8 or coded S2
 * @return time in BLE slots (625us multiples) needed for packet.
 **/
uint32_t calcPacketInterval(uint8_t len, uint8_t phy);

/**
 * @brief util function for determining packet air time based on length
 * and phy. Adapted from packetcraft DTM
 * @param len: length of payload buffer
 * @param phy: requested phy: either 1M, 2M coded S8 or coded S2
 * @return time in microseconds packet will spend over the air.
 **/
uint32_t calcPacketAirTime(uint8_t len, uint8_t phy);

/**
 * @brief Inline util function for building packet buffer (in place). Adapted
 * from packetcraft DTM
 * @param len: length of payload buffer
 * @param pktType: type of DTM packet (payload type)
 * @param pBuf: pointer to packet buffer
 **/
void buildTxPkt(uint8_t len, uint8_t pktType, uint8_t *pBuf);

/**
 * @brief util function for building 802.15.4 packet buffer.
 * @param len: length of payload buffer
 * @param pktType: type of DTM packet (payload type)
 * @param pBuf: pointer to packet buffer
 * @return length of packet used
 */
uint8_t buildTx_802_15_4_Pkt(uint8_t len, uint8_t pktType, uint8_t *pBuf);

/**
 * @brief start transmitting packets given the input parameters. Calls the QPC
 * task construtor and sends start event to the task. No need to manually call
 * TASKSENDPKTS_ctor or TASKSENDPKTS_Start.
 * @param transmitPowerLevel: power level in dBm to transmit
 * @param packetPayload: which type of data to use in the packet
 * @param testDataLength: length of packet payload
 * @param channel: RF channel to use
 * @param phy: which physical layer to use
 * @param pEventParams: return event parameters of the HCI command
 * @param pReturnParamsLength: Length of HCI command return parameters
 * @return BLE vendor specific (em_system) status code.
 **/
uint8_t EMSystemStack_TransmitStart(int8_t transmitPowerLevel,
                                    uint8_t packetPayload,
                                    uint8_t testDataLength, uint8_t phy,
                                    uint8_t channel, uint8_t *pEventParams,
                                    uint8_t *pReturnParamsLength);

/**
 * @brief stop transmitting packets. Sends the stop event to the QPC task.
 * No need to manually call TASKSENDPKTS_Stop
 * @return BLE vendor specific (em_system) status code.
 **/
uint8_t EMSystemStack_TransmitStop(void);

/**
 * @brief start receiving packets similar to what would happen in BLE DTM.
 * @param channel: RF channel (not BLE channel!) to listen on
 * @param phy: either 1M, 2M, LE Coded S=8, LE Coded S=2
 * @param syncword: access address matching what is transmitted. Typically
 * 0x71764129 for DTM.
 * @return BLE vendor specific (em_system) status code.
 **/
uint8_t EMSystemStack_ReceiveStart(uint8_t channel, uint8_t phy,
                                   uint32_t syncword);

/**
 * @brief stop receiving packets similar to what would happen in BLE DTM.
 * @param pRssiAvg: pointer to the rssiAvg. rssiAvg value will be the average
 * RSSI of all packets found during the receive.
 * @param pLqiAvg: pointer to the linq quality indicator. lqiAvg value will be
 * the average of all packets found during the receive.
 * @return BLE vendor specific (em_system) status code.
 **/
uint8_t EMSystemStack_ReceiveStop(int16_t *pRssiAvg, uint8_t* pLqiAvg);

/**
 * @brief start advertising given the following default values
 * Advertising_Interval_Min: 0x000F
 * Advertising_Interval_Max: 0x000F
 * Advertising_Type: 0x00
 * Own_Address_Type: 0x00
 * Peer_Address_Type: 0x00
 * Peer_Address: 0x000000000000
 * Advertising_Channel_Map: 0x07
 * Advertising_Filter_Policy: 0x00
 * Advertising_Data_Length: 0x14
 * Advertising_Data: 31x 0x00
 * @return BLE vendor specific (em_system) status code.
 */
uint8_t EMSystemStack_StartAdvertising(void);

/**
 * @brief Start 802.15.4 Channel Scanning using valid syncword or not
 * @param mode mode of scan, can be 0 = use 802.15.4 syncword, 1 = invalid syncword
 * @param frequency frequency offset from 2400 MHz, range from 0 to 80.
 * @param rx_time duration of scan in us
 * @param pEventParams return event parameters of the HCI command
 * @param pReturnParamsLength Length of HCI command return parameters
 * @return vendor specific (em_system) status code
 */
uint8_t EMSystemStack_StartScan_15_4(uint8_t mode,
                                     uint8_t frequency,
                                     uint16_t rx_time,
                                     uint8_t* pEventParams,
                                     uint8_t* pReturnParamsLength);

/**
 * @brief @brief Resume system stack after wakeup
 */
void EMSystemStack_Resume(void);

/**
 * @brief Constructor for QPC task to send packets
 */
void TASKSENDPKTS_ctor(void);

/**
 * @brief start transmitting packets given the input parameters. Called by
 * EMSystemStack_TransmitStart. Use that function instead of this one.
 * @param transmitPowerLevel: power level in dBm to transmit
 * @param packetPayload: which type of data to use in the packet
 * @param testDataLength: length of packet payload
 * @param channel: RF channel to use
 * @param phy: which physical layer to use
 * @param pEventParams: return event parameters of the HCI command
 * @param pReturnParamsLength: Length of HCI command return parameters
 */
void TASKSENDPKTS_Start(int8_t transmitPowerLevel, uint8_t packetPayload,
                        uint8_t testDataLength, uint8_t phy, uint8_t channel,
                        uint8_t *pEventParams, uint8_t *pReturnParamsLength);
/**
 * @brief stop transmitting packets. Sends the stop event to the QPC task.
 * Called by EMSystemStack_TransmitStop. Use that function instead of this one.
 **/
void TASKSENDPKTS_Stop(void);

#endif
