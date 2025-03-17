/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example GATT and GAP service implementations.
 *
 *  Copyright (c) 2009-2019 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_CORE_H
#define SVC_CORE_H


#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup GATT_AND_GAP_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/
/** \name GAP Service Handles
 * \note GAP -- RPAO characterstic added only when DM Privacy enabled
 * \note GAP -- SL characterstic added only when enabled
 */
/**@{*/
#define GAP_START_HDL               0x01               /*!< GAP start handle */
#define GAP_END_HDL                 (GAP_MAX_HDL - 7)  /*!< GAP end handle */
/**@}*/

/** \name GATT Service Handles
 *
 */
/**@{*/
#define GATT_START_HDL              0x10                /*!< GATT start handle */
#define GATT_END_HDL                (GATT_MAX_HDL - 1)  /*!< GATT end handle */
/**@}*/

/**************************************************************************************************
 Handles
**************************************************************************************************/

/** \name GAP Service Handles
 *
 */
/**@{*/
/*! \brief GAP service handle */
enum
{
  GAP_SVC_HDL = GAP_START_HDL,      /*!< GAP service declaration */
  GAP_DN_CH_HDL,                    /*!< Device name characteristic */
  GAP_DN_HDL,                       /*!< Device name */
  GAP_AP_CH_HDL,                    /*!< Appearance characteristic */
  GAP_AP_HDL,                       /*!< Appearance */
  GAP_CAR_CH_HDL,                   /*!< Central address resolution characteristic */
  GAP_CAR_HDL,                      /*!< Central address resolution */
  GAP_KEY_CH_MATERIAL_HDL,         /*!< EAD Key material characteristic */
  GAP_KEY_MATERIAL_HDL,            /*!< EAD Key material */
  GAP_GSL_CH_HDL,                   /*!< GATT security levels characteristic */
  GAP_GSL_HDL,                      /*!< GATT security levels */
  GAP_RPAO_CH_HDL,                  /*!< Resolvable private address only characteristic */
  GAP_RPAO_HDL,                     /*!< Resolvable private address only */
  GAP_MAX_HDL                       /*!< GAP maximum handle */
};
/**@}*/

/** \name GATT Service Handles
 *
 */
/**@{*/
/*! \brief GATT service handles */
enum
{
  GATT_SVC_HDL = GATT_START_HDL,    /*!< GATT service declaration */
  GATT_SC_CH_HDL,                   /*!< Service changed characteristic */
  GATT_SC_HDL,                      /*!< Service changed */
  GATT_SC_CH_CCC_HDL,               /*!< Service changed client characteristic configuration descriptor */
  GATT_CSF_CH_HDL,                  /*!< Client supported features characteristic */
  GATT_CSF_HDL,                     /*!< Client supported features */
  GATT_DBH_CH_HDL,                  /*!< Database hash characteristic */
  GATT_DBH_HDL,                     /*!< Database hash */
  GATT_SSF_CH_HDL,                  /*!< Server supported features characteristic */
  GATT_SSF_HDL,                     /*!< Server supported features */
  GATT_MAX_HDL                      /*!< GATT maximum handle */
};
/**@}*/

/**************************************************************************************************
Macros
**************************************************************************************************/

/** \name Size of Attributes
 *
 */
/**@{*/
#ifndef GAP_MAXSIZE_GSL_ATT
#define GAP_MAXSIZE_GSL_ATT         6  /*!< Size of Gatt Security Levels value */
#endif

/**@}*/

/**************************************************************************************************
 Values
**************************************************************************************************/

/** \name LE Security Modes and Levels
 *
 */
/**@{*/
enum {
    GAP_SEC_M1_L1 = 0x0101,          /*!< Mode 1 Level 1: Connection has no security */
    GAP_SEC_M1_L2 = 0x0102,          /*!< Mode 1 Level 2: Connection is encrypted with unauthenticated key */
    GAP_SEC_M1_L3 = 0x0103,          /*!< Mode 1 Level 3: Connection is encrypted with authenticated key */
    GAP_SEC_M1_L4 = 0x0104,          /*!< Mode 1 Level 4: Connection is encrypted with LE Secure Connections */
    GAP_SEC_M2_L1 = 0x0201,          /*!< Mode 2 Level 1: Use of unauthenticated pairing with data signing */
    GAP_SEC_M2_L2 = 0x0202,          /*!< Mode 2 Level 1: Use of authenticated pairing with data signing */
    GAP_SEC_M3_L1 = 0x0301,          /*!< Mode 3 Level 1: No security broadcast (no authentication and no encryption) */
    GAP_SEC_M3_L2 = 0x0302,          /*!< Mode 3 Level 2: Use of unauthenticated Broadcast_Code */
    GAP_SEC_M3_L3 = 0x0303,          /*!< Mode 3 Level 3: Use of authenticated Broadcast_Code */
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
void SvcCoreAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Add the services to the attribute server.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void SvcCoreGapAddSlCh(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the RPAO characteristic from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreRemoveRpaoGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the GATT Security Levels characteristic from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreRemoveGSlcGroup(void);

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
void SvcCoreGattCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

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
void SvcCoreGapCbackRegister(attsReadCback_t readCback, attsWriteCback_t writeCback);

/*************************************************************************************************/
/*!
 *  \brief  Update the central address resolution attribute value.
 *
 *  \param  value   New value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapCentAddrResUpdate(bool value);

/*************************************************************************************************/
/*!
 *  \brief  Add the GATT Key Material to the attribute server.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void SvcCoreGapAddEadKeyMaterialCh(void);

/*************************************************************************************************/
/*!
 *  \brief  Add the Resolvable Private Address Only (RPAO) characteristic to the GAP service.
 *          The RPAO characteristic should be added only when DM Privacy is enabled.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapAddRpaoCh(void);

/*************************************************************************************************/
/*!
 *  \brief  Set the Server Supported Features (SSF) bitmask.
 *
 *  \param  value   New value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGattSetSsf(uint8_t value);

/*************************************************************************************************/
/*!
 *  \brief  Set the EAD Key.
 *
 *  \param  pRand   Pointer to the key.
 *  \param  pIv   Pointer to the iv.
 *
 *  \return None.
 */
 /*************************************************************************************************/
void SvcCoreGapSetEadKey(uint8_t* pRand, uint8_t* pIv);

/*************************************************************************************************/
/*!
 *  \brief  Set the SLC value
 *
 *  \param  value   New value.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcCoreGapSetGSlc(uint8_t numReqs, uint16_t *secLvlReqs);

/*! \} */    /* GATT_AND_GAP_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_CORE_H */
