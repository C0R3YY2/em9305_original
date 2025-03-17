////////////////////////////////////////////////////////////////////////////////
///
/// @file       rpa.h
///
/// @project    EM9305
///
/// @brief      RPA configuration and types
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2023-24, EM Microelectronic-Marin SA
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

#ifndef __RPA_H
#define __RPA_H

#include "bsp.h"
#include "hci_api.h"
#include "ll_api.h"
#include "app_api.h"
#include "app_task.h"
#include "att_api.h"
#include "bas/bas_api.h"
#include "dm_api.h"
#include "svc_batt.h"
#include "svc_core.h"

/******************************************************************************\
 *  Types
\******************************************************************************/


/*! \brief Bonding info database record. Stored in NVM after pairing completes. */
typedef struct
{
    /*! Common for all roles */
    dmSecIrk_t  peerIrk;          /*! Peer IRK */
    dmSecCsrk_t peerCsrk;         /*! Peer CSRK */
    uint8_t     peerCsrkSecLevel; /*! Peer CSRK security level */
    uint8_t     keyValidMask;     /*! Valid keys in this record */
    bool        inUse;            /*! TRUE if record in use */
    bool        valid;            /*! TRUE if record is valid */
    bool        peerRpao;         /*! TRUE if RPA Only attribute's present on peer device */

    /*! For slave local device */
    dmSecLtk_t localLtk;         /*! Local LTK */
    uint8_t    localLtkSecLevel; /*! Local LTK security level */
    bool       peerAddrRes;      /*! TRUE if address resolution's supported on peer device (master) */

    /*! For master local device */
    dmSecLtk_t peerLtk;         /*! Peer LTK */
    uint8_t    peerLtkSecLevel; /*! Peer LTK security level */

    /*! for ATT server local device */
    uint32_t peerSignCounter; /*! Peer Sign Counter */
} rpaDbRec_t;

/*! \brief      Application message type */
typedef union
{
    wsfMsgHdr_t  hdr;
    dmEvt_t      dm;
    attsCccEvt_t ccc;
    attEvt_t     att;
} rpaMsg_t;

/*! \brief     Application control block */
typedef struct
{
    wsfHandlerId_t handlerId;
    dmConnId_t     connId;
    uint8_t        connUpdateAttempts;
    bool           doConnUpdate;
    bool           bondable;
    bool           bonded;
    uint8_t        state;
    uint8_t        rcvdKeys; /*! Bitmask of keys received during pairing */
    bdAddr_t       peerAddr;
    uint8_t        peerAddrType;
} rpacb_t;

/******************************************************************************\
 *  Identifiers
\******************************************************************************/
#define ADV_INTERVAL_MIN 0x0200U
#define ADV_INTERVAL_MAX 0x0200U

#define START_ADDR             0x300000U
#define END_ADDR               (START_ADDR + NVM_PAGE_SIZE_BYTE * NVM_PAGE_COUNT_MAIN)
#define RPA_DB_BONDING_PAGE    (NVM_PAGE_COUNT_MAIN - 1)
#define RPA_DB_NVM_ADDR        (START_ADDR + (RPA_DB_BONDING_PAGE * NVM_PAGE_SIZE_BYTE)) // last page of nvm

/******************************************************************************\
 *  Enumerations
\******************************************************************************/

/*! \brief RPA timeout in seconds. RPA will change continuously at this interval. */
enum
{
    RPA_TIMEOUT_SEC = (60U * 5U)  // Timeout Value set to 300 seconds
};

/*! \brief Enumeration of client characteristic configuration descriptors. */
enum
{
    RPA_GATT_SC_CCC_IDX,             // GATT service, service changed characteristic.
    RPA_BATT_LVL_CCC_IDX,            // Battery service, battery level characteristic.
    RPA_NUM_CCC_IDX
};

/******************************************************************************\
 *  Public functions prototypes
\******************************************************************************/

/******************************************************************************/
/* RPA QPC interface */
/******************************************************************************/
APP_TASK_EVT_ST_t APP_HandleEvent(APP_TASK_SIG_t signal, void *pParams);

/******************************************************************************/
/* RPA DB interface */
/******************************************************************************/
bool        RPADB_LoadBondingInfo(void);
bool        RPADB_StoreBondingInfo(void);
dmSecKey_t *RPADB_GetKey(uint8_t type, uint8_t *pSecLevel);
void        RPADB_SetCSRK(uint8_t secLevel, dmSecCsrk_t csrk);
void        RPADB_SetIRK(dmSecIrk_t irk, uint8_t addrType, bdAddr_t bdAddr);
void        RPADB_SetKeyValidMask(uint8_t keyMask);
void        RPADB_SetPeerLTK(uint8_t secLevel, dmSecLtk_t ltk);
void        RPADB_SetLocalLTK(uint8_t secLevel, dmSecLtk_t ltk);

/******************************************************************************/
/* RPA DM interface */
/******************************************************************************/
bool   RPA_DmHandler(dmEvt_t *pDmEvt);
void   RPA_DmCback(dmEvt_t *pEvt);
void   RPA_Handler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
void   RPA_HandlerInit(wsfHandlerId_t handlerId);

/******************************************************************************/
/* RPA Host interface */
/******************************************************************************/
void   RPA_Init(void);
bool   RPA_AttHandler(wsfMsgHdr_t *pMsg);
void   RPA_AttCback(attEvt_t *pEvt);
void   RPA_CccCback(attsCccEvt_t *pEvt);
void   RPA_ServerConnCback(dmEvt_t *pDmEvt);

#endif // __RPA_H
