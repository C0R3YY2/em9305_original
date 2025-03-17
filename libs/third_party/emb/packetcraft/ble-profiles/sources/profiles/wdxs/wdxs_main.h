/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Wireless Data Exchange profile implementation.
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

#ifndef WDXS_MAIN_H
#define WDXS_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WIRELESS_DATA_EXCHANGE_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name WDXS Default Feature Set
 *
 */
/**@{*/
#ifndef WDXS_DC_ENABLED
#define WDXS_DC_ENABLED             true
#endif

#ifndef WDXS_AU_ENABLED
#define WDXS_AU_ENABLED             true
#endif

#ifndef WDXS_OTA_ENABLED
#define WDXS_OTA_ENABLED            true
#endif
/**@}*/

/*! \brief Special length for streaming file */
#define WDXS_STREAM_FILE_LEN        0xFFFFFFFF

/** \name WSF event types for application event handler
 *
 */
/**@{*/
#define WDXS_EVT_TX_PATH            0x01      /*!< Trigger tx data path */
#define WDXS_EVT_AU_SEC_COMPLETE    0x02      /*!< AU encryption of challenge ready */
/**@}*/

/** \name TX Ready Mask Bits
 *
 */
/**@{*/
#define WDXS_TX_MASK_READY_BIT    (1<<0) /*!< Ready bit */
#define WDXS_TX_MASK_DC_BIT       (1<<1) /*!< DC bit */
#define WDXS_TX_MASK_FTC_BIT      (1<<2) /*!< FTC bit */
#define WDXS_TX_MASK_FTD_BIT      (1<<3) /*!< FTD bit */
#define WDXS_TX_MASK_AU_BIT       (1<<4) /*!< AU bit */
/**@}*/

/** \name Authentication states
 *
 */
/**@{*/
#define WDXS_AU_STATE_UNAUTHORIZED      0x00        /*!< Authentication has not started */
#define WDXS_AU_STATE_HASHING           0x01        /*!< Authentication hash is being calculated */
#define WDXS_AU_STATE_WAIT_SEC          0x02        /*!< Authentication challenge sent */
#define WDXS_AU_STATE_WAIT_REPLY        0x03        /*!< Authentication waiting for challenge reply */
#define WDXS_AU_STATE_AUTHORIZED        0x04        /*!< Authentication completed successfully */
/**@}*/

/** \name Actions to take after a connection closes
 *
 */
/**@{*/
#define WDXS_ON_CLOSE_NONE              0x00        /*!< Take no action on conn close */
#define WDXS_ON_CLOSE_REBOOT            0x01        /*!< Reboot on conn close */
#define WDXS_ON_CLOSE_CFG_FOTA          0x02        /*!< Configure FOTA on conn close */
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief WDXS Device Configuration PHY Write Callback */
typedef uint8_t (*wdxsDcPhyWriteCback_t)(dmConnId_t connId, uint8_t op, uint8_t id, uint16_t len,
                                         uint8_t *pValue);

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! \brief WDXS response pending information */
typedef struct
{
  dmConnId_t        connId;           /*!< Connection ID */
  uint8_t           op;               /*!< Operation of response pending */
  uint16_t          handle;           /*!< File handle of response pending */
  uint8_t           status;           /*!< Status of response pending */
} wdxsRspPending_t;

/*! \brief WDXS profile control block */
typedef struct
{
  wsfHandlerId_t    handlerId;        /*!< WSF handler ID */
  uint8_t           txReadyMask;      /*!< Bits indicate DC, FTC, FTD, and/or AU wish to transmit */
  wdxsRspPending_t  rspPending;       /*!< Response pending filesystem callback */

  /* connection parameters */
  uint16_t          connInterval;     /*!< connection interval */
  uint16_t          connLatency;      /*!< connection latency */
  uint16_t          supTimeout;       /*!< supervision timeout */

  /* Phy parameters */
  uint8_t           txPhy;            /*!< transmitter PHY */
  uint8_t           rxPhy;            /*!< receiver PHY */

  /* for file transfer */
  uint32_t          ftOffset;         /*!< file data offset */
  uint32_t          ftLen;            /*!< remaining data length for current operation */
  uint32_t          ftTotalLen;       /*!< file total length */
  uint16_t          ftHandle;         /*!< file handle */
  uint16_t          ftcMsgLen;        /*!< message length */
  uint8_t           ftcMsgBuf[ATT_DEFAULT_PAYLOAD_LEN]; /*!< message buffer */
  uint8_t           ftInProgress;     /*!< operation in progress */
  uint8_t           ftPrefXferType;   /*!< Preferred transport type */

  WdxsFtcCallback_t *pFtcCallback;    /*!< File Transfer Control Application Callback */

  /* ccc index */
  uint8_t           dcCccIdx;         /*!< device configuration ccc index */
  uint8_t           auCccIdx;         /*!< authentication ccc index */
  uint8_t           ftcCccIdx;        /*!< file transfer control ccc index */
  uint8_t           ftdCccIdx;        /*!< file transfer data ccc index */
} wdxsCb_t;

/*! \brief WDXS Device Configuration Control Block */
typedef struct
{
  uint16_t          dcMsgLen;                           /*!< message length */
  uint8_t           dcMsgBuf[ATT_DEFAULT_PAYLOAD_LEN];  /*!< message buffer */
  uint8_t           postCloseAction;                    /*!< Action to take after a connection close */
  wdxsDcPhyWriteCback_t phyWriteCback;                  /*!< Device config PHY write callback */
} wdxsDcCb_t;

/*! \brief WDXS Authentication Control Block */
typedef struct
{
  uint8_t           auMsgBuf[ATT_DEFAULT_PAYLOAD_LEN];  /*!< message buffer */
  uint8_t           auRand[WDX_AU_RAND_LEN];            /*!< random challenge */
  uint8_t           sessionKey[WDX_AU_KEY_LEN];         /*!< session key */
  uint8_t           auHash[WDX_AU_HASH_LEN];            /*!< session key */
  uint16_t          auMsgLen;                           /*!< message length */
  uint8_t           authLevel;                          /*!< current authentication level */
  uint8_t           authMode;                           /*!< current authentication mode */
  uint8_t           reqAuthLevel;                       /*!< requested authentication level */
  uint8_t           authState;                          /*!< authentication protocol state */
} wdxsAuCb_t;

/*! \brief WDXS event message union */
typedef union
{
  wsfMsgHdr_t       hdr; /*!< header */
  dmEvt_t           dm;  /*!< DM event */
  attsCccEvt_t      ccc; /*!< ATT CCC event */
} wdxsMsg_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/** \name WDXS Control Block External Declaration
 *
 */
/**@{*/
extern wdxsCb_t wdxsCb;     /*!< WDXS control block */
extern wdxsAuCb_t wdxsAuCb; /*!< WDXS AU control block */
extern wdxsDcCb_t wdxsDcCb; /*!< WDXS DC control block */
/**@}*/

/**************************************************************************************************
  Global Function Prototypes
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Send device configuration notification
 *
 *  \param  connId   DM connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wdxsDcSend(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Send a file transfer control characteristic notification.
 *
 *  \param  connId   DM connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wdxsFtcSend(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Send a file transfer data characteristic notification.
 *
 *  \param  connId   DM connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wdxsFtdSend(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Transmit to authentication characteristic.
 *
 *  \param  connId   DM connection identifier.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
void wdxsAuSend(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Process a write to the device configuration characteristic.
 *
 *  \param  connId   DM connection identifier.
 *  \param  len      Length to write.
 *  \param  pValue   value to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t wdxsDcWrite(dmConnId_t connId, uint16_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Process a write to the file transfer control characteristic.
 *
 *  \param  connId   DM connection identifier.
 *  \param  len      Length to write.
 *  \param  pValue   Value to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t wdxsFtcWrite(dmConnId_t connId, uint16_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Process a write to the file transfer data characteristic.
 *
 *  \param  connId   DM connection identifier.
 *  \param  len      Length to write.
 *  \param  pValue   Value to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t wdxsFtdWrite(dmConnId_t connId, uint16_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Process a write to the authentication characteristic.
 *
 *  \param  connId   DM connection identifier.
 *  \param  len      Length to write.
 *  \param  pValue   Value to write
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t wdxsAuWrite(dmConnId_t connId, uint16_t len, uint8_t *pValue);

/*************************************************************************************************/
/*!
 *  \brief  Send update message for connection parameters.
 *
 *  \param  connId   DM connection identifier.
 *  \param  status   Update status.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t wdxsDcUpdateConnParam(dmConnId_t connId, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Send update message for PHY.
 *
 *  \param  connId   DM connection identifier.
 *  \param  status   Update status.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t wdxsDcUpdatePhy(dmConnId_t connId, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Register a PHY write callback for the device configuration characteristic.
 *
 *  \param  cback  PHY callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void wdxsDcPhyRegister(wdxsDcPhyWriteCback_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Create the file list.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WdxsUpdateListing(void);

/*! \} */    /* WIRELESS_DATA_EXCHANGE_PROFILE */

#ifdef __cplusplus
}
#endif

#endif /* WDXS_MAIN_H */
