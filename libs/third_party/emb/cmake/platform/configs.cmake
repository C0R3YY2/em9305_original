# EMB Configuration Table of Contents:
# 0: Templates and Base Configs
# 1: Qualified Configurations
#         These are the configurations that were specifically used for BLE qualification to get our QDIDs
#         - EMB 5.3 qualification configuration
#         - EMB 5.4 qualification configuration
# 2: Verified SDK Configurations
#         These are configurations that we officially support, test, and include in our public SDK
#         - EMB Controller Configuration (5.4)
#         - EMB Peripheral Configuration (5.4)
#         - EMB Central Configuration (5.4)
#         - EMB Peripheral Configuration (4.2)
# 3: Internal Development Configurations
#         These are configurations that we DO NOT officially support. They are used for internal development, and should NOT be included in the SDK
#         - EMB 4.2 Internal
#         - EMB 5.2 RS CMW Configuration (Used for testing the EMB Controller with the Rhode Schwars CMW 270 Test Harness)
#         - EMB LE Audio (5.4)

##############################################################################
# 0: Templates and Base Configs
##############################################################################
SET(EMB_BLE_BASE_CONFIG

    ### BLE Features Enabled ###
    -DINIT_FEAT_PC          # Power Control Enabled

    ### Controller Configurations ###
    # Link Layer
    -DLL_MAX_CONN=4
    -DLL_MAX_ADV_SETS=2
    -DLL_MAX_CIS=2
    -DLL_MAX_BIS=2
    -DLL_MAX_BIG=1
    -DLL_MAX_CIG=1
    # Scheduler Layer
    -DSCH_CHECK_LIST_INTEGRITY=1
    # BaseBand
    -DBB_ENABLE_INLINE_ENC_TX=1
    -DBB_ENABLE_INLINE_DEC_RX=1
    # Lower HCI
    -DLHCI_ENABLE_VS=1
    -DLHCI_ENABLE_VS_TEST=1

    ### HCI Configurations ###
    -DCHCI_TR_UART=1        # DCHCI_TR_UART must be set to 1 for any type of transport (USB, UART or SPI)
    -DHCI_TR_EXACTLE=1      # Use ExactLE for HCI Shim layer

    ### Host Configurations ###
    # Device Manager
    -DDM_CONN_MAX=4
    # Security Manager
    -DSEC_CMAC_CFG=1
    -DSEC_ECC_CFG=2
    -DSEC_CCM_CFG=1

    ### Vendor Specific Configurations ###
    -DENABLE_PC_EMS_CMDS=1  # Enable PacketCraft Layer EM System Commands
)

##############################################################################
# 1: SIG Qualified Configurations
# DO NOT CHANGE THESE AFTER THEY HAVE BEEN ADDED, THESE ARE MEANT TO ALWAYS
# MATCH THE EXACT CONFIGURATION USED FOR THE QUALIFICATION BUILD
##############################################################################

# Bluetooth 5.3 Qualification Configuration (Used for qualification on 22-4-2022)
SET(EMB_BLE_53_QUAL_CONFIG

    # Note: This does not use EMB_BLE_BASE_CONFIG
    # It uses the exact config that was used for qualification
    -DLL_MAX_CONN=4
    -DLL_MAX_ADV_SETS=2
    -DLL_MAX_CIS=2
    -DLL_MAX_BIS=2
    -DLL_MAX_BIG=1
    -DLL_MAX_CIG=1

    -DDM_CONN_MAX=4

    -DSCH_CHECK_LIST_INTEGRITY=1

    -DCHCI_TR_TM=1
    -DCHCI_TR_UART=1

    -DLHCI_ENABLE_VS=1
    -DLHCI_ENABLE_VS_TEST=1

    -DBB_ENABLE_INLINE_ENC_TX=1
    -DBB_ENABLE_INLINE_DEC_RX=1

    -DSEC_CMAC_CFG=1
    -DSEC_ECC_CFG=2
    -DSEC_CCM_CFG=1

    -DHCI_TR_EXACTLE=1

    -DBT_VER=11

    -DINIT_BROADCASTER
    -DINIT_PERIPHERAL
    -DINIT_OBSERVER
    -DINIT_CENTRAL
    -DINIT_ENCRYPTED

    -DEM_BLE_53_MANDATORY_QUAL=1

    CACHE INTERNAL "EMB 5.3 Mandatory Qual Configuration"
)

# Bluetooth 5.4 Qualification Configuration (Used for qualification on 8-1-2024)
SET(EMB_BLE_54_QUAL_CONFIG

    # Note: This does not use EMB_BLE_BASE_CONFIG
    # It uses the exact config that was used for qualification
    -DLL_MAX_CONN=4
    -DLL_MAX_ADV_SETS=2
    -DLL_MAX_CIS=2
    -DLL_MAX_BIS=2
    -DLL_MAX_BIG=1
    -DLL_MAX_CIG=1

    -DDM_CONN_MAX=4

    -DSCH_CHECK_LIST_INTEGRITY=1

    # DCHCI_TR_UART must be set to 1 for any type of transport (USB, UART or SPI)
    -DCHCI_TR_UART=1

    -DLHCI_ENABLE_VS=1
    -DLHCI_ENABLE_VS_TEST=1

    -DBB_ENABLE_INLINE_ENC_TX=1
    -DBB_ENABLE_INLINE_DEC_RX=1

    -DSEC_CMAC_CFG=1
    -DSEC_ECC_CFG=2
    -DSEC_CCM_CFG=1

    -DHCI_TR_EXACTLE=1

    -DENABLE_PC_EMS_CMDS=1

    # Power Control Enabled
    -DINIT_FEAT_PC

    # Sleep clock accuracy (SCA) update
    -DFEAT_SCA_UPDATE=0

    -DBT_VER=13

    -DINIT_FEAT_ECU=1
    -DINIT_FEAT_PAST=1
    -DINIT_FEAT_PAWR=1
    -DLL_MAX_PAWR_RSP_SLOTS=10

    -DINIT_BROADCASTER
    -DINIT_PERIPHERAL
    -DINIT_OBSERVER
    -DINIT_CENTRAL
    -DINIT_ENCRYPTED

    CACHE INTERNAL "EMB 5.4 Qualification Configuration"
)


##############################################################################
# 2: Verified Configurations for SDK
##############################################################################
# EMB Controller Only Configuration (5.4) (Controller Only)
# See EMB BT Feature Matrix for Full Role/Features Supported
SET(EMB_CONTROLLER_CONFIG

    ${EMB_BLE_BASE_CONFIG}

    # BLE Version 5.4
    -DBT_VER=13

    ### BLE Roles Enabled ###
    -DINIT_BROADCASTER
    -DINIT_PERIPHERAL
    -DINIT_OBSERVER
    -DINIT_CENTRAL

    ### BLE Features Enabled/Configured ###

    # Encryption
    -DINIT_ENCRYPTED

    # Periodic Advertising Sync Transfer (PAST)
    -DINIT_FEAT_PAST=1

    # Periodic Advertising with Response (PAwR)
    -DINIT_FEAT_PAWR=1
    -DLL_MAX_PAWR_RSP_SLOTS=10

    # Enhanced Connection Update (ECU)
    -DINIT_FEAT_ECU=1

    CACHE INTERNAL "EMB Controller Configuration"
)

# EMB Peripheral Configuration (5.2) (Host + Controller)
# See EMB BT Feature Matrix for Full Role/Features Supported
SET(EMB_PERIPHERAL_CONFIG

    ${EMB_BLE_BASE_CONFIG}

    # BLE Version 5.2
    -DBT_VER=11

    ### BLE Roles Enabled ###
    -DINIT_PERIPHERAL

    ### BLE Features Enabled/Configured ###

    # Encryption
    -DINIT_ENCRYPTED

    CACHE INTERNAL "EMB Peripheral Configuration"
)

# EMB Central Configuration (5.2) (Host + Controller)
# See EMB BT Feature Matrix for Full Role/Features Supported
SET(EMB_CENTRAL_CONFIG

    ${EMB_BLE_BASE_CONFIG}

    # BLE Version 5.2
    -DBT_VER=11

    ### BLE Roles Enabled ###
    -DINIT_CENTRAL

    ### BLE Features Enabled/Configured ###

    # Encryption
    -DINIT_ENCRYPTED

    CACHE INTERNAL "EMB Central Configuration"
)

# EMB PAwR Advertiser Configuration (5.4) (Host + Controller)
# See EMB BT Feature Matrix for Full Role/Features Supported
SET(EMB_PAWR_ADV_CONFIG

    ${EMB_BLE_BASE_CONFIG}

    # BLE Version 5.4
    -DBT_VER=13

    ### BLE Roles Enabled ###
    -DINIT_BROADCASTER
    -DINIT_CENTRAL

    ### BLE Features Enabled/Configured ###

    # Encryption
    -DINIT_ENCRYPTED

    # Periodic Advertising Sync Transfer (PAST)
    -DINIT_FEAT_PAST=1

    # Periodic Advertising with Response (PAwR)
    -DINIT_FEAT_PAWR=1
    -DLL_MAX_PAWR_RSP_SLOTS=10

    # Enhanced Connection Update (ECU)
    -DINIT_FEAT_ECU=1

    CACHE INTERNAL "EMB PAwR Advertiser Configuration"
)

# EMB PAwR Synchronizer Configuration (5.4) (Host + Controller)
# See EMB BT Feature Matrix for Full Role/Features Supported
SET(EMB_PAWR_SYNC_CONFIG

    ${EMB_BLE_BASE_CONFIG}

    # BLE Version 5.4
    -DBT_VER=13

    ### BLE Roles Enabled ###
    -DINIT_OBSERVER
    -DINIT_PERIPHERAL

    ### BLE Features Enabled/Configured ###

    # Encryption
    -DINIT_ENCRYPTED

    # Periodic Advertising Sync Transfer (PAST)
    -DINIT_FEAT_PAST=1

    # Periodic Advertising with Response (PAwR)
    -DINIT_FEAT_PAWR=1
    -DLL_MAX_PAWR_RSP_SLOTS=10

    # Enhanced Connection Update (ECU)
    -DINIT_FEAT_ECU=1

    CACHE INTERNAL "EMB PAwR Synchronizer Configuration"
)

# EMB Peripheral Legacy Configuration (4.2) (Host + Controller)
# See EMB BT Feature Matrix for Full Role/Features Supported
SET(EMB_PERIPHERAL_LEGACY_CONFIG

    ${EMB_BLE_BASE_CONFIG}

    # BLE Version 4.2
    -DBT_VER=8

    ### BLE Roles Enabled ###
    -DINIT_PERIPHERAL

    ### BLE Features Enabled/Configured ###

    # Encryption
    -DINIT_ENCRYPTED

    CACHE INTERNAL "EMB Peripheral Legacy (4.2) Configuration"
)

SET(EMB_AUDIO_CONFIG
    ${EMB_BLE_BASE_CONFIG}
    # BLE Version 5.4
    -DBT_VER=13
    ### BLE Roles Enabled ###
    -DINIT_BROADCASTER
    -DINIT_PERIPHERAL
    -DINIT_OBSERVER
    -DINIT_CENTRAL
    ### BLE Features Enabled/Configured ###
    -DINIT_FEAT_ISO=1
    -DINIT_FEAT_BIS=1
    -DINIT_FEAT_CIS=1

    -DLL_MAX_BIS=2          # 2 BIS Streams
    -DLL_MAX_BIG=1          # 1 BIS Group
    -DLL_MAX_CIS=2          # 2 CIS Streams
    -DLL_MAX_CIG=1          # 1 CIS Group

    # Encryption
    -DINIT_ENCRYPTED
    # Periodic Advertising Sync Transfer (PAST)
    -DINIT_FEAT_PAST=1
    # Enhanced Connection Update (ECU)
    -DINIT_FEAT_ECU=1
    CACHE INTERNAL "EMB Audio Configuration"
)

