PROJECT(ti_example C)


# List of C source files required to build the application
set(${PROJECT_NAME}_SRCS
    my_app/source/my_app.c
    my_app/source/my_app_task.c
    nvm_main.c
)

# Set libraries to use
SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}
    QPC
    unitimer
    uart_dma
    printf
    temperature_indicator
)

# Set include paths
SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    ${LIBS_DIR}/temperature_indicator/includes

    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em

    my_app/includes
)

APP_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})
