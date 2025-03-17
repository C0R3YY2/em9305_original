/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE Baseband interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
 *  Packetcraft, Inc. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef PAL_BB_BLE_H
#define PAL_BB_BLE_H

#include "pal_bb.h"
#include "pal_crypto.h"
#include "pal_radio2.h"
#include "ll_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef LL_ENABLE_TESTER
#define LL_ENABLE_TESTER        0       /*!< Enable LL tester extensions. */
#endif


#define PAL_BB_MAX_SW_PATT      17      /*!< Maximum number of switch patterns. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Nonce modes. */
enum
{
  PAL_BB_NONCE_MODE_PKT_CNTR,           /*!< Packet counter mode (default). */
  PAL_BB_NONCE_MODE_EXT16_CNTR,         /*!< 16-bit counter mode, PalCryptoEnc_t::pEventCounter must be non-NULL. */
  PAL_BB_NONCE_MODE_EXT64_CNTR          /*!< 64-bit counter mode, PalCryptoEnc_t::pTxPktCounter/pRxPktCounter must be non-NULL. */
};

/*! \brief      Connection type. */
enum
{
  PAL_BB_TYPE_ACL,                      /*!< ACL. */
  PAL_BB_TYPE_CIS,                      /*!< CIS. */
  PAL_BB_TYPE_BIS                       /*!< BIS. */
};

/*! \addtogroup PAL_BB_BLE_CHAN
 *  \{ */

/*! \brief     CTE parameters. */
typedef struct
{
  bool  enable;                        /*!< Enable status. */
  uint8_t cteTime;                     /*!< CTE Time (implies enable for Tx). */
  uint8_t cteType;                     /*!< CTE type. */
  uint8_t slotDur;                     /*!< Sample and switch slot duration. */
  uint8_t swPattLen;                   /*!< Switching pattern length. */
  uint8_t swPatt[PAL_BB_MAX_SW_PATT];  /*!< Antenna switching pattern list. */
  int8_t *pISampleBuf;                 /*!< Sample buffer to use in I sampling. */
  int8_t *pQSampleBuf;                 /*!< Sample buffer to use in Q sampling. */
  uint8_t numSampRcvd;                 /*!< Number of samples received from IQ sampling. */
} PalCteParam_t;                       /*!< CTE Transmit parameters. */

/*! \brief     CTE states. */
enum
{
  PAL_BB_CTE_STATE_DISABLED,
  PAL_BB_CTE_STATE_TX,
  PAL_BB_CTE_STATE_RX
};

/*! \brief      BLE channelization parameters. */
typedef struct
{
  uint8_t       opType;                 /*!< Operation type. */
  uint8_t       chanIdx;                /*!< Channel index. */
  int8_t        txPower;                /*!< Active transmit power, unit is dBm. */
  uint8_t       antID;                  /*!< Primary antenna. */
  uint32_t      accAddr;                /*!< Access address. */
  uint32_t      crcInit;                /*!< CRC initialization value. */
  PalBbPhy_t    txPhy;                  /*!< Transmitter PHY. */
  PalBbPhy_t    rxPhy;                  /*!< Receiver PHY. */
  uint8_t       initTxPhyOptions;       /*!< Initial Tx PHY options. */
  uint8_t       tifsTxPhyOptions;       /*!< TIFS Tx PHY options. */
  bool          peerTxStableModIdx;     /*!< Peer uses stable modulation index on transmitter. */
  bool          peerRxStableModIdx;     /*!< Peer uses stable modulation index on receiver. */
  bool          optimizeSetParam;       /*!< Optimize setting of parameters. */
#if (CTE_PRESENT)
  PalCteParam_t txCteParam;             /*!< Tx CTE parameters. */
  PalCteParam_t rxCteParam;             /*!< Rx CTE parameters. */
#endif
  PalCryptoEnc_t enc;                   /*!< Encryption parameters (NULL if disabled). */
  uint32_t      accAddrPawr;            /*!< Access address for AUX_SYNC_SUBEVENT_RSP. */
#if (LL_ENABLE_RSPOBS)
  uint32_t      accAddr2;               /*!< Access address copy for Response observation. */
#endif
#if (LL_ENABLE_TESTER)
  uint32_t      accAddrRx;              /*!< Access address override for receptions. */
  uint32_t      accAddrTx;              /*!< Access address override for transmissions. */
  uint32_t      crcInitRx;              /*!< CRC initialization override for receptions. */
  uint32_t      crcInitTx;              /*!< CRC initialization override for transmissions. */
  int8_t        txPwrOffset;            /*!< Override offset for txPower. */
#endif
} PalBbBleChan_t;

/*! \} */    /* PAL_BB_BLE_CHAN */

/*! \addtogroup PAL_BB_BLE_DATA
 *  \{
 *  This section contains driver routines used for packet transmission.
 */

/*! \brief      Transmit complete ISR callback signature. */
typedef void (*PalBbBleTxIsr_t)(uint8_t status);

/*! \brief      Receive complete ISR callback signature. */
typedef void (*PalBbBleRxIsr_t)(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions);

/*! \brief      IFS modes. */
enum
{
  PAL_BB_IFS_MODE_CLR,              /*!< Clear IFS (last packet). */
  PAL_BB_IFS_MODE_TOGGLE_TIFS,      /*!< Toggle operation with TIFS timing. */
  PAL_BB_IFS_MODE_SAME_ABS,         /*!< Same operation with absolute timing. */
  PAL_BB_IFS_MODE_SAME_TIFS,        /*!< Same operation with TIFS timing. */
  PAL_BB_IFS_MODE_TOGGLE_CS         /*!< Toggle operation with CS timing. */
};
typedef uint8_t PalBbIfsMode_t;

/*! \brief      BLE data transfer parameters. */
typedef struct
{
  PalBbBleTxIsr_t   txCback;        /*!< Transmit completion callback. */
  PalBbBleRxIsr_t   rxCback;        /*!< Receive completion callback. */

  uint32_t          dueUsec;        /*!< Due time of the first packet in microseconds. */
  uint32_t          rxTimeoutUsec;  /*!< Receive timeout in microseconds. */
} PalBbBleDataParam_t;

/*! \brief    Operation parameters. */
typedef struct
{
  PalBbIfsMode_t    ifsMode:8;      /*!< IFS mode for next operation. */
  uint32_t          ifsTime;        /*!< Absolute time of next PDU. */
  PalBbBleChan_t    *pIfsChan;      /*!< Channel of next PDU, NULL for no change. */
  uint32_t          csStepIfsUsec;  /*!< CS step IFS time. */
  uint32_t          rxTimeout;      /*!< Timeout value. */
} PalBbBleOpParam_t;

/*! \brief    Transmit buffer descriptor. */
typedef struct
{
  uint16_t          len;            /*!< Length of buffer. */
  uint8_t           *pBuf;          /*!< Pointer to buffer. */
} PalBbBleTxBufDesc_t;

/*! \brief       Call signature for set CTE params function. */
typedef void (*palBbBleSetCteParamsFn_t)(PalCteParam_t *pTxCteParam, PalCteParam_t *pRxCteParam, bool cteInHdr);

/*! \} */    /* PAL_BB_BLE_DATA */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup PAL_BB_BLE_INIT
 *  \{
 *  This section contains driver routines which initialize as well as enable the BLE mode of the
 *  BB hardware.
 */

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BLE baseband driver.
 *
 *  One-time initialization of BLE baseband driver.
 */
/*************************************************************************************************/
void PalBbBleInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Enable the BB hardware.
 *
 *  Wake the BB hardware out of sleep and enable for operation. All BB functionality is
 *  available when this routine completes. BB clock is set to zero and started.
 */
/*************************************************************************************************/
void PalBbBleEnable(void);

/*************************************************************************************************/
/*!
 *  \brief      Disable the BB hardware.
 *
 *  Disable the baseband and put radio hardware to sleep. Must be called from an idle state.
 *  A radio operation cannot be in progress.
 */
/*************************************************************************************************/
void PalBbBleDisable(void);

/*************************************************************************************************/
/*!
 *  \brief      Low power operation.
 *
 *  \note       Called by upper baseband code.
 */
/*************************************************************************************************/
void PalBbBleLowPower(void);

/*! \} */    /* PAL_BB_BLE_INIT */

/*! \addtogroup PAL_BB_BLE_CHAN
 *  \{
 *  This section contains the driver routine used to set the chanelization parameters.
 */

/*************************************************************************************************/
/*!
 *  \brief      Set channelization parameters.
 *
 *  \param      pChan       Channelization parameters.
 *
 *  Calling this routine will set parameters for all future transmit and receive operations
 *  until this routine is called again providing new parameters.
 *
 *  The setting of channelization parameters influence the operations of the following listed
 *  routines. Therefore, this routine is called to set the channel characteristics before
 *  the use of data routines described in \a PAL_BB_BLE_DATA.
 *
 *  \note       The \a pParam contents are not guaranteed to be static and is only valid in the
 *              context of the call to this routine. Therefore parameters requiring persistence
 *              should be copied.
 */
/*************************************************************************************************/
void PalBbBleSetChannelParam(PalBbBleChan_t *pChan);

/*************************************************************************************************/
/*!
 *  \brief      Set channelization parameters without applying RF output power.
 *
 *  \param      pChan       Channelization parameters.
 *
 *  This function is a wrapper for PalBbBleSetChannelParam with the side-effect
 *  of not applying RF output power. This is done to meet T_MAFS timing on
 *  some platforms, an a no-op on other platforms. This is a workaround
 *  for BLE 5.4 qualification tests LL/DDI/SCN/BV-79 and LL/DDI/SCN/BV-80.
 *
 */
/*************************************************************************************************/
static inline void PalBbBleSetChannelParamNoApplyOutputPower(PalBbBleChan_t *pChan)
{
  PalRadioBypassSetRFPower();
  PalBbBleSetChannelParam(pChan);
}

/*************************************************************************************************/
/*!
 *  \brief      Set the inline encryption packet count for transmit.
 *
 *  \param      pktCtr  Packet counter value, a 39-bit value (upper bits are ignored).
 */
/*************************************************************************************************/
void PalBbBleSetInlineEncryptPacketCount(uint64_t pktCtr);

/*************************************************************************************************/
/*!
 *  \brief      Set the inline decryption packet count for reception.
 *
 *  \param      pktCtr  Packet counter value, a 39-bit value (upper bits are ignored).
 */
/*************************************************************************************************/
void PalBbBleSetInlineDecryptPacketCount(uint64_t pktCtr);

/*! \} */    /* PAL_BB_BLE_CHAN */

/*! \addtogroup PAL_BB_BLE_DATA
 *  \{
 *  This section contains driver routines used for packet transmission.
 */

/*************************************************************************************************/
/*!
 *  \brief      Set the data packet exchange parameters.
 *
 *  \param      pParam      Data exchange parameters.
 *
 *  Calling this routine will set parameters for all future transmit and receive operations
 *  until this routine is called again providing new parameters.
 */
/*************************************************************************************************/
void PalBbBleSetDataParams(const PalBbBleDataParam_t *pParam);

/*************************************************************************************************/
/*!
 *  \brief      Set the operation parameters.
 *
 *  \param      pOpParam    Operations parameters.
 *
 *  Calling this routine will set parameters for the next transmit or receive operations.
 */
/*************************************************************************************************/
void PalBbBleSetOpParams(const PalBbBleOpParam_t *pOpParam);

/*************************************************************************************************/
/*!
 *  \brief      Transmit a packet.
 *
 *  \param      descs       Array of transmit buffer descriptors.
 *  \param      cnt         Number of descriptors.
 *
 *  Set the first data buffer for the first packet of an alternating Tx-Rx data exchange cycle.
 */
/*************************************************************************************************/
void PalBbBleTxData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Transmit packet at TIFS after the last packet received.
 *
 *  \param      descs       Array of transmit buffer descriptor.
 *  \param      cnt         Number of descriptors.
 *
 *  If possible, the transmit will occur at the TIFS timing. If not possible, the callback status
 *  will indicate this.
 */
/*************************************************************************************************/
void PalBbBleTxTifsData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Receive packet.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Length of data buffer.
 *
 *  Set the first data buffer for the first packet of an alternating Rx-Tx data exchange cycle.
 */
/*************************************************************************************************/
void PalBbBleRxData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Enable set maximum PDU length for packet processor.
 *
 *  Enable set of max PDU length in packet processor. Packet processor stop the reception of
 *  data when max length is reached and generated an ISR with PDU len error.
 */
/*************************************************************************************************/
void PalBbEnableMaxRxLen();

/*************************************************************************************************/
/*!
 *  \brief      Receive CS packet.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Length of data buffer.
 *
 *  Set the first data buffer for the first packet of an alternating Rx-Tx data exchange cycle.
 */
/*************************************************************************************************/
void PalBbBleCsRxData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Receive packet at TIFS after the last packet transmitted.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Length of data buffer.
 *
 *  If possible, the receive will occur on the TIFS timing. If not possible, the callback status
 *  will indicate this.
 */
/*************************************************************************************************/
void PalBbBleRxTifsData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Transmit packet at TIFS after the last packet received.
 *
 *  \param      descs       Array of transmit buffer descriptor.
 *  \param      cnt         Number of descriptors.
 */
/*************************************************************************************************/
void PalBbBleCsTxIfsData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Receive CS packet after the last packet transmitted.
 *
 *  \param      pBuf        Receive data buffer.
 *  \param      len         Length of data buffer.
 */
/*************************************************************************************************/
void PalBbBleCsRxIfsData(uint8_t *pBuf, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief      Cancel TIFS timer.
 *
 *  This stops any active TIFS timer operation. This routine is always called in the callback
 *  (i.e. ISR) context.
 */
/*************************************************************************************************/
void PalBbBleCancelTifs(void);

/*************************************************************************************************/
/*!
 *  \brief      Cancel a pending transmit or receive.
 *
 *  This stops any active radio operation. This routine is never called in the callback
 *  (i.e. ISR) context.
 */
/*************************************************************************************************/
void PalBbBleCancelData(void);

/*! \} */    /* PAL_BB_BLE_DATA */

/*! \addtogroup PAL_BB_BLE_TEST
 *  \{
 *  This section contains driver routines used for test modes.
 */

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable data whitening.
 *
 *  \param      enable       Flag to indicate data whitening.
 *
 *  Sets an internal variable that indicates if data whitening is enabled or not.
 */
/*************************************************************************************************/
void PalBbBleEnableDataWhitening(bool enable);

/*************************************************************************************************/
/*!
 *  \brief      Enable or disable PRBS15.
 *
 *  \param      enable       Flag to indicate PRBS15.
 *
 *  Immediately enable or disable continuous PRBS15 bitstream. Setting the channelization
 *  parameters with \a PalBbBleSetChannelParam() must precede enabling PRBS15.
 *
 *  Use of \a PAL_BB_BLE_DATA routines is not allowed while PRBS15 is enabled.
 */
/*************************************************************************************************/
void PalBbBleEnablePrbs15(bool enable);

/*! \} */    /* PAL_BB_BLE_TEST */

/*! \addtogroup PAL_BB_BLE_CTE
 *  \{
 *  This section contains driver routines used for constant tone expression.
 */

/*************************************************************************************************/
/*!
 *  \brief      Initialize BB CTE resources.
 */
/*************************************************************************************************/
void PalRadioCteInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Set Antenna ID.
 *
 *  \param      ID     Antenna ID.
 */
/*************************************************************************************************/
void PalRadioFemSetAntId(uint8_t ID);

/*************************************************************************************************/
/*!
 *  \brief      Transmit a CS packet.
 *
 *  \param      descs       Array of transmit buffer descriptors.
 *  \param      cnt         Number of descriptors.
 *
 *  Set the first data buffer for the first CS packet of an alternating Tx-Rx CS exchange cycle.
 */
/*************************************************************************************************/
void PalBbBleCsTxData(PalBbBleTxBufDesc_t descs[], uint8_t cnt);

/*************************************************************************************************/
/*!
 *  \brief      Set CS channelization parameters.
 *
 *  \param      pChan       Channelization parameters.
 *
 *  Calling this routine will set parameters for all future transmit and receive operations
 *  until this routine is called again providing new parameters.
 *
 *  The setting of channelization parameters influence the operations of the following listed
 *  routines. Therefore, this routine is called to set the channel characteristics before
 *  the use of data routines described in \a PAL_BB_BLE_DATA.
 *
 *  \note       The \a pParam contents are not guaranteed to be static and is only valid in the
 *              context of the call to this routine. Therefore parameters requiring persistence
 *              should be copied.
 */
/*************************************************************************************************/
void PalBbBleCsSetChannelParam(PalBbBleChan_t *pChan);

/*! \} */    /* PAL_BB_BLE_CTE */

/*! \addtogroup PAL_BB_BLE_CS
 *  \{
 *  This section contains driver routines used for Channel Sounding.
 */

/*! \brief      Channel Sounding roles values. */
enum
{
  PAL_BB_BLE_CS_ROLE_INITIATOR,
  PAL_BB_BLE_CS_ROLE_REFLECTOR
};

/*! \brief      Channel Sounding role type. */
typedef uint8_t PalBbBleCsRole_t;

/*! \brief      Channel Sounding Step completion call signature. */
typedef void (*PalBbBleCsStepComp_t)(uint8_t status, int8_t rssi, uint32_t startTs);

/*! \brief      Channel Sounding Step parameters. */
typedef struct
{
  PalBbBleCsRole_t role;            /*!< Role. */
  uint8_t mode;                     /*!< Mode. */
  PalBbBleCsStepComp_t stepCback;   /*!< Step complete callback. */

  /* Channel */
  uint8_t csChan;                   /*!< Channel Sounding channel index. */
  uint8_t phy;                      /*!< PHY. */
  /* Antenna */
  uint8_t pattSwLen;                        /*!< Switching pattern length. */
  uint8_t antId[LL_CS_MAX_NUM_ANT_PATHS];   /*!< Antenna ID array. */
  /* Timing */
  uint32_t rxTimeoutUsec;           /*!< Receive timeout in microseconds. */
  uint16_t ssUsec;                  /*!< Sub-step spacing microseconds. */
  uint16_t tsUsec;                  /*!< Tone spacing microseconds (valid when \a pattSwLen > 0). */
  struct
  {
    uint32_t aa;                    /*!< Access Address. */
    uint8_t *pSyncBuf;              /*!< CS_SYNC buffer. */
    uint16_t syncLen;               /*!< CS_SYNC buffer length. */
    uint16_t toneDurUsec;           /*!< Tone duration (single antenna). */
  } init;                           /*!< Initiator step parameters. */
  struct
  {
    uint32_t aa;                    /*!< Access Address. */
    uint8_t *pSyncBuf;              /*!< CS_SYNC buffer. */
    uint16_t syncLen;               /*!< CS_SYNC buffer length. */
    uint16_t toneDurUsec;           /*!< Tone duration (single antenna). */
  } ref;                            /*!< Reflector step parameters. */
} PalBbBleCsStep_t;

/*! \brief      Channel Sounding report. */
typedef struct
{
  uint8_t pktQual;                  /*!< Packet quality. */
  uint8_t pktNadm;                  /*!< Packet NADM. */
  uint8_t pktRssi;                  /*!< Packet RSSI. */
  uint8_t pktAnt;                   /*!< Packet antenna. */
  uint32_t pktPct1;                 /*!< Packet PCT 1. */
  uint32_t pktPct2;                 /*!< Packet PCT 2. */
  uint16_t measFreqOff;             /*!< Measured frequency offset. */
  uint16_t toAToDInitiator;         /*!< To A to D initiator. */
  uint16_t toDToAReflector;         /*!< To D to A reflector. */
  uint8_t antPermIdx;               /*!< Antenna permutation index. */
  struct
  {
    uint32_t pct;                       /*!< Tone PCT. */
    uint8_t qualInd;                    /*!< Tone quality indicator. */
  } tone[LL_CS_MAX_NUM_ANT_PATHS + 1];  /*!< Tone data per antenna path. */
} PalBbBleCsStepReport_t;

/*************************************************************************************************/
/*!
*  \brief      Initialize radio resources for Channel Sounding.
*/
/*************************************************************************************************/
void PalBbBleCsInit(void);

/*************************************************************************************************/
/*!
*  \brief      Set Channel Sounding step parameters.
*
*  \param      pParam     Step parameters.
*/
/*************************************************************************************************/
void PalBbBleCsSetParam(PalBbBleCsStep_t *pParam);

/*************************************************************************************************/
/*!
*  \brief      Set Channel Sounding start time.
*
*  \param      dueTime     Due time.
*
*  Radio may sleep until \a dueTime.
*/
/*************************************************************************************************/
void PalBbBleCsStartStep(uint32_t dueTime);

/*************************************************************************************************/
/*!
*  \brief      Get Channel Sounding Step report.
*
*  \param      pRpt       Report buffer.
*/
/*************************************************************************************************/
void PalBbBleCsGetStepReport(PalBbBleCsStepReport_t *pRpt);

/*! \} */    /* PAL_BB_BLE_CS */

#ifdef __cplusplus
};
#endif

#endif /* PAL_BB_BLE_H */
