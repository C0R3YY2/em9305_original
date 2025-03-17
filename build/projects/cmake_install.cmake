# Install script for directory: C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects

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
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/bootloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_qpc_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_unit_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/printf_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/rtc_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/adc_single_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/adc_continuous_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/qdec_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/ti_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/spi_master_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/spi_slave_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/i2s_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/i2c_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/watchdog_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_audio_control/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_assettag/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_controller/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_datatransfer_client/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_datatransfer_server/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_ext_adv/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_fit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_lite_hrs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_power_control/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_rpa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_tag/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emsas_example/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/boot_selector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/firmware_updater/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_fwu_target/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_ancs_client/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/backscatter/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
