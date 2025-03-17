/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      802.15.4 MAC interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef MAC_154_INT_H
#define MAC_154_INT_H

#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_timer.h"
#include "wsf_os.h"

#include "mac_154_api.h"
#include "mac_154_cfg.h"
#include "mac_154_defs.h"

#include "pal_bb_154.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  Test address modes. */
#define MAC_154_TEST_GET_DAM(x)     ((x) & 0x3)                  /*!< TBD */
#define MAC_154_TEST_SET_DAM(x, y)  ((x) |= ((y) & 0x3))         /*!< TBD */
#define MAC_154_TEST_GET_SAM(x)     (((x) & 0xc) >> 2)           /*!< TBD */
#define MAC_154_TEST_SET_SAM(x, y)  ((x) |= (((y) << 2) & 0xc))  /*!< TBD */

/*! \brief  MAC states. */
enum
{
  MAC_154_STATE_IDLE = 0,   /*!< MAC is idle. */
  MAC_154_STATE_SCAN = 1,   /*!< MAC is scanning. */
  MAC_154_STATE_RX = 2,     /*!< MAC is receiving. */
  MAC_154_STATE_TX = 3,     /*!< MAC is transmitting. */
  MAC_154_STATE_POLL = 4,   /*!< MAC is polling. */
};

/*! \brief  MAC status. */
enum
{
  MAC_154_SUCCESS,    /*!< Success */
  MAC_154_ERROR       /*!< General error */
};

/*! \brief  MAC rx assessment modes. */
enum
{
  MAC_154_RX_ASSESS_RXWI,   /*!< Assess rx due to rx on when idle change */
  MAC_154_RX_ASSESS_RXEN,   /*!< Assess rx due to rx enabled change */
  MAC_154_RX_ASSESS_PROM    /*!< Assess rx due to promiscuous change */
};
typedef uint8_t Mac154RxAssess_t;

/*! \brief  Parameter Timer callback. */
typedef void (*Mac154ParamTimerFn_t)(void *);

/*! \brief  Parameter Timer. */
typedef struct
{
  Mac154ParamTimerFn_t cback;   /*!< Callback associated with parameter timer */
  void *param;                  /*!< Parameter associated with parameter timer */
  wsfTimer_t timer;             /*!< Timer */
} Mac154ParamTimer_t;

/*! \brief  Obtain the address of the Mac154ParamTimer_t from timer message.
 *
 * The message "param" element is used to hold the negative offset from the
 * address of the message itself to point to the enclosing Mac154ParamTimer_t
 * structure.
 */
#define MAC_154_PARAM_TIMER_FROM_MSG(pMsg)  (Mac154ParamTimer_t *)((uint8_t *)(pMsg) - \
                                                                   ((wsfMsgHdr_t *)(pMsg))->param)

#define MAC_154_ED_SCAN_TEST_MODE_MASK      0x3  /*!< TBD */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize 802.15.4 MAC subsystem with task handler.
 */
/*************************************************************************************************/
void Mac154HandlerInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize MAC for test operations.
 */
/*************************************************************************************************/
void Mac154TestInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Reset test operations.
 */
/*************************************************************************************************/
void Mac154TestReset(void);

/*************************************************************************************************/
/*!
 *  \brief      Set network parameters for test operations.
 *
 *  \param      addr64      Extended address.
 *  \param      addr16      Short address.
 *  \param      panId       PAN ID.
 *
 *  \return     Status code.
 */
/*************************************************************************************************/
uint8_t Mac154TestSetNetParams(uint64_t addr64, uint16_t addr16, uint16_t panId);

/*************************************************************************************************/
/*!
 *  \brief      Start transmit test.
 *
 *  \param      chan            Channel.
 *  \param      power           Power (dBm).
 *  \param      len             Length of test data.
 *  \param      pydType         Test packet payload type.
 *  \param      numPkt          Auto terminate after number of packets, 0 for infinite.
 *  \param      interPktSpace   Interpacket space in microseconds.
 *  \param      rxAck           Receive ACK after transmit.
 *  \param      addrModes       Destination and source address modes.
 *  \param      dstAddr         Destination address.
 *  \param      dstPanId        Destination PAN ID.
 *
 *  \return     Status code.
 */
/*************************************************************************************************/
uint8_t Mac154TestTx(uint8_t chan,
                     uint8_t power,
                     uint8_t len,
                     uint8_t pydType,
                     uint16_t numPkt,
                     uint32_t interPktSpace,
                     bool rxAck,
                     uint8_t addrModes,
                     uint64_t dstAddr,
                     uint16_t dstPanId);

/*************************************************************************************************/
/*!
 *  \brief      Start receive test.
 *
 *  \param      chan            Channel.
 *  \param      numPkt          Auto terminate after number of packets, 0 for infinite.
 *  \param      txAck           Transmit ACK after receive.
 *  \param      promiscuous     Promiscuous mode.
 *
 *  \return     Status error code.
 */
/*************************************************************************************************/
uint8_t Mac154TestRx(uint8_t chan, uint16_t numPkt, bool txAck, bool promiscuous);

/*************************************************************************************************/
/*!
 *  \brief      Get packet statistics.
 *
 *  \param[out] pPktCount     Packet (frame) count.
 *  \param[out] pAckCount     Acknowledgement count.
 *  \param[out] pErrCount     Packet error count.
 */
/*************************************************************************************************/
void Mac154TestGetPktStats(uint32_t *pPktCount,
                           uint32_t *pAckCount,
                           uint32_t *pErrCount);

/*************************************************************************************************/
/*!
 *  \brief      End test.
 */
/*************************************************************************************************/
void Mac154TestEnd(void);

/*************************************************************************************************/
/*!
 *  \brief      Schedule data receive.
 */
/*************************************************************************************************/
void Mac154ScheduleDataRx(void);

/*************************************************************************************************/
/*!
 *  \brief      Start transaction persistence timer.
 *
 *  \param      pTimer    Handle of associated timer.
 */
/*************************************************************************************************/
void Mac154StartTransactionPersistenceTimer(wsfTimer_t *pTimer);

/*************************************************************************************************/
/*!
 *  \brief      Start rx enable timer.
 *
 *  \param      symDuration   Duration in symbols.
 *
 *  \note       The use of a ms timer does not meet the accuracy requirements in 802.15.4 [108,16].
 */
/*************************************************************************************************/
void Mac154StartRxEnableTimer(uint32_t symDuration);

/*************************************************************************************************/
/*!
 *  \brief      Start timer with parameter.
 *
 *  \param      pParamTimer   Timer with parameter.
 *  \param      cback         Callback associated with timer.
 *  \param      param        Parameter associated with timer.
 *  \param      timeout       Timeout value.
 */
/*************************************************************************************************/
void Mac154StartParamTimer(Mac154ParamTimer_t *pParamTimer,
                           Mac154ParamTimerFn_t cback,
                           void *param,
                           uint32_t timeout);

/*************************************************************************************************/
/*!
 *  \brief      Assess whether rx should be enabled or disabled.
 *
 *  \param      assess      Basis to assess rx enabled/disabled.
 *  \param      nextState   What the next state will be.
 *
 *  \return     Start/stop flags.
 */
/*************************************************************************************************/
uint8_t Mac154AssessRxEnable(Mac154RxAssess_t assess, bool nextState);

/*************************************************************************************************/
/*!
 *  \brief      Take appropriate 15.4 receive action.
 *
 *  \param      flags       Start or stop receive.
 */
/*************************************************************************************************/
void Mac154ActionRx(uint8_t flags);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC initialize PIB.
 */
/*************************************************************************************************/
void Mac154ResetPIB(void);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC get PIB attribute.
 *
 *  \param      attrEnum    Attribute enumeration as specified in 802.15.4-2006 Table 86.
 *  \param      pAttrLen    Attribute length in bytes (returned).
 *
 *  \return     Pointer to attribute as byte string
 */
/*************************************************************************************************/
uint8_t *Mac154PibGetAttr(uint8_t attrEnum, uint8_t *pAttrLen);
uint8_t *Mac154PibGetVsAttr(uint8_t attrEnum, uint8_t *pAttrLen);
uint8_t *Mac154PhyPibGetAttr(uint8_t attrEnum, uint8_t *pAttrLen);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC set PIB attribute.
 *
 *  \param      attrEnum    Attribute enumeration as specified in 802.15.4-2006 Table 86.
 *  \param      attrLen     Attribute length in bytes.
 *  \param      pAttr       Pointer to attribute as byte string.
 */
/*************************************************************************************************/
void Mac154PibSetAttr(uint8_t attrEnum, uint8_t attrLen, uint8_t *pAttr);
void Mac154PibSetVsAttr(uint8_t attrEnum, uint8_t attrLen, uint8_t *pAttr);
void Mac154PhyPibSetAttr(uint8_t attrEnum, uint8_t attrLen, uint8_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC get DSN and increment.
 *
 *  \return     DSN
 */
/*************************************************************************************************/
uint8_t Mac154GetDSNIncr(void);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC get BSN and increment.
 *
 *  \return     DSN
 */
/*************************************************************************************************/
uint8_t Mac154GetBSNIncr(void);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC get state.
 *
 *  \return     MAC state.
 */
/*************************************************************************************************/
uint8_t Mac154GetState(void);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC set state
 *
 *  \param      state   State to set the MAC to.
 */
/*************************************************************************************************/
void Mac154SetState(uint8_t state);

/*************************************************************************************************/
/*!
 *  \brief      Initialize MAC for scan operations.
 */
/*************************************************************************************************/
void Mac154ScanInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Start scan.
 *
 *  \param      scanType      Type of scan (ED, active, passive).
 *  \param      scanChannels  Bitmap of channels to scan.
 *  \param      scanDuration  Scan duration (exponential).
 *  \param      testMode      ED/CCA test mode - 10 scans in the list.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154ScanStart(uint8_t scanType, uint32_t scanChannels, uint8_t scanDuration, uint8_t testMode);

/*************************************************************************************************/
/*!
 *  \brief      Start single channel ED scan.
 *
 *  \param      channel         Channel to scan.
 *  \param      scanDurationMs  Scan duration in ms.
 *
 *  \return     Result code.
 *
 *  \note       This function is required for OpenThread ED scan.
 */
/*************************************************************************************************/
uint8_t Mac154SingleChanEDScanStart(uint8_t channel, uint32_t scanDurationMs);

/*************************************************************************************************/
/*!
 *  \brief      Initialize MAC for association operations.
 */
/*************************************************************************************************/
void Mac154AssocInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Start association request.
 *
 *  \param      pCoordAddr    Pointer to coordinator address.
 *  \param      capInfo       Capability information.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154AssocReqStart(Mac154Addr_t *pCoordAddr, uint8_t capInfo);

/*************************************************************************************************/
/*!
 *  \brief      Start association response.
 *
 *  \param      deviceAddr    Device address.
 *  \param      assocShtAddr  Associated short address.
 *  \param      status        Status of association request.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154AssocRspStart(uint64a_t deviceAddr, uint16a_t assocShtAddr, uint8_t status);

#if MAC_154_OPT_DISASSOC

/*************************************************************************************************/
/*!
 *  \brief      Compare device address with Coordinator PIB address [SR 87,39].
 *
 *  \param      pDevAddr      Device address.
 *
 *  \return     true: Matches coordinator address.
 */
/*************************************************************************************************/
bool Mac154AssocDisassocToCoord(Mac154Addr_t *pDevAddr);

/*************************************************************************************************/
/*!
 *  \brief      Start disassociation.
 *
 *  \param      pDevAddr      Device address.
 *  \param      reason        Disassociation reason.
 *  \param      txIndirect    Whether to send indirect or not.
 *  \param      toCoord       true if directed at coordinator.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154AssocDisassocStart(Mac154Addr_t *pDevAddr,
                                 uint8_t reason,
                                 uint8_t txIndirect,
                                 bool toCoord);

#endif /* MAC_154_OPT_DISASSOC */

/*************************************************************************************************/
/*!
 *  \brief      Initialize MAC data.
 */
/*************************************************************************************************/
void Mac154DataInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Execute raw frame callback.
 *
 *  \param      mpduLen       Length of MPDU.
 *  \param      pMpdu         Pointer to MPDU.
 *  \param      linkQuality   Link quality.
 *  \param      timestamp     Timestamp.
 *
 *  \note       It is recommended that the data is queued if it cannot be processed very quickly.
 */
/*************************************************************************************************/
void Mac154ExecuteRawFrameCback(uint8_t mpduLen,
                                uint8_t *pMpdu,
                                uint8_t linkQuality,
                                uint32_t timestamp);

/*************************************************************************************************/
/*!
 *  \brief      Execute Event callback.
 *
 *  \param      pBuf    Buffer formatted as per MAC API document.
 *
 *  \note       It is recommended that the data is queued if it cannot be process very quickly.
 */
/*************************************************************************************************/
bool Mac154ExecuteEvtCback(uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Execute Data callback.
 *
 *  \param      pBuf    Buffer formatted as per MAC API document.
 *
 *  \note       It is recommended that the data is queued if it cannot be process very quickly.
 */
/*************************************************************************************************/
bool Mac154ExecuteDataCback(uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Start data transmit.
 *
 *  \param      srcAddrMode   Source address mode.
 *  \param      pDstAddr      Pointer to destination address.
 *  \param      msduHandle    Handle (used for purge).
 *  \param      txOptions     Bitmap of transmit options.
 *  \param      timestamp     Timestamp when to send it (Zigbee GP support).
 *  \param      msduLen       Length of MSDU.
 *  \param      pMsdu         Pointer to MSDU.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154DataTxStart(uint8_t srcAddrMode,
                          Mac154Addr_t *pDstAddr,
                          uint8_t msduHandle,
                          uint8_t txOptions,
                          uint32_t timestamp,
                          uint8_t msduLen,
                          uint8_t *pMsdu);

/*************************************************************************************************/
/*!
 *  \brief      Start raw frame transmit.
 *
 *  \param      disableCCA    Disable CCA on transmission.
 *  \param      mpduLen       Length of MPDU.
 *  \param      pMpdu         Pointer to MPDU.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154RawFrameTxStart(bool disableCCA, uint8_t mpduLen, uint8_t *pMpdu);

/*************************************************************************************************/
/*!
 *  \brief      Handle transmit complete.
 *
 *  \param      pTxFrame      Pointer to frame transmitted/failed.
 *  \param      handle        Associated handle (if any).
 *  \param      status        Associated status.
 *
 *  \return     true if frame was successfully tx'ed and was a boradcast coord realignment.
 *
 *  Determines the correct indication/response to send based on packet transmitted/failed.
 */
/*************************************************************************************************/
bool Mac154HandleTxComplete(uint8_t *pTxFrame, uint8_t handle, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Start data poll.
 *
 *  \param      pCoordAddr    Pointer to coordinator address.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154DataPollStart(Mac154Addr_t *pCoordAddr);

/*************************************************************************************************/
/*!
 *  \brief      Start data receive.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154DataRxStart(void);

/*************************************************************************************************/
/*!
 *  \brief      Start response to Rx frame that requires a response.
 *
 *  \param      pTxDesc       Tx descriptor built in Rx callback
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154DataRxRspStart(PalBb154TxBufDesc_t *pTxDesc);

#if MAC_154_OPT_ORPHAN
/*************************************************************************************************/
/*!
 *  \brief      Start orphan response.
 *
 *  \param      orphanAddr    Extended address of orphaned.
 *  \param      shtAddr       Short address of orphaned device.
 *  \param      assocMember   Whether orphaned device is associated through coordinator.
 *
 *  \return     Result code.
 */
/*************************************************************************************************/
uint8_t Mac154DataOrphanRspStart(uint64_t orphanAddr, uint16_t shtAddr, uint8_t assocMember);
#endif /* MAC_154_OPT_ORPHAN */

/*************************************************************************************************/
/*!
 *  \brief      Start coordinator realignment.
 *
 *  \param      panId     PAN ID start parameter.
 *  \param      panCoord  PAN Coordinator start parameter.
 *  \param      logChan   Logical channel start parameter.
 *  \param      txPower   Transmit power start parameter.
 *
 *  \return     Result code.
 *
 *  \note       Starts a coordinator realignment frame due to a superframe configuration
 *              change from an MLME-START.req.
 */
/*************************************************************************************************/
uint8_t Mac154DataCoordRealignStart(uint16_t panId,
                                    uint8_t panCoord,
                                    uint8_t logChan,
                                    uint8_t txPower);

/*************************************************************************************************/
/*!
 *  \brief      Get the current FRC time.
 *
 *  \param      pTime   Pointer to return the current time.
 *
 *  \return     Status error code.
 *
 *  Get the current FRC time.
 *
 *  \note       FRC is limited to the same bit-width as the BB clock. Return value is available
 *              only when the BB is active.
 */
/*************************************************************************************************/
uint8_t Mac154GetTime(uint32_t *pTime);

/*************************************************************************************************/
/*!
 *  \brief      Calculate the difference between two timestamps in microseconds.
 *
 *  \param      endTime     Ending time.
 *  \param      startTime   Starting time.
 *
 *  \return     Time in microseconds.
 *
 *  Calculate the difference between two timestamps in microseconds.
 */
/*************************************************************************************************/
uint32_t Mac154CalcDeltaTimeUsec(uint32_t endTime, uint32_t startTime);

#ifdef __cplusplus
};
#endif

#endif /* MAC_154_API_H */
