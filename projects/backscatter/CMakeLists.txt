PROJECT(backscatter C)


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
    emdt_service
    emdt_profile_server

    unitimer
    uart_dma
    printf
    adc

    i2c
)

SET(${PROJECT_NAME}_EMCORE_LIBS
    emb_peripheral
    emdt_service
    emdt_profile_server
)

SET(${PROJECT_NAME}_INCLUDES
    ${EMB_COMMON_INCLUDES}
    my_app/includes
    ble/includes
    ${LIBS_DIR}/emb_vs/emdt/profile/server/includes
    ${LIBS_DIR}/emb_vs/emdt/service/includes

    ${COMMON_INCLUDES}
    ${LIBS_DIR}/adc/includes

    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em

    ${LIBS_DIR}/i2c/includes
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})

