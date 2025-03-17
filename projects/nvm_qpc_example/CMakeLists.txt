PROJECT(nvm_qpc_example C)


SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    bsp.c
    my_app_1/source/app_timer.c
    my_app_1/source/my_app_1_task.c
    my_app_1/source/my_app_1.c
    my_app_2/source/my_app_2_task.c
    my_app_2/source/my_app_2.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}

    QPC
)

SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    my_app_1/includes
    my_app_2/includes
    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
)


APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME}_spi ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME}_spi ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME}_spi ${${PROJECT_NAME}_INCLUDES})
