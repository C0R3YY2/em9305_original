PROJECT(nvm_emb_assettag C)


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
    emb_cte_transmitter
)

SET(${PROJECT_NAME}_EMCORE_LIBS
    emb_cte_transmitter
    aoad
)

SET(${PROJECT_NAME}_INCLUDES
    ${EMB_COMMON_INCLUDES}
    my_app/includes
    ble/includes
)

# asset tag example is BLE standard stack
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

#set(targets_emcore_flavor "my_flavor;v4.1.0")
set(targets_emcore_flavor "my_flavor")

GENERATE_EMCORE_APPLICATION(${PROJECT_NAME}_emcore_my_flavor "${targets_emcore_flavor}"
    "${${PROJECT_NAME}_SRCS}" "${${PROJECT_NAME}_INCLUDES}"
    "-DNO_FLINT_ERRORS"
    "${${PROJECT_NAME}_EMCORE_LIBS}"
)

