/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Coordinated Set Identification Service implementation.
 *
 *  Copyright (c) 2019-2022 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_CSIS_H
#define SVC_CSIS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup COORDINATED_SET_IDENTIFICATION_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Coordinated Set attribute lengths */
#define CSIS_SIRK_ATT_LEN               17
#define CSIS_SIRK_TYPE_LEN              1
#define CSIS_SIZE_ATT_LEN               1
#define CSIS_LOCK_ATT_LEN               1
#define CSIS_RANK_ATT_LEN               1

/* Coordinated Set Identity Resolving Key length */
#define CSIS_SIRK_LEN                   16

/* Coordinated Set Identity Resolving Key attribute types */
#define CSIS_SIRK_TYPE_ENCRYPTED        0
#define CSIS_SIRK_TYPE_PLAINTEXT        1

/* CSIS Private Set Random Identifier length */
#define CSIS_PSRI_LEN                   6
#define CSIS_PRSI_HASH_LEN              3
#define CSIS_PRSI_RAND_LEN              3

/* CSIS Private Set Random Identifier footer bit masks */
#define CSIS_PRSI_RAND_ZERO_MASK        (0x80)
#define CSIS_PRSI_RAND_ONE_MASK         (0x40)

/* CSIS Lock states */
#define CSIS_UNLOCKED                   0x01
#define CSIS_LOCKED                     0x02

/* CSIS Lock requests */
#define CSIS_UNLOCK                     CSIS_UNLOCKED
#define CSIS_LOCK                       CSIS_LOCKED

/* CSIS lock write response values */
#define CSIS_LOCK_DENIED                0x80
#define CSIS_LOCK_RELEASE_NOT_ALLOWED   0x81
#define CSIS_LOCK_INVALID_VALUE         0x82
#define CSIS_LOCK_OOB_ONLY              0x83
#define CSIS_LOCK_ALREADY_OWNED         0x84

/* CCCD Index Offsets */
#define CSICS_NTF_IDX_SIRK              0
#define CSICS_NTF_IDX_SIZE              1
#define CSICS_NTF_IDX_LOCK              2

/* CCCD Index Offsets */
#define CSICS_CCCD_IDX_SIRK             CSICS_NTF_IDX_SIRK
#define CSICS_CCCD_IDX_SIZE             CSICS_NTF_IDX_SIZE
#define CSICS_CCCD_IDX_LOCK             CSICS_NTF_IDX_LOCK

/* Number of CCCD attributes per service */
#define CSICS_NUM_CCCD                  3

/* Max supported Coordinated Set Identification services for the device. */
/* TODO: A device can have multiple coordinated set identification services.
 *       Modify the max services as needed. */
#define CSIS_SVC_MAX                    2

/* Unknown CSIS service ID */
#define CSIS_SVC_UNKNOWN                0xFF

/* SIRK Types for internal use */
#define CSIS_SIRK_ITYPE_ENCRYPTED       CSIS_SIRK_TYPE_ENCRYPTED
#define CSIS_SIRK_ITYPE_PLAINTEXT       CSIS_SIRK_TYPE_PLAINTEXT
#define CSIS_SIRK_ITYPE_OOB             2
#define CSIS_SIRK_ITYPE_NONE            0xFF

/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Coordinated Set Identification Service Handles
 *
 */
/**@{*/
/*!< \brief Start handle. */
#define CSIS_START_HDL                   0x0880

/*!< \brief Number of CSIS handles. */
#define CSIS_NUM_HDL                     (CSIS_MAX_HDL_0 - CSIS_START_HDL)

/*!< \brief Base value handles. */
#define CSIS_SIRK_HDL                    CSIS_SIRK_HDL_0
#define CSIS_SIZE_HDL                    CSIS_SIZE_HDL_0
#define CSIS_LOCK_HDL                    CSIS_LOCK_HDL_0
#define CSIS_RANK_HDL                    CSIS_RANK_HDL_0

/* Calculate an value handle from a base value handle and a service id. */
#define CSIS_HANDLE_ID(base, index)      (base + CSIS_NUM_HDL * index)

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Coordinated Set Identification Service Handles */
#define CSIS_HANDLE_DEFS(_a, b)  \
enum \
{ \
  CSIS_SVC_HDL_##_a = b,                /*!< \brief Coordinated Set Identification Service declaration */ \
  CSIS_SIRK_CH_HDL_##_a,                /*!< \brief Coordinated Set Identify Resolving Key characteristic */ \
  CSIS_SIRK_HDL_##_a,                   /*!< \brief Coordinated Set Identify Resolving Key value */ \
  CSIS_SIRK_CH_CCC_HDL_##_a,            /*!< \brief Coordinated Set Identify Resolving Key Client Characteristic Configuration Descriptor */ \
  CSIS_SIZE_CH_HDL_##_a,                /*!< \brief Coordinated Set Size characteristic */ \
  CSIS_SIZE_HDL_##_a,                   /*!< \brief Coordinated Set Size value */ \
  CSIS_SIZE_CH_CCC_HDL_##_a,            /*!< \brief Coordinated Set Size Client Characteristic Configuration Descriptor */ \
  CSIS_LOCK_CH_HDL_##_a,                /*!< \brief Coordinated Set Lock characteristic */ \
  CSIS_LOCK_HDL_##_a,                   /*!< \brief Coordinated Set Lock value */ \
  CSIS_LOCK_CH_CCC_HDL_##_a,            /*!< \brief Coordinated Set Lock Client Characteristic Configuration Descriptor */ \
  CSIS_RANK_CH_HDL_##_a,                /*!< \brief Coordinated Set Rank characteristic */ \
  CSIS_RANK_HDL_##_a,                   /*!< \brief Coordinated Set Rank value */ \
  CSIS_MAX_HDL_##_a                     /*!< \brief Maximum handle. */ \
};
/**@}*/

/*! \brief Coordinated Set Identification Service Handles */
/* TODO: A device can have multiple coordinated set identification services.
 *       Add or remove handle definitions as needed. */
CSIS_HANDLE_DEFS(0, CSIS_START_HDL)
CSIS_HANDLE_DEFS(1, CSIS_MAX_HDL_0)

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCsisAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCsisRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  readCback   Read callback function.
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCsisCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* COORDINATED_SET_IDENTIFICATION_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_CSIS_H */
