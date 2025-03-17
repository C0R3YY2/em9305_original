PROJECT(nvm_emb_lite_hrs C)


SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    ble/source/ble_task.c
    ble/source/ble.c
    ble/source/embLite_config.c
    my_app/source/my_app_task.c
    my_app/source/my_app.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}
    QPC
    emb_peripheral
    emb_lite_peripheral
)

SET(${PROJECT_NAME}_EMCORE_LIBS
    emb_peripheral
    emb_lite_peripheral
)

SET(${PROJECT_NAME}_INCLUDES
    ${EMB_COMMON_INCLUDES}
    ${LIBS_DIR}/third_party/emb/emb_lite/include
    my_app/includes
    ble/includes
)

SET(targets_emcore_flavor "standard")

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})

### Using EMCORE
GENERATE_EMCORE_APPLICATION(${PROJECT_NAME}_emcore "${targets_emcore_flavor}"
    "${${PROJECT_NAME}_SRCS}" "${${PROJECT_NAME}_INCLUDES}"
    ""
    "${${PROJECT_NAME}_EMCORE_LIBS}"
)

