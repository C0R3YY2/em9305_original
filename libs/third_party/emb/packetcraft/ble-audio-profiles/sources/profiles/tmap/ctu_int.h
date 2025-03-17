/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  TMAP Unicast Sink internal headfile.
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

#ifndef TMAPCTU_INT_H
#define TMAPCTU_INT_H


#ifdef __cplusplus
extern "C" {
#endif

enum
{
  TMASCTUNICAST_GATT_SC_CCC_IDX,               /*! GATT service, service changed characteristic */
  TMASCTUNICAST_SNK_ASE_CCC_1_IDX,             /*! ASE service, Sink ASE #1 characteristic */
  TMASCTUNICAST_SNK_ASE_CCC_2_IDX,             /*! ASE service, Sink ASE #2 characteristic */
  TMASCTUNICAST_SRC_ASE_CCC_1_IDX,             /*! ASE service, Source ASE #1 characteristic */
  TMASCTUNICAST_SRC_ASE_CCC_2_IDX,             /*! ASE service, Source ASE #2 characteristic */
  TMASCTUNICAST_ASE_CP_CCC_IDX,                /*! ASE service, ASE control point characteristic */
  TMASCTUNICAST_SNK_PAC_CCC_IDX,               /*! PAC service, sink PAC characteristic */
  TMASCTUNICAST_SNK_LOC_CCC_IDX,               /*! PAC service, sink audio locations characteristic */
  TMASCTUNICAST_SRC_PAC_CCC_IDX,               /*! PAC service, source PAC characteristic */
  TMASCTUNICAST_SRC_LOC_CCC_IDX,               /*! PAC service, source audio locations characteristic */
  TMASCTUNICAST_AV_CTX_CCC_IDX,                /*! PAC service, avaible audio contexts characteristic */
  TMASCTUNICAST_SUP_CTX_CCC_IDX,               /*! PAC service, supported audio contexts characteristic */
  TMAPCTU_VCP_GATT_SC_CCC_IDX,                 /*! GATT service, service changed characteristic */
  TMAPCTU_VCP_VCPS_STATE_CCC_IDX,              /*! Volume control service state characteristic */
  TMAPCTU_VCP_VCPS_FLAGS_CCC_IDX,              /*! Volume control service flags characteristic */
  TMAPCTU_VCP_VOCS_STATE_CCC_IDX_0,            /*! VOCS volume state characteristic */               \
  TMAPCTU_VCP_VOCS_LOCATION_CCC_IDX_0,         /*! VOCS location characteristic */                   \
  TMAPCTU_VCP_VOCS_DESC_CCC_IDX_0,             /*! VOCS audio output description characteristic */
  TMAPCTU_VCP_VOCS_STATE_CCC_IDX_1,            /*! VOCS volume state characteristic */               \
  TMAPCTU_VCP_VOCS_LOCATION_CCC_IDX_1,         /*! VOCS location characteristic */                   \
  TMAPCTU_VCP_VOCS_DESC_CCC_IDX_1,             /*! VOCS audio output description characteristic */
  TMAPCTU_MICP_MUTE_CCC_IDX,                   /*! Microphone service mute characteristic */
  TMAPCTU_AICS_STATE_CCC_IDX_0,                /*! AICS input state characteristic */                \
  TMAPCTU_AICS_STATUS_CCC_IDX_0,               /*! AICS input status characteristic */               \
  TMAPCTU_AICS_AID_CCC_IDX_0,                  /*! AICS audio input description characteristic */
  TMAPCTU_AICS_STATE_CCC_IDX_1,                /*! AICS input state characteristic */                \
  TMAPCTU_AICS_STATUS_CCC_IDX_1,               /*! AICS input status characteristic */               \
  TMAPCTU_AICS_AID_CCC_IDX_1,                  /*! AICS audio input description characteristic */
  TMAPCTU_CSIS_SIRK_CCC_IDX_0,                 /*! CSIS SIRK characteristic */              \
  TMAPCTU_CSIS_SIZE_CCC_IDX_0,                 /*! CSIS size characteristic */              \
  TMAPCTU_CSIS_LOCK_CCC_IDX_0,                 /*! CSIS lock characteristic */
  TMAPCTU_CSIS_SIRK_CCC_IDX_1,                 /*! CSIS SIRK characteristic */              \
  TMAPCTU_CSIS_SIZE_CCC_IDX_1,                 /*! CSIS size characteristic */              \
  TMAPCTU_CSIS_LOCK_CCC_IDX_1,                 /*! CSIS lock characteristic */
  TMASCTUNICAST_NUM_CCC_IDX
};

#ifdef __cplusplus
};
#endif

#endif /* TMAPCTU_INT_H */
