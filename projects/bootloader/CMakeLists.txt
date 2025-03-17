PROJECT(bootloader C)



SET(${PROJECT_NAME}_SRCS
    source/bl_app.c
    source/bl_app_task.c
    source/bl_bsp.c
    source/bl_nvm_main.c
)

SET(${PROJECT_NAME}_LIBS
    ${NVM_COMMON_LIBS}
    em_system_bl
    QPC
)

SET(${PROJECT_NAME}_INCLUDES
    ${COMMON_INCLUDES}
    includes
    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
)


BOOTLOADER_IN_NVM()
ARC_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
ARC_LINK_LIBRARIES(${PROJECT_NAME} ${${PROJECT_NAME}_LIBS})
ARC_INCLUDE_DIRECTORIES(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDES})
