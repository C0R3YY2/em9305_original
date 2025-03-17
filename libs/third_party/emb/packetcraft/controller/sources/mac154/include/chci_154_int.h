/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      802.15.4 controller HCI interface file.
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

#ifndef CHCI_154_INT_H
#define CHCI_154_INT_H

#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "mac_154_cfg.h"
#include "mac_154_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Message header length. */
#define CHCI_154_MSG_HDR_LEN                    3   /*!< TBD */

/*! \brief      Command handlers. */
#ifndef CHCI_154_CMD_HANDLER_NUM
#define CHCI_154_CMD_HANDLER_NUM                8   /*!< TBD */
#endif

/**************************************************************************************************
  Message ID definitions
     0x00 - 0x3F : Packetcraft Command
     0x40 - 0x7F : Reserved
     0x80 - 0xBF : Packetcraft Event
     0xA0 - 0xAF : Packetcraft VS
     0xB0 - 0xCF : User Command
     0xD0 - 0xEF : User Event
     0xF0 - 0xFF : Packetcraft Test
**************************************************************************************************/

/*! \brief      Test command defines. */
#define CHCI_154_CMD_TEST_GET_BB_STATS_REQ      0xF1     /*!< TBD */
#define CHCI_154_CMD_TEST_GET_BB_STATS_CNF      0xF2     /*!< TBD */
#define CHCI_154_CMD_TEST_CONTINOUS_STOP        0xF3     /*!< TBD */
#define CHCI_154_CMD_TEST_CONTINOUS_TX          0xF4     /*!< TBD */
#define CHCI_154_CMD_TEST_CONTINOUS_RX          0xF5     /*!< TBD */
#define CHCI_154_CMD_TEST_GET_PKT_STATS_REQ     0xF6     /*!< TBD */
#define CHCI_154_CMD_TEST_GET_PKT_STATS_CNF     0xF7     /*!< TBD */
#define CHCI_154_CMD_TEST_GET_LAST_RSSI_REQ     0xF8     /*!< TBD */
#define CHCI_154_CMD_TEST_GET_LAST_RSSI_CNF     0xF9     /*!< TBD */
#define CHCI_154_CMD_TEST_SET_NET_PARAMS_REQ    0xFA     /*!< TBD */
#define CHCI_154_CMD_TEST_SET_NET_PARAMS_CNF    0xFB     /*!< TBD */
#define CHCI_154_CMD_TEST_TX                    0xFC     /*!< TBD */
#define CHCI_154_CMD_TEST_RX                    0xFD     /*!< TBD */
#define CHCI_154_CMD_TEST_END                   0xFE     /*!< TBD */
#define CHCI_154_CMD_TEST_END_IND               0xFF     /*!< TBD */

/*! \brief      Test command lengths. */
#define CHCI_154_LEN_TEST_GET_BB_STATS          0        /*!< TBD */
#define CHCI_154_LEN_TEST_GET_BB_STATS_CNF      60       /*!< TBD. Vendor specific? */
#define CHCI_154_LEN_TEST_CONTINUOUS_STOP       0        /*!< TBD */
#define CHCI_154_LEN_TEST_CONTINUOUS_TX         4        /*!< TBD */
#define CHCI_154_LEN_TEST_CONTINUOUS_RX         2        /*!< TBD */
#define CHCI_154_LEN_TEST_SET_NET_PARAMS_MIN    12       /*!< TBD */
#define CHCI_154_LEN_TEST_SET_NET_PARAMS_CNF    1        /*!< TBD */
#define CHCI_154_LEN_TEST_GET_PKT_STATS_CNF     16       /*!< TBD */
#define CHCI_154_LEN_TEST_GET_LAST_RSSI_CNF     2        /*!< TBD */
#define CHCI_154_LEN_TEST_TX                    22       /*!< TBD */
#define CHCI_154_LEN_TEST_RX                    5        /*!< TBD */
#define CHCI_154_LEN_TEST_END                   0        /*!< TBD */
#define CHCI_154_LEN_TEST_END_IND               16       /*!< TBD */

/*! brief       Command: CHCI host to controller */
#define CHCI_154_CMD_MLME_ASSOC_REQ             0x01     /*!< TBD */
#define CHCI_154_CMD_MLME_ASSOC_RSP             0x02     /*!< TBD */
#define CHCI_154_CMD_MLME_DISASSOC_REQ          0x03     /*!< TBD */
#define CHCI_154_CMD_MLME_GET_REQ               0x04     /*!< TBD */
#define CHCI_154_CMD_MLME_ORPHAN_RSP            0x05     /*!< TBD */
#define CHCI_154_CMD_MLME_RESET_REQ             0x06     /*!< TBD */
#define CHCI_154_CMD_MLME_RX_ENABLE_REQ         0x07     /*!< TBD */
#define CHCI_154_CMD_MLME_SCAN_REQ              0x08     /*!< TBD */
#define CHCI_154_CMD_MLME_SET_REQ               0x09     /*!< TBD */
#define CHCI_154_CMD_MLME_START_REQ             0x0A     /*!< TBD */
#define CHCI_154_CMD_MLME_POLL_REQ              0x0B     /*!< TBD */
#define CHCI_154_CMD_MCPS_PURGE_REQ             0x0C     /*!< TBD */
#define CHCI_154_CMD_VS_DIAG_CFG_REQ            0xA1     /*!< TBD */

/*! brief       Command: CHCI host to controller - 802.15.4-2020z */
#define CHCI_154_CMD_MLME_CALIBRATE_REQ         0x11     /*!< TBD */
#define CHCI_154_CMD_MLME_SOUNDING_REQ          0x12     /*!< TBD */
#define CHCI_154_CMD_MLME_STS_REQ               0x13     /*!< TBD */
#define CHCI_154_CMD_MLME_DPS_REQ               0x14     /*!< TBD */
#define CHCI_154_CMD_MCPS_ACRR_VERIFIER_REQ     0x15     /*!< TBD */
#define CHCI_154_CMD_MCPS_ACRR_PROVER_REQ       0x16     /*!< TBD */
#define CHCI_154_CMD_MLME_RIT_RES_REQ           0x17     /*!< TBD */
#define CHCI_154_CMD_MLME_EN_RX_ENABLE_REQ      0x18     /*!< TBD */

/*! brief       Event: CHCI controller to host */
#define CHCI_154_EVT_MLME_ASSOC_CFM             0x81     /*!< TBD */
#define CHCI_154_EVT_MLME_ASSOC_IND             0x82     /*!< TBD */
#define CHCI_154_EVT_MLME_DISASSOC_CFM          0x83     /*!< TBD */
#define CHCI_154_EVT_MLME_DISASSOC_IND          0x84     /*!< TBD */
#define CHCI_154_EVT_MLME_BEACON_NOTIFY_IND     0x85     /*!< TBD */
#define CHCI_154_EVT_MLME_GET_CFM               0x86     /*!< TBD */
#define CHCI_154_EVT_MLME_ORPHAN_IND            0x87     /*!< TBD */
#define CHCI_154_EVT_MLME_RESET_CFM             0x88     /*!< TBD */
#define CHCI_154_EVT_MLME_RX_ENABLE_CFM         0x89     /*!< TBD */
#define CHCI_154_EVT_MLME_SCAN_CFM              0x8A     /*!< TBD */
#define CHCI_154_EVT_MLME_COMM_STATUS_IND       0x8B     /*!< TBD */
#define CHCI_154_EVT_MLME_SET_CFM               0x8C     /*!< TBD */
#define CHCI_154_EVT_MLME_START_CFM             0x8D     /*!< TBD */
#define CHCI_154_EVT_MLME_POLL_CFM              0x8E     /*!< TBD */
#define CHCI_154_EVT_MCPS_PURGE_CFM             0x8F     /*!< TBD */
#define CHCI_154_EVT_MCPS_DATA_CFM              0x90     /*!< TBD */
#define CHCI_154_EVT_MLME_POLL_IND              0x91     /*!< TBD */
#define CHCI_154_EVT_VS_DIAG_IND                0xA0     /*!< TBD */
#define CHCI_154_EVT_VS_TOKEN_IND               0xA1     /*!< TBD */

/*! brief       Event: CHCI controller to host - 802.15.4-2020z */
#define CHCI_154_EVT_MLME_CALIBRATE_CNF         0x92     /*!< TBD */
#define CHCI_154_EVT_MLME_SOUNDING_CNF          0x93     /*!< TBD */
#define CHCI_154_EVT_MLME_STS_CNF               0x94     /*!< TBD */
#define CHCI_154_EVT_MLME_DPS_CNF               0x95     /*!< TBD */
#define CHCI_154_EVT_MLME_DPS_IND               0x96     /*!< TBD */
#define CHCI_154_EVT_MCPS_ACRR_VERIFIER_CNF     0x97     /*!< TBD */
#define CHCI_154_EVT_MCPS_ACRR_VERIFIER_IND     0x98     /*!< TBD */
#define CHCI_154_EVT_MCPS_ACRR_PROVER_CNF       0x99     /*!< TBD */
#define CHCI_154_EVT_MCPS_ACRR_PROVER_IND       0x9A     /*!< TBD */
#define CHCI_154_EVT_MLME_RX_ENABLE_IND         0x9B     /*!< TBD */
#define CHCI_154_EVT_MLME_RIT_RES_IND           0x9C     /*!< TBD */

/*! brief       Command: User-Defined CHCI host to controller */
#define CHCI_154_CMD_USER_DEF_00                0xB0     /*!< TBD */

/*! brief       Event: User-Defined CHCI controller to host */
#define CHCI_154_EVT_USER_DEF_00                0xD0     /*!< TBD */

/*! brief       Data: CHCI host to controller */
#define CHCI_154_DATA_MCPS_DATA_REQ             0x01     /*!< TBD */

/*! brief       Data: CHCI controller to host */
#define CHCI_154_DATA_MCPS_DATA_IND             0x81     /*!< TBD */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      HCI header */
typedef struct
{
  uint8_t  code;                    /*!< Code field. */
  uint16_t len;                     /*!< Parameter length. */
} Chci154Hdr_t;

/*! \brief      Command handler call signature. */
typedef bool (*Chci154CmdHandler_t)(Chci154Hdr_t *pHdr, uint8_t *pBuf);

/*! \brief      Data handler call signature. */
typedef void (*Chci154DataHandler_t)(Chci154Hdr_t *pHdr, uint8_t *pBuf);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize 802.15.4 controller HCI handler.
 */
/*************************************************************************************************/
void Chci154HandlerInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Register a command handler.
 *
 *  \param      cmdHandler  Command handler.
 */
/*************************************************************************************************/
void Chci154RegisterCmdHandler(Chci154CmdHandler_t cmdHandler);

/*************************************************************************************************/
/*!
 *  \brief      Invoke a command handler.
 *
 *  \param      pHdr  Command header.
 *  \param      pBuf  Command buffer.
 *
 *  \return     TBD.
 */
/*************************************************************************************************/
bool Chci154InvokeCmdHandler(Chci154Hdr_t *pHdr, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Register data handler.
 *
 *  \param      dataHandler  Data handler.
 */
/*************************************************************************************************/
void Chci154RegisterDataHandler(Chci154DataHandler_t dataHandler);

/*************************************************************************************************/
/*!
 *  \brief      Send an event and service the event queue.
 *
 *  \param      pBuf        Buffer containing event to send or NULL to service the queue.
 */
/*************************************************************************************************/
void Chci154SendEvent(uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Send data and service the data queue.
 *
 *  \param      pBuf        Buffer containing data to send or NULL to service the queue.
 */
/*************************************************************************************************/
void Chci154SendData(uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for test operations.
 */
/*************************************************************************************************/
void Chci154TestInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Send test end indication.
 *
 *  \param      pktCount    Packet count.
 *  \param      pktErrCount Packet error count.
 *  \param      ackCount    ACK count.
 *  \param      ackErrCount ACK error count.
 */
/*************************************************************************************************/
void Chci154TestSendTestEndInd(uint32_t pktCount,
                               uint32_t pktErrCount,
                               uint32_t ackCount,
                               uint32_t ackErrCount);

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for VS operations.
 */
/*************************************************************************************************/
void Chci154VsInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for association operations.
 */
/*************************************************************************************************/
void Chci154AssocInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Send associate confirm.
 *
 *  \param      assocShtAddr  Associated short address.
 *  \param      status        Association status.
 */
/*************************************************************************************************/
void Chci154AssocSendAssocCfm(uint16_t assocShtAddr, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Send associate indication.
 *
 *  \param      deviceAddr  Device address.
 *  \param      capInfo     Capability information.
 */
/*************************************************************************************************/
void Chci154AssocSendAssocInd(uint64a_t deviceAddr, uint8_t capInfo);

#if MAC_154_OPT_DISASSOC

/*************************************************************************************************/
/*!
 *  \brief      Send disassociate indication.
 *
 *  \param      deviceAddr    Address of device disassociating.
 *  \param      reason        Disassociation reason.
 */
/*************************************************************************************************/
void Chci154AssocSendDisassocInd(uint64a_t deviceAddr, uint8_t reason);

/*************************************************************************************************/
/*!
 *  \brief      Send disassociate confirm.
 *
 *  \param      pDeviceAddr  Address disassoc. notification was sent to.
 *  \param      status       Disassociation status.
 */
/*************************************************************************************************/
void Chci154AssocSendDisassocCfm(Mac154Addr_t *pDeviceAddr, uint8_t status);

#endif /* MAC_154_OPT_DISASSOC */

#if MAC_154_OPT_ORPHAN

/*************************************************************************************************/
/*!
 *  \brief      Send orphan indication.
 *
 *  \param      orphanAddr    Address of orphaned device.
 */
/*************************************************************************************************/
void Chci154AssocSendOrphanInd(uint64a_t orphanAddr);

#endif /* MAC_154_OPT_ORPHAN */

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for scan operations.
 */
/*************************************************************************************************/
void Chci154ScanInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Send scan confirm.
 *
 *  \param      channels     Unscanned channels.
 *  \param      type         Scan type.
 *  \param      listSize     Results list size.
 *  \param      pScanResults TBD.
 */
/*************************************************************************************************/
void Chci154ScanSendCfm(uint32_t channels,
                        uint8_t type,
                        uint8_t listSize,
                        Mac154ScanResults_t *pScanResults,
                        uint8_t statusOverride);

/*************************************************************************************************/
/*!
 *  \brief      Send scan beacon notify indication.
 *
 *  \param      bsn         Beacon sequence number.
 *  \param      pPanDescr   Pointer to PAN descriptor.
 *  \param      sduLength   SDU (beacon payload) length.
 *  \param      pSdu        pointer to SDU (beacon payload).
 */
/*************************************************************************************************/
void Chci154ScanSendBeaconNotifyInd(uint8_t bsn,
                                    Mac154PanDescr_t *pPanDescr,
                                    uint8_t sduLength,
                                    uint8_t *pSdu);

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for miscellaneous operations.
 */
/*************************************************************************************************/
void Chci154MiscInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Send reset confirm.
 *
 *  \param      status  Reset status.
 */
/*************************************************************************************************/
void Chci154MiscSendResetCfm(const uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Send rx enable confirm.
 *
 *  \param      status  Rx enable status.
 */
/*************************************************************************************************/
void Chci154MiscSendRxEnableCfm(const uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Send start confirm.
 *
 *  \param      status  Start status.
 */
/*************************************************************************************************/
void Chci154MiscSendStartCfm(const uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for data transmit operations.
 */
/*************************************************************************************************/
void Chci154DataInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Send data transmit confirm.
 *
 *  \param      msduHandle    Handle of corresponding MCPS-DATA.req.
 *  \param      status        Purge status.
 *  \param      timestamp     Timestamp of MSDU.
 */
/*************************************************************************************************/
void Chci154DataTxSendCfm(const uint8_t msduHandle, const uint8_t status, const uint32_t timestamp);

/*************************************************************************************************/
/*!
 *  \brief      Send data poll confirm.
 *
 *  \param      status    Poll status.
 */
/*************************************************************************************************/
void Chci154DataSendPollCfm(const uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Send data poll indication.
 *
 *  \param      srcAddr         Source address.
 *  \param      dataFrameSent   Poll status.
 *
 *  \note       This is a vendor-specific extension.
 */
/*************************************************************************************************/
void Chci154DataSendPollInd(const Mac154Addr_t *srcAddr, const uint8_t dataFrameSent);

/*************************************************************************************************/
/*!
 *  \brief      Send data purge confirm.
 *
 *  \param      msduHandle    Handle of corresponding MCPS-DATA.req.
 *  \param      status        Purge status.
 */
/*************************************************************************************************/
void Chci154DataSendPurgeCfm(const uint8_t msduHandle, const uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Send data indication.
 *
 *  \param      srcAddr           Source address.
 *  \param      dstAddr           Destination address.
 *  \param      mpduLinkQuality   Link quality.
 *  \param      dsn               Data sequence number.
 *  \param      timestamp         Timestamp.
 *  \param      msduLength        MAC SDU length.
 *  \param      pMsdu             Pointer to MAC SDU.
 */
/*************************************************************************************************/
void Chci154DataRxSendInd(const Mac154Addr_t *srcAddr,
                          const Mac154Addr_t *dstAddr,
                          const uint8_t mpduLinkQuality,
                          const uint8_t dsn,
                          const uint32_t timestamp,
                          const uint8_t msduLength,
                          const uint8_t *pMsdu);

/*************************************************************************************************/
/*!
 *  \brief      Send comm status indication.
 *
 *  \param      srcAddr   Source address.
 *  \param      dstAddr   Destination address.
 *  \param      status    Status.
 */
/*************************************************************************************************/
void Chci154DataSendCommStatusInd(const Mac154Addr_t *srcAddr,
                                  const Mac154Addr_t *dstAddr,
                                  const uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for PIB operations.
 */
/*************************************************************************************************/
void Chci154PibInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Send string indication. Will be picked up by test app.
 *
 *  \param      pStr  Pointer to string to send.
 *  \param      len   Length of string.
 */
/*************************************************************************************************/
void Chci154DiagSendStrInd(const char *pStr, uint32_t len);

#if (WSF_TOKEN_ENABLED == true)

/*************************************************************************************************/
/*!
 *  \brief      Send token indication.
 *
 *  \param      pTkn  Pointer to token buffer to send.
 *  \param      len   Length of token buffer.
 *
 *  \return     true if buffer sent.
 */
/*************************************************************************************************/
bool Chci154DiagSendTokenInd(const uint8_t *pTkn, uint32_t len);

#endif /* (WSF_TOKEN_ENABLED == true) */

/*************************************************************************************************/
/*!
 *  \brief      Initialize controller HCI for diagnostic operations.
 */
/*************************************************************************************************/
void Chci154DiagInit(void);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_154_API_H */
