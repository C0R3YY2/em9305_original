PROJECT(nvm_emb_datatransfer_server C)


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
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})

