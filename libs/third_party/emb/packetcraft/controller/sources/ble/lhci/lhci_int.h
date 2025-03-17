/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI main module interface file.
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

#ifndef LHCI_INT_H
#define LHCI_INT_H

#include "lhci_api.h"
#include "hci_defs.h"
#include "ll_api.h"
#include "wsf_os.h"
#include "wsf_queue.h"
#include "util/bstream.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LHCI_INT
 *  \{
 */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief   Maximum number initiating of PHY. */
#define LHCI_MAX_INIT_PHY   HCI_MAX_NUM_PHYS

/*! \brief  Maximum size of Data_Length field in periodic advertising report. */
#define LHCI_PER_ADV_RPT_DATA_LEN_MAX       247

/*! \brief  Maximum size of Data_Length field in periodic advertising report V2. */
#define LHCI_PER_ADV_RPT_V2_DATA_LEN_MAX    244

/*! \brief  Maximum size of Data_Length field in periodic advertising response report. */
#define LHCI_PER_ADV_RSP_RPT_DATA_LEN_MAX   244

/* Command complete parameter lengths */

/* --- Controller Group --- */
#define LHCI_LEN_SET_EVENT_MASK_EVT             1       /*!< Set event mask command complete event length. */
#define LHCI_LEN_RESET_EVT                      1       /*!< Reset command complete event length. */
#define LHCI_LEN_READ_CONN_ACCEPT_TIMEOUT_EVT   3       /*!< Read connection accept timeout event length. */
#define LHCI_LEN_WRITE_CONN_ACCEPT_TIMEOUT_EVT  1       /*!< Read connection accept timeout event length. */
#define LHCI_LEN_READ_DAA_CHAN_ASSESSMENT_MODE  2       /*!< Read DAA channel assessment mode command complete event length. */
#define LHCI_LEN_WRITE_DAA_CHAN_ASSESSMENT_MODE 1       /*!< Write DAA channel assessment mode command complete event length. */
#define LHCI_LEN_READ_PWR_LVL_EVT               4       /*!< Read power level command complete event length. */
#define LHCI_LEN_READ_AUTH_PAYLOAD_TO_EVT       5       /*!< Read authenticated payload timeout command complete event length. */
#define LHCI_LEN_WRITE_AUTH_PAYLOAD_TO_EVT      3       /*!< Write authenticated payload timeout command complete event length. */
#define LHCI_LEN_SET_MIN_ENCRYPTION_KEY_SIZE    1       /*!< Set minimum encryption keysize command complete event length. */

/* --- Informational commands --- */
#define LHCI_LEN_READ_LOCAL_VER_EVT             9       /*!< Read local version command complete event length. */
#define LHCI_LEN_READ_LOCAL_SUP_CMDS_EVT        65      /*!< Read local supported commands command complete event length. */
#define LHCI_LEN_READ_LOCAL_SUP_FEAT_EVT        9       /*!< Read local supported feature command complete event length. */
#define LHCI_LEN_READ_BUF_SIZE_EVT              8       /*!< Read buffer size command complete event length. */
#define LHCI_LEN_READ_BD_ADDR_EVT               7       /*!< Read BD address command complete event length. */

/* --- LE controller commands --- */
#define LHCI_LEN_LE_SET_EVENT_MASK_EVT          1       /*!< LE set event mask command complete event length. */
#define LHCI_LEN_LE_READ_BUF_SIZE_EVT           4       /*!< LE read buffer size command complete event length. */
#define LHCI_LEN_LE_READ_LOCAL_SUP_FEAT_EVT     9       /*!< LE read local supported feature command complete event length. */
#define LHCI_LEN_LE_SET_RAND_ADDR_EVT           1       /*!< LE set random address command complete event length. */
#define LHCI_LEN_LE_SET_ADV_PARAM_EVT           1       /*!< LE set advertising parameter command complete event length. */
#define LHCI_LEN_LE_READ_ADV_TX_POWER_EVT       2       /*!< LE read advertising Tx power command complete event length. */
#define LHCI_LEN_LE_SET_ADV_DATA_EVT            1       /*!< LE set advertising data command complete event length. */
#define LHCI_LEN_LE_SET_SCAN_RESP_DATA_EVT      1       /*!< LE set scan response data command complete event length. */
#define LHCI_LEN_LE_SET_ADV_ENABLE_EVT          1       /*!< LE set advertising enable command complete event length. */
#define LHCI_LEN_LE_SET_SCAN_PARAM_EVT          1       /*!< LE set scan parameter command complete event length. */
#define LHCI_LEN_LE_SET_SCAN_ENABLE_EVT         1       /*!< LE set scan enable command complete event length. */
#define LHCI_LEN_LE_CREATE_CONN_CANCEL_EVT      1       /*!< LE create connection cancel command complete event length. */
#define LHCI_LEN_LE_READ_WL_SIZE_EVT            2       /*!< LE read white list size command complete event length. */
#define LHCI_LEN_LE_CLEAR_WHITE_LIST_EVT        1       /*!< LE clear white list command complete event length. */
#define LHCI_LEN_LE_ADD_DEV_WHITE_LIST_EVT      1       /*!< LE add device to white list command complete event length. */
#define LHCI_LEN_LE_REMOVE_DEV_WHITE_LIST_EVT   1       /*!< LE remove device from white list command complete event length. */
#define LHCI_LEN_LE_SET_HOST_CHAN_CLASS         1       /*!< LE set host channel class command complete event length. */
#define LHCI_LEN_LE_READ_CHAN_MAP_EVT           8       /*!< LE read channel map command complete event length. */
#define LHCI_LEN_LE_ENCRYPT_EVT                 17      /*!< LE encrypt command complete event length. */
#define LHCI_LEN_LE_LTK_REQ_REPL_EVT            3       /*!< LE LTK request reply command complete event length. */
#define LHCI_LEN_LE_LTK_REQ_NEG_REPL_EVT        3       /*!< LE LTK request negative reply command complete event length. */
#define LHCI_LEN_LE_RAND_EVT                    9       /*!< LE random command complete event length. */
#define LHCI_LEN_LE_READ_SUP_STATES_EVT         9       /*!< LE read supported states command complete event length. */
#define LHCI_LEN_LE_RECEIVER_TEST_EVT           1       /*!< LE receiver test command complete event length. */
#define LHCI_LEN_LE_TRANSMITTER_TEST_EVT        1       /*!< LE transmitter test command complete event length. */
#define LHCI_LEN_LE_TEST_END_EVT                3       /*!< LE test end command complete event length. */
/* New in version 4.1 */
#define LHCI_LEN_LE_REM_CONN_PARAM_REP          3       /*!< LE remote connection parameter reply command complete event length. */
#define LHCI_LEN_LE_REM_CONN_PARAM_NEG_REP      3       /*!< LE remote connection parameter negative reply command complete event length. */
/* New in version 4.2 */
#define LHCI_LEN_LE_SET_DATA_LEN                3       /*!< LE set data length command complete event length. */
#define LHCI_LEN_LE_READ_DEF_DATA_LEN           5       /*!< LE read default data length command complete event length. */
#define LHCI_LEN_LE_WRITE_DEF_DATA_LEN          1       /*!< LE write default data length command complete event length. */
#define LHCI_LEN_LE_ADD_DEV_RES_LIST_EVT        1       /*!< LE add device to resolving list command complete event length. */
#define LHCI_LEN_LE_REMOVE_DEV_RES_LIST_EVT     1       /*!< LE remove device from resolving list command complete event length. */
#define LHCI_LEN_LE_CLEAR_RES_LIST_EVT          1       /*!< LE clear resolving list command complete event length. */
#define LHCI_LEN_LE_READ_RES_LIST_SIZE_EVT      2       /*!< LE read resolving list size command complete event length. */
#define LHCI_LEN_LE_READ_PEER_RES_ADDR_EVT      7       /*!< LE read peer resolving address command complete event length. */
#define LHCI_LEN_LE_READ_LOCAL_RES_ADDR_EVT     7       /*!< LE read local resolving address command complete event length. */
#define LHCI_LEN_LE_SET_ADDR_RES_ENABLE_EVT     1       /*!< LE set address resolution enable command complete event length. */
#define LHCI_LEN_LE_SET_RES_PRIV_ADDR_TO_EVT    1       /*!< LE set resolving private address timeout command complete event length. */
#define LHCI_LEN_LE_READ_MAX_DATA_LEN           9       /*!< LE read maximum data length command complete event length. */
/* New in version 5.0 */
#define LHCI_LEN_LE_SET_PRIVACY_MODE            1       /*!< LE set privacy mode command complete event length. */
#define LHCI_LEN_LE_SET_ADV_SET_RAND_ADDR       1       /*!< LE set advertising set random address command complete event length. */
#define LHCI_LEN_LE_SET_EXT_ADV_PARAM           2       /*!< LE set extended advertising parameters command complete event length. */
#define LHCI_LEN_LE_SET_EXT_ADV_DATA            1       /*!< LE set extended advertising data command complete event length. */
#define LHCI_LEN_LE_SET_EXT_SCAN_RESP_DATA      1       /*!< LE set extended scan response data command complete event length. */
#define LHCI_LEN_LE_SET_EXT_ADV_ENABLE          1       /*!< LE set extended advertising enable command complete event length. */
#define LHCI_LEN_LE_READ_MAX_ADV_DATA_LEN       3       /*!< LE read maximum advertising data length command complete event length. */
#define LHCI_LEN_LE_READ_NUM_OF_SUP_ADV_SETS    2       /*!< LE read number of supported advertising sets command complete event length. */
#define LHCI_LEN_LE_REMOVE_ADV_SET              1       /*!< LE remove advertising set command complete event length. */
#define LHCI_LEN_LE_CLEAR_ADV_SETS              1       /*!< LE clear advertising sets command complete event length. */
#define LHCI_LEN_LE_SET_PER_ADV_PARAM           1       /*!< LE set periodic advertising parameters command complete event length. */
#define LHCI_LEN_LE_SET_PER_ADV_DATA            1       /*!< LE set periodic advertising data command complete event length. */
#define LHCI_LEN_LE_SET_PER_ADV_ENABLE          1       /*!< LE set periodic advertising enable command complete event length. */
#define LHCI_LEN_LE_SET_EXT_SCAN_PARAM          1       /*!< LE set extended scan parameter command complete event length. */
#define LHCI_LEN_LE_SET_EXT_SCAN_ENABLE         1       /*!< LE set extended scan enable command complete event length. */
#define LHCI_LEN_LE_PER_ADV_CREATE_SYNC_CANCEL  1       /*!< LE periodic advertising create sync cancel command complete event length. */
#define LHCI_LEN_LE_PER_ADV_TERMINATE_SYNC      1       /*!< LE periodic advertising terminate sync command complete event length. */
#define LHCI_LEN_LE_ADD_DEV_PER_ADV_LIST        1       /*!< LE add device from periodic advertising list command complete event length. */
#define LHCI_LEN_LE_REM_DEV_PER_ADV_LIST        1       /*!< LE remove device from periodic advertising list command complete event length. */
#define LHCI_LEN_LE_CLEAR_PER_ADV_LIST          1       /*!< LE clear periodic advertising list command complete event length. */
#define LHCI_LEN_LE_READ_PER_ADV_LIST_SIZE      2       /*!< LE read periodic advertising list size command complete event length. */
#define LHCI_LEN_LE_READ_SUP_TX_POWER           3       /*!< LE read supported Tx power command complete event length. */
#define LHCI_LEN_LE_WRITE_RF_PATH_COMP          1       /*!< LE write RF path compensation command complete event length. */
#define LHCI_LEN_LE_READ_RF_PATH_COMP           5       /*!< LE read RF path compensation command complete event length. */
#define LHCI_LEN_LE_SET_EXT_SCAN_ENABLE_EVT     1       /*!< LE set extended scan enable command complete event length. */
#define LHCI_LEN_LE_READ_PHY_EVT                5       /*!< LE read PHY command complete event length. */
#define LHCI_LEN_LE_SET_DEF_PHY_EVT             1       /*!< LE set default PHY command complete event length. */
#define LHCI_LEN_LE_RECEIVER_TEST_V2_EVT        1       /*!< LE enhanced receiver test command complete event length. */
#define LHCI_LEN_LE_TRANSMITTER_TEST_V2_EVT     1       /*!< LE enhanced transmitter test command complete event length. */
/* New in version 5.1 */
#define LHCI_LEN_LE_SET_CONNLESS_CTE_TX_PARAM   1       /*!< LE set connectionless CTE transmit parameters command complete event length. */
#define LHCI_LEN_LE_SET_CONNLESS_CTE_TX_ENABLE  1       /*!< LE set connectionless CTE transmit enable command complete event length. */
#define LHCI_LEN_LE_SET_CONNLESS_IQ_SAMPLE_ENABLE 3     /*!< LE set connectionless IQ sample enable command complete event length. */
#define LHCI_LEN_LE_SET_CONN_CTE_RX_PARAM       3       /*!< LE set connection CTE receive parameters command complete event length. */
#define LHCI_LEN_LE_SET_CONN_CTE_TX_PARAM       3       /*!< LE set connection CTE transmit parameters command complete event length. */
#define LHCI_LEN_LE_CONN_CTE_REQ_ENABLE         3       /*!< LE set connection CTE request command complete event length. */
#define LHCI_LEN_LE_CONN_CTE_RSP_ENABLE         3       /*!< LE set connection CTE response command complete event length. */
#define LHCI_LEN_LE_READ_ANTENNA_INFO           5       /*!< LE read antenna info command complete event length. */
#define LHCI_LEN_LE_SET_PER_ADV_RCV_ENABLE      1       /*!< LE set periodic advertising receive enable command complete event length. */
#define LHCI_LEN_LE_PER_ADV_SYNC_TRANSFER       3       /*!< LE periodic advertising sync transfer command complete event length. */
#define LHCI_LEN_LE_PER_ADV_SET_INFO_TRANSFER   3       /*!< LE periodic advertising set info transfer command complete event length. */
#define LHCI_LEN_LE_SET_PAST_PARAM              3       /*!< LE set periodic advertising sync transfer parameters command complete event length. */
#define LHCI_LEN_LE_SET_DEFAULT_PAST_PARAM      1       /*!< LE set default periodic advertising sync transfer parameters command complete event length. */
#define LHCI_LEN_LE_MODIFY_SCA_EVT              1       /*!< LE Modify sleep clock accuracy event length. */
/* New in version 5.2 */
#define LHCI_LEN_LE_READ_BUF_SIZE_V2            7       /*!< LE read ISO buffer size command complete event length. */
#define LHCI_LEN_LE_READ_TX_SYNC                12      /*!< LE read ISO Tx sync. */
#define LHCI_LEN_LE_REMOVE_CIG                  2       /*!< LE remove CIG. */
#define LHCI_LEN_LE_REJECT_CIS_REQ              3       /*!< LE reject CIS request. */
#define LHCI_LEN_LE_BIG_TERMINATE_SYNC          2       /*!< LE BIG terminate sync. */
#define LHCI_LEN_LE_SETUP_ISO_DATA_PATH         3       /*!< LE setup ISO data path. */
#define LHCI_LEN_LE_REMOVE_ISO_DATA_PATH        3       /*!< LE remove ISO data path. */
#define LHCI_LEN_LE_ISO_TX_TEST                 3       /*!< LE ISO Tx Test. */
#define LHCI_LEN_LE_ISO_RX_TEST                 3       /*!< LE ISO Rx Test. */
#define LHCI_LEN_LE_ISO_READ_TEST_COUNTER       15      /*!< LE ISO read test counter. */
#define LHCI_LEN_LE_ISO_TEST_END                15      /*!< LE ISO test end. */
#define LHCI_LEN_LE_SET_HOST_FEATURE            1       /*!< LE Set Host Feature. */
#define LHCI_LEN_LE_READ_ISO_LINK_QUAL          31      /*!< LE Read ISO link quality. */
#define LHCI_LEN_LE_READ_ENH_TX_POWER_EVT       6       /*!< LE Read enhanced TX power. */
#define LHCI_LEN_LE_SET_TX_POWER_REPORT_EVT     3       /*!< LE Set transmit power reporting enable event. */
#define LHCI_LEN_LE_SET_PATH_LOSS_RPT_PARAMS    3       /*!< LE Set path loss reporting parameters event. */
#define LHCI_LEN_LE_SET_PATH_LOSS_RPT_ENABLE    3       /*!< LE Set path loss reporting enable event. */
#define LHCI_LEN_SET_ECOSYSTEM_BASE_INTERVAL    1       /*!< Set ecosystem base interval. */
#define LHCI_LEN_CONFIG_DATA_PATH               1       /*!< Configure data path. */
#define LHCI_LEN_READ_LOCAL_SUP_CODECS          3       /*!< Read local supported codecs. */
#define LHCI_LEN_READ_LOCAL_SUP_CODEC_CAP       2       /*!< Read local supported codec configuration. */
#define LHCI_LEN_READ_LOCAL_SUP_CONTROLLER_DLY  7       /*!< Read local supported controller delay. */
/* New in version 5.3 */
#define LHCI_LEN_LE_SET_DATA_REL_ADDR_CHANGES   1       /*!< LE Set data related address changes event. */
#define LHCI_LEN_LE_SET_DEF_SUBRATE             1       /*!< LE Set default subrate event. */
/* New in version 5.4 */
#define LHCI_LEN_LE_SET_PER_ADV_PARAM_V2        2       /*!< LE Set periodic advertising parameters. */
#define LHCI_LEN_LE_SET_PER_ADV_SUB_DATA        2       /*!< LE Set periodic advertising subevent data. */
#define LHCI_LEN_LE_SET_PER_ADV_RESP_DATA       3       /*!< LE Set periodic advertising response data. */
#define LHCI_LEN_LE_SET_PER_SYNC_SUB_DATA       3       /*!< LE Set periodic sync subevent data. */
#define LHCI_LEN_LE_SET_EXT_ADV_PARAM_V2        2       /*!< LE set extended advertising parameters command complete event length. */
/* Version Atlanta */
#define LHCI_LEN_LE_CS_READ_LOCAL_SUP_CAP             29      /*!< LE CS read local supported capabilities. */
#define LHCI_LEN_LE_CS_READ_REMOTE_SUP_CAP            1       /*!< LE CS read remote supported capabilities. */
#define LHCI_LEN_LE_CS_WRITE_CACH_REMOTE_SUP_CAP      3       /*!< LE CS write cached remote supported capabilities. */
#define LHCI_LEN_LE_CS_SECURITY_ENABLE                1       /*!< LE CS security enable. */
#define LHCI_LEN_LE_CS_SET_DEFAULT_SETTINGS           3       /*!< LE CS set default settings. */
#define LHCI_LEN_LE_CS_READ_REMOTE_FAE_TABLE          1       /*!< LE CS read remote FAE table. */
#define LHCI_LEN_LE_CS_WRITE_CACH_REMOTE_FAE_TABLE    3       /*!< LE CS write cached remote FAE table. */
#define LHCI_LEN_LE_CS_CREATE_CONFIG                  1       /*!< LE CS create config. */
#define LHCI_LEN_LE_CS_REMOVE_CONFIG                  1       /*!< LE CS remove config. */
#define LHCI_LEN_LE_CS_SET_CHAN_CLASSIFICATION        1       /*!< LE CS set channel classification. */
#define LHCI_LEN_LE_CS_SET_PROC_PARAMS                3       /*!< LE CS set procedure parameters. */
#define LHCI_LEN_LE_CS_PROCEDURE_ENABLE               1       /*!< LE CS procedure enable. */
#define LHCI_LEN_LE_CS_TEST                           1       /*!< LE CS test. */
#define LHCI_LEN_LE_CS_TEST_END                       1       /*!< LE CS test end. */
#define LHCI_LEN_LE_SET_DECISION_DATA                 1       /*!< LE set decision data. */
#define LHCI_LEN_LE_SET_DECISION_INSTRUCTIONS         1       /*!< LE set decision instructions. */

#ifndef LHCI_DEF_EVT_MASK
/*! \brief  Mandatory event mask. */
#define LHCI_DEF_EVT_MASK                   UINT64_C(0x00001FFFFFFFFFFF);
#endif

/*! \brief  Default page 2 event mask. */
#define LHCI_DEF_EVT_PG2_MASK               UINT64_C(0x0000000000000000);

#ifndef LHCI_DEF_LE_EVT_MASK
/*! \brief  Default LE specific event mask. */
#define LHCI_DEF_LE_EVT_MASK                UINT64_C(0x000000000000001F);
#endif

/*! \brief  Local supported features value. */
#define LHCI_LOCAL_SUP_FEAT_VAL             UINT64_C(0x0000006000000000)
                                            /* bit 37: BR/EDR Not Supported */
                                            /* bit 38: LE Supported (Controller) */

/*! \brief  Convert bytes to bits. */
#define LHCI_BYTE_TO_BITS(x)                (8 * x)

/*! \brief  Vendor specific event length. */
#define LHCI_LEN_VS_EVT                     2

/*! \brief  Indicate command status event shall be returned. */
#define LHCI_LEN_CMD_STATUS_EVT             0xFFU

/* Packetcraft vendor specific OCF range is 0x3C0-0x3FF */
#define LHCI_OPCODE_VS_SET_SCAN_CH_MAP           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E0)  /*!< Set Scan Channel Map opcode. */
#define LHCI_OPCODE_VS_SET_EVENT_MASK            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E1)  /*!< Set Vendor Specific Event Mask opcode. */
#define LHCI_OPCODE_VS_SET_RSRC_MGR_MODE         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E2)  /*!< DEPRECATED. */
#define LHCI_OPCODE_VS_ENA_ACL_SINK              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E3)  /*!< Enable ACL Packet Sink opcode. */
#define LHCI_OPCODE_VS_GENERATE_ACL              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E4)  /*!< Generate ACL Packets opcode. */
#define LHCI_OPCODE_VS_ENA_AUTO_GEN_ACL          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E5)  /*!< Enable Auto Generate ACL Packets opcode. */
#define LHCI_OPCODE_VS_SET_TX_TEST_ERR_PATT      HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E6)  /*!< Set Tx Test Error Pattern opcode. */
#define LHCI_OPCODE_VS_SET_CONN_OP_FLAGS         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E7)  /*!< Set Connection Operational Flags opcode. */
#define LHCI_OPCODE_VS_SET_P256_PRIV_KEY         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E8)  /*!< Set P-256 Private Key opcode. */
#define LHCI_OPCODE_VS_GET_PER_CHAN_MAP          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DE)  /*!< Get channel map of periodic scan/adv. */
#define LHCI_OPCODE_VS_SET_HCI_SUP_CMD           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DF)  /*!< DEPRECATED. */
#define LHCI_OPCODE_VS_GET_ACL_TEST_REPORT       HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3E9)  /*!< Get ACL Test Report opcode. */
#define LHCI_OPCODE_VS_SET_LOCAL_MIN_USED_CHAN   HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3EA)  /*!< Set local minimum number of used channels. */
#define LHCI_OPCODE_VS_GET_PEER_MIN_USED_CHAN    HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3EB)  /*!< Get peer minimum number of used channels. */
#define LHCI_OPCODE_VS_VALIDATE_PUB_KEY_MODE     HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3EC)  /*!< Set validate public key mode between ALT1 and ALT2. */

#define LHCI_OPCODE_VS_SET_BD_ADDR               HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F0)  /*!< Set BD address opcode. */
#define LHCI_OPCODE_VS_GET_RAND_ADDR             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F1)  /*!< Get Random Address opcode. */
#define LHCI_OPCODE_VS_SET_LOCAL_FEAT            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F2)  /*!< Set Local Feature opcode. */
#define LHCI_OPCODE_VS_SET_OP_FLAGS              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F3)  /*!< Set Operational Flags opcode. */
#define LHCI_OPCODE_VS_SET_ADV_TX_PWR            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F5)  /*!< Set Advertising Tx Power opcode. */
#define LHCI_OPCODE_VS_SET_CONN_TX_PWR           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F6)  /*!< Set Connection Tx Power opcode. */
#define LHCI_OPCODE_VS_SET_ENC_MODE              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F7)  /*!< Set Encryption Mode opcode. */
#define LHCI_OPCODE_VS_SET_CHAN_MAP              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F8)  /*!< Set Channel Map opcode. */

#define LHCI_OPCODE_VS_SET_DIAG_MODE             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F9)  /*!< Set Diagnostic Mode opcode. */
#define LHCI_OPCODE_VS_SET_SNIFFER_ENABLE        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3CD)  /*!< Enable sniffer packet forwarding. */

#define LHCI_OPCODE_VS_GET_PDU_FILT_STATS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3F4)  /*!< Get PDU Filter Statistics opcode. */
#define LHCI_OPCODE_VS_GET_SYS_STATS             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FA)  /*!< Get Memory Statistics opcode. */
#define LHCI_OPCODE_VS_GET_CONTEXT_SIZES         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C0)  /*!< Get context sizes opcode. */
#define LHCI_OPCODE_VS_GET_ADV_STATS             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FB)  /*!< Get Advertising Statistics opcode. */
#define LHCI_OPCODE_VS_GET_SCAN_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FC)  /*!< Get Scan Statistics opcode. */
#define LHCI_OPCODE_VS_GET_CONN_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FD)  /*!< Get Connection Statistics opcode. */
#define LHCI_OPCODE_VS_GET_TEST_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FE)  /*!< Get Test Statistics opcode. */
#define LHCI_OPCODE_VS_GET_POOL_STATS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3FF)  /*!< Get Pool Statistics opcode. */

#define LHCI_OPCODE_VS_SET_AUX_DELAY             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D0)  /*!< Set Additional AuxPtr offset. */
#define LHCI_OPCODE_VS_SET_EXT_ADV_FRAG_LEN      HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D1)  /*!< Set extended advertising data fragmentation length. */
#define LHCI_OPCODE_VS_SET_EXT_ADV_PHY_OPTS      HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D2)  /*!< Set extended advertising PHY options. */
#define LHCI_OPCODE_VS_SET_EXT_ADV_DEF_PHY_OPTS  HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D3)  /*!< DEPRECATED */
#define LHCI_OPCODE_VS_SET_EXT_SCAN_PHY_OPTS     HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D4)  /*!< Set extended scanning default PHY options. */
#define LHCI_OPCODE_VS_SET_PER_SCAN_IMMED_SKIP   HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3B3)  /*!< Set Periodic Scanner immediate skip. */

#define LHCI_OPCODE_VS_GENERATE_ISO              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D5)  /*!< Generate ISO Packets opcode. */
#define LHCI_OPCODE_VS_GET_ISO_TEST_REPORT       HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D6)  /*!< Get ISO Test Report opcode. */
#define LHCI_OPCODE_VS_ENA_ISO_SINK              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D7)  /*!< Enable ISO Packet Sink opcode. */
#define LHCI_OPCODE_VS_ENA_AUTO_GEN_ISO          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D8)  /*!< Enable Auto Generate ISO Packets opcode. */
#define LHCI_OPCODE_VS_GET_CIS_STATS             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3D9)  /*!< Get CIS Statistics opcode. */
#define LHCI_OPCODE_VS_GET_BIS_STATS             HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C1)  /*!< Get BIS Statistics opcode. */

#define LHCI_OPCODE_VS_GET_AUX_ADV_STATS         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DA)  /*!< Get Auxiliary Advertising Statistics opcode. */
#define LHCI_OPCODE_VS_GET_AUX_SCAN_STATS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DB)  /*!< Get Auxiliary Scanning Statistics opcode. */
#define LHCI_OPCODE_VS_GET_PER_SCAN_STATS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DC)  /*!< Get Periodic Scanning Statistics opcode. */

#define LHCI_OPCODE_VS_SET_CONN_PHY_TX_PWR       HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3DD)  /*!< Set Connection Phy Tx Power opcode. */

#define LHCI_OPCODE_VS_SYSTEM_RESET              HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3A0)  /*!< System Reset opcode. */
#define LHCI_OPCODE_VS_ENTER_BOOTLOADER          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3A1)  /*!< Enter Bootloader opcode. */
#define LHCI_OPCODE_VS_SET_LED_PIN               HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3A2)  /*!< DEPRECATED */
#define LHCI_OPCODE_VS_SET_RADIO_FE_CFG          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3A3)  /*!< DEPRECATED */
#define LHCI_OPCODE_VS_GCOV_EXIT                 HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3A4)  /*!< Gcov exit to record coverage data */

#define LHCI_OPCODE_VS_START_EDS                 HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C2)  /*!< Start DAA EDS. */
#define LHCI_OPCODE_VS_STOP_EDS                  HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C3)  /*!< Stop DAA EDS. */
#define LHCI_OPCODE_VS_GET_EDS_ASSESSMENT        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C4)  /*!< Get current EDS value. */
#define LHCI_OPCODE_VS_START_DAA                 HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C5)  /*!< Start DAA closed loop. */
#define LHCI_OPCODE_VS_STOP_DAA                  HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C6)  /*!< Stop DAA closed loop. */

#define LHCI_OPCODE_VS_ENABLE_RSP_OBS            HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3B0)  /*!< Enable Response Observation. */
#define LHCI_OPCODE_VS_SET_RSP_OBS_SLOTS         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3B1)  /*!< Set Periodic Advertising Response Observer Slots. */
#define LHCI_OPCODE_VS_SET_RSP_OBS_DATA          HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3B2)  /*!< Set Response Observation Response Data. */

#define LHCI_OPCODE_VS_SET_BIS_RXD_ENABLE        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C7)  /*!< Enable/disable RX receiver diversity for BIS. */
#define LHCI_OPCODE_VS_SET_BIS_RXD_INT           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C8)  /*!< Set BIS RX antenna selection interval. */
#define LHCI_OPCODE_VS_SET_BIS_RXD_THRES         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3C9)  /*!< Set BIS RX antenna selection threshold. */
#define LHCI_OPCODE_VS_SET_ACL_RXD_ENABLE        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3CA)  /*!< Enable/disable RX receiver diversity for ACL. */
#define LHCI_OPCODE_VS_SET_ACL_RXD_INT           HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3CB)  /*!< Set ACL RX antenna selection interval. */
#define LHCI_OPCODE_VS_SET_ACL_RXD_THRES         HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3CC)  /*!< Set ACL RX antenna selection threshold. */
#define LHCI_OPCODE_VS_GET_BIS_RXD_STATUS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3CE)  /*!< Get the current average RSSI value for all antennas and the current selected primary antenna for BIS. */
#define LHCI_OPCODE_VS_GET_ACL_RXD_STATUS        HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x3CF)  /*!< Get the current average RSSI value for all antennas and the current selected primary antenna for ACL. */

#define LHCI_OPCODE_VS_CS_CALCULATE_CHAN_SEQ     HCI_OPCODE(HCI_OGF_VENDOR_SPEC, 0x390)  /*!< CS calculate channel sequence. */

/* Vendor specific event masks. */
#define LHCI_VS_EVT_MASK_SCAN_REPORT_EVT    0x01   /*!< (Byte 0) VS event bit, scan report. */
#define LHCI_VS_EVT_MASK_DIAG_TRACE_EVT     0x02   /*!< (Byte 0) VS event bit, diagnostic tracing. */
#define LHCI_VS_EVT_MASK_ISO_EVENT_CMPL_EVT 0x04   /*!< (Byte 0) VS event bit, ISO event complete. */
#define LHCI_VS_EVT_MASK_PA_TIME_RPT        0x08   /*!< (Byte 0) VS event bit, timing report. */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Message handler types. */
enum
{
  LHCI_MSG_PAWR_CEN,                    /*!< PAwR central command handler type. Must be before PRIV.*/
  LHCI_MSG_PAWR_PER,                    /*!< PAwR peripheral command handler type. Must be before PRIV.*/
  LHCI_MSG_PRIV,                        /*!< Privacy command handler type.  Must be before CONN. */
  LHCI_MSG_ISO,                         /*!< Isochronous features command handler type. Must be before CONN. */
  LHCI_MSG_ECU,                         /*!< Enhanced connection update command handler type. */
  LHCI_MSG_CONN,                        /*!< Connection command handler type. */
  LHCI_MSG_CONN_CS2,                    /*!< Connection Channel Selection 2 command handler type. */
  LHCI_MSG_ENC,                         /*!< Encryption command handler type. */
  LHCI_MSG_SCAN,                        /*!< Scan command handler type. */
  LHCI_MSG_EXT_SCAN,                    /*!< Extended scan command handler type. */
  LHCI_MSG_PER_SCAN,                    /*!< Periodic scan command handler type. */
  LHCI_MSG_ADV,                         /*!< Advertising command handler type. */
  LHCI_MSG_EXT_ADV,                     /*!< Extended advertising command handler type. */
  LHCI_MSG_PER_ADV,                     /*!< Periodic advertising command handler type. */
  LHCI_MSG_SC,                          /*!< Secure connections command handler type. */
  LHCI_MSG_PHY,                         /*!< PHY features command handler type. */
  LHCI_MSG_CTE,                         /*!< Constant Tone extension command handler type. */
  LHCI_MSG_PAST,                        /*!< Periodic advertising sync transfer command handler type. */
  LHCI_MSG_CIS_MST,                     /*!< Connected isochronous stream master features command handler type. */
  LHCI_MSG_CIS_SLV,                     /*!< Connected isochronous stream slave features command handler type. */
  LHCI_MSG_BIS_MST,                     /*!< Broadcast isochronous stream master features command handler type. */
  LHCI_MSG_BIS_SLV,                     /*!< Broadcast isochronous stream slave features command handler type. */
  LHCI_MSG_CS_CEN,                      /*!< Channel Sounding central features command handler type. */
  LHCI_MSG_CS_PER,                      /*!< Channel Sounding peripheral features command handler type. */
  LHCI_MSG_DBAF_CEN,                    /*!< Decision-based advertising filtering central features command handler type. */
  LHCI_MSG_DBAF_PER,                    /*!< Decision-based advertising filtering peripheral features command handler type. */
  LHCI_MSG_CODEC,                       /*!< Codec command handler type. */
  LHCI_MSG_DAA,                         /*!< DAA vendor specific command handler type. */
  LHCI_MSG_PC,                          /*!< Power control handler type. */
  LHCI_MSG_VS_EXT,                      /*!< Extended vendor specific command handler type. */
  LHCI_MSG_DIAG,                        /*!< Diagnostics vendor specific command handler type. */
  LHCI_MSG_TESTER,                      /*!< Tester vendor specific command handler type. */
  LHCI_MSG_VS_RSPOBS,                   /*!< Response observation vendor specific command handler type. */
  LHCI_MSG_ACL_RXD,                     /*!< Receiver diversity for ACL vendor specific command handler type. */
  LHCI_MSG_BIS_RXD,                     /*!< Receiver diversity for BIS vendor specific command handler type. */
  LHCI_MSG_TOTAL                        /*!< Total number of command handlers. */
};

/*! \brief      Task event types. */
enum
{
  LHCI_EVT_ACL_RCVD       = (1 << 0),   /*!< HCI ACL packet received. */
  LHCI_EVT_CMD_RCVD       = (1 << 1),   /*!< HCI command packet received. */
  LHCI_EVT_SEND_CMPL      = (1 << 2),   /*!< HCI event packet send completion. */
  LHCI_EVT_HW_ERR         = (1 << 3),   /*!< HCI hardware error. */
  LHCI_EVT_ISO_RCVD       = (1 << 4),   /*!< HCI ISO packet received. */
};

/*! \brief      Event handler call signature. */
typedef bool (*lhciEvtHandler_t)(LlEvt_t *pEvt);

/*! \brief      Service ACL call signature. */
typedef uint8_t *(*lhciServiceAcl_t)(void);

/*! \brief      Service SCO call signature. */
typedef uint8_t *(*lhciServiceIso_t)(void);

/*! \brief      Receive ISO data has been deallocated. */
typedef void (*lhciRecvIsoComplete_t)(uint8_t numBufs);

/*! \brief      Iso event complete enable. */
typedef void (*lhciIsoEventCompleteEnable_t)(uint8_t enable);

/*! \brief      Control block of the LL HCI subsystem (persists with resets). */
typedef struct
{
  wsfHandlerId_t    handlerId;          /*!< Task handler ID. */

  wsfQueue_t        aclQ;               /*!< ACL queue. */
  wsfQueue_t        cmdQ;               /*!< Command queue. */
  wsfQueue_t        evtQ;               /*!< Event queue. */
  wsfQueue_t        isoQ;               /*!< ISO queue. */
  lhciCompHandler_t evtCompCback;       /*!< Event complete handler. */
  bool              evtTrPending;       /*!< Event transport in progress. */

  uint8_t           supCmds[HCI_SUP_CMD_LEN];
                                        /*!< Supported HCI commands bit mask. */
} lhciPersistCb_t;

/*! \brief      Control block of the LL HCI subsystem (cleared with resets). */
typedef struct
{
  uint64_t          evtMsk;             /*!< General event mask. */
  uint64_t          evtMskPg2;          /*!< General event mask page 2. */
  uint64_t          leEvtMsk;           /*!< LE specific event mask. */

  uint8_t           numScanReqRcvd;     /*!< Number of scan request received. */
  uint8_t           hwErrorCode;        /*!< Hardware error code. */

  bool              recvAclSink;        /*!< Receive ACL sink. */
  uint32_t          recvAclPktCnt;      /*!< Receive ACL packet count. */
  uint32_t          recvAclOctetCnt;    /*!< Receive ACL octet count. */
  bool              genEnaFlag;         /*!< Generate enable flag. */
  uint8_t           genPldCnt;          /*!< Generate ACL packet fill value. */
  uint16_t          genPktLen;          /*!< Generate ACL packet length (0 to disable). */
  uint32_t          genPktCnt;          /*!< Generate ACL packet count. */
  uint32_t          genOctetCnt;        /*!< Generate ACL octet count. */

  uint8_t           numAdvReport;       /*!< Number of pending advertising reports. */

  uint8_t           numIqReport;      /*!< Number of pending CTE samples. */

  uint8_t           chanAssesmentMode;  /*!< Channel assessment mode. */
} lhciCb_t;

/*! \brief      Control block for ISO data generator. */
typedef struct
{
  bool      recvIsoSink;        /*!< Receive ISO sink. */
  uint32_t  recvIsoPktCnt;      /*!< Receive ISO packet count. */
  uint32_t  recvIsoOctetCnt;    /*!< Receive ISO octet count. */
  bool      genEnaFlag;         /*!< Generate ISO enable flag. */
  uint8_t   genPldCnt;          /*!< Generate ISO packet fill value. */
  uint16_t  genPktLen;          /*!< Generate ISO packet length (0 to disable). */
  uint32_t  genPktCnt;          /*!< Generate ISO packet count. */
  uint32_t  genOctetCnt;        /*!< Generate ISO octet count. */

  bool      isoRxTest;          /*!< true if ISO Rx test is enabled. */
} lhciIsoCb_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Command handler table. */
extern lhciCmdHandler_t lhciCmdTbl[LHCI_MSG_TOTAL];

/*! \brief      Event handler table. */
extern lhciEvtHandler_t lhciEvtTbl[LHCI_MSG_TOTAL];

/*! \brief      Receive pending handler. */
extern lhciServiceAcl_t lhciServiceAcl;

/*! \brief      Receive pending handler. */
extern lhciServiceIso_t lhciServiceIso;

/*! \brief      Receive ISO data has been deallocated. */
extern lhciRecvIsoComplete_t lhciRecvServiceIsoComplete;

/*! \brief      Iso event complete enable. */
extern lhciIsoEventCompleteEnable_t lhciIsoEventCompleteEnable;

/* Persistent control block */
extern lhciPersistCb_t lhciPersistCb;

/* Control block */
extern lhciCb_t lhciCb;

/* Control block for ISO data generation. */
extern lhciIsoCb_t lhciIsoCb;

/* Handler duration watermark in microseconds. */
extern uint16_t lhciHandlerWatermarkUsec;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Reset */
void lhciReset(void);

/* Data exchange */
void lhciRecv(uint8_t type, uint8_t *pBuf);
void lhciSendComplete(uint8_t type, uint8_t *pBuf);
bool lhciService(uint8_t *pType, uint16_t *pLen, uint8_t **pBuf);
void lhciSendHwError(uint8_t code);

/* Handlers */
bool lhciLlEvtHandler(LlEvt_t *pEvt);
void lhciAclSendComplete(uint16_t handle, uint8_t numBufs);
uint8_t *lhciRecvAcl(void);
void lhciAclRecvPending(uint16_t handle, uint8_t numBufs);
void lhciGenerateAcl(uint16_t handle, uint16_t pktLen, uint8_t numPkts);

void lhciIsoSendComplete(uint8_t numHandles, uint16_t *pHandle, uint16_t *pNumPkts);
uint8_t *lhciRecvIso(void);
void lhciRecvIsoDataComplete(uint8_t numBufs);
void lhciIsoRecvPending(uint8_t numHandles, uint16_t *pHandle, uint16_t *pNumPkts);
void lhciGenerateIso(uint16_t handle, uint16_t pktLen, uint8_t numPkts);

/* Command parser */
bool lhciCommonDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstExtConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstEncDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvEncDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstScanDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvAdvDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstExtScanDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvExtAdvDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPrivAdvDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPrivConnDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciScDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPhyDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPastDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstCisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvCisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvBisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstBisDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciIsoDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciCodecDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPclDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciEcuDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciCteDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciCenPawrDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPerPawrDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciCenCsDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPerCsDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciCenDbafDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciPerDbafDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);

bool lhciCommonVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciConnVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstConnVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstScanVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvAdvVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvEncVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciScVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciMstExtScanVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciSlvExtAdvVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciVsExtDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciIsoVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciDaaVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciRspObsVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciAclRxdVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciBisRxdVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);
bool lhciVsCsDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);

/* Event builders */
void lhciSendCmdStatusEvt(LhciHdr_t *pCmdHdr, uint8_t status);
bool lhciConnEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciConnCsEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciMstConnEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciMstScanEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciSlvAdvEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciMstExtScanEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciSlvExtAdvEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciMstEncEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciSlvEncEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciPrivConnEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciScEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciPhyEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciMstCisEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciSlvCisEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciSlvBigEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciMstBisEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciIsoEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciPclEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciEcuEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciCteEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciPerPawrEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciCenCsEncodeEvtPkt(LlEvt_t *pEvt);
bool lhciPerCsEncodeEvtPkt(LlEvt_t *pEvt);

bool lhciSlvVsStdEncodeEvtPkt(LlEvt_t *pEvt);

/* Events */
uint8_t *lhciAllocEvt(uint8_t evtCode, uint8_t paramLen);
uint8_t *lhciAllocCmdCmplEvt(uint8_t paramLen, uint16_t opCode);
void lhciConnSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam, uint16_t handle);
void lhciPclSendCmdCmplEvt(LhciHdr_t *pCmdHdr, uint8_t status, uint8_t paramLen, uint8_t *pParam, uint16_t handle);
uint8_t *lhciAllocPerAdvRptEvt(uint8_t evtCode, uint8_t paramLen);
uint8_t *lhciAllocPerAdvRptV2Evt(uint8_t evtCode, uint8_t paramLen);
uint8_t *lhciAllocPerAdvRspRptEvt(uint8_t evtCode, uint8_t paramLen);

/* Command packet. */
uint8_t lhciUnpackConnSpec(LlConnSpec_t *pConnSpec, const uint8_t *pBuf);
uint8_t lhciUnpackExtInitParamV2(LlExtInitParam_t *pInitParam, LlExtInitScanParam_t initScanParam[], LlConnSpec_t connSpec[], const uint8_t *pBuf);
void lhciPackPerAdvRspRptEvt(uint8_t *pBuf, const LlPerAdvRspReportInd_t *pEvt, const uint8_t *pFragStart, uint8_t fragLen);

/* Common events. */

/*************************************************************************************************/
/*!
 *  \brief  Pack a CIS established event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  pEvt        CIS established event data.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
uint8_t lhciPackCisEstEvt(uint8_t *pBuf, const LlCisEstInd_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Pack an event packet header.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  evtCode     Event code.
 *  \param  paramLen    Parameter length.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static inline uint8_t lhciPackEvtHdr(uint8_t *pBuf, uint8_t evtCode, uint8_t paramLen)
{
  const uint8_t len = HCI_EVT_HDR_LEN;

  UINT8_TO_BSTREAM(pBuf, evtCode);
  UINT8_TO_BSTREAM(pBuf, paramLen);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a command status event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *  \param  opCode      OpCode.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static inline uint8_t lhciPackCmdStatusEvt(uint8_t *pBuf, uint8_t status, uint16_t opCode)
{
  const uint8_t len = HCI_LEN_CMD_STATUS;

  UINT8_TO_BSTREAM (pBuf, status);
  UINT8_TO_BSTREAM (pBuf, 1);       /* Num_HCI_Command_Packets is always 1 */
  UINT16_TO_BSTREAM(pBuf, opCode);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a command complete event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  opCode      OpCode.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static inline uint8_t lhciPackCmdCompleteEvt(uint8_t *pBuf, uint16_t opCode)
{
  const uint8_t len = HCI_LEN_CMD_CMPL;

  UINT8_TO_BSTREAM (pBuf, 1);       /* Num_HCI_Command_Packets is always 1 */
  UINT16_TO_BSTREAM(pBuf, opCode);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a command complete status parameter.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  status      Completion status.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static inline uint8_t lhciPackCmdCompleteEvtStatus(uint8_t *pBuf, uint8_t status)
{
  const uint8_t len = sizeof(uint8_t);

  UINT8_TO_BSTREAM (pBuf, status);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Pack a vendor specific event packet.
 *
 *  \param  pBuf        Packed packet buffer.
 *  \param  vsEvtCode   Event code.
 *
 *  \return Packet length.
 */
/*************************************************************************************************/
static inline uint8_t lhciPackVsEvt(uint8_t *pBuf, uint16_t vsEvtCode)
{
  const uint8_t len = LHCI_LEN_VS_EVT;

  UINT16_TO_BSTREAM(pBuf, vsEvtCode);

  return len;
}

/*************************************************************************************************/
/*!
 *  \brief  Send an event.
 *
 *  \param  pEvtBuf   Buffer containing event.
 */
/*************************************************************************************************/
static inline void lhciSendEvt(uint8_t *pEvtBuf)
{
  LhciSendEvent(pEvtBuf - HCI_EVT_HDR_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  Send a command complete event.
 *
 *  \param  pEvtBuf   Buffer containing command complete event.
 */
/*************************************************************************************************/
static inline void lhciSendCmdCmplEvt(uint8_t *pEvtBuf)
{
  LhciSendEvent(pEvtBuf - (HCI_EVT_HDR_LEN + HCI_LEN_CMD_CMPL));
}

/*! \} */    /* LL_INT_LHCI */

#ifdef __cplusplus
};
#endif

#endif /* LHCI_INT_H */
