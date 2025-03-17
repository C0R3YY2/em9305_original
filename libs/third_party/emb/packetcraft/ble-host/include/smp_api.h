/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  SMP subsystem API.
 *
 *  Copyright (c) 2010-2019 Arm Ltd. All Rights Reserved.
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
#ifndef SMP_API_H
#define SMP_API_H

#include "wsf_os.h"
#include "smp_defs.h"
#include "dm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_SMP_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name SMP Events
 * Events recognized and handled by the SMP state machine.
 */
/**@{*/
/*! \brief Event handler messages for SMP state machines */
enum
{
  SMP_MSG_API_PAIR_REQ = 1,               /*!< API pairing request */
  SMP_MSG_API_PAIR_RSP,                   /*!< API pairing response */
  SMP_MSG_API_CANCEL_REQ,                 /*!< API cancel request */
  SMP_MSG_API_AUTH_RSP,                   /*!< API pin response */
  SMP_MSG_API_SECURITY_REQ,               /*!< API security request */
  SMP_MSG_CMD_PKT,                        /*!< SMP command packet received */
  SMP_MSG_CMD_PAIRING_FAILED,             /*!< SMP pairing failed packet received */
  SMP_MSG_DM_ENCRYPT_CMPL,                /*!< Link encrypted */
  SMP_MSG_DM_ENCRYPT_FAILED,              /*!< Link encryption failed */
  SMP_MSG_DM_CONN_CLOSE,                  /*!< Connection closed */
  SMP_MSG_WSF_AES_CMPL,                   /*!< AES calculation complete */
  SMP_MSG_INT_SEND_NEXT_KEY,              /*!< Send next key to be distributed */
  SMP_MSG_INT_MAX_ATTEMPTS,               /*!< Maximum pairing attempts reached */
  SMP_MSG_INT_PAIRING_CMPL,               /*!< Pairing complete */
  SMP_MSG_INT_RSP_TIMEOUT,                /*!< Pairing protocol response timeout */
  SMP_MSG_INT_WI_TIMEOUT,                 /*!< Pairing protocol wait interval timeout */
  SMP_MSG_INT_LESC,                       /*!< Pair with Secure Connections */
  SMP_MSG_INT_LEGACY,                     /*!< Pair with Legacy Security */
  SMP_MSG_INT_JW_NC,                      /*!< LESC Just-Works/Numeric Comparison pairing */
  SMP_MSG_INT_PASSKEY,                    /*!< LESC Passkey pairing */
  SMP_MSG_INT_OOB,                        /*!< LESC Out-of-Band Pairing */
  SMP_MSG_API_USER_CONFIRM,               /*!< User confirms valid numeric comparison */
  SMP_MSG_API_USER_KEYPRESS,              /*!< User keypress in passkey pairing */
  SMP_MSG_API_KEYPRESS_CMPL,              /*!< User keypress complete in passkey pairing */
  SMP_MSG_WSF_ECC_CMPL,                   /*!< WSF ECC operation complete */
  SMP_MSG_INT_PK_NEXT,                    /*!< Continue to next passkey bit */
  SMP_MSG_INT_PK_CMPL,                    /*!< Passkey operation complete */
  SMP_MSG_WSF_CMAC_CMPL,                  /*!< WSF CMAC operation complete */
  SMP_MSG_DH_CHECK_FAILURE,               /*!< DHKey check failure */
  SMP_MSG_EARLY_CNF,                      /*!< An early Confirm from the initiator in passkey pairing */
  SMP_MSG_INT_CLEANUP,                    /*!< Cleanup control information and return to IDLE state */
  SMP_NUM_MSGS                            /*!< Number of SMP message types. */
};
/**@}*/

/**@{*/
/*! \brief Additional SMP messages */
enum
{
  SMP_DB_SERVICE_IND = SMP_NUM_MSGS       /*!< SMP DB Service timer indication */
};
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Configurable parameters */
typedef struct
{
  uint32_t            attemptTimeout;     /*!< 'Repeated attempts' timeout in msec */
  uint8_t             ioCap;              /*!< I/O Capability */
  uint8_t             minKeyLen;          /*!< Minimum encryption key length */
  uint8_t             maxKeyLen;          /*!< Maximum encryption key length */
  uint8_t             maxAttempts;        /*!< Attempts to trigger 'repeated attempts' timeout */
  uint8_t             auth;               /*!< Device authentication requirements */
  uint32_t            maxAttemptTimeout;  /*!< Maximum 'Repeated attempts' timeout in msec */
  uint32_t            attemptDecTimeout;  /*!< Time msec before attemptExp decreases */
  uint16_t            attemptExp;         /*!< Exponent to raise attemptTimeout on maxAttempts */
} smpCfg_t;

/*! \brief Data type for SMP_MSG_API_PAIR_REQ and SMP_MSG_API_PAIR_RSP */
typedef struct
{
  wsfMsgHdr_t         hdr;        /*!< Message header */
  uint8_t             oob;        /*!< Out-of-band data present flag */
  uint8_t             auth;       /*!< authentication flags */
  uint8_t             iKeyDist;   /*!< Initiator key distribution flags */
  uint8_t             rKeyDist;   /*!< Responder key distribution flags */
} smpDmPair_t;

/*! \brief Data type for SMP_MSG_API_AUTH_RSP */
typedef struct
{
  wsfMsgHdr_t         hdr;                    /*!< Message header */
  uint8_t             authData[SMP_OOB_LEN];  /*!< Authentication data to display */
  uint8_t             authDataLen;            /*!< Length of authentication data */
} smpDmAuthRsp_t;

/*! \brief Data type for SMP_MSG_API_USER_KEYPRESS */
typedef struct
{
  wsfMsgHdr_t         hdr;        /*!< Message header */
  uint8_t             keypress;   /*!< Keypress */
} smpDmKeypress_t;

/*! \brief Data type for SMP_MSG_API_SECURITY_REQ */
typedef struct
{
  wsfMsgHdr_t         hdr;   /*!< Message header */
  uint8_t             auth;  /*!< Authentication flags */
} smpDmSecurityReq_t;

/*! \brief Union SMP DM message data types */
typedef union
{
  wsfMsgHdr_t         hdr;           /*!< Message header */
  smpDmPair_t         pair;          /*!< Pairing request/response message */
  smpDmAuthRsp_t      authRsp;       /*!< Authentication message */
  smpDmSecurityReq_t  securityReq;   /*!< Security Request message */
  smpDmKeypress_t     keypress;      /*!< Keypress message */
} smpDmMsg_t;

/*! \} */    /* STACK_SMP_API */

/**************************************************************************************************
  Global Variables;
**************************************************************************************************/

/*! \addtogroup STACK_INIT
 *  \{ */

/** \name SMP Configuration Structure
 * Pointer to structure containing initialization details of the SMP Subsystem.  To be configured
 * by Application.
 */
/**@{*/
/*! \brief Configuration pointer */
extern smpCfg_t *pSmpCfg;
/**@}*/

/*! \} */    /* STACK_INIT */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*! \addtogroup STACK_SMP_API
 *  \{ */

/** \name SMP Initialization Functions
 * Legacy and Secure Connections initialization for Initiator and Responder roles.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpiInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP responder role.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmprInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP initiator role utilizing BTLE Secure Connections.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpiScInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize SMP responder role utilizing BTLE Secure Connections.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmprScInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Use this SMP init function when SMP is not supported.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpNonInit(void);

/**@}*/

/** \name SMP DM Interface Functions
 * Functions that allow the DM to send messages to SMP.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to send a message to SMP.
 *
 *  \param  pMsg      Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDmMsgSend(smpDmMsg_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by DM to notify SMP of encrypted link status.
 *
 *  \param  pMsg    Pointer to HCI message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDmEncryptInd(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Check if LE Secure Connections is enabled on the connection.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return true is Secure Connections is enabled, else false
 */
/*************************************************************************************************/
bool SmpDmLescEnabled(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Return the STK for the given connection.
 *
 *  \param  connId    Connection identifier.
 *  \param  pSecLevel Returns the security level of pairing when STK was created.
 *
 *  \return Pointer to STK or NULL if not available.
 */
/*************************************************************************************************/
uint8_t *SmpDmGetStk(dmConnId_t connId, uint8_t *pSecLevel);

/*************************************************************************************************/
/*!
 *  \brief  Format a cancel message with consideration for the attempts counter
 *
 *  \param  connId  Connection Id.
 *  \param  pHdr    Pointer to header of message to fill.
 *  \param  status  Status to include.
 *
 *  \return none.
 */
/*************************************************************************************************/
void SmpScGetCancelMsgWithReattempt(dmConnId_t connId, wsfMsgHdr_t *pHdr, uint8_t status);

/*************************************************************************************************/
/*!
 *  \brief  Initialize the SMP Database.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpDbInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called to force the DhKey to zero for qualification test purposes.
 *
 *  \param  enable  true - Force DhKey to zero.  false - Use calculated key
 *
 *  \return None.
 */
/*************************************************************************************************/
void SmpScEnableZeroDhKey(bool enable);

/**@}*/

/*! \} */    /* STACK_SMP_API */

#ifdef __cplusplus
};
#endif

#endif /* SMP_API_H */
