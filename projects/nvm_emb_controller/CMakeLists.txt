PROJECT(nvm_emb_controller C)


SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    ble/source/ble_task.c
    ble/source/ble.c
    my_app/source/my_app_task.c
    my_app/source/my_app.c
)

SET(${PROJECT_NAME}_LIBS_COMMON
    ${NVM_COMMON_LIBS}
    QPC
)

SET(${PROJECT_NAME}_LIBS
    ${${PROJECT_NAME}_LIBS_COMMON}
    emb_controller
)

SET(${PROJECT_NAME}_EMCORE_LIBS
    emb_controller
)


SET(${PROJECT_NAME}_INCLUDES
    ${EMB_COMMON_INCLUDES}
    my_app/includes
    ble/includes
)

SET(targets_emcore_flavor "standard")

# Link Layer with BT5.3 qualified configuration (Standard version - no optional feature)
### Standard
## SPI FULL FEATURE
APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})

### Using EMCORE
GENERATE_EMCORE_APPLICATION(${PROJECT_NAME}_emcore
    ${targets_emcore_flavor} "${${PROJECT_NAME}_SRCS}"
    "${${PROJECT_NAME}_INCLUDES}" "" "${${PROJECT_NAME}_EMCORE_LIBS}"
)
