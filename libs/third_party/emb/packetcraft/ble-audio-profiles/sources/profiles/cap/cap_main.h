/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Common Audio Profile.
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

#ifndef CAP_MAIN_H
#define CAP_MAIN_H

#include "cap_api.h"
#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************************************
  Macros
**************************************************************************************************/

/* State machine table constants */
#define CAP_SM_POS_EVENT          0       /* Column position for event */
#define CAP_SM_POS_NEXT_STATE     1       /* Column position for next state */
#define CAP_SM_POS_ACTION         2       /* Column position for action */
#define CAP_SM_NUM_COLS           3       /* Number of columns in state table */

/*! CAP initiator active procedures */
enum
{
  CAPI_PROC_NONE,
  CAPI_PROC_UNI_AUDIO_START
};

/*! CAPI state machine states common to all procedures */
enum
{
  CAP_STATE_IDLE,
  CAP_STATE_LOCKING,
  CAP_STATE_RELEASING,
  //CAP_STATE_COMPLETING,

  CAP_STATE_COMMON_MAX,
};

/*! CAPI state machine states - unicast audio start procedure */
enum
{
  CAPI_STATE_UASP_AV_CAPS_LOC = CAP_STATE_COMMON_MAX,
  CAPI_STATE_UASP_CODEC_CFG,
  CAPI_STATE_UASP_CIG_CFG,
  CAPI_STATE_UASP_QOS_CFG,
  CAPI_STATE_UASP_METADATA_SET,
  CAPI_STATE_UASP_ENABLE_ASE,
  CAPI_STATE_UASP_OPEN_CIS,
  CAPI_STATE_UASP_START_READY_RX
};

/*! CAPI state machine states - unicast audio update procedure */
enum
{
  CAPI_STATE_UAUP_AV = CAP_STATE_COMMON_MAX,
  CAPI_STATE_UAUP_METADATA_SET,
  CAPI_STATE_UAUP_METADATA_UPDATE
};

/*! CAPI state machine states - unicast audio stop (disable) procedure */
enum
{
  CAPI_STATE_UASP_STOP_DIS_ASE = CAP_STATE_COMMON_MAX,
  CAPI_STATE_UASP_STOP_READY_RX
};

/*! CAPI state machine states - unicast audio stop (release) procedure */
enum
{
  CAPI_STATE_UASP_STOP_REL_ASE = CAP_STATE_COMMON_MAX
};

/*! CAPI state machine states - broadcast audio start (configure) procedure */
enum
{
  CAPI_STATE_BASP_BAS_CFG = CAP_STATE_COMMON_MAX,
  CAPI_STATE_BASP_BCAST_ANNC_START,
  CAPI_STATE_BASP_BASIC_ANNC_START,
  CAPI_STATE_BASP_BASIC_ANNC_UPD,
  CAPI_STATE_BASP_BAS_ESTABLISH
};

/*! CAPI state machine states - broadcast metadata update procedure */
enum
{
  CAPI_STATE_BASP_METADATA_UPDATE = CAP_STATE_COMMON_MAX,
  CAPI_STATE_BASP_BASIC_ANNC_UPD2
};

/*! CAPI state machine states - broadcast stop (disable) procedure */
enum
{
  CAPI_STATE_BASP_BAS_STOP_DISABLE = CAP_STATE_COMMON_MAX,
};

/*! CAPI state machine states - broadcast stop (release) procedure */
enum
{
  CAPI_STATE_BASP_BAS_REL_DISABLE = CAP_STATE_COMMON_MAX,
  CAPI_STATE_BASP_BAS_RELEASE,
  CAPI_STATE_BASP_BASIC_ANNC_STOP,
  CAPI_STATE_BASP_BAS_DELETE
};

/*! CAPC state machine states - broadcast audio reception start procedure */
enum
{
  CAPI_STATE_BARSP_SYNC_METADATA_SET = CAP_STATE_COMMON_MAX
};

/*! CAPC state machine states - broadcast audio reception stop procedure */
enum
{
  CAPI_STATE_BARSP_STOP_BCAST_AUD_RX = CAP_STATE_COMMON_MAX,
  CAPI_STATE_BARSP_STOP_PA_SYNC,
  CAPI_STATE_BARSP_REMOVE_BCAST_SRC
};

/*! CAPC state machine states - distribute Broadcast_Code procedure */
enum
{
  CAPI_STATE_DBC_SET_BCAST_CODE = CAP_STATE_COMMON_MAX
};

/*! CAPC state machine states - change abs volume procedure */
enum
{
  CAPC_STATE_CVP_SET_ABS_VOL = CAP_STATE_COMMON_MAX,
};

/*! CAPC state machine states - change volume offset procedure */
enum
{
  CAPC_STATE_CVP_SET_VOL_OFFSET = CAP_STATE_COMMON_MAX,
};

/*! CAPC state machine states - change VCP and MICP mute procedure */
enum
{
  CAPC_STATE_CMP_SET_MUTE = CAP_STATE_COMMON_MAX,
};

/*! CAPC state machine states - change MICP gain setting procedure */
enum
{
  CAPC_STATE_CGSP_SET_GAIN_SETTING = CAP_STATE_COMMON_MAX,
};

/* CAP state machine event identifiers */
enum
{
  CAP_EVT_API_CANCEL,

  /* CSIS events */
  CAP_EVT_CSIS_LOCK_UNLOCK,
  CAP_EVT_CSIS_LOCK_CMPL,
  CAP_EVT_CSIS_UNLOCK_CMPL,
  CAP_EVT_CSIS_LOCK_TIMEOUT,

  /* Procedure complete event */
  //CAP_EVT_PROC_CMPL,

  /* ASE Start events */
  CAPI_EVT_DISC_AV_CAPS_LOC,
  CAPI_EVT_DISC_AV_CAPS_LOC_CMPL,
  CAPI_EVT_DISC_AV_CAPS_LOC_FAIL,
  CAPI_EVT_CODEC_CFG_IND,
  CAPI_EVT_CODEC_CFG_CMPL,
  CAPI_EVT_CODEC_CFG_FAIL,
  CAPI_EVT_CIG_CFG_IND,
  CAPI_EVT_CIG_CFG_CMPL,
  CAPI_EVT_CIG_CFG_FAIL,
  CAPI_EVT_QOS_CFG_IND,
  CAPI_EVT_QOS_CFG_CMPL,
  CAPI_EVT_QOS_CFG_FAIL,
  CAPI_EVT_METADATA_SET_IND,
  CAPI_EVT_METADATA_SET_CMPL,
  CAPI_EVT_METADATA_SET_FAIL,
  CAPI_EVT_ASE_ENABLE_IND,
  CAPI_EVT_ASE_ENABLE_CMPL,
  CAPI_EVT_ASE_ENABLE_FAIL,
  CAPI_EVT_CIS_OPEN_IND,
  CAPI_EVT_CIS_OPEN_CMPL,
  CAPI_EVT_CIS_OPEN_FAIL,
  CAPI_EVT_RX_START_READY_IND,
  CAPI_EVT_RX_START_READY_CMPL,
  CAPI_EVT_RX_START_READY_FAIL,
  CAPI_EVT_DATAPATH_CREATE_CMPL,
  CAPI_EVT_DATAPATH_CREATE_FAIL,

  /* ASE Update events */
  CAPI_EVT_DISC_AV,
  CAPI_EVT_DISC_AV_CMPL,
  CAPI_EVT_DISC_AV_FAIL,
  CAPI_EVT_METADATA_UPDATE_IND,
  CAPI_EVT_METADATA_UPDATE_CMPL,
  CAPI_EVT_METADATA_UPDATE_FAIL,

  /* ASE Stop (Disable) events */
  CAPI_EVT_ASE_STOP_DIS_IND,
  CAPI_EVT_ASE_STOP_DIS_CMPL,
  CAPI_EVT_ASE_STOP_DIS_FAIL,
  CAPI_EVT_RX_STOP_READY_IND,
  CAPI_EVT_RX_STOP_READY_CMPL,
  CAPI_EVT_RX_STOP_READY_FAIL,

  /* ASE Stop (Release) events */
  CAPI_EVT_ASE_STOP_REL_IND,
  CAPI_EVT_ASE_STOP_REL_CMPL,
  CAPI_EVT_ASE_STOP_REL_FAIL,

  /* BAS Start events */
  CAPI_EVT_BAS_CFG_IND,
  CAPI_EVT_BAS_CFG_CMPL,
  CAPI_EVT_BAS_CFG_FAIL,
  CAPI_EVT_BCAST_ANNC_START_IND,
  CAPI_EVT_BCAST_ANNC_START_CMPL,
  CAPI_EVT_BCAST_ANNC_START_FAIL,
  CAPI_EVT_BASIC_ANNC_START_IND,
  CAPI_EVT_BASIC_ANNC_START_CMPL,
  CAPI_EVT_BASIC_ANNC_START_FAIL,
  CAPI_EVT_BASIC_ANNC_UPD_IND,
  CAPI_EVT_BASIC_ANNC_UPD_CMPL,
  CAPI_EVT_BASIC_ANNC_UPD_FAIL,
  CAPI_EVT_BAS_ESTABLISH_IND,
  CAPI_EVT_BAS_ESTABLISH_CMPL,
  CAPI_EVT_BAS_ESTABLISH_FAIL,

  /* BAS Metadata Update events */
  CAPI_EVT_BAS_METADATA_UPD_IND,
  CAPI_EVT_BAS_METADATA_UPD_CMPL,
  CAPI_EVT_BAS_METADATA_UPD_FAIL,
  CAPI_EVT_BASIC_ANNC_UPD2_IND,
  CAPI_EVT_BASIC_ANNC_UPD2_CMPL,
  CAPI_EVT_BASIC_ANNC_UPD2_FAIL,

  /* BAS Stop (Disable) events */
  CAPI_EVT_BAS_STOP_DIS_IND,
  CAPI_EVT_BAS_STOP_DIS_CMPL,
  CAPI_EVT_BAS_STOP_DIS_FAIL,

  /* BAS Stop (Release) events */
  CAPI_EVT_BAS_STOP_REL_IND,
  CAPI_EVT_BAS_STOP_REL_CMPL,
  CAPI_EVT_BAS_STOP_REL_FAIL,
  CAPI_EVT_BAS_RELEASE_IND,
  CAPI_EVT_BAS_RELEASE_CMPL,
  CAPI_EVT_BAS_RELEASE_FAIL,
  CAPI_EVT_BCAST_ANNC_STOP_IND,
  CAPI_EVT_BCAST_ANNC_STOP_CMPL,
  CAPI_EVT_BCAST_ANNC_STOP_FAIL,
  CAPI_EVT_BASE_DELETE_IND,
  CAPI_EVT_BASE_DELETE_CMPL,
  CAPI_EVT_BASE_DELETE_FAIL,

  /* Commander broadcast events */
  CAPC_EVT_SYNC_METADATA_SET_IND,
  CAPC_EVT_SYNC_METADATA_SET_CMPL,
  CAPC_EVT_SYNC_METADATA_SET_FAIL,
  CAPC_EVT_STOP_BCAST_AUD_RX_IND,
  CAPC_EVT_STOP_BCAST_AUD_RX_CMPL,
  CAPC_EVT_STOP_BCAST_AUD_RX_FAIL,
  CAPC_EVT_STOP_PA_SYNC_IND,
  CAPC_EVT_STOP_PA_SYNC_CMPL,
  CAPC_EVT_STOP_PA_SYNC_FAIL,
  CAPC_EVT_REMOVE_BCAST_SRC_IND,
  CAPC_EVT_REMOVE_BCAST_SRC_CMPL,
  CAPC_EVT_REMOVE_BCAST_SRC_FAIL,
  CAPC_EVT_SET_BCAST_CODE_IND,
  CAPC_EVT_SET_BCAST_CODE_CMPL,
  CAPC_EVT_SET_BCAST_CODE_FAIL,

  /* Volume events */
  CAPC_EVT_VCS_CP_WRITE_RSP,
  CAPC_EVT_VCS_ABS_VOL_CMPL,
  CAPC_EVT_VCS_VOL_OFFSET_CMPL,
  CAPC_EVT_VCS_MUTE_CMPL,

  /* Microphone events */
  CAPC_EVT_MICS_MUTE_NTF,
  CAPC_EVT_MICS_MUTE_CMPL,
  CAPC_EVT_MICS_GAIN_NTF,
  CAPC_EVT_MICS_GAIN_CMPL,

  CAP_EVT_TIMEOUT,
  CAP_EVT_NULL = 0xFF
};

/* CAP state machine event identifiers - Procedure complete event */
enum
{
  CAP_EVT_PROC_CMPL = CAP_EVT_CSIS_UNLOCK_CMPL
};

/* CAP action identifiers */
enum
{
  /* Common action identifiers */
  CAP_ACT_NONE,
  CAP_ACT_CANCEL,

  /* CSIS action identifiers */
  CAP_ACT_LOCK_NEXT,
  CAP_ACT_LOCK_COMPLETE,
  CAP_ACT_BEGIN_UNLOCK,
  CAP_ACT_UNLOCK_NEXT,

  /* Unicast audio action identifiers */
  CAPI_ACT_DISC_AV_CAPS_LOC_NEXT,
  CAPI_ACT_CFG_CODEC_NEXT,
  CAPI_ACT_CFG_CIG_NEXT,
  CAPI_ACT_CFG_QOS_NEXT,
  CAPI_ACT_SET_METADATA_NEXT,
  CAPI_ACT_ENABLE_ASE_NEXT,
  CAPI_ACT_OPEN_CIS_NEXT,
  CAPI_ACT_START_READY_RX_NEXT,

  CAPI_ACT_DISC_AV_NEXT,
  CAPI_ACT_UPDATE_METADATA_NEXT,

  CAPI_ACT_STOP_DIS_ASE_NEXT,
  CAPI_ACT_STOP_READY_RX_NEXT,

  CAPI_ACT_STOP_REL_ASE_NEXT,

  /* Broadcast audio action identifiers */
  CAPI_ACT_CFG_CODEC,
  CAPI_ACT_START_BCAST_ANNC,
  CAPI_ACT_START_BASIC_ANNC,
  CAPI_ACT_UPDATE_BASIC_ANNC,
  CAPI_ACT_ESTABLISH_BAS,
  CAPI_ACT_UPDATE_METADATA,
  CAPI_ACT_DISABLE_BAS,
  CAPI_ACT_RELEASE_BAS,
  CAPI_ACT_STOP_BASIC_ANNC,
  CAPI_ACT_DELETE_BASE,

  /* Commander broadcast audio action identifiers */
  CAPC_ACT_SYNC_SET_METADATA_NEXT,
  CAPC_ACT_STOP_BCAST_AUD_RX_NEXT,
  CAPC_ACT_STOP_PA_SYNC_NEXT,
  CAPC_ACT_REMOVE_BCAST_SRC_NEXT,
  CAPC_ACT_SET_BCAST_CODE_NEXT,

  /* Volume and microphone action identifiers */
  CAPC_ACT_SET_ABS_VOL_NEXT,
  CAPC_ACT_SET_REL_VOL_NEXT,
  CAPC_ACT_SET_VCP_MUTE_NEXT,
  CAPC_ACT_SET_MICP_MUTE_NEXT,
  CAPC_ACT_SET_MICP_GAIN_SETTING_NEXT
};

/* Action timeout in seconds */
#define CAP_ACTION_TIMEOUT      5

/* CAP profiles */
enum
{
  CAP_PROFILE_INITIATOR,
  CAP_PROFILE_ACCEPTOR,
  CAP_PROFILE_COMMANDER,

  CAP_PROFILE_MAX
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/* Data type for state machine table entry */
typedef uint8_t capProcTblEntry_t[CAP_SM_NUM_COLS];

typedef void (*capActionFunc_t)(capEvt_t *pEvt);

/* CAP procedure configuration */
typedef struct
{
  union
  {
  capUcastAudioStartCfg_t     ucastAudioStart;        /*!< Unicast audio start configuration */
  capUcastAudioUpdateCfg_t    ucastAudioUpdate;       /*!< Unicast audio update configuration */
  capUcastAudioStopCfg_t      ucastAudioStop;         /*!< Unicast audio stop configuration */
  capBcastAudioCfg_t          bcastAudio;             /*!< Broadcast audio configuration */
  capBcastAudioUpdateCfg_t    bcastAudioUpdate;       /*!< Broadcast audio update configuration */
  capBcastAudioStopCfg_t      bcastAudioStop;         /*!< Broadcast audio stop configuration */
  capBcastAudioRxStartCfg_t   bcastAudioRxStart;      /*!< Broadcast audio reception start configuration */
  capBcastAudioRxStopCfg_t    bcastAudioRxStop;       /*!< Broadcast audio reception stop configuration */
  capDistributeBcastCodeCfg_t distributeBcastCode;    /*!< Distribute Broadcast_Code configuration */
  capAbsVolCfg_t              absVol;                 /*!< Absolute volume setting */
  capVolOffsetCfg_t           volOffset;              /*!< Volume offset setting */
  capMuteCfg_t                mute;                   /*!< Mute setting */
  capGainCfg_t                gain;                   /*!< Gain setting */
  } proc;

} capProcedureCfg_t;

typedef void (*capProcMsgCback_t)(wsfMsgHdr_t *pEvt);
typedef void (*capProcDmMsgCback_t)(dmEvt_t *pEvt);

/*! profile interface structure */
typedef struct
{
  capProcMsgCback_t           procMsg;                /*!< Profiles's message process function */
} capIf_t;

/*! Common Audio Profile procedure control block. */
typedef struct
{
  const capProcTblEntry_t     *const *sm;             /*!< State table for active procedure */
  capSetId_t                  setId;                  /*!< Coordinated set ID for active procedure */
  capProcedureCfg_t           cfg;                    /*!< Procedure configuration */
  uint8_t                     state;                  /*!< Procedure state */
  uint8_t                     cigId[DM_CIG_MAX];      /*!< CIG IDs */
  uint8_t                     bigHandle[DM_BIG_MAX];  /*!< BIG handles */
  uint16_t                    connCmplMask;           /*!< Mask of connId task complete */
  wsfTimer_t                  timer;                  /*!< Procedure timeout timer */
  uint8_t                     capProc;                /*!< CAP procedure currently in progress */
  bool                        cmdRepeated;            /*!< true if command has been repeated due to wrong change counter */
  uint8_t                     numBis;                 /*!< Number of BIS */
  uint16_t                    bisHandle[HCI_MAX_BIS_COUNT]; /*!< Connection handles of the BIS's */
  wsfMsgHdr_t                 cbackEvt;               /*!< Application's callback event */
  capCback_t                  cback;                  /*!< Application's callback */
} capProcedureCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern capProcedureCb_t capProcCb;

extern capIf_t capBapiIf;
extern capIf_t capBapaIf;
extern capIf_t capBapcIf;

extern capActionFunc_t *pCapiActionFuncTbl;
extern capActionFunc_t *pCapcActionFuncTbl;

/**************************************************************************************************
  Globals
**************************************************************************************************/

/* Common action functions */
extern void capActNone(capEvt_t *pEvt);
extern void capActCancel(capEvt_t *pEvt);
extern void capActLockNext(capEvt_t *pEvt);
extern void capActLockComplete(capEvt_t *pEvt);
extern void capActBeginUnlock(capEvt_t *pEvt);
extern void capActUnlockNext(capEvt_t *pEvt);

/* Initiator action functions */
extern void capiActDiscAvCapsLocNext(capEvt_t *pEvt);
extern void capiActCfgCodecNext(capEvt_t *pEvt);
extern void capiActCfgCigNext(capEvt_t *pEvt);
extern void capiActCfgQosNext(capEvt_t *pEvt);
extern void capiActSetMetadataNext(capEvt_t *pEvt);
extern void capiActEnableAseNext(capEvt_t *pEvt);
extern void capiActOpenCisNext(capEvt_t *pEvt);
extern void capiActStartRxReadyNext(capEvt_t *pEvt);

extern void capiActDiscAvNext(capEvt_t *pEvt);
extern void capiActUpdateMetadataNext(capEvt_t *pEvt);

extern void capiActStopDisAseNext(capEvt_t *pEvt);
extern void capiActStopRxReadyNext(capEvt_t *pEvt);

extern void capiActStopRelAseNext(capEvt_t *pEvt);

extern void capiActCfgBas(capEvt_t *pEvt);
extern void capiActStartBcastAnnc(capEvt_t *pEvt);
extern void capiActStartBasicAnnc(capEvt_t *pEvt);
extern void capiActEstablishBas(capEvt_t *pEvt);
extern void capiActUpdateBasicAnnc(capEvt_t *pEvt);
extern void capiActUpdateMetadata(capEvt_t *pEvt);
extern void capiActDisableBas(capEvt_t *pEvt);
extern void capiActReleaseBas(capEvt_t *pEvt);
extern void capiActStopBcastAnnc(capEvt_t *pEvt);
extern void capiActDeleteBase(capEvt_t *pEvt);

/* Commander action functions */
extern void capcActSetAbsVolNext(capEvt_t *pEvt);
extern void capcActSetVolOffsetNext(capEvt_t *pEvt);
extern void capcActSetVcpMuteNext(capEvt_t *pEvt);
extern void capcActSetMicpMuteNext(capEvt_t *pEvt);
extern void capcActSetMicpGainSettingNext(capEvt_t *pEvt);
extern void capcActSyncSetMetadataNext(capEvt_t *pEvt);
extern void capcActStopBcastAudioRxNext(capEvt_t *pEvt);
extern void capcActStopPaSyncNext(capEvt_t* pEvt);
extern void capcActRemoveBcastSrcNext(capEvt_t *pEvt);
extern void capcActSetBcastCodeNext(capEvt_t *pEvt);

extern void capExecuteEvent(capEvt_t *pEvt);

extern const capProcTblEntry_t capCommonProcessLockState[];
extern const capProcTblEntry_t capCommonProcessReleasingState[];
extern const capProcTblEntry_t capCommonProcessProcCompleteState[];

extern uint8_t capGetCoordinatedSet(capSetId_t setId, dmConnId_t **pConnIdList);

extern dmConnId_t capGetNextConnId(dmConnId_t connId, bool reversed);

extern void capSetCbackEvt(uint8_t event, uint8_t status, uint16_t param);

extern void capiProcMsg(wsfMsgHdr_t *pEvt);
extern void capaProcMsg(wsfMsgHdr_t *pEvt);
extern void capcProcMsg(wsfMsgHdr_t *pEvt);

/* Subsystem initialization */
extern void capiInit(void);
extern void capcInit(void);
extern void capaInit(void);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/



#ifdef __cplusplus
};
#endif

#endif /* CAP_MAIN_H */
