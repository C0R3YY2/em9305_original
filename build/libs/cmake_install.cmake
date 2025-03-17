# Install script for directory: C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/t9305_sdk")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/MinGW/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/aoad/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/app_entry/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/button_support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/em_core/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/em_hw_api/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/em_system/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/em_system_stack/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/firmware_header/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/firmware_update_core/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/firmware_update_crypto/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/firmware_update_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/led_support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/memory_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/nvm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/nvm_entry/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/nvm_info/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/nvm_operation_scheduler/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/pml/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/prot_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/qdec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/radio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/rc_calib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/sleep_manager/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/sleep_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/spi_master/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/spi_slave/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/temperature_indicator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/transport/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/unitimer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/usb/core/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/third_party/printf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/third_party/QPC/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/third_party/Metaware/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/third_party/unit_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/third_party/emb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/emb_vs/fwu/service/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/emb_vs/fwu/profile/target/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/emb_vs/emdt/service/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/emb_vs/emdt/profile/server/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/emb_vs/emdt/profile/client/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/emb_vs/rscps/profile/server/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/nvm_bootloader/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/libs/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
