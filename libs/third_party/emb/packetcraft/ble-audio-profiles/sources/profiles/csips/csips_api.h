/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Coordinated Set Identification Profile server.
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
#ifndef CSIPS_API_H
#define CSIPS_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup COORDINATED_SET_IDENTIFICATION_PROFILE_SERVER
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#if 0
/*! \brief Audio Input Control Service Handles macro */
#define CSIPC_CSIS_DISC_HANDLES(_a, _b)                                                           \
enum                                                                                              \
{                                                                                                 \
  CSIPC_CSIS_SIRK_HDL_IDX_##_a=_b,   /*!< \brief Coordinated Set SIRK */                          \
  CSIPC_CSIS_SIRK_CCC_HDL_IDX_##_a,  /*!< \brief Coordinated Set SIRK CCC */                      \
  CSIPC_CSIS_SIZE_HDL_IDX_##_a=_b,   /*!< \brief Coordinated Set Size */                          \
  CSIPC_CSIS_SIZE_CCC_HDL_IDX_##_a,  /*!< \brief Coordinated Set Size CCC */                      \
  CSIPC_CSIS_LOCK_HDL_IDX_##_a=_b,   /*!< \brief Coordinated Set Lock */                          \
  CSIPC_CSIS_LOCK_CCC_HDL_IDX_##_a,  /*!< \brief Coordinated Set Lock CCC */                      \
  CSIPC_CSIS_RANK_HDL_IDX_##_a=_b,   /*!< \brief Coordinated Set Rank */                          \
  CSIPC_CSIS_HDL_LIST_LEN_##_a,      /*!< \brief Handle list length */                            \
};

/* CSIS service handle sub-index */
#define CSIPC_CSIS_SIRK_HDL_IDX           CSIPC_CSIS_SIRK_HDL_IDX_0
#define CSIPC_CSIS_SIRK_CCC_HDL_IDX       CSIPC_CSIS_SIRK_CCC_HDL_IDX_0
#define CSIPC_CSIS_SIZE_HDL_IDX           CSIPC_CSIS_SIZE_HDL_IDX_0
#define CSIPC_CSIS_SIZE_CCC_HDL_IDX       CSIPC_CSIS_SIZE_CCC_HDL_IDX_0
#define CSIPC_CSIS_LOCK_HDL_IDX           CSIPC_CSIS_LOCK_HDL_IDX_0
#define CSIPC_CSIS_LOCK_CCC_HDL_IDX       CSIPC_CSIS_LOCK_CCC_HDL_IDX_0
#define CSIPC_CSIS_RANK_HDL_IDX           CSIPC_CSIS_RANK_HDL_IDX_0

/*! \brief Enumeration of handle indexes of characteristics to be discovered - CSIS */
/* TODO: A device can have multiple CSIS services.
 *       Add service handle indicies as needed. */
CSIPC_CSIS_DISC_HANDLES(0, 0);
CSIPC_CSIS_DISC_HANDLES(1, CSIPC_CSIS_HDL_LIST_LEN_0);
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! CSIPS configuration */
typedef struct
{
  uint8_t         cccIdxOffset;     /* Starting index of CCC descriptors used by the CSIPS */
  wsfHandlerId_t  handlerId;        /* Application handler ID */
  uint8_t         lockTimerEvt;     /* Event ID for lock timer */
  bool            oobOnly;          /* true if only using OOB SIRK */
  uint8_t         encSirkEvt;       /* Event ID for encryption of SIRK */
} csipsCfg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Generate a Private Set Random Identifier.
 *
 *  \param  svcId       Service Identifier
 *  \param  handlerId   Handler Identifier
 *  \param  event       AES complete event
 *
 *  \return none.
 *
 *  \note   After the application reveives the AES complete event, the application should call
 *          CsipsGenPrivateSetRandomIdComplete to complete the calculation and return the PSRI.
 */
/*************************************************************************************************/
void CsipsGenPrivateSetRandomId(uint8_t svcId, wsfHandlerId_t handlerId, uint8_t event);

/*************************************************************************************************/
/*!
 *  \brief  Complete the process of generating a Private Set Random Identifier.
 *
 *  \param  svcId       Service Identifier
 *  \param  pAes        The result of the SecAes operation performed in CsipsGenPrivateSetRandomId.
 *
 *  \return A pointer to the PSRI.
 */
/*************************************************************************************************/
uint8_t *CsipsGenPrivateSetRandomIdComplete(uint8_t svcId, secAes_t *pAes);

/*************************************************************************************************/
/*!
 *  \brief  Set the Coordinated Set Identity Resolving Key attribute value.
 *
 *  \param  svcId       Service Identifier
 *  \param  type        Key encrytion state (CSIS_KEY_TYPE_ENC or CSIS_SIRK_TYPE_PLAINTEXT)
 *  \param  pSirk       Set Identity Resolving Key
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipsSetSirk(uint8_t svcId, uint8_t type, uint8_t *pSirk);

/*************************************************************************************************/
/*!
 *  \brief  Set the size attribute value.
 *
 *  \param  svcId       Service Identifier
 *  \param  size        Size attribute value
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipsSetSize(uint8_t svcId, uint8_t size);

/*************************************************************************************************/
/*!
 *  \brief  Set the rank attribute value.
 *
 *  \param  svcId       Service Identifier
 *  \param  rank        Rank attribute value
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipsSetRank(uint8_t svcId, uint8_t rank);

/*************************************************************************************************/
/*!
 *  \brief  Set the lock attribute value.
 *
 *  \param  svcId       Service Identifier
 *  \param  lock        Lock attribute value
 *
 *  \return none.
 */
/*************************************************************************************************/
void CsipsSetLock(uint8_t svcId, uint8_t lock);

/*************************************************************************************************/
/*!
 *  \brief  Set the service configuration for a service ID.
 *
 *  \param  svcId   Service Identifier
 *  \param  pCfg    Pointer to configuration structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipsSetCfg(uint8_t svcId, const csipsCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  Coordinated Set Identification Profile server initialization.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipsInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the Coordinated Set Identification Profile server of DM Events.
 *
 *  \param  pEvt        Pointer to the DM Event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipsProcDmMsg(dmEvt_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the CSIPS server of ATT messages.
 *
 *  \param  pMsg        Message containing event.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CsipsProcAttMsg(wsfMsgHdr_t *pMsg);

/*! \} */    /* COORDINATED_SET_IDENTIFICATION_PROFILE_SERVER */

#ifdef __cplusplus
};
#endif

#endif /* CSIPS_API_H */
