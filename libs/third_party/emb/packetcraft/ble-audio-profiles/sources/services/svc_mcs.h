/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Example Media Control Service implementation.
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

#ifndef SVC_MCS_H
#define SVC_MCS_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup MEDIA_CONTROL_SERVICE
 *  \{ */

/**************************************************************************************************
Constants
**************************************************************************************************/

/** \name
 * .
 */
/**@{*/

/* Characteristic value lengths */
#define MCS_MAX_STRING_LEN              64
#define MCS_PLAYER_NAME_LEN             MCS_MAX_STRING_LEN
#define MCS_ICON_URI_LEN                MCS_MAX_STRING_LEN
#define MCS_TRACK_TITLE_LEN             MCS_MAX_STRING_LEN
#define MCS_TRACK_DURATION_LEN          4
#define MCS_TRACK_POSITION_LEN          4
#define MCS_PLAYBACK_SPEED_LEN          1
#define MCS_SEEK_SPEED_LEN              1
#define MCS_PLAYING_ORDER_LEN           1
#define MCS_PLAYING_ORDER_SUP_LEN       2
#define MCS_STATE_LEN                   1
#define MCS_CONTROL_POINT_LEN           5
#define MCS_CP_OPCODE_LEN               1
#define MCS_CONTROL_PT_OC_SUP_LEN       1
#define MCS_SEARCH_CP_LEN               MCS_MAX_STRING_LEN
#define MCS_SEARCH_CP_HDR_LEN           2
#define MCS_CCID_LEN                    1

/* Notification value lengths */
#define MCS_CONTROL_POINT_NTF_LEN       2

/* Play order values */
#define MCS_PLAY_ORDER_SINGLE_ONCE      0x01
#define MCS_PLAY_ORDER_SINGLE_REPEAT    0x02
#define MCS_PLAY_ORDER_IN_ORDER_ONCE    0x03
#define MCS_PLAY_ORDER_IN_ORDER_REPEAT  0x04
#define MCS_PLAY_ORDER_OLDEST_ONCE      0x05
#define MCS_PLAY_ORDER_OLDEST_REPEAT    0x06
#define MCS_PLAY_ORDER_NEWEST_ONCE      0x07
#define MCS_PLAY_ORDER_NEWEST_REPEAT    0x08
#define MCS_PLAY_ORDER_SHUFFLE_ONCE     0x09
#define MCS_PLAY_ORDER_SHUFFLE_REPEAT   0x0A

/* Play orders supported bits */
#define MCS_PO_SUPPORT_SINGLE_ONCE      0x0001
#define MCS_PO_SUPPORT_SINGLE_REPEAT    0x0002
#define MCS_PO_SUPPORT_IN_ORDER_ONCE    0x0004
#define MCS_PO_SUPPORT_IN_ORDER_REPEAT  0x0008
#define MCS_PO_SUPPORT_OLDEST_ONCE      0x0010
#define MCS_PO_SUPPORT_OLDEST_REPEAT    0x0020
#define MCS_PO_SUPPORT_NEWEST_ONCE      0x0040
#define MCS_PO_SUPPORT_NEWEST_REPEAT    0x0080
#define MCS_PO_SUPPORT_SHUFFLE_ONCE     0x0100
#define MCS_PO_SUPPORT_SHUFFLE_REPEAT   0x0200
#define MCS_PO_SUPPORT_ALL              0x03FF

/* Media control states */
#define MCS_STATE_INACTIVE              0x00
#define MCS_STATE_PLAYING               0x01
#define MCS_STATE_PAUSED                0x02
#define MCS_STATE_SEEKING               0x03

/* Media control point opcodes */
#define MCS_CP_OP_PLAY                  0x01
#define MCS_CP_OP_PAUSE                 0x02
#define MCS_CP_OP_FAST_RW               0x03
#define MCS_CP_OP_FAST_FW               0x04
#define MCS_CP_OP_STOP                  0x05
#define MCS_CP_OP_MOVE_REL              0x10
#define MCS_CP_OP_PREV_SEG              0x20
#define MCS_CP_OP_NEXT_SEG              0x21
#define MCS_CP_OP_FIRST_SEG             0x22
#define MCS_CP_OP_LAST_SEG              0x23
#define MCS_CP_OP_GOTO_SEG              0x24
#define MCS_CP_OP_PREV_TRACK            0x30
#define MCS_CP_OP_NEXT_TRACK            0x31
#define MCS_CP_OP_FIRST_TRACK           0x32
#define MCS_CP_OP_LAST_TRACK            0x33
#define MCS_CP_OP_GOTO_TRACK            0x34
#define MCS_CP_OP_PREV_GROUP            0x40
#define MCS_CP_OP_NEXT_GROUP            0x41
#define MCS_CP_OP_FIRST_GROUP           0x42
#define MCS_CP_OP_LAST_GROUP            0x43
#define MCS_CP_OP_GOTO_GROUP            0x44

/* Opcode sets */
#define MCS_CP_OP_MOVE_SET              0x10
#define MCS_CP_OP_SEGMENT_SET           0x20
#define MCS_CP_OP_TRACK_SET             0x30
#define MCS_CP_OP_GROUP_SET             0x40

/* Opcode set mask */
#define MCS_CP_OP_SET_MASK              0x70

/* Media control point result codes */
#define  MCS_CP_RESULT_SUCCESS          0x01
#define  MCS_CP_RESULT_NOT_SUPPORTED    0x02
#define  MCS_CP_RESULT_INACTIVE         0x03
#define  MCS_CP_RESULT_NOT_COMPLETED    0x04
#define  MCS_CP_RESULT_UNSUCCESSFUL     0x05

/* Media control point opcode supported bits */
#define MCS_CO_SUPPORT_PLAY             0x00000001
#define MCS_CO_SUPPORT_PAUSE            0x00000002
#define MCS_CO_SUPPORT_FAST_RW          0x00000004
#define MCS_CO_SUPPORT_FAST_FW          0x00000008
#define MCS_CO_SUPPORT_STOP             0x00000010
#define MCS_CO_SUPPORT_MOVE_REL         0x00000020
#define MCS_CO_SUPPORT_PREV_SEG         0x00000040
#define MCS_CO_SUPPORT_NEXT_SEG         0x00000080
#define MCS_CO_SUPPORT_FIRST_SEG        0x00000100
#define MCS_CO_SUPPORT_LAST_SEG         0x00000200
#define MCS_CO_SUPPORT_GOTO_SEG         0x00000400
#define MCS_CO_SUPPORT_PREV_TRACK       0x00000800
#define MCS_CO_SUPPORT_NEXT_TRACK       0x00001000
#define MCS_CO_SUPPORT_FIRST_TRACK      0x00002000
#define MCS_CO_SUPPORT_LAST_TRACK       0x00004000
#define MCS_CO_SUPPORT_GOTO_TRACK       0x00008000
#define MCS_CO_SUPPORT_PREV_GROUP       0x00010000
#define MCS_CO_SUPPORT_NEXT_GROUP       0x00020000
#define MCS_CO_SUPPORT_FIRST_GROUP      0x00040000
#define MCS_CO_SUPPORT_LAST_GROUP       0x00080000
#define MCS_CO_SUPPORT_GOTO_GROUP       0x00100000
#define MCS_CO_SUPPORT_ALL              0x001FFFFF

/* Media control point opcode category bits. */
#define MCS_STATE_CP_CAT_SEGMENT        0x20
#define MCS_STATE_CP_CAT_TRACK          0x30
#define MCS_STATE_CP_CAT_GROUP          0x40
#define MCS_STATE_CP_CAT_MASK           0x70

/* Media control point opcode direction bits. */
#define MCS_STATE_CP_DIR_PREV           0x00
#define MCS_STATE_CP_DIR_NEXT           0x01
#define MCS_STATE_CP_DIR_FIRST          0x02
#define MCS_STATE_CP_DIR_LAST           0x03
#define MCS_STATE_CP_DIR_GOTO           0x04
#define MCS_STATE_CP_DIR_MASK           0x07

/* CCCD Index Offsets */
#define MCS_NTF_IDX_NAME               0
#define MCS_NTF_IDX_TRACK_CHANGED      1
#define MCS_NTF_IDX_TRACK_TITLE        2
#define MCS_NTF_IDX_TRACK_DURATION     3
#define MCS_NTF_IDX_TRACK_POS          4
#define MCS_NTF_IDX_PLAY_SPEED         5
#define MCS_NTF_IDX_SEEK_SPEED         6
#define MCS_NTF_IDX_PLAY_ORDER         7
#define MCS_NTF_IDX_STATE              8
#define MCS_NTF_IDX_CTRL_PT            9
#define MCS_NTF_IDX_CP_OC_SUP          10

/* CCCD Index Offsets */
#define MCS_CCCD_IDX_NAME               MCS_NTF_IDX_NAME
#define MCS_CCCD_IDX_TRACK_CHANGED      MCS_NTF_IDX_TRACK_CHANGED
#define MCS_CCCD_IDX_TRACK_TITLE        MCS_NTF_IDX_TRACK_TITLE
#define MCS_CCCD_IDX_TRACK_DURATION     MCS_NTF_IDX_TRACK_DURATION
#define MCS_CCCD_IDX_TRACK_POS          MCS_NTF_IDX_TRACK_POS
#define MCS_CCCD_IDX_PLAY_SPEED         MCS_NTF_IDX_PLAY_SPEED
#define MCS_CCCD_IDX_SEEK_SPEED         MCS_NTF_IDX_SEEK_SPEED
#define MCS_CCCD_IDX_PLAY_ORDER         MCS_NTF_IDX_PLAY_ORDER
#define MCS_CCCD_IDX_STATE              MCS_NTF_IDX_STATE
#define MCS_CCCD_IDX_CTRL_PT            MCS_NTF_IDX_CTRL_PT
#define MCS_CCCD_IDX_CP_OC_SUP          MCS_NTF_IDX_CP_OC_SUP

/* Number of CCCD attributes per service */
#define MCS_NUM_CCCD                    11

/* Max supported media control services for the device. */
/* TODO: A device can have multiple media control services.
 *       Modify the max services as needed. */
#define MCS_SVC_MAX                     2

/* Unknown MCS service ID */
#define MCS_SVC_UNKNOWN                 0xFF

/*!< \brief  ATT Application error codes */
#define MCS_ERR_VALUE_CHANGED           0x80  /*!< Value Changed During Read Long */

/**@}*/

/**************************************************************************************************
 Handle Ranges
**************************************************************************************************/

/** \name Media Control Service Handles
 *
 */
/**@{*/

/*!< \brief Start handle. */
#define MCS_START_HDL                   0x0650

/*!< \brief Number of MCS handles. */
#define MCS_NUM_HDL                     (MCS_MAX_HDL_0 - MCS_START_HDL)

/*!< \brief Base value handles. */
#define MCS_NAME_HDL                    MCS_NAME_HDL_0
#define MCS_ICON_URL                    MCS_ICON_URL_0
#define MCS_TRK_CHANGED_HDL             MCS_TRK_CHANGED_HDL_0
#define MCS_TRK_TITLE_HDL               MCS_TRK_TITLE_HDL_0
#define MCS_TRK_DUR_HDL                 MCS_TRK_DUR_HDL_0
#define MCS_TRK_POS_HDL                 MCS_TRK_POS_HDL_0
#define MCS_PB_SPEED_HDL                MCS_PB_SPEED_HDL_0
#define MCS_SEEK_SPEED_HDL              MCS_SEEK_SPEED_HDL_0
#define MCS_ORDER_HDL                   MCS_ORDER_HDL_0
#define MCS_ORDER_SUP_HDL               MCS_ORDER_SUP_HDL_0
#define MCS_STATE_HDL                   MCS_STATE_HDL_0
#define MCS_CP_HDL                      MCS_CP_HDL_0
#define MCS_CP_OC_SUP_HDL               MCS_CP_OC_SUP_HDL_0
#define MCS_SEARCH_CP_HDL               MCS_SEARCH_CP_HDL_0
#define MCS_CCID_HDL                    MCS_CCID_HDL_0

/* Calculate an value handle from a base value handle and a service id. */
#define MCS_HANDLE_ID(base, index)      (base + MCS_NUM_HDL * index)

/**************************************************************************************************
 Handles
**************************************************************************************************/

/*! \brief Media Control Service Handles macro */
#define MCS_HANDLE_DEFS(_a, b)  \
enum \
{ \
  MCS_SVC_HDL_##_a = b,             /*!< \brief Media Control Service declaration */\
  MCS_NAME_CH_HDL_##_a,             /*!< \brief Media Control player name characteristic */\
  MCS_NAME_HDL_##_a,                /*!< \brief Media Control player name */\
  MCS_NAME_CH_CCC_HDL_##_a,         /*!< \brief Media Control player name CCCD */\
  MCS_ICON_URL_CH_HDL_##_a,         /*!< \brief Media Control icon url characteristic */\
  MCS_ICON_URL_##_a,                /*!< \brief Media Control icon url */\
  MCS_TRK_CHANGED_CH_HDL_##_a,      /*!< \brief Media Control track changed characteristic */\
  MCS_TRK_CHANGED_HDL_##_a,         /*!< \brief Media Control track changed */\
  MCS_TRK_CHANGED_CH_CCC_HDL_##_a,  /*!< \brief Media Control track changed CCCD */\
  MCS_TRK_TITLE_CH_HDL_##_a,        /*!< \brief Media Control track title characteristic */\
  MCS_TRK_TITLE_HDL_##_a,           /*!< \brief Media Control track title */\
  MCS_TRK_TITLE_CH_CCC_HDL_##_a,    /*!< \brief Media Control track title CCCD */\
  MCS_TRK_DUR_CH_HDL_##_a,          /*!< \brief Media Control track duration characteristic */\
  MCS_TRK_DUR_HDL_##_a,             /*!< \brief Media Control track duration */\
  MCS_TRK_DUR_CH_CCC_HDL_##_a,      /*!< \brief Media Control track duration CCCD */\
  MCS_TRK_POS_CH_HDL_##_a,          /*!< \brief Media Control track position characteristic */\
  MCS_TRK_POS_HDL_##_a,             /*!< \brief Media Control track position */\
  MCS_TRK_POS_CH_CCC_HDL_##_a,      /*!< \brief Media Control track position CCCD */\
  MCS_PB_SPEED_CH_HDL_##_a,         /*!< \brief Media Control playback speed characteristic */\
  MCS_PB_SPEED_HDL_##_a,            /*!< \brief Media Control playback speed */\
  MCS_PB_SPEED_CH_CCC_HDL_##_a,     /*!< \brief Media Control playback speed CCCD */\
  MCS_SEEK_SPEED_CH_HDL_##_a,       /*!< \brief Media Control seek speed characteristic */\
  MCS_SEEK_SPEED_HDL_##_a,          /*!< \brief Media Control seek speed */\
  MCS_SEEK_SPEED_CH_CCC_HDL_##_a,   /*!< \brief Media Control seek speed CCCD */\
  MCS_ORDER_CH_HDL_##_a,            /*!< \brief Media Control play order characteristic */\
  MCS_ORDER_HDL_##_a,               /*!< \brief Media Control play order */\
  MCS_ORDER_CH_CCC_HDL_##_a,        /*!< \brief Media Control play order CCCD */\
  MCS_ORDER_SUP_CH_HDL_##_a,        /*!< \brief Media Control play order supported characteristic */\
  MCS_ORDER_SUP_HDL_##_a,           /*!< \brief Media Control play order supported */\
  MCS_STATE_CH_HDL_##_a,            /*!< \brief Media Control media state characteristic */\
  MCS_STATE_HDL_##_a,               /*!< \brief Media Control media state */\
  MCS_STATE_CH_CCC_HDL_##_a,        /*!< \brief Media Control media state CCCD */\
  MCS_CP_CH_HDL_##_a,               /*!< \brief Media Control media control point characteristic */\
  MCS_CP_HDL_##_a,                  /*!< \brief Media Control media control point */\
  MCS_CP_CH_CCC_HDL_##_a,           /*!< \brief Media Control media control point CCCD */\
  MCS_CP_OC_SUP_CH_HDL_##_a,        /*!< \brief Media Control media control point opcodes supported characteristic */\
  MCS_CP_OC_SUP_HDL_##_a,           /*!< \brief Media Control media control point opcodes supported */\
  MCS_CP_OC_SUP_CH_CCC_HDL_##_a,    /*!< \brief Media Control media control point opcodes supported CCCD */\
  MCS_CCID_CH_HDL_##_a,             /*!< \brief Media Control content control ID characteristic */\
  MCS_CCID_HDL_##_a,                /*!< \brief Media Control content control ID */\
  MCS_MAX_HDL_##_a                  /*!< \brief Maximum handle. */\
};
/**@}*/

/*! \brief Media Control Service Handles */
/* TODO: A device can have multiple media control services.
 *       Add or remove handle definitions as needed. */
MCS_HANDLE_DEFS(0, MCS_START_HDL)
MCS_HANDLE_DEFS(1, MCS_MAX_HDL_0)

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
void SvcMcsAddGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Remove the services from the attribute server.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcMcsRemoveGroup(void);

/*************************************************************************************************/
/*!
 *  \brief  Register callbacks for the service.
 *
 *  \param  writeCback  Write callback function.
 *  \param  readCback   Read callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SvcMcsCbackRegister(attsWriteCback_t writeCback, attsReadCback_t readCback);

/*! \} */    /* MEDIA_CONTROL_SERVICE */

#ifdef __cplusplus
};
#endif

#endif /* SVC_MCS_H */
