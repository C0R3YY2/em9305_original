/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Broadcast Audio Scan (BAS) service implementations.
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

#ifndef SVC_BAS_H
#define SVC_BAS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup BAS_SERVICE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*!< \brief Maximum number of Broadcast Receive State (BRS) characteristics supported by the server.
 *
 *   \note The server should expose a number of Broadcast Receive State characteristics at least
 *         equal to the number of BIGs that the server can simultaneously maintain synchronization
 *         to.
 *
 *   \note When adjusting the maximum number of Broadcast Receive State, the number of Broadcast
 *         Receive State (BRS) characteristics defined in the BAS service must be adjusted accordingly.
 */
#define BASS_BCAST_RX_STATE_MAX             2

/** \name BASS Maximum number of BIGes
 * BASS Maximum number of BIG groups supported
 */
/**@{*/
#ifndef BASS_GRP_MAX
#define BASS_GRP_MAX                        2
#endif
/**@}*/

/** \name BASS Number of subgroups
 * BASS Number of subgroups supported per BIG group
 */
/**@{*/
#ifndef BASS_SUBGRP_MAX
#define BASS_SUBGRP_MAX                     2
#endif
/**@}*/

/** \name BASS Number of BISes
 * BASS Number of BISes supported per subgroup
 */
/**@{*/
#ifndef BASS_BIS_MAX
#define BASS_BIS_MAX                        2
#endif

/** \name Broadcast Audio Scan (BAS) Control Point Characteristic Length Fields
* Length constant of BAS Control Point characteristic common and fixed length field
*/
/**@{*/
#define BASS_BAS_CP_FIX_LEN                 1     /*!< \brief BAS CP fixed field length */
#define BASS_BAS_CP_ADD_SRC_FIX_LEN         15    /*!< \brief BAS CP Add Source fixed field length */
#define BASS_BAS_CP_MOD_SRC_FIX_LEN         5     /*!< \brief BAS CP Modify Source fixed field length */
#define BASS_BAS_CP_SET_BC_CODE_FIX_LEN     17    /*!< \brief BAS CP Set Broadcast_Code fixed field length */
#define BASS_BAS_CP_REM_SRC_FIX_LEN         1     /*!< \brief BAS CP Remove Source fixed field length */
#define BASS_BAS_CP_SUBGRP_INFO_FIX_LEN     5     /*!< \brief BAS CP subgroup info fixed field length */
/**@}*/

/** \name Broadcast Audio Scan (BAS) Control Point Opcode Length Fields
* Length constants of BAS opcodes fixed length fields
*/
/**@{*/
#define BASS_BAS_CP_RMT_SCAN_LEN            0
#define BASS_BAS_CP_ADD_SRC_LEN             (BASS_BAS_CP_ADD_SRC_FIX_LEN + GA_MAX_METADATA_LEN)
#define BASS_BAS_CP_MOD_SRC_LEN             (BASS_BAS_CP_MOD_SRC_FIX_LEN + GA_MAX_METADATA_LEN)
#define BASS_BAS_CP_SET_BC_CODE_LEN         BASS_BAS_CP_SET_BC_CODE_FIX_LEN
#define BASS_BAS_CP_REM_SRC_LEN             BASS_BAS_CP_REM_SRC_FIX_LEN
/**@}*/

/** \name Broadcast Receive State Characteristic Length Fields
* Length constant of Broadcast Receive State characteristic fixed length fields
*/
/**@{*/
#define BASS_BRS_COMMON_LEN                 15    /*!< \brief BAS Broadcast Receive State common field length */
/**@}*/

/**@}*/

/*!< \brief Maximum length of the Broadcast Audio Scan Control Point characteristic value */
#define BASS_MAX_BAS_CP_LEN                 251

/*!< \brief Maximum length of the Broadcast Receive State characteristic value */
#define BASS_MAX_BRS_LEN                    251

/*!< \brief Length of the BIS Sync State field in the Broadcast Receive State characteristic value */
#define BASS_BIS_SYNC_STATE_LEN             4

/*!< \brief Length of the Broadcast Audio Scan Control Point characteristic value */
#define BASS_BAS_CP_LEN                     (BASS_BAS_CP_FIX_LEN + BASS_BAS_CP_ADD_SRC_FIX_LEN + \
                                             BASS_SUBGRP_MAX * (BASS_BAS_CP_SUBGRP_INFO_FIX_LEN + GA_MAX_METADATA_LEN))

/*!< \brief Length of the Broadcast Receive State characteristic value */
#define BASS_BRS_LEN                        (BASS_BRS_COMMON_LEN + HCI_BC_LEN + \
                                             BASS_SUBGRP_MAX * (BASS_BIS_SYNC_STATE_LEN + 1 + GA_MAX_METADATA_LEN))

/*!< \brief  ATT Application error codes */
#define BASS_ERR_OPCODE_NOT_SUP             0x80  /*!< \brief Unsupported opcode used in BAS CP operation */
#define BASS_ERR_INVALID_SRC_ID             0x81  /*!< \brief Source_ID does not match any Source_ID exposed in BRS characteristic value by server */

/**************************************************************************************************
  Handle Ranges
**************************************************************************************************/
/** \name BAS Service Handles
 *
 */
/**@{*/
#define BASS_START_HDL                      0x0650              /*!< BASS start handle */
#define BASS_END_HDL                        (BASS_MAX_HDL - 1)  /*!< BASS end handle */
/**@}*/

/**************************************************************************************************
  Handles
**************************************************************************************************/

/** \name ASC Service Handles
 *
 */
/**@{*/
/*! \brief ASC service handle */
enum
{
  BASS_SVC_HDL = BASS_START_HDL,    /*!< BAS service declaration */
  BASS_BAS_CP_CH_HDL,               /*!< BAS Control Point characteristic */
  BASS_BAS_CP_HDL,                  /*!< BAS Control Point value */
  BASS_BRS_CH_1_HDL,                /*!< BAS BRS #1 characteristic */
  BASS_BRS_1_HDL,                   /*!< BAS BRS #1 value */
  BASS_BRS_CH_CCC_1_HDL,            /*!< BAS BRS #1 client characteristic configuration */
  BASS_BRS_CH_2_HDL,                /*!< BAS BRS #2 characteristic */
  BASS_BRS_2_HDL,                   /*!< BAS BRS #2 value */
  BASS_BRS_CH_CCC_2_HDL,            /*!< BAS BRS #2 client characteristic configuration */
  BASS_MAX_HDL                      /*!< BASS maximum handle */
};
/**@}*/

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
void SvcBasAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcBasRemoveGroup(void);

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
void SvcBasCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*! \} */    /* BAS_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* #define SVC_BAS_H */
