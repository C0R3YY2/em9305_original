/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Device Information Service implementation.
 *
 *  Copyright (c) 2011-2020 Arm Ltd. All Rights Reserved.
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

#ifndef SVC_DIS_H
#define SVC_DIS_H

#include "svc_ch.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup DEVICE_INFORMATION_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Device Information Service Handles
 *
 */
/**@{*/
/*! \brief Device Information Service */
#define DIS_START_HDL               0x30                  /*!< Start handle. */
#define DIS_END_HDL                 (DIS_MAX_HDL - 1)     /*!< End handle. */
/*! \brief Device Information Service Secondary Service */
#define DIS_START_SEC_HDL           0x500                 /*!< Start handle. */
#define DIS_END_SEC_HDL             (DIS_MAX_SEC_HDL - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Device Information Service Handles */
enum
{
  DIS_SVC_HDL = DIS_START_HDL,      /*!< Information service declaration */
  DIS_MFR_CH_HDL,                   /*!< Manufacturer name string characteristic */
  DIS_MFR_HDL,                      /*!< Manufacturer name string */
  DIS_SID_CH_HDL,                   /*!< System ID characteristic */
  DIS_SID_HDL,                      /*!< System ID */
  DIS_MN_CH_HDL,                    /*!< Model number string characteristic */
  DIS_MN_HDL,                       /*!< Model number string */
  DIS_SN_CH_HDL,                    /*!< Serial number string characteristic */
  DIS_SN_HDL,                       /*!< Serial number string */
  DIS_FWR_CH_HDL,                   /*!< Firmware revision string characteristic */
  DIS_FWR_HDL,                      /*!< Firmware revision string */
  DIS_HWR_CH_HDL,                   /*!< Hardware revision string characteristic */
  DIS_HWR_HDL,                      /*!< Hardware revision string */
  DIS_SWR_CH_HDL,                   /*!< Software revision string characteristic */
  DIS_SWR_HDL,                      /*!< Software revision string */
  DIS_RCD_CH_HDL,                   /*!< IEEE 11073-20601 regulatory certificate data characteristic */
  DIS_RCD_HDL,                      /*!< IEEE 11073-20601 regulatory certificate data */
  DIS_PNP_ID_CH_HDL,                /*!< PnP ID characteristic */
  DIS_PNP_ID_HDL,                   /*!< PnP ID */
  DIS_UDI_MED_CH_HDL,               /*!< UDI for Medical Devices characteristic */
  DIS_UDI_MED_HDL,                  /*!< UDI for Medical Devices */
  DIS_MAX_HDL                       /*!< Maximum handle. */
};
/**@}*/

/*! \brief Device Information Service Secondary Service Handles */
enum
{
  DIS_SVC_SEC_HDL = DIS_START_SEC_HDL,  /*!< Information service declaration Secondary Service */
  DIS_MFR_CH_SEC_HDL,                   /*!< Manufacturer name string characteristic Secondary Service */
  DIS_MFR_SEC_HDL,                      /*!< Manufacturer name string Secondary Service */
  DIS_SID_CH_SEC_HDL,                   /*!< System ID characteristic Secondary Service */
  DIS_SID_SEC_HDL,                      /*!< System ID Secondary Service */
  DIS_MN_CH_SEC_HDL,                    /*!< Model number string characteristic Secondary Service */
  DIS_MN_SEC_HDL,                       /*!< Model number string Secondary Service */
  DIS_SN_CH_SEC_HDL,                    /*!< Serial number string characteristic Secondary Service */
  DIS_SN_SEC_HDL,                       /*!< Serial number string Secondary Service */
  DIS_FWR_CH_SEC_HDL,                   /*!< Firmware revision string characteristic Secondary Service */
  DIS_FWR_SEC_HDL,                      /*!< Firmware revision string Secondary Service */
  DIS_HWR_CHSEC__HDL,                   /*!< Hardware revision string characteristic Secondary Service */
  DIS_HWR_SEC_HDL,                      /*!< Hardware revision string Secondary Service */
  DIS_SWR_CH_SEC_HDL,                   /*!< Software revision string characteristic Secondary Service */
  DIS_SWR_SEC_HDL,                      /*!< Software revision string Secondary Service */
  DIS_RCD_CH_SEC_HDL,                   /*!< IEEE 11073-20601 regulatory certificate data characteristic Secondary Service */
  DIS_RCD_SEC_HDL,                      /*!< IEEE 11073-20601 regulatory certificate data Secondary Service */
  DIS_PNP_ID_CH_SEC_HDL,                /*!< PnP ID characteristic Secondary Service */
  DIS_PNP_ID_SEC_HDL,                   /*!< PnP ID Secondary Service */
  DIS_UDI_MED_CH_SEC_HDL,               /*!< UDI for Medical Devices characteristic Secondary Service */
  DIS_UDI_MED_SEC_HDL,                  /*!< UDI for Medical Devices Secondary Service */
  DIS_MAX_SEC_HDL                       /*!< Maximum handle Secondary Service. */
};
/**@}*/

/**************************************************************************************************
Macros
**************************************************************************************************/

/** \name Size of Attributes
 *
 */
/**@{*/
#ifndef DIS_MAXSIZE_MFR_ATT
#define DIS_MAXSIZE_MFR_ATT         20  /*!< Size of manufacturer name string attribute */
#endif

#ifndef DIS_MAXSIZE_MN_ATT
#define DIS_MAXSIZE_MN_ATT          25  /*!< Size of model number string attribute */
#endif

#ifndef DIS_MAXSIZE_SN_ATT
#define DIS_MAXSIZE_SN_ATT          25  /*!< Size of serial number string attribute */
#endif

#ifndef DIS_MAXSIZE_FWR_ATT
#define DIS_MAXSIZE_FWR_ATT         21 /*!< Size of firmware revision string attribute */
#endif

#ifndef DIS_MAXSIZE_HWR_ATT
#define DIS_MAXSIZE_HWR_ATT         21  /*!< Size of hardware revision string attribute */
#endif

#ifndef DIS_MAXSIZE_SWR_ATT
#define DIS_MAXSIZE_SWR_ATT         21  /*!< Size of software revision string attribute */
#endif

#ifndef DIS_SIZE_SID_ATT
#define DIS_SIZE_SID_ATT            8   /*!< Size of system id attribute */
#endif

#ifndef DIS_SIZE_RCD_ATT
#define DIS_SIZE_RCD_ATT            6   /*!< Size of registration certificate data attribute */
#endif

#ifndef DIS_MAXSIZE_UDILABEL_ATT
#define DIS_MAXSIZE_UDILABEL_ATT    16   /*!< Size of UDI for Medical Devices UDI Label */
#endif

#ifndef DIS_MAXSIZE_UDIDID_ATT
#define DIS_MAXSIZE_UDIDID_ATT      20   /*!< Size of UDI for Medical Devices Device Identfier */
#endif

#ifndef DIS_MAXSIZE_UDIISS_ATT
#define DIS_MAXSIZE_UDIISS_ATT      20   /*!< Size of UDI for Medical Devices Issuer */
#endif

#ifndef DIS_MAXSIZE_UDIAUTH_ATT
#define DIS_MAXSIZE_UDIAUTH_ATT     17   /*!< Size of UDI for Medical Devices Authority */
#endif
/**@}*/

/** \name UDI for Medical Devices Flags selection
 *
 */
/**@{*/
#define DIS_UDI_MED_FLAGS           (CH_UDI_MED_FLAG_UDI_LABEL | CH_UDI_MED_FLAG_UDI_AUTH)
/**@}*/

/**************************************************************************************************
 Data Types
**************************************************************************************************/
/*! \brief UDI for Medical Devices structure */
typedef struct
{
  uint8_t flags;                                        /* Flags */
  #if (DIS_UDI_MED_FLAGS & CH_UDI_MED_FLAG_UDI_LABEL)
  uint8_t disUdiMedUdiLabel[DIS_MAXSIZE_UDILABEL_ATT];  /* UDI Label */
  #endif
  #if (DIS_UDI_MED_FLAGS & CH_UDI_MED_FLAG_UDI_DID)
  uint8_t disUdiMedUdiDid[DIS_MAXSIZE_UDIDID_ATT];      /* Device Identifier */
  #endif
  #if (DIS_UDI_MED_FLAGS & CH_UDI_MED_FLAG_UDI_ISS)
  uint8_t disUdiMedUdiIss[DIS_MAXSIZE_UDIISS_ATT];      /* Issuer */
  #endif
  #if (DIS_UDI_MED_FLAGS & CH_UDI_MED_FLAG_UDI_AUTH)
  uint8_t disUdiMedUdiAuth[DIS_MAXSIZE_UDIAUTH_ATT];    /* Authority */
  #endif
} disValUdiMed_t;


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
void SvcDisAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcDisRemoveGroup(void);

/*! \} */    /* DEVICE_INFORMATION_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_DIS_H */
