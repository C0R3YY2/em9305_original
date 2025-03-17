/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  UriCfg configuration service implementation.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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

#ifndef SVC_URICFG_H
#define SVC_URICFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup URICFG_CONFIGURATION_SERVICE
 *  \{ */

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name URI Config Service Handles
 *
 */
/**@{*/
#define URICFG_HANDLE_START  0x80                            /*!< Start handle. */
#define URICFG_HANDLE_END   (URICFG_HANDLE_END_PLUS_ONE - 1) /*!< End handle. */

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief UriBeacon configuration service handles */
enum
{
  URICFG_HANDLE_SVC = URICFG_HANDLE_START,  /*!< Service declaration. */

  URICFG_HANDLE_LOCKSTATE_CHR,              /*!< Lock state characteristic declaration. */
  URICFG_HANDLE_LOCKSTATE,                  /*!< Lock state characteristic value. */
  URICFG_HANDLE_LOCKSTATE_CHR_USR_DESCR,    /*!< Lock state characteristic user description. */

  URICFG_HANDLE_LOCK_CHR,                   /*!< Lock characteristic declaration. */
  URICFG_HANDLE_LOCK,                       /*!< Lock characteristic value. */
  URICFG_HANDLE_LOCK_CHR_USR_DESCR,         /*!< Lock characteristic user declaration. */

  URICFG_HANDLE_UNLOCK_CHR,                 /*!< Unlock characteristic declaration. */
  URICFG_HANDLE_UNLOCK,                     /*!< Unlock characteristic value. */
  URICFG_HANDLE_UNLOCK_CHR_USR_DESCR,       /*!< Unlock characteristic user description. */

  URICFG_HANDLE_URIDATA_CHR,                /*!< URI data characteristic declaration. */
  URICFG_HANDLE_URIDATA,                    /*!< URI data characteristic value. */
  URICFG_HANDLE_URIDATA_CHR_USR_DESCR,      /*!< URI data characteristic user description. */

  URICFG_HANDLE_URIFLAGS_CHR,               /*!< URI flags characteristic declaration. */
  URICFG_HANDLE_URIFLAGS,                   /*!< URI flags characteristic value. */
  URICFG_HANDLE_URIFLAGS_CHR_USR_DESCR,     /*!< URI flags characteristic user description. */

  URICFG_HANDLE_TXPWRLEVELS_CHR,            /*!< Tx power levels characteristic declaration. */
  URICFG_HANDLE_TXPWRLEVELS,                /*!< Tx power levels characteristic value. */
  URICFG_HANDLE_TXPWRLEVELS_CHR_USR_DESCR,  /*!< Tx power levels characteristic user description. */

  URICFG_HANDLE_TXPWRMODE_CHR,              /*!< Tx power mode characteristic declaration. */
  URICFG_HANDLE_TXPWRMODE,                  /*!< Tx power mode characteristic value. */
  URICFG_HANDLE_TXPWRMODE_CHR_USR_DESCR,    /*!< Tx power mode characteristic user description. */

  URICFG_HANDLE_BEACONPERIOD_CHR,           /*!< Beacon period characteristic declaration. */
  URICFG_HANDLE_BEACONPERIOD,               /*!< Beacon period characteristic value. */
  URICFG_HANDLE_BEACONPERIOD_CHR_USR_DESCR, /*!< Beacon period characteristic user description. */

  URICFG_HANDLE_RESET_CHR,                  /*!< Reset characteristic declaration. */
  URICFG_HANDLE_RESET,                      /*!< Reset characteristic value. */
  URICFG_HANDLE_RESET_CHR_USR_DESCR,        /*!< Reset characteristic user description. */

  URICFG_HANDLE_END_PLUS_ONE                /*!< Maximum handle. */
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
void SvcUriCfgAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcUriCfgRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcUriCfgCbackRegister(attsWriteCback_t writeCback);

/*! \} */    /* URICFG_CONFIGURATION_SERVICE */

#ifdef __cplusplus
}
#endif

#endif /* SVC_URICFG_H */
