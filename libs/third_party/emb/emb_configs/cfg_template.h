////////////////////////////////////////////////////////////////////////////////
///
/// @project    EM9305
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2024, EM Microelectronic-Marin SA
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

/** NOTE: These configurations are READ-ONLY.
 *  Changes to this file will not be reflected in ANY EMB Library Binary
*/

/**
 * Table of Contents:
 *
 * BLE Version
 * Role Initializations
 * Feature Initializations
 * Host Configurations
 *  DM/GAP Configurations
 *  L2CAP Configurations
 *  SMP Configurations
 *  SEC Configurations
 * HCI Configurations
 * Controller Configurations
 *  Link Layer Configurations
 *  Scheduler Configurations
 *  Baseband Configurations
 * Miscelaneous Configurations
*/

/** NOTE: These configurations are READ-ONLY.
 *  Changes to this file will not be reflected in ANY EMB Library Binary
*/

// BLE Version
// v4.0 = 6
// v4.1 = 7
// v4.2 = 8
// v5.0 = 9
// v5.1 = 10
// v5.2 = 11
// v5.3 = 12
// v5.4 = 13

/*! \brief BLE Version to be used by the stack */
#define BT_VER                  13

// Role Initializations ----------------------------------------------------------------------------------------

/*! \brief Initialize Peripheral Role Related Features */
#define INIT_PERIPHERAL

/*! \brief Initialize Central Role Related Features */
#define INIT_CENTRAL

/*! \brief Initialize Observer Role Related Features */
#define INIT_OBSERVER

/*! \brief Initialize Broadcaster Role Related Features */
#define INIT_BROADCASTER

// Feature Initializations ----------------------------------------------------------------------------------------

/*! \brief Initialize Encryption Feature */
#define INIT_ENCRYPTED

/*! \brief Initialize Periodic Advertising Sync Transfer Feature, requires BLE Version of 5.1 or newer, requires (either Peripheral or Central role) AND (either Observer or Broadcaster role) */
#define INIT_FEAT_PAST          1

/*! \brief Initialize Constant Tone Extension Feature, requires BLE Version of 5.1 or newer */
#define INIT_FEAT_CTE           0

/*! \brief Initialize Power Control Feature, requires BLE Version of 5.2 or newer */
#define INIT_FEAT_PC            1

/*! \brief Initialize Isochronous Channels Feature, requires BLE Version of 5.2 or newer */
#define INIT_FEAT_ISO           0

/*! \brief Initialize Connected Isochronous Streams Feature, requires BLE Version of 5.2 or newer, requires initialization of the ISO Channels feature (INIT_FEAT_ISO) */
#define INIT_FEAT_CIS           0

/*! \brief Initializae Broadcast Isochronous Streams Feature, requires BLE Version of 5.2 or newer, requires initialization of the ISO Channels feature (INIT_FEAT_ISO) */
#define INIT_FEAT_BIS           0

/*! \brief Initialize Enhanced Connection Feature, requires BLE Version of 5.3 or newer, requires either Peripheral or Central role */
#define INIT_FEAT_ECU           1

/*! \brief Initialize Periodic Advertising with Response Feature, requires BLE Version of 5.4 or newer, requires either (Central AND Observer roles) OR (Periphreal AND Broadcaster roles) */
#define INIT_FEAT_PAWR          1

/*! \brief Enable the use of the Sleep Clock Accuracy Update Feature */
#define FEAT_SCA_UPDATE         1


//Host Configurations ---------------------------------------------------------------------------------------------

//DM/GAP Configurations
/*! \brief Maximum number of connections */
#define DM_CONN_MAX             4

/*! \brief Maximum number of periodic advertising synchronizations */
#define DM_SYNC_MAX             3

/*! \brief Number of supported advertising sets: must be set to 1 for legacy advertising */
#define DM_NUM_ADV_SETS         1

/*! \brief Number of scanner and initiator PHYs (LE 1M, LE 2M and LE Coded): must be set to 1 for legacy scanner and initiator */
#define DM_NUM_PHYS             1

/*! \brief Maximum number of connected isochronous groups */
#define DM_CIG_MAX              2

/*! \brief Maximum number of connected isochronous streams, it is shared by CIGs */
#define DM_CIS_MAX              1

/*! \brief Maximum number of broadcast isochronous groups */
#define DM_BIG_MAX              2

/*! \brief Maximum number of broadcast isochronous streams, it is shared by BIGs */
#define DM_BIS_MAX              1

/*! \brief REF_ONLY Maximum number of isochronous data paths, it is shared by CISes and BISes */
#define DM_ISO_DATA_PATH_MAX    (DM_CIS_MAX + DM_BIS_MAX)

//L2CAP Configurations
/*! \brief Maximum number of connection oriented channels */
#define L2C_COC_CHAN_MAX        8

/*! \brief Maximum number of connection oriented channel registered clients */
#define L2C_COC_REG_MAX         4

//ATT Configurations
/*! \brief Maximum number of simultaneous ATT write commands */
#define ATT_NUM_SIMUL_WRITE_CMD 1

/*! \brief Maximum number of simultaneous ATT notifications */
#define ATT_NUM_SIMUL_NTF       1

/*! \brief Maximum number of EATT channels per DM connection.  Set to zero if EATT is not required. */
#define EATT_CONN_CHAN_MAX      0

//SMP Configurations
/*! \brief Max number of devices in the database */
#define SMP_DB_MAX_DEVICES      3

//SEC Configurations
/*! \brief REF_ONLY Select AES and Random Number Security Service to be implemented using HCI. */
#define SEC_CMAC_CFG            1

/*! \brief REF_ONLY Select ECC to be implemented using HCI */
#define SEC_ECC_CFG             2

/*! \brief REF_ONLY Counter with CBC-MAC (CCM) mode security using AES over HCI */
#define SEC_CCM_CFG             1

// HCI Configurations --------------------------------------------------------------------------------------------

/*! \brief REF_ONLY Tx data tailroom. Extra byte allocation required for LL operations (i.e. MIC) in single-chip implementation */
#define HCI_TX_DATA_TAILROOM    0

/*! \brief REF_ONLY Utilize "ExactLE" for the HCI Shim Layer when running a full stack (Host + Controller) */
#define HCI_TR_EXACTLE          1

/*! \brief REF_ONLY Utilize the UART Transport for the Stack. NOTE: The CHCI "UART Transport" used for this stack is connected to the EM Proprietary Transport Layer, which can be selected as SPI or UART elsewhere. This value is not changed if utilizing SPI for the EM Transport Layer */
#define CHCI_TR_UART            1

/*! \brief REF_ONLY Indicate the use of the SPI Transport */
#define CHCI_TR_TM              1

// EM System Configurations --------------------------------------------------------------------------------------------

/*! \brief Enable Stack Layer EM System Commands */
#define ENABLE_PC_EMS_CMDS      1

// Controller Configurations --------------------------------------------------------------------------------------------

//Link Layer Configurations
/*! \brief Absolute maximum number of connections (Max setting is 32). */
#define LL_MAX_CONN             4

/*! \brief Maximum number of Data PDU fragments. */
#define LL_MAX_FRAG             8

/*! \brief Table size for advertising filter. */
#define LL_NUM_ADV_FILT         8

/*! \brief Absolute maximum number of advertising sets. */
#define LL_MAX_ADV_SETS         2

/*! \brief Absolute maximum number of PHYs supported. */
#define LL_SCAN_PHY_MAX         3

/*! \brief Absolute maximum number of periodic scanners (maximum is 32). */
#define LL_MAX_PER_SCAN         6

/*! \brief Enable to share scanner response data and advertising data buffer. */
#define LL_SHARE_ADV_SCAN_DATA  0

/*! \brief Check API parameters. */
#define LL_API_PARAM_CHECK      1

/*! \brief REF_ONLY Enable LL tester extensions. */
#define LL_ENABLE_TESTER        0

/*! \brief REF_ONLY Enable LL calibration extensions. */
#define LL_ENABLE_CALIBRATION   0

/*! \brief Absolute maximum number of connected isochronous groups. */
#define LL_MAX_CIG              1

/*! \brief Absolute maximum number of connected isochronous streams, it is shared by CIGs. */
#define LL_MAX_CIS              2

/*! \brief Absolute maximum number of broadcast isochronous groups. */
#define LL_MAX_BIG              1

/*! \brief Absolute maximum number of broadcast isochronous streams, it is shared by BIGs. */
#define LL_MAX_BIS              2

/*! \brief Absolute maximum number of bursts. */
#define LL_MAX_BN               6

 /*! \brief Absolute maximum value of PTO. */
#define LL_MAX_PTO              0

/*! \brief Maximum value for ISO number of subevent. */
#define LL_MAX_NSE              8

/*! \brief Maximum value for ISO immediate repetition count. */
#define LL_MAX_IRC              4

/*! \brief Running average power control array sizes, must be a power of 2. */
#define LL_PC_TBL_POW           2

/*! \brief Power control service intervals in milliseconds. */
#define LL_PC_SERVICE_MS        1000

/*! \brief Request of increase/decrease in power value in units of dBm. */
#define LL_PC_REQ_CHANGE_DBM    4

/*! \brief Enable vendor specific command processing. */
#define LHCI_ENABLE_VS          1

/*! \brief Enable vendor specific test commands */
#define LHCI_ENABLE_VS_TEST     1

/*! \brief Max sample size. */
#define LL_MAX_CTE_SAMPLE       82

/*! \brief Absolute maximum number of advertising sets using PAwR. */
#define LL_MAX_PAWR_ADV_SETS    2

/*! \brief Max number of PAwR response slots. */
#define LL_MAX_PAWR_RSP_SLOTS   10

/*! \brief Maximum number of subevent data. */
#define LL_MAX_SUBEVENT         15

/*! \brief REF_ONLY Enable PAwR response observation. */
#define LL_ENABLE_RSPOBS        0

/*! \brief Maximum number of PAwR subevents. Must be less than 0x40. It is used as a 64-bit bitmask. */
#define LL_NUM_PAWR_SUBEVT      15

/*! \brief REF_ONLY Share init and scan memory, N.B. Application must guarantee initiate and scan states are mutually exclusive. */
#define LL_SHARE_INIT_SCAN_MEM  0

/*! \brief Maximum size of a single complete advertising data buffer. */
#define LL_COMP_EXT_ADV_DATA_MAX_LEN  251

//Scheduler Configurations
/*! \brief REF_ONLY Maximum number of reservations (maximum is 32). */
#define SCH_RM_MAX_RSVN         (LL_MAX_CONN + LL_MAX_ADV_SETS + LL_MAX_CIG + LL_MAX_BIG)

/*! \brief Preferred periodicity. */
#define SCH_RM_PREF_PER_USEC    10000

/*! \brief Lowest preferred periodicity (for cases where intervalMax < SCH_RM_PREF_PER_USEC). */
#define SCH_RM_PREF_PER_USEC_LOWEST 5000

/*! \brief REF_ONLY Check list requirements upon insertions and removals. */
#define SCH_CHECK_LIST_INTEGRITY    1

//Baseband Configurations
/*! \brief Maximum data channel payload length (valid range 27 to 251, exclude MIC). */
#define BB_DATA_PLD_MAX_LEN     251

/*! \brief REF_ONLY Maximum advertising channel payload length (valid range 95 to 255). */
#define BB_ADV_PLD_MAX_LEN      (BB_DATA_PLD_MAX_LEN + 4)

/*! \brief Fixed packet length required for BB data receive buffers (0 to disable). */
#define BB_FIXED_DATA_PKT_LEN   0

/*! \brief Fixed packet length required for BB advertisement receive buffers (0 to disable). */
#define BB_FIXED_ADVB_PKT_LEN   0

/*! \brief REF_ONLY Extra byte allocation required for BB operations (i.e. MIC). */
#define BB_DATA_PDU_TAILROOM    4

/*! \brief REF_ONLY Enable inline encryption mode. */
#define BB_ENABLE_INLINE_ENC_TX 1

/*! \brief REF_ONLY Enable inline decryption mode. */
#define BB_ENABLE_INLINE_DEC_RX 1

/*! \brief REF_ONLY Enable statistics in Baseband. */
#define BB_ENABLE_STATS         0

/*! \brief Symmetric PHY required on connections. */
#define BB_SYM_PHY_REQ          0

/*! \brief Absolute maximum number of antenna switching pattern for AOAD. */
#define BB_MAX_SWITCH_LEN       17

/*! \brief Maximum number of antennas for AOAD. */
#define BB_MAX_ANT              16

//Miscelaneous Configurations --------------------------------------------------------------------------------------------

#if (INIT_FEAT_CTE)
/*! \brief REF_ONLY Redudant Intialization Flag for CTE, uses ifdef logic */
#define INIT_CTE
#endif

/*! \brief REF_ONLY Only indicate the presence of CTE Hardware if the configuration is initializing the feature*/
#define CTE_PRESENT             INIT_FEAT_CTE

/*! \brief REF_ONLY Only indicate the availability of CTE is the configuration is initializing the feature */
#define CTE_AVAILABLE           INIT_FEAT_CTE

/*! \brief Used for special feature selection for the 5.3 Qualification build */
#define EM_BLE_53_MANDATORY_QUAL            0
