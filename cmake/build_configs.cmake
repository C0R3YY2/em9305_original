# Target: ASIC
ARC_REGISTER_BUILD_CONFIG(
    NAME "ASIC"
    TARGET_POSTFIX ""
    CFLAGS ""
    IROM_SIZE "65536"
    LINKER_SCRIPT "linker.cmd"
    ALL_TARGET
)

# Target: Build all di03 target
ADD_CUSTOM_TARGET(all_di03
COMMENT "Build all di03 target"
)

# Target: Build all di04 target
ADD_CUSTOM_TARGET(all_di04
COMMENT "Build all di04 target"
)

# Target: Build all di05 target
ADD_CUSTOM_TARGET(all_di05
COMMENT "Build all di05 target"
)

