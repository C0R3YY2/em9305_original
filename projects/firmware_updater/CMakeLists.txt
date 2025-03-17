PROJECT(firmware_updater C)


SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    ble/source/ble_task.c
    ble/source/ble.c
    my_app/source/my_app_task.c
    my_app/source/my_app.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}
    QPC
    emb_peripheral
    firmware_update_core
    firmware_update_crypto
    firmware_update_spi
    fwu_profile_target
    fwu_service
    nvm_op_sched
)

SET(${PROJECT_NAME}_INCLUDE
    ${EMB_COMMON_INCLUDES}
    ${LIBS_DIR}/emb_vs/fwu/profile/target/includes
    ${LIBS_DIR}/emb_vs/fwu/service/includes
    ${LIBS_DIR}/firmware_update_core/includes
    ${LIBS_DIR}/firmware_update_crypto/includes
    ${LIBS_DIR}/firmware_update_spi/includes
    ${LIBS_DIR}/nvm_operation_scheduler/includes
    my_app/includes
    ble/includes
)

FIRMWARE_UPDATER_IN_NVM()
APP_VERSION("0x0100") # Version 1.0 of the firmware updater
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDE})

FIRMWARE_UPDATER_IN_NVM()
APP_VERSION("0x0200") # Version 2.0 of the firmware updater
ARC_EXECUTABLE(${PROJECT_NAME}_v2 ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_v2 ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_v2 ${${PROJECT_NAME}_INCLUDE})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME}_v2 "-DUPDATER_VER_2")

APP_IN_NVM()
APP_VERSION("0x0180") # Version 1.128 of the firmware updater (temporary application)
ARC_EXECUTABLE(${PROJECT_NAME}_tmp ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_tmp ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_tmp ${${PROJECT_NAME}_INCLUDE})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME}_tmp "-DUPDATER_TMP")

