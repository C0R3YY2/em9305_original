PROJECT(qdec_example C)


SET(${PROJECT_NAME}_SRCS
    nvm_main.c
    my_app/source/my_app_task.c
    my_app/source/my_app.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}

    QPC
    unitimer
    uart_dma
    printf
    qdec
)

SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    ${LIBS_DIR}/qdec/includes
    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
    my_app/includes
)


APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})
