PROJECT(nvm_fwu_target C)

ADD_C_FLAGS(-DNO_FLINT_ERRORS)

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
    firmware_update_spi_light
    fwu_profile_target_light
    fwu_service_light
    printf
    nvm_op_sched
)

SET(${PROJECT_NAME}_EMCORE_LIBS
    emb_peripheral
    firmware_update_core
    firmware_update_crypto
    firmware_update_spi_light
    fwu_profile_target_light
    fwu_service_light
    printf
    nvm_op_sched
)

SET(${PROJECT_NAME}_INCLUDE
    ${EMB_COMMON_INCLUDES}
    ${LIBS_DIR}/emb_vs/fwu/profile/target/includes
    ${LIBS_DIR}/emb_vs/fwu/service/includes
    ${LIBS_DIR}/firmware_update_core/includes
    ${LIBS_DIR}/firmware_update_crypto/includes
    ${LIBS_DIR}/firmware_update_spi/includes
    my_app/includes
    ble/includes
)

# hrs example is BLE standard stack
SET(targets_emcore_flavor "standard")

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDE})

### Using EMCORE
GENERATE_EMCORE_APPLICATION(${PROJECT_NAME}_emcore "${targets_emcore_flavor}"
    "${${PROJECT_NAME}_SRCS}" "${${PROJECT_NAME}_INCLUDE}"
    ""
    "${${PROJECT_NAME}_EMCORE_LIBS}"
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_v2 ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_v2 ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_v2 ${${PROJECT_NAME}_INCLUDE})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME}_v2 "-DAPP_VER_2")

### Using EMCORE
GENERATE_EMCORE_APPLICATION(${PROJECT_NAME}_v2_emcore "${targets_emcore_flavor}"
    "${${PROJECT_NAME}_SRCS}" "${${PROJECT_NAME}_INCLUDE}"
    "-DAPP_VER_2"
    "${${PROJECT_NAME}_EMCORE_LIBS}"
)

