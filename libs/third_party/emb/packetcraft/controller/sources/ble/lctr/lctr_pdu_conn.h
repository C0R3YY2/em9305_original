/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller data channel packet interface file.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_PDU_CONN_SLAVE_H
#define LCTR_PDU_CONN_SLAVE_H

#include "wsf_types.h"
#include "bb_ble_api.h"
#include "ll_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

#define LCTR_OFFSET_COUNT         6       /*!< Number of offsets in a connection parameter request. */

/*! \brief      Packet boundary flags. */
enum
{
  LCTR_PB_START_NON_AUTO_FLUSH  = 0,    /*!< Start of a non-automatically flushable L2CAP PDU. */
  LCTR_PB_CONT_FRAG             = 1,    /*!< Continuing fragment of Higher Layer Message. */
  LCTR_PB_START_AUTO_FLUSH      = 2,    /*!< Start of an automatically flushable L2CAP PDU. */
  /* N.B. next two enumerations intentionally use identical values. */
  LCTR_PB_COMPLETE_AUTO_FLUSH   = 3,    /*!< A complete L2CAP PDU automatically flushable (not allowed for LE-U). */
  LCTR_PB_VS_DATA               = 3     /*!< Vendor specific data. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ACL header. */
typedef struct
{
  uint16_t          connHandle;         /*!< Connection handle. */
  uint8_t           pktBound;           /*!< Packet boundary flag. */
  uint16_t          len;                /*!< Data length. */
} lctrAclHdr_t;

/*! \brief      Data channel PDU header. */
typedef struct
{
  uint8_t           llid;               /*!< PDU type. */
  uint8_t           nesn;               /*!< Next Expected Sequence Number. */
  uint8_t           sn;                 /*!< Sequence Number. */
  uint8_t           md;                 /*!< More Data. */
  uint8_t           cp;                 /*!< CTE Present. */
  uint8_t           len;                /*!< Payload length. */

  /* CTE Info field. */
  uint8_t           cteType;            /*!< CTE Type. */
  uint8_t           cteTime;            /*!< CTE Time. */
} lctrDataPduHdr_t;

/*! \brief      Connection update indication PDU. */
typedef struct
{
  uint8_t           txWinSize;          /*!< transmitWindowSize value. */
  uint16_t          txWinOffset;        /*!< transmitWindowOffset value. */
  uint16_t          interval;           /*!< connInterval value. */
  uint16_t          latency;            /*!< connSlaveLatency value. */
  uint16_t          timeout;            /*!< connSupervisionTimeout value. */
  uint16_t          instant;            /*!< connInstant value. */
} lctrConnUpdInd_t;

/*! \brief      Connection update indication PDU. */
typedef struct
{
  uint64_t          chanMask;           /*!< Channel mask. */
  uint16_t          instant;            /*!< connInstant value. */
} lctrChanMapInd_t;

/*! \brief      Terminate indication control PDU. */
typedef struct
{
  uint8_t           errorCode;          /*!< Error code. */
} lctrTermInd_t;

/*! \brief      Encryption request control PDU. */
typedef struct
{
  uint8_t           rand[LL_RAND_LEN];  /*!< Random number. */
  uint16_t          ediv;               /*!< Encrypted diversifier. */
  uint8_t           skd_m[LL_SKD_LEN/2];/*!< Master's session key diversifier. */
  uint8_t           iv_m[LL_IV_LEN/2];  /*!< Master's initialization vector. */
} lctrEncReq_t;

/*! \brief      Encryption response control PDU. */
typedef struct
{
  uint8_t           skd_s[LL_SKD_LEN/2];/*!< Slave's session key diversifier. */
  uint8_t           iv_s[LL_IV_LEN/2];  /*!< Slave's initialization vector. */
} lctrEncRsp_t;

/*! \brief      Unknown response control PDU. */
typedef struct
{
  uint8_t           unknownType;        /*!< Unknown type. */
} lctrUnknownRsp_t;

/*! \brief      Feature request or response control PDU. */
typedef struct
{
  uint64_t          featSet;            /*!< Feature set. */
} lctrFeat_t;

/*! \brief      Connection parameter request/response values. */
typedef struct
{
  uint16_t          connIntervalMin;    /*!< Minimum connection interval. */
  uint16_t          connIntervalMax;    /*!< Maximum connection interval. */
  uint16_t          connLatency;        /*!< Connection latency. */
  uint16_t          supTimeout;         /*!< Supervision timeout. */
  uint8_t           prefPeriod;         /*!< Preferred periodicity. */
  uint16_t          refConnEvtCnt;      /*!< Reference connection event count. */
  uint16_t          offset[LCTR_OFFSET_COUNT];      /*!< Anchor point offset from \a refConnEvtCnt, first preference. */
} lctrConnParam_t;

/*! \brief      Version indication control PDU. */
typedef struct
{
  uint8_t           versNr;             /*!< Bluetooth controller specification version. */
  uint16_t          compId;             /*!< Company identifier. */
  uint16_t          subVersNr;          /*!< Sub-Version number. */
} lctrVerInd_t;

/*! \brief      Reject indication control PDU. */
typedef struct
{
  uint8_t           opcode;             /*!< Reject opcode. */
  uint8_t           reason;             /*!< Reject reason code. */
} lctrRejInd_t;

/*! \brief      Data length request or response PDU. */
typedef struct
{
  uint16_t          maxRxLen;           /*!< Maximum receive length. */
  uint16_t          maxRxTime;          /*!< Maximum receive time. */
  uint16_t          maxTxLen;           /*!< Maximum transmit length. */
  uint16_t          maxTxTime;          /*!< Maximum transmit time. */
} lctrDataLen_t;

/*! \brief      PHY request or response PDU. */
typedef struct
{
  uint8_t           txPhys;             /*!< Transmitter PHYs. */
  uint8_t           rxPhys;             /*!< Receiver PHYs. */
} lctrPhy_t;

/*! \brief      PHY update indication PDU. */
typedef struct
{
  uint8_t           masterToSlavePhy;   /*!< Master-to-slave PHY. */
  uint8_t           slaveToMasterPhy;   /*!< Slave-to-master PHY. */
  uint16_t          instant;            /*!< Instant. */
} lctrPhyUpdInd_t;

/*! \brief      Minimum number of used channels indication PDU. */
typedef struct
{
  uint8_t           phys;               /*!< Bitmask for the affected PHYs. */
  uint8_t           minUsedChan;        /*!< Minimum number of used channels. */
} lctrMinUsedChanInd_t;

/*! \brief      Periodic sync indication PDU. */
typedef struct
{
  uint16_t          id;                 /*!< identifier provided by the host. */
  uint8_t           syncInfo[LL_SYNC_INFO_LEN];   /*!< Syncinfo field. */
  uint16_t          ceCounter;          /*!< Reference connection event counter. */
  uint16_t          lastPECounter;      /*!< paEventCounter applying to AUX_SYNC_IND PDU. */
  uint8_t           sid;                /*!< Advertising SID subfield. */
  uint8_t           aType;              /*!< Address type of periodic advertising. */
  uint8_t           sca;                /*!< Sleep clock accuracy of the device sending this PDU. */
  uint8_t           phy;                /*!< PHY used by the periodic advertising. */
  uint64_t          advA;               /*!< Advertiser's address in the periodic advertising. */
  uint16_t          syncConnEvtCounter; /*!< Connection event counter when the contents of the PDU is determined. */
} lctrPerSyncInd_t;

/*! \brief      Periodic sync with response indication PDU. */
typedef struct
{
  uint16_t          id;                 /*!< identifier provided by the host. */
  uint8_t           syncInfo[LL_SYNC_INFO_LEN];   /*!< Syncinfo field. */
  uint16_t          ceCounter;          /*!< Reference connection event counter. */
  uint16_t          lastPECounter;      /*!< paEventCounter applying to AUX_SYNC_IND PDU. */
  uint8_t           sid;                /*!< Advertising SID subfield. */
  uint8_t           aType;              /*!< Address type of periodic advertising. */
  uint8_t           sca;                /*!< Sleep clock accuracy of the device sending this PDU. */
  uint8_t           phy;                /*!< PHY used by the periodic advertising. */
  uint64_t          advA;               /*!< Advertiser's address in the periodic advertising. */
  uint16_t          syncConnEvtCounter; /*!< Connection event counter when the contents of the PDU is determined. */
  uint32_t          rspAA;              /*!< Access address to be used when transmitting a response packet to advertiser. */
  uint8_t           numSubevents;       /*!< Number of subevents. */
  uint8_t           subeventInter;      /*!< Time in 1.25 ms units from the start of one subevent to the start of the next subevent. */
  uint8_t           rspSlotDelay;       /*!< Time in 1.25 ms units from the start of one subevent to the first response slot. */
  uint8_t           rspSlotSpacing;     /*!< Time in 0.625 ms units from the start of one response slot to the start of the next response slot. */
} lctrPerSyncWithRspInd_t;

/*! \brief      CTE request PDU. */
typedef struct
{
  uint8_t          reqCteTime; /*!< Requested CTE Time. */
  uint8_t          reqCteType; /*!< Requested CTE Type. */
} lctrCteReq_t;

/*! \brief      Peer SCA response PDU. */
typedef struct
{
  uint8_t           sca;                /*!< Peer SCA. */
} lctrPeerSca_t;

/*! \brief      CIS request PDU. */
typedef struct
{
  uint8_t           cigId;              /*!< CIG identifier. */
  uint8_t           cisId;              /*!< CIS identifier. */
  uint8_t           phyMToS;            /*!< Master to slave PHY. */
  uint8_t           phySToM;            /*!< Slave to Master PHY. */
  LlFraming_t       framing;            /*!< PDU framing type. */
  uint16_t          sduSizeMToS;        /*!< Maximum SDU size from the master Host. */
  uint16_t          sduSizeSToM;        /*!< Maximum SDU size from the slave Host. */
  uint32_t          sduIntervalMToS;    /*!< Time interval between the start of consecutive SDUs from the master Host  */
  uint32_t          sduIntervalSToM;    /*!< Time interval between the start of consecutive SDUs from the master Host  */
  uint16_t          plMToS;             /*!< Master to slave payload. */
  uint16_t          plSToM;             /*!< Slave to master payload. */
  uint8_t           nse;                /*!< Number of subevent. */
  uint32_t          subIntervUsec;      /*!< Contain the time between the start of a subevent and the start of the next subevent, 24 significant bits. */
  uint8_t           bnMToS;             /*!< Master to slave burst number, 4 significant bits. */
  uint8_t           bnSToM;             /*!< Slave to master burst number, 4 significant bits. */
  uint8_t           ftMToS;             /*!< Master to slave flush time. */
  uint8_t           ftSToM;             /*!< Slave to master flush time. */
  uint16_t          isoInterval;        /*!< Contain the time between two CIS anchor points in 1.25msec unit. */
  uint32_t          cisOffMinUsec;      /*!< Contain the minimum time between the CE and the first CIS anchor point. */
  uint32_t          cisOffMaxUsec;      /*!< Contain the maximum time between the CE and the first CIS anchor point. */
  uint16_t          ceRef;              /*!< Contain the reference CE where offsets are applied. */
} lctrCisReq_t;

/*! \brief      CIS response PDU. */
typedef struct
{
  uint32_t          cisOffMinUsec;      /*!< Contain the minimum time between the CE and the first CIS anchor point. */
  uint32_t          cisOffMaxUsec;      /*!< Contain the maximum time between the CE and the first CIS anchor point. */
  uint16_t          ceRef;              /*!< Contain the reference CE where offsets are applied. */
} lctrCisRsp_t;

/*! \brief      CIS indication PDU. */
typedef struct
{
  uint32_t          accessAddr;         /*!< Contain the access address of the CIS. */
  uint32_t          cisOffUsec;         /*!< Contain the time from the start of the referenced CE to the first CIS anchor point. */
  uint32_t          cigSyncDelayUsec;   /*!< CIG synchronization delay in usec. */
  uint32_t          cisSyncDelayUsec;   /*!< CIG synchronization delay in usec. */
  uint16_t          ceRef;              /*!< Contain the reference CE where offsets are applied. */
} lctrCisInd_t;

/*! \brief      CIS terminate PDU. */
typedef struct
{
  uint8_t           cigId;              /*!< CIG identifier. */
  uint8_t           cisId;              /*!< CIS identifier. */
  uint8_t           reason;             /*!< Reason for termination. */
} lctrCisTermInd_t;

/*! \brief      Subrate request PDU. */
typedef struct
{
  uint16_t          srMin;              /*!< Subrate minimum value. */
  uint16_t          srMax;              /*!< Subrate maximum value. */
  uint16_t          maxLatency;         /*!< Maximum latency. */
  uint16_t          contNum;            /*!< Continuation number. */
  uint16_t          svt;                /*!< Supervision timeout in 10ms units. */
} lctrSubrateReq_t;

/*! \brief      Subrate indication PDU. */
typedef struct
{
  uint16_t          srFactor;           /*!< Subrate factor. */
  uint16_t          srBaseEvent;        /*!< Subrate base event. */
  uint16_t          latency;            /*!< Latency. */
  uint16_t          contNum;            /*!< Continuation number. */
  uint16_t          svt;                /*!< Supervision timeout in 10ms units. */
} lctrSubrateInd_t;

/*! \brief      Power control request PDU. */
typedef struct
{
  uint8_t           phy;                /*!< PHY. */
  int8_t            delta;              /*!< Requested delta. */
  int8_t            txPower;            /*!< Local transmit power. */
} lctrPwrCtrlReq_t;

/*! \brief      Power control RSP PDU. */
typedef struct
{
  uint8_t           limits;             /*!< Limits field. */
  int8_t            delta;              /*!< Change in power. */
  int8_t            txPower;            /*!< Local txPower. */
  uint8_t           apr;                /*!< Acceptable power reduction. */
} lctrPwrCtrlRsp_t;

/*! \brief      Power change indication PDU. */
typedef struct
{
  uint8_t           phy;                /*!< PHY. */
  uint8_t           limits;             /*!< Limits field. */
  int8_t            delta;              /*!< Device txPower change. */
  int8_t            txPower;            /*!< Local txPower. */
} lctrPwrChngInd_t;

/*! \brief      Channel reporting indication PDU. */
typedef struct
{
  uint8_t           enable;             /*!< Enable. */
  uint8_t           minSpacing;         /*!< Minimum spacing. */
  uint8_t           maxDelay;           /*!< Maximum delay. */
} lctrChanRptInd_t;

/*! \brief      Channel status indication PDU. */
typedef struct
{
  uint8_t           chanStatusMap[LL_CH_STATUS_LEN - 1];
                                        /*!< Map of current peer channel status. */
} lctrChanStatusInd_t;

/*! \brief      CS security PDU. */
typedef struct
{
  uint8_t           csInitVector[LL_CS_SEC_INIT_VECTOR_LEN];  /*!< Security initialization vector. */
  uint8_t           csInstNonce[LL_CS_SEC_INST_NONCE_LEN];    /*!< Security instantiation nonce vector. */
  uint8_t           csPerVector[LL_CS_SEC_PER_VECTOR_LEN];    /*!< Security personalization vector. */
} lctrCsSecPdu_t;

/*! \brief      CS capabilities PDU. */
typedef struct
{
  uint8_t           modeType;           /*!< Mode types. */
  uint8_t           rttCap;             /*!< RTT capability. */
  uint8_t           rttAaOnly;          /*!< RTT Coarse support. */
  uint8_t           rttSounding;        /*!< RTT Sounding support. */
  uint8_t           rttRand;            /*!< RTT Random sequence support. */
  uint16_t          nadmSoundCap;       /*!< Support for NADM when a CS SYNC with sounding sequence is received. */
  uint16_t          nadmRandCap;        /*!< Support for NADM when a CS SYNC with random sequence is received. */
  uint8_t           csSyncPhyCap;       /*!< PHYs supported. */
  uint8_t           numAnt;             /*!< Number of antennas. */
  uint8_t           maxAntPath;         /*!< Maximum number of antenna paths. */
  uint8_t           role;               /*!< Role. */
  uint8_t           compSignal;         /*!< Companion signal. */
  uint8_t           noFAE;              /*!< No FAE.*/
  uint8_t           chanSel3C;          /*!< Channel selction #3C. */
  uint8_t           soundingPctEst;     /*!< Sounding PCT estimate. */
  uint8_t           numCfgs;            /*!< Number of configurations. */
  uint16_t          maxProcSup;         /*!< Maximum number of procedures supported. */
  uint8_t           tSw;                /*!< T SW times. */
  uint16_t          tIp1Cap;            /*!< T IP1 capability. */
  uint16_t          tIp2Cap;            /*!< T IP2 capability. */
  uint16_t          tFcsCap;            /*!< T FCS capability. */
  uint16_t          tPmCap;             /*!< T PM capability. */
  uint8_t           txSnrCap;           /*!< TX SNR capability. */
} lctrCsCapPdu_t;

/*! \brief      CS config request PDU. */
typedef struct
{
  uint8_t           configId;           /*!< Configuration identifier. */
  uint8_t           state;              /*!< State. */
  uint8_t           chanMap[LL_CS_CHAN_MAP_SIZE]; /*!< Channel map. */
  uint8_t           chanMapRep;         /*!< Channel map repetition. */
  uint8_t           mainModeType;       /*!< Main mode type. */
  uint8_t           subModeType;        /*!< Sub mode type. */
  uint8_t           minMainModeSteps;   /*!< Minimum number of CS main mode steps to be executed before sup mode step. */
  uint8_t           maxMainModeSteps;   /*!< Maximum number of CS main mode steps to be executed before sup mode step. */
  uint8_t           mainModeRep;        /*!< Number of main mode steps taken from the end of the last CS subevent to be repeated. */
  uint8_t           mode0Steps;         /*!< Number of CS mode 0 steps to be included at the beginning of each CS subevent. */
  uint8_t           csSyncPhy;          /*!< CS Synch PHY. */
  uint8_t           rttType;            /*!< RTT type. */
  uint8_t           role;               /*!< Role. */
  uint8_t           compSignalEn;       /*!< Companion signal enable. */
  uint8_t           chanSelType;        /*!< Channel selection type. */
  uint8_t           ch3cShape;          /*!< Shape for user-specified channel sequence. */
  uint8_t           ch3cJump;           /*!< Number of channels skipped in each rising and falling sequence. */
  uint8_t           tIp1Time;           /*!< T IP1 time. */
  uint8_t           tIp2Time;           /*!< T IP2 time. */
  uint8_t           tFcsTime;           /*!< T FCS time. */
  uint8_t           tPmTime;            /*!< T PM time. */
} lctrCsConfigReq_t;

/* \brief       CS configuration response PDU. */
typedef struct
{
  uint8_t             configId;           /*!< Configuration identifier. */
} lctrCsConfigRsp_t;

/*! \brief      CS PDU. */
typedef struct
{
  uint8_t             configId;           /*!< Configuration identifier. */
  uint16_t            connEvtCnt;         /*!< Connection event count. */
  uint32_t            minOffset;          /*!< Minimum offset. */
  uint32_t            maxOffset;          /*!< Maximum offset. */
  uint32_t            offset;             /*!< Offset. */
  uint16_t            maxProcLen;         /*!< Maximum procedure length. */
  uint16_t            evtInterval;        /*!< Event interval. */
  uint8_t             subEvtPerEvt;       /*!< Subevents per event. */
  uint16_t            subEvtInterval;     /*!< Gap between the start of two consecutive CS subevents. */
  uint32_t            subEvtLen;          /*!< Duration for each CS subevent in us. */
  uint16_t            procInterval;       /*!< Procedure interval. */
  uint16_t            procCnt;            /*!< Procedure count. */
  uint8_t             aci;                /*!< ACI. */
  uint8_t             prefPeerAnt;        /*!< Preferred peer antenna. */
  uint8_t             phy;                /*!< PHY. */
  uint8_t             pwrDelta;           /*!< Power delta. */
  uint8_t             txSnrI;           /*!< Power delta. */
  uint8_t             txSnrR;           /*!< Power delta. */
} lctrCsPdu_t;

/* \brief       CS terminate indication PDU. */
typedef struct
{
  uint8_t             configId;           /*!< Configuration identifier. */
  uint16_t            procCount;          /*!< Procedure count*/
  uint8_t             error;              /*!< Error code. */
} lctrCsTermInd_t;

/* \brief       CS FAE response PDU. */
typedef struct
{
  uint8_t             chFae[LL_CS_REM_FAE_TBL_SIZE];           /*!< Mode-0 FAE table. */
} lctrCsFaeRsp_t;

/* \brief       CS channel map indication PDU. */
typedef struct
{
  uint8_t             chanMap[LL_CS_CHAN_MAP_SIZE]; /*!< Channel map. */
  uint16_t            instant;                      /*!< Instant.*/
} lctrCsChanMapInd_t;

/*! \brief      Data channel control PDU. */
typedef struct
{
  lctrDataPduHdr_t  hdr;                /*!< Unpacked PDU header. */
  uint8_t opcode;                       /*!< Control PDU opcode. */

  union
  {
    lctrConnUpdInd_t connUpdInd;        /*!< Connection update indication. */
    lctrChanMapInd_t chanMapInd;        /*!< Channel map request. */
    lctrTermInd_t    termInd;           /*!< Terminate indication. */
    lctrEncReq_t     encReq;            /*!< Encryption request. */
    lctrEncRsp_t     encRsp;            /*!< Encryption response. */
    lctrUnknownRsp_t unknownRsp;        /*!< Unknown response. */
    lctrFeat_t       featReqRsp;        /*!< Feature request or response or slave feature request. */
    lctrVerInd_t     verInd;            /*!< Version indication. */
    lctrConnParam_t  connParamReqRsp;   /*!< Connection parameter request or response. */
    lctrRejInd_t     rejInd;            /*!< Reject indication. */
    lctrDataLen_t    lenReq;            /*!< Data length request. */
    lctrDataLen_t    lenRsp;            /*!< Data length response. */
    lctrPhy_t        phyReq;            /*!< PHY request. */
    lctrPhy_t        phyRsp;            /*!< PHY response. */
    lctrPhyUpdInd_t  phyUpdInd;         /*!< PHY update indication. */
    lctrMinUsedChanInd_t minUsedChanInd;/*!< Minimum number of used channels indication. */
    lctrPerSyncInd_t perSyncInd;        /*!< Periodic sync indication. */
    lctrCteReq_t     cteReq;            /*!< CTE Request. */
    lctrPeerSca_t    peerSca;           /*!< Peer SCA request/response. */
    lctrCisReq_t     cisReq;            /*!< CIS request. */
    lctrCisRsp_t     cisRsp;            /*!< CIS response. */
    lctrCisInd_t     cisInd;            /*!< CIS indication. */
    lctrCisTermInd_t cisTerm;           /*!< CIS terminate indication. */
    lctrPwrCtrlReq_t pwrCtrlReq;        /*!< Power control request. */
    lctrPwrCtrlRsp_t pwrCtrlRsp;        /*!< Power control response. */
    lctrPwrChngInd_t pwrChngInd;        /*!< Power change indication. */
    lctrChanRptInd_t chanRptInd;        /*!< Channel report indication. */
    lctrChanStatusInd_t chanStatusInd;
                                        /*!< Channel status indication. */
    lctrSubrateReq_t subrateReq;        /*!< Subrate request. */
    lctrSubrateInd_t subrateInd;        /*!< Subrate indication. */
    lctrPerSyncWithRspInd_t perSyncWRInd;/*!< Periodic sync indication. */
#ifdef INIT_FEAT_CHANNEL_SOUNDING
    lctrCsSecPdu_t   csSecPdu;          /*!< CS security request/response. */
    lctrCsCapPdu_t   csCapPdu;          /*!< CS capabilities request/response. */
    lctrCsConfigReq_t csConfigReq;      /*!< CS configuration request. */
    lctrCsConfigRsp_t csConfigRsp;      /*!< CS configuration response. */
    lctrCsPdu_t      cs;                /*!< CS request/response/indication. */
    lctrCsTermInd_t  csTermInd;         /*!< CS termination indication. */
    lctrCsFaeRsp_t   csFaeRsp;          /*!< CS FAE response. */
    lctrCsChanMapInd_t csChanMapInd;    /*!< CS channel map indication. */
#endif /* INIT_FEAT_CHANNEL_SOUNDING */
  } pld;                                /*!< Unpacked PDU payload. */
} lctrDataPdu_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Pack */
uint8_t lctrPackAclHdr(uint8_t *pBuf, const lctrAclHdr_t *pHdr);
uint8_t lctrPackDataPduHdr(uint8_t *pBuf, const lctrDataPduHdr_t *pHdr);
uint8_t lctrPackConnUpdInd(uint8_t *pBuf, const lctrConnUpdInd_t *pPdu);

/* Unpack */
uint8_t lctrUnpackAclHdr(lctrAclHdr_t *pHdr, const uint8_t *pBuf);
uint8_t lctrUnpackDataPduHdr(lctrDataPduHdr_t *pHdr, const uint8_t *pBuf);
uint8_t lctrUnpackConnUpdateIndPdu(lctrConnUpdInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackChanMapIndPdu(lctrChanMapInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackTerminateIndPdu(lctrTermInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackUnknownRspPdu(lctrUnknownRsp_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackFeaturePdu(lctrFeat_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackRejectIndPdu(uint8_t *pReason, const uint8_t *pBuf);
uint8_t lctrUnpackVersionIndPdu(lctrVerInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackConnParamPdu(lctrConnParam_t *pConnParam, const uint8_t *pBuf);
uint8_t lctrUnpackRejectExtIndPdu(lctrRejInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackDataLengthPdu(lctrDataLen_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackPhyPdu(lctrPhy_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackPhyUpdateIndPdu(lctrPhyUpdInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackMinUsedChanIndPdu(lctrMinUsedChanInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackPerSyncIndPdu(lctrPerSyncInd_t *pPdu, const uint8_t *pBuf);
uint8_t lctrUnpackPerSyncWithRspIndPdu(lctrPerSyncWithRspInd_t *pPdu, const uint8_t *pBuf);

/* Decode */
uint8_t lctrDecodeCtrlPdu(lctrDataPdu_t *pPdu, const uint8_t *pBuf, uint8_t role);
uint8_t lctrDecodeEncPdu(lctrDataPdu_t *pPdu, const uint8_t *pBuf, uint8_t role);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_PDU_CONN_SLAVE_H */
