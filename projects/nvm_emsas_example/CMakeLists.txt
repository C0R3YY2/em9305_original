PROJECT(nvm_emsas_example C)


SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    bsp.c
    my_app/source/em_system_cmd_user.c
    my_app/source/emsas_example_cmd.c
    my_app/source/my_app_task.c
    my_app/source/my_app.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}

    QPC
    unitimer
)

SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    my_app/includes
    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_spi ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_spi ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_spi ${${PROJECT_NAME}_INCLUDES})
