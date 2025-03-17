PROJECT(nvm_emb_ancs_client C)


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
    printf
)

SET(${PROJECT_NAME}_EMCORE_LIBS
    emb_peripheral
)

SET(${PROJECT_NAME}_INCLUDES
    ${EMB_COMMON_INCLUDES}
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
    "-DNO_FLINT_ERRORS"
    "${${PROJECT_NAME}_EMCORE_LIBS}"
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_ancs ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_ancs ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_ancs ${${PROJECT_NAME}_INCLUDES})
ADD_TARGET_COMPILE_FLAGS_ALL_BUILD_CONFIGS(${PROJECT_NAME}_ancs)

### Using EMCORE
GENERATE_EMCORE_APPLICATION(${PROJECT_NAME}_ancs_emcore "${targets_emcore_flavor}"
    "${${PROJECT_NAME}_SRCS}" "${${PROJECT_NAME}_INCLUDES}"
    "-DNO_FLINT_ERRORS"
    "${${PROJECT_NAME}_EMCORE_LIBS}"
)

