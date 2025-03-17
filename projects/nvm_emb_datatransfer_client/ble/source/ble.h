////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble.h
///
/// @project    T9305
///
/// @brief      Bluetooth LE management header file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#ifndef BLE_H_
#define BLE_H_

#include <wsf_os.h>
#include <gatt/gatt_api.h>
#include <gap/gap_api.h>

/******************************************************************************\
 *  Macros
\******************************************************************************/

/******************************************************************************\
 *  Constants
\******************************************************************************/

/** @brief Start of each service's handles in the the handle list */
#define DATC_DISC_GATT_START    0
#define DATC_DISC_GAP_START     (DATC_DISC_GATT_START + GATT_HDL_LIST_LEN)
#define DATC_DISC_EMMP_START    (DATC_DISC_GAP_START + GAP_HDL_LIST_LEN)
#define DATC_DISC_HDL_LIST_LEN  (DATC_DISC_EMMP_START + EMM_P_HDL_LIST_LEN)

/******************************************************************************\
 *  Types
\******************************************************************************/

/**
 * @brief Ble task signal structure.
 */
typedef struct
{
  /// Header structure.
  wsfMsgHdr_t           hdr;
  /// Signal parameter / user data.
  void                 *pParams;
} BleTask_Evt_t;

/** @brief Timer configuration structure */
typedef struct
{
    wsfTimerTicks_t period; /*!< timer expiration period */
    uint16_t count;         /*!< Perform after this many timer periods */
} BLE_Timer_Cfg_t;

/// Client characteristic configuration descriptors.
/** @brief Discovery states:  enumeration of services to be discovered */
enum
{
  DATC_DISC_GATT_SVC,      /*! GATT service */
  DATC_DISC_GAP_SVC,       /*! GAP service */
  DATC_DISC_EMMP_SVC,     /*! EMM proprietary service */
  DATC_DISC_SVC_MAX,        /*! Discovery complete */
};

/******************************************************************************\
 *  Exported protected global variables
\******************************************************************************/

/// WSF handler ID.
extern wsfHandlerId_t gBLE_SubsysHandlerId;

/******************************************************************************\
 *  Protected functions prototypes
\******************************************************************************/

/// Initialize the BLE module.
void BLE_Init(void);

#endif // BLE_H_
