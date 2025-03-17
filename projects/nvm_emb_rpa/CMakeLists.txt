PROJECT(nvm_emb_rpa C)

ADD_C_FLAGS(-DNO_FLINT)

SET(${PROJECT_NAME}_RPA_SRCS
    app.c
    app_task.c
    nvm_main.c
    rpa_main.c
)

SET(${PROJECT_NAME}_RPA_LIBS
    ${NVM_COMMON_LIBS}
    printf # can be removed for non-debug
    uart
    QPC
    emb_peripheral
    emb_database
)

SET(${PROJECT_NAME}_INCLUDES
    ${EMB_COMMON_INCLUDES}
    ${LIBS_DIR}/third_party/emb/emb_database/includes
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_RPA_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_RPA_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})

IF(BUILD_SDK_INSTALLER AND "${SDK_TYPE}" STREQUAL "sdk-emb")
    ARC_INSTALL_PROJECTS(${PROJECT_NAME})
ENDIF()

IF(NOT SDK_BUILD)
    ADD_TARGET_TO_FW_PACKAGE(${PROJECT_NAME})
ENDIF()
